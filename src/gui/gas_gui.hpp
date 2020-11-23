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
  using resourece_tree_type =
      std::vector<std::pair<QString, std::vector<std::pair<int, QString>>>>;
  using commits_list_type =
      std::vector<std::pair<int, std::pair<QString, QString>>>;
  using developers_list_type =
      std::vector<std::pair<int, std::pair<QString, QString>>>;
  using logs_list_type = std::vector<std::pair<QString, QString>>;

  using resourece_info_type = std::vector<std::pair<QString, QString>>;
  using commit_info_type = std::vector<std::pair<QString, QString>>;
  using developer_info_type = std::vector<std::pair<QString, QString>>;

 public:
  GasGui(QWidget *parent = nullptr);
  ~GasGui();

 public slots:

  // tabs
  void slot_tab_click(int index);

  void slot_draw_resource_tree(resourece_tree_type resourece_tree);
  void slot_draw_commits_list(commits_list_type commit_list);
  void slot_draw_developers_list(developers_list_type devs_list);
  void slot_draw_logs_list(logs_list_type log_list);

  void slot_draw_resource_info(resourece_info_type info);
  void slot_draw_commit_info(commit_info_type info);
  void slot_draw_developer_info(developer_info_type info);

  // resource controls
  void slot_add_new_resource() {}
  void slot_update_resource() {}
  void slot_download_resource() {}
  void slot_delete_resource() {}

  // resource tree
  void slot_resource_click(QTreeWidgetItem *item);

  // commits
  void slot_commit_click(QTreeWidgetItem *item);

  // logs
  void slot_logs_click() {}

  // setttings
  void prepare_for_new_connection_settings();
  void get_current_settings() {}


 signals:

  void signal_create_resource_tree();
  void signal_create_commits_list();
  void signal_create_developers_list();
  void signal_create_logs_list();

  void signal_create_resource_info(int id);
  void signal_create_commit_info(int id);

  void signal_add_new_resource();
  void signal_update_resource();
  void signal_download_resourece();
  void signal_delete_resource();

 private:
  Ui::MainWindow *ui;

  std::map<QTreeWidgetItem *, int> resource_tree_ids;
  std::map<QTreeWidgetItem *, int> commits_ids;
};
#endif  // MAINWINDOW_H
