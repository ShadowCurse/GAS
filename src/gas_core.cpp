#include "gas_core.hpp"

GasCore::GasCore() : QObject() {
  // signals from gui
  connect(&gui_, &GasGui::signal_first_connect, this,
          &GasCore::slot_first_connect);
  connect(&gui_, &GasGui::signal_first_login, this, &GasCore::slot_first_login);
  connect(&gui_, &GasGui::signal_first_create_user, this,
          &GasCore::slot_first_create_user);

  connect(&gui_, &GasGui::signal_connector_state_change, this,
          &GasCore::slot_connector_state_change);

  connect(&gui_, &GasGui::signal_create_resource_tree, this,
          &GasCore::slot_create_resource_tree);
  connect(&gui_, &GasGui::signal_create_commits_list, this,
          &GasCore::slot_create_commits_list);
  connect(&gui_, &GasGui::signal_create_users_list, this,
          &GasCore::slot_create_users_list);
  connect(&gui_, &GasGui::signal_create_logs_list, this,
          &GasCore::slot_create_logs_list);

  connect(&gui_, &GasGui::signal_get_resource_info, this,
          &GasCore::slot_send_resource_info);
  connect(&gui_, &GasGui::signal_get_commit_info, this,
          &GasCore::slot_send_commit_info);
  connect(&gui_, &GasGui::signal_get_user_info, this,
          &GasCore::slot_send_user_info);

  connect(&gui_, &GasGui::signal_add_new_resource, this,
          &GasCore::slot_add_new_resource);
  connect(&gui_, &GasGui::signal_update_resource, this,
          &GasCore::slot_update_resource);
  connect(&gui_, &GasGui::signal_download_resource, this,
          &GasCore::slot_download_resource);
  connect(&gui_, &GasGui::signal_remove_resource, this,
          &GasCore::slot_remove_resource);

  connect(&gui_, &GasGui::signal_request_connector_settings, this,
          &GasCore::slot_send_connector_settings);
  connect(&gui_, &GasGui::signal_request_connector_create, this,
          &GasCore::slot_create_connector);
  connect(&gui_, &GasGui::signal_request_connector_remove, this,
          &GasCore::slot_remove_connector);

  // signals for gui
  connect(this, &GasCore::signal_starting_connection_state_change, &gui_,
          &GasGui::slot_starting_connection_state_change);
  connect(this, &GasCore::signal_accept_user, &gui_, &GasGui::slot_accept_user);
  connect(this, &GasCore::signal_deny_user, &gui_, &GasGui::slot_deny_user);

  connect(this, &GasCore::signal_draw_resource_tree, &gui_,
          &GasGui::slot_draw_resource_tree);
  connect(this, &GasCore::signal_draw_commits_list, &gui_,
          &GasGui::slot_draw_commits_list);
  connect(this, &GasCore::signal_draw_users_list, &gui_,
          &GasGui::slot_draw_users_list);
  connect(this, &GasCore::signal_draw_logs_list, &gui_,
          &GasGui::slot_draw_logs_list);

  connect(this, &GasCore::signal_draw_resource_info, &gui_,
          &GasGui::slot_draw_resource_info);
  connect(this, &GasCore::signal_draw_commit_info, &gui_,
          &GasGui::slot_draw_commit_info);
  connect(this, &GasCore::signal_draw_users_info, &gui_,
          &GasGui::slot_draw_users_info);

  connect(this, &GasCore::signal_send_connector_settings, &gui_,
          &GasGui::slot_show_connector_settings);
  connect(this, &GasCore::signal_create_connector, &gui_,
          &GasGui::slot_create_connector);
  connect(this, &GasCore::signal_remove_connector, &gui_,
          &GasGui::slot_remove_connector);

  connect(this, &GasCore::signal_log_message, &gui_, &GasGui::log_message);
  connect(this, &GasCore::signal_log_warning, &gui_, &GasGui::log_warning);
  connect(this, &GasCore::signal_log_error, &gui_, &GasGui::log_error);
}

void GasCore::slot_first_connect(GasCore::connector_info info) {
  gas::Settings settings;
  settings.host = std::move(info.host);
  settings.port = info.port;
  settings.db_name = std::move(info.db_name);
  settings.username = std::move(info.username);
  settings.password = std::move(info.password);

  auto st = core_.add_storage(std::move(settings));
  if (core_.connect_storage(st)) {
    info.id = st;
    main_storage_ = st;
    emit signal_starting_connection_state_change(true);
    emit signal_create_connector(std::move(info));
  } else {
    core_.remove_storage(st);
    emit signal_starting_connection_state_change(false);
    emit signal_log_error("can not connect to db: invalid settings");
  }
}

void GasCore::slot_first_login(GasCore::login_info info) {
  if (auto result =
          core_.search_user(main_storage_, info.username, info.password)) {
    current_user = std::move(*result);
    emit signal_accept_user();
  } else
    emit signal_deny_user();
}

void GasCore::slot_first_create_user(GasCore::create_user_info info) {
  if (core_.search_user(main_storage_, info.username)) {
    emit signal_log_warning("such user already exists");
    emit signal_deny_user();
    return;
  } else {
    if (auto result =
            core_.insert_user(main_storage_, info.username, info.password,
                              info.email, info.description)) {
      current_user = std::move(*result);
      emit signal_accept_user();
    } else {
      emit signal_log_error("can not create user");
      emit signal_deny_user();
    }
  }
}

void GasCore::slot_connector_state_change(GasCore::id_type id, bool state) {
  if (state)
    core_.connect_storage(id);
  else
    core_.disconnect_storage(id);
}

void GasCore::slot_create_resource_tree() {
  resource_tree_type tree;
  auto type_view = core_.create_view<gas::ResourceType>();
  auto resource_view = core_.create_view<gas::Resource>();
  for (const auto& [type_storage_id, type] : type_view) {
    for (const auto& [resource_storage_id, res] : resource_view) {
      if (type_storage_id == resource_storage_id && res.type == type.id)
        tree[type.name].push_back(
            {{static_cast<id_type>(res.id), resource_storage_id}, res.name});
    }
  }
  emit signal_draw_resource_tree(std::move(tree));
}

void GasCore::slot_create_commits_list() {
  commits_list_type list;
  auto commit_view = core_.create_view<gas::Commit>();
  //  for (const auto& [storage_id, commit] : commit_view) {
  auto commit{std::begin(commit_view)};
  auto end{std::end(commit_view)};
  for (; commit != end; ++commit) {
    list.push_back(
        {{static_cast<id_type>((*commit).second.id), (*commit).first},
         (*commit).second.date,
         (*commit).second.message});
  }
  emit signal_draw_commits_list(std::move(list));
}

void GasCore::slot_create_users_list() {
  users_list_type list;
  auto users_view = core_.create_view<gas::User>();
  for (const auto& [storage_id, user] : users_view) {
    list.push_back(
        {{static_cast<id_type>(user.id), storage_id}, user.username});
  }
  emit signal_draw_users_list(std::move(list));
}

void GasCore::slot_create_logs_list() {
  logs_list_type list;
  auto logs_view = core_.create_view<gas::Log>();
  for (const auto& [storage_id, log] : logs_view)
    list.push_back({log.date, log.description});
  emit signal_draw_logs_list(std::move(list));
}

void GasCore::slot_send_resource_info(GasCore::item_id id) {
  if (auto result = core_.search<gas::Resource>(
          id.storage_id,
          [&](const auto& res) { return res.id == id.item_id; })) {
    resource_info_type info;
    info.name = result->name;
    info.description = result->description;
    info.size = result->size;
    if (auto type_result = core_.search<gas::ResourceType>(
            id.storage_id,
            [&](const auto& type) { return type.id == result->type; }))
      info.type = type_result->name;
    auto commits = core_.search_all<gas::Commit>(
        id.storage_id,
        [&](const auto& commit) { return commit.resource == result->id; });
    for (const auto& commit : commits)
      info.commits.push_back({commit.date, commit.message});
    auto dependencies = core_.search_all<gas::Dependency>(
        id.storage_id, [&](const auto& dependency) {
          return dependency.requesting_resource == result->id;
        });
    auto resources = core_.create_view<gas::Resource>(id.storage_id);
    for (const auto& dep : dependencies)
      for (const auto& res : resources)
        if (res.id == dep.required_resource)
          info.dependencies.push_back(res.name);
    emit signal_draw_resource_info(std::move(info));
  }
}

void GasCore::slot_send_commit_info(GasCore::item_id id) {
  if (auto result = core_.search<gas::Commit>(
          id.storage_id,
          [&](const auto& commit) { return commit.id == id.item_id; })) {
    commit_info_type info;
    if (auto user = core_.search<gas::User>(
            id.storage_id,
            [&](const auto& user) { return user.id == result->user; }))
      info.username = user->username;
    if (auto resource = core_.search<gas::Resource>(
            id.storage_id,
            [&](const auto& res) { return res.id == result->resource; }))
      info.resource_name = resource->name;
    info.date = result->date;
    info.message = result->message;
    emit signal_draw_commit_info(std::move(info));
  }
}

void GasCore::slot_send_user_info(GasCore::item_id id) {
  if (auto result = core_.search<gas::User>(
          id.storage_id,
          [&](const auto& user) { return user.id == id.item_id; })) {
    user_info_type info;
    info.name = result->username;
    info.email = result->email;
    info.description = result->description;
    emit signal_draw_users_info(std::move(info));
  }
}

void GasCore::slot_add_new_resource(GasCore::resource_create_type info) {
  if (auto type = core_.search<gas::ResourceType>(
          info.connector_id,
          [&](const auto& type) { return type.name == info.type; })) {
    gas::Resource resource;
    resource.name = std::move(info.name);
    resource.description = std::move(info.description);
    resource.type = type->id;
    core_.create_resource(info.connector_id, resource, info.file_path);
  } else
    return;
}

void GasCore::slot_update_resource(GasCore::resource_update_type info) {
  if (auto old_res = core_.search<gas::Resource>(
          info.id.storage_id,
          [&](const auto& res) { return res.id == info.id.item_id; })) {
    old_res->name = std::move(info.name);
    old_res->description = std::move(info.description);
    if (auto type = core_.search<gas::ResourceType>(
            info.id.storage_id,
            [&](const auto& type) { return type.name == info.type; }))
      old_res->type = type->id;
    else
      return;
    gas::Commit commit;
    commit.user = current_user.id;
    commit.resource = info.id.item_id;
    commit.message = std::move(info.commit_message);
    if (!core_.update_resource(info.id.storage_id, *old_res, info.file_path) ||
        !core_.insert_commit(info.id.storage_id, commit))
      emit signal_log_error("could not update resource");
  }
}

void GasCore::slot_download_resource(GasCore::resource_download_type info) {
  if (auto res = core_.search<gas::Resource>(
          info.id.storage_id,
          [&](const auto& res) { return res.id == info.id.item_id; })) {
    core_.download_resource(info.id.storage_id, *res, info.file_path);
  }
}

void GasCore::slot_remove_resource(GasCore::resource_delete_type info) {
  if (auto res = core_.search<gas::Resource>(
          info.storage_id,
          [&](const auto& res) { return res.id == info.item_id; })) {
    core_.remove_resource(info.storage_id, *res);
  }
}

void GasCore::slot_send_connector_settings(GasCore::id_type id) {
  if (auto settings = core_.get_storage_settings(id)) {
    connector_info info;
    info.host = std::move(settings->host);
    info.port = settings->port;
    info.db_name = std::move(settings->db_name);
    info.username = std::move(settings->username);
    emit signal_send_connector_settings(std::move(info));
  }
}

void GasCore::slot_create_connector(GasCore::connector_info info) {
  gas::Settings settings;
  settings.host = std::move(info.host);
  settings.port = info.port;
  settings.db_name = std::move(info.db_name);
  settings.username = std::move(info.username);
  settings.password = std::move(info.password);
  auto storage = core_.add_storage(std::move(settings));
  info.id = storage;
  emit signal_create_connector(std::move(info));
}

void GasCore::slot_remove_connector(GasCore::id_type id) {
  core_.remove_storage(id);
}
