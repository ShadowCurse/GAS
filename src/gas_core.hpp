#ifndef RESOURCE_TREE_H
#define RESOURCE_TREE_H

#include <QObject>

#include "core/core.hpp"
#include "gui/gas_gui.hpp"

class GasCore final : public QObject {
  Q_OBJECT

 public:
  using id_type = GasGui::id_type;
  using item_id = GasGui::item_id;
  using connector_info = GasGui::connector_info;
  using login_info = GasGui::login_info;
  using create_user_info = GasGui::create_user_info;

  using resource_create_type = GasGui::resource_create_type;
  using resource_update_type = GasGui::resource_update_type;
  using resource_download_type = GasGui::resource_download_type;
  using resource_delete_type = GasGui::resource_delete_type;

  using resource_tree_type = GasGui::resource_tree_type;
  using commits_list_type = GasGui::commits_list_type;
  using users_list_type = GasGui::users_list_type;
  using logs_list_type = GasGui::logs_list_type;

  using resource_info_type = GasGui::resource_info_type;
  using commit_info_type = GasGui::commit_info_type;
  using user_info_type = GasGui::user_info_type;

 public:
  GasCore();
  ~GasCore() final = default;

  auto show_gui() -> void { gui_.show(); }

 public slots:

  void slot_first_connect(connector_info);
  void slot_first_login(login_info);
  void slot_first_create_user(create_user_info);

  void slot_connector_state_change(id_type, bool);

  void slot_create_resource_tree();
  void slot_create_commits_list();
  void slot_create_users_list();
  void slot_create_logs_list();

  void slot_send_resource_info(item_id);
  void slot_send_commit_info(item_id);
  void slot_send_user_info(item_id);

  void slot_add_new_resource(resource_create_type);
  void slot_update_resource(resource_update_type);
  void slot_download_resource(resource_download_type);
  void slot_remove_resource(resource_delete_type);

  void slot_send_connector_settings(id_type);
  void slot_create_connector(connector_info);
  void slot_remove_connector(id_type);

 signals:

  void signal_starting_connection_state_change(bool);
  void signal_accept_user();
  void signal_deny_user();

  void signal_draw_resource_tree(resource_tree_type);
  void signal_draw_commits_list(commits_list_type);
  void signal_draw_users_list(users_list_type);
  void signal_draw_logs_list(logs_list_type);

  void signal_draw_resource_info(resource_info_type);
  void signal_draw_commit_info(commit_info_type);
  void signal_draw_users_info(user_info_type);

  void signal_send_connector_settings(connector_info);
  void signal_create_connector(connector_info);
  void signal_remove_connector(id_type);

  void signal_log_message(std::string);
  void signal_log_warning(std::string);
  void signal_log_error(std::string);

 private:
  GasGui gui_;
  gas::Core core_;
  GasGui::id_type main_storage_{};
  gas::User current_user;
};

#endif  // RESOURCE_TREE_H
