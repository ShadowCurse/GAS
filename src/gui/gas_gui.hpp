#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class GasGui : public QMainWindow {
  Q_OBJECT

 public:
  using id_type = uint32_t;

  struct login_info {
    std::string username;
    std::string password;
  };
  struct create_user_info {
    std::string username;
    std::string password;
    std::string email;
    std::string description;
  };

  // resource
  struct resource_info_type {
    struct resource_commit {
      std::string date;
      std::string description;
    };

    std::string name;
    std::string description;
    uint32_t size;
    std::string type;
    std::vector<resource_commit> commits;
    std::vector<std::string> dependencies;
  };
  struct resource_tree_node_type {
      std::string name;
      std::vector<std::pair<id_type, std::string>> children;
  };
  using resource_tree_type =
      std::vector<resource_tree_node_type>;

  // commit
  struct commit_info_type {
      id_type id{};
      std::string username;
      std::string resource_name;
      std::string date;
      std::string message;
  };
  struct commit_list_node {
      id_type id{};
      std::string date;
      std::string message;
  };
  using commits_list_type =
      std::vector<commit_list_node>;

  // users
  struct user_info_type {
      std::string name;
      std::string email;
      std::string description;
  };
  struct user_list_node {
      id_type id{};
      std::string name;
  };
  using users_list_type = std::vector<user_list_node>;

  // logs
  struct log_list_node {
      std::string date;
      std::string message;
  };
  using logs_list_type = std::vector<log_list_node>;

  // resource action types
  struct resource_create_type {
    std::string name;
    std::string description;
    std::string file_path;
    std::string type;
  };

  struct resource_update_type {
    id_type id{};
    std::string name;
    std::string description;
    std::string file_path;
    std::string type;
  };

  using resource_download_type = std::string;
  using resource_delete_type = id_type;

  struct connector_info {
    id_type id{};
    std::string name;
    std::string host;
    uint32_t port;
    std::string db_name;
    std::string username;
    std::string password;
  };

 public:
  GasGui(QWidget *parent = nullptr);
  ~GasGui();

 public slots:

  // starting page
  void slot_starting_connection_state_change(bool);
  void slot_accept_user();
  void slot_deny_user();

  // tabs
  void slot_draw_resource_tree(resource_tree_type resourece_tree);
  void slot_draw_commits_list(commits_list_type commit_list);
  void slot_draw_users_list(users_list_type devs_list);
  void slot_draw_logs_list(logs_list_type log_list);

  void slot_draw_resource_info(resource_info_type info);
  void slot_draw_commit_info(commit_info_type info);
  void slot_draw_users_info(user_info_type info);

  // setttings
  void slot_show_connector_settings(connector_info);
  void slot_create_connector(connector_info);
  void slot_remove_connector(id_type);

  // app logs
  void log_message(std::string msg);
  void log_warning(std::string msg);
  void log_error(std::string msg);

 private slots:
  // starting page
  void slot_starting_connect();
  void slot_starting_login();
  void slot_starting_create_user();

  // connectors bar
  void slot_connector_state_change();

  // tabs
  void slot_tab_click(int index);

  // resource controls
  void slot_add_new_resource();
  void slot_update_resource();
  void slot_download_resource();
  void slot_remove_resource();

  // resource tree
  void slot_resource_click(QTreeWidgetItem *item);

  // commits
  void slot_commit_click(QTreeWidgetItem *item);

  // users
  void slot_user_click(QTreeWidgetItem *item);

  // setttings
  void slot_prepare_for_new_connector_settings();
  void slot_request_connector_settings();

  void slot_request_connector_creation();
  void slot_request_connector_remove();

 signals:

  void signal_first_connect(connector_info);
  void signal_first_login(login_info);
  void signal_first_create_user(create_user_info);

  void signal_connector_state_change(id_type, bool);

  void signal_create_resource_tree();
  void signal_create_commits_list();
  void signal_create_users_list();
  void signal_create_logs_list();

  void signal_get_resource_info(id_type id);
  void signal_get_commit_info(id_type id);
  void signal_get_user_info(id_type id);

  void signal_add_new_resource(resource_create_type);
  void signal_update_resource(resource_update_type);
  void signal_download_resource(resource_download_type);
  void signal_remove_resource(resource_delete_type);

  void signal_request_connector_settings(id_type);
  void signal_request_connector_create(connector_info);
  void signal_request_connector_remove(id_type);

 private:
  Ui::MainWindow *ui;

  std::map<QTreeWidgetItem *, id_type> resource_tree_ids;
  std::map<QTreeWidgetItem *, id_type> commits_ids;
  std::map<QTreeWidgetItem *, id_type> users_ids;
  std::map<QPushButton *, id_type> connections_bar_ids;
  std::map<QPushButton *, id_type> connections_settings_ids;
};
#endif  // MAINWINDOW_H
