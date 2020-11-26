#include "gas_core.hpp"

GasCore::GasCore() : QObject() {
  // signals from gui
  connect(&gui, &GasGui::signal_first_connect, this,
          &GasCore::slot_first_connect);
  connect(&gui, &GasGui::signal_first_login, this, &GasCore::slot_first_login);
  connect(&gui, &GasGui::signal_first_create_user, this,
          &GasCore::slot_first_create_user);

  connect(&gui, &GasGui::signal_connector_state_change, this,
          &GasCore::slot_connector_state_change);

  connect(&gui, &GasGui::signal_create_resource_tree, this,
          &GasCore::slot_create_resource_tree);
  connect(&gui, &GasGui::signal_create_commits_list, this,
          &GasCore::slot_create_commits_list);
  connect(&gui, &GasGui::signal_create_users_list, this,
          &GasCore::slot_create_users_list);
  connect(&gui, &GasGui::signal_create_logs_list, this,
          &GasCore::slot_create_logs_list);

  connect(&gui, &GasGui::signal_get_resource_info, this,
          &GasCore::slot_send_resource_info);
  connect(&gui, &GasGui::signal_get_commit_info, this,
          &GasCore::slot_send_commit_info);
  connect(&gui, &GasGui::signal_get_user_info, this,
          &GasCore::slot_send_user_info);

  connect(&gui, &GasGui::signal_add_new_resource, this,
          &GasCore::slot_add_new_resource);
  connect(&gui, &GasGui::signal_update_resource, this,
          &GasCore::slot_update_resource);
  connect(&gui, &GasGui::signal_download_resource, this,
          &GasCore::slot_download_resource);
  connect(&gui, &GasGui::signal_remove_resource, this,
          &GasCore::slot_remove_resource);

  connect(&gui, &GasGui::signal_request_connector_settings, this,
          &GasCore::slot_send_connector_settings);
  connect(&gui, &GasGui::signal_request_connector_create, this,
          &GasCore::slot_create_connector);
  connect(&gui, &GasGui::signal_request_connector_remove, this,
          &GasCore::slot_remove_connector);

  // signals for gui
  connect(this, &GasCore::signal_starting_connection_state_change, &gui,
          &GasGui::slot_starting_connection_state_change);
  connect(this, &GasCore::signal_accept_user, &gui, &GasGui::slot_accept_user);
  connect(this, &GasCore::signal_deny_user, &gui, &GasGui::slot_deny_user);

  connect(this, &GasCore::signal_draw_resource_tree, &gui,
          &GasGui::slot_draw_resource_tree);
  connect(this, &GasCore::signal_draw_commits_list, &gui,
          &GasGui::slot_draw_commits_list);
  connect(this, &GasCore::signal_draw_users_list, &gui,
          &GasGui::slot_draw_users_list);
  connect(this, &GasCore::signal_draw_logs_list, &gui,
          &GasGui::slot_draw_logs_list);

  connect(this, &GasCore::signal_draw_resource_info, &gui,
            &GasGui::slot_draw_resource_info);
  connect(this, &GasCore::signal_draw_commit_info, &gui,
            &GasGui::slot_draw_commit_info);
  connect(this, &GasCore::signal_draw_users_info, &gui,
            &GasGui::slot_draw_users_info);

  connect(this, &GasCore::signal_send_connector_settings, &gui,
          &GasGui::slot_show_connector_settings);
  connect(this, &GasCore::signal_create_connector, &gui,
          &GasGui::slot_create_connector);
  connect(this, &GasCore::signal_remove_connector, &gui,
          &GasGui::slot_remove_connector);

  connect(this, &GasCore::signal_log_message, &gui, &GasGui::log_message);
  connect(this, &GasCore::signal_log_warning, &gui, &GasGui::log_warning);
  connect(this, &GasCore::signal_log_error, &gui, &GasGui::log_error);
}

void GasCore::slot_first_connect(GasCore::connector_info info) {
  emit signal_starting_connection_state_change(true);
  info.id = 0;
  emit signal_create_connector(std::move(info));
}

void GasCore::slot_first_login(GasCore::login_info) {
  emit signal_accept_user();
}

void GasCore::slot_first_create_user(GasCore::create_user_info) {}

void GasCore::slot_connector_state_change(GasCore::id_type, bool) {}

void GasCore::slot_create_resource_tree() {}

void GasCore::slot_create_commits_list() {}

void GasCore::slot_create_users_list() {}

void GasCore::slot_create_logs_list() {}

void GasCore::slot_send_resource_info(GasCore::id_type) {}

void GasCore::slot_send_commit_info(GasCore::id_type) {}

void GasCore::slot_send_user_info(GasCore::id_type) {}

void GasCore::slot_add_new_resource(GasCore::resource_create_type) {}

void GasCore::slot_update_resource(GasCore::resource_update_type) {}

void GasCore::slot_download_resource(GasCore::resource_download_type) {}

void GasCore::slot_remove_resource(GasCore::resource_delete_type) {}

void GasCore::slot_send_connector_settings(GasCore::id_type) {}

void GasCore::slot_create_connector(GasCore::connector_info) {}

void GasCore::slot_remove_connector(GasCore::id_type) {}
