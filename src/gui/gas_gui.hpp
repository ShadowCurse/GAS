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
    using login_info = std::string;
    struct create_user_info {
        std::string name;
        std::string email;
        std::string description;
    };

  using resourece_tree_type =
      std::vector<std::pair<QString, std::vector<std::pair<int, QString>>>>;
  using commits_list_type =
      std::vector<std::pair<int, std::pair<QString, QString>>>;
  using developers_list_type =
      std::vector<std::pair<int, std::pair<QString, QString>>>;
  using logs_list_type = std::vector<std::pair<QString, QString>>;

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

  using commit_info_type = std::vector<std::pair<QString, QString>>;
  using developer_info_type = std::vector<std::pair<QString, QString>>;

  struct resource_create_info_type {
    std::string name;
    std::string description;
    std::string file_path;
    std::string type;
  };

  struct resource_update_info_type {
    int id{};
    std::string name;
    std::string description;
    std::string file_path;
    std::string type;
  };

  using resource_download_info = std::string;
  using resource_delete_info = int;

  struct connector_info {
      int id{};
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
  void slot_starting_connect();
  void slot_starting_login();
  void slot_starting_create_user();
  void slot_accept_user();


  // connectors bar
  void slot_connector_state_change();

  // tabs
  void slot_tab_click(int index);

  void slot_draw_resource_tree(resourece_tree_type resourece_tree);
  void slot_draw_commits_list(commits_list_type commit_list);
  void slot_draw_developers_list(developers_list_type devs_list);
  void slot_draw_logs_list(logs_list_type log_list);

  void slot_draw_resource_info(resource_info_type info);
  void slot_draw_commit_info(commit_info_type info);
  void slot_draw_developer_info(developer_info_type info);

  // resource controls
  void slot_add_new_resource();
  void slot_update_resource();
  void slot_download_resource();
  void slot_delete_resource();

  // resource tree
  void slot_resource_click(QTreeWidgetItem *item);

  // commits
  void slot_commit_click(QTreeWidgetItem *item);

  // setttings
  void slot_prepare_for_new_connector_settings();
  void slot_request_connector_settings();
  void slot_show_connector_settings(connector_info);
  void slot_submit_connector_creation();
  void slot_create_connector(connector_info);
  void slot_remove_connector();

  // app logs
  void log_message(std::string msg);
  void log_warning(std::string msg);
  void log_error(std::string msg);

 signals:

  void signal_first_connect(connector_info);
  void signal_first_login(login_info);
  void signal_first_create_user(create_user_info);

  void signal_connector_state_change(bool);

  void signal_create_resource_tree();
  void signal_create_commits_list();
  void signal_create_developers_list();
  void signal_create_logs_list();

  void signal_create_resource_info(int id);
  void signal_create_commit_info(int id);

  void signal_add_new_resource(resource_create_info_type);
  void signal_update_resource(resource_update_info_type);
  void signal_download_resource(resource_download_info);
  void signal_delete_resource(resource_delete_info);


  void signal_request_connector_settings(int);
  void signal_create_connector(connector_info);

 private:
  Ui::MainWindow *ui;

  std::map<QTreeWidgetItem *, int> resource_tree_ids;
  std::map<QTreeWidgetItem *, int> commits_ids;
  std::map<QPushButton *, int> connections_bar_ids;
  std::map<QPushButton *, int> connections_settings_ids;
};
#endif  // MAINWINDOW_H
