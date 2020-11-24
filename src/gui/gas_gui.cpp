#include "gas_gui.hpp"

#include <QCheckBox>
#include <QFileSystemModel>
#include <QLabel>

#include "./ui_mainwindow.h"

GasGui::GasGui(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->resource_tree->setColumnCount(1);
  ui->resource_commits->setColumnCount(2);
  ui->commit_list->setColumnCount(2);
  ui->logs_list->setColumnCount(2);

  //  auto new_button = new QPushButton("some text", ui->connections);
  //    new_button->setCheckable(true);
  //  ui->connections->layout()->addWidget(new_button);

  // login page
  connect(ui->add_new_user, &QPushButton::clicked,
          [&]() { ui->user_tabs->setCurrentIndex(1); });
  connect(ui->create_new_user_back, &QPushButton::clicked,
          [&]() { ui->user_tabs->setCurrentIndex(0); });
  connect(ui->user_login, &QPushButton::clicked,
          [&]() { ui->main_window_tabs->setCurrentIndex(0); });
  connect(ui->change_user_button, &QPushButton::clicked,
          [&]() { ui->main_window_tabs->setCurrentIndex(1); });

  // logs panel
  // hiding at startup
  ui->app_logs->hide();
  connect(ui->show_logs_button, &QPushButton::clicked, [&](bool checked) {
    checked ? ui->app_logs->show() : ui->app_logs->hide();
  });

  // tabs
  connect(ui->tabWidget, &QTabWidget::tabBarClicked, this,
          &GasGui::slot_tab_click);

  // controls for resource tab
  connect(ui->add_resource_button, &QPushButton::clicked,
          [&]() { ui->stackedWidget->setCurrentIndex(1); });
  connect(ui->update_resource_button, &QPushButton::clicked,
          [&]() { ui->stackedWidget->setCurrentIndex(2); });
  connect(ui->download_resource_button, &QPushButton::clicked,
          [&]() { ui->stackedWidget->setCurrentIndex(3); });

  connect(ui->add_resource_page_back, &QPushButton::clicked,
          [&]() { ui->stackedWidget->setCurrentIndex(0); });
  connect(ui->update_resource_page_back, &QPushButton::clicked,
          [&]() { ui->stackedWidget->setCurrentIndex(0); });
  connect(ui->download_resource_page_back, &QPushButton::clicked,
          [&]() { ui->stackedWidget->setCurrentIndex(0); });

  connect(ui->add_resource_page_button, &QPushButton::clicked, this,
          &GasGui::slot_add_new_resource);
  connect(ui->update_resource_page_button, &QPushButton::clicked, this,
          &GasGui::slot_update_resource);
  connect(ui->download_resource_page_button, &QPushButton::clicked, this,
          &GasGui::slot_download_resource);
  connect(ui->delete_resource_button, &QPushButton::clicked, this,
          &GasGui::slot_delete_resource);

  // controls for showing resources, commits, logs ...
  connect(ui->resource_tree, &QTreeWidget::itemClicked, this,
          &GasGui::slot_resource_click);
  connect(ui->commit_list, &QTreeWidget::itemClicked, this,
          &GasGui::slot_commit_click);

  // settings
  connect(ui->add_connection_button, &QPushButton::clicked, this,
          &GasGui::slot_prepare_for_new_connector_settings);
  connect(ui->connection_add_button, &QPushButton::clicked, this,
          &GasGui::slot_submit_connector_creation);

  connector_info info;
  info.name = "new connector";
  slot_create_connector(info);
}

GasGui::~GasGui() { delete ui; }

void GasGui::slot_starting_connection_state_change(bool status)
{
   ui->first_connection_status->setChecked(status);
   if (!status)
       log_error("Could not connect to database");
}

void GasGui::slot_starting_connect()
{
    connector_info info;
    info.name = ui->first_connectuin_name_value->text().toStdString();
    info.host = ui->first_host_value->text().toStdString();
    info.port = ui->first_port_value->value();
    info.db_name = ui->first_db_name_value->text().toStdString();
    info.username = ui->first_username_value->text().toStdString();
    info.password = ui->first_password_value->text().toStdString();
    emit signal_first_connect(std::move(info));
}

void GasGui::slot_starting_login()
{
    if (!ui->first_connection_status->isChecked()) {
        log_warning("can not login before connecting to database");
        return;
    }
    emit signal_first_login(ui->user_label->text().toStdString());
}

void GasGui::slot_starting_create_user()
{
    if (!ui->first_connection_status->isChecked()) {
        log_warning("can not create user before connecting to database");
        return;
    }
    create_user_info info;
    info.name = ui->user_name_value->text().toStdString();
    info.email = ui->user_email_value->text().toStdString();
    info.description = ui->user_description_value->text().toStdString();
    emit signal_first_create_user(std::move(info));
}

void GasGui::slot_accept_user()
{
    ui->main_window_tabs->setCurrentIndex(0);
}

void GasGui::slot_connector_state_change() {
  auto caller = static_cast<QPushButton*>(sender());
  emit signal_connector_state_change(caller->isChecked());
}

void GasGui::slot_tab_click(int index) {
  switch (index) {
    case 0: {
      ui->stackedWidget->setCurrentIndex(0);
      emit signal_create_resource_tree();
      break;
    }
    case 1: {
      emit signal_create_commits_list();
      break;
    }
    case 2: {
      emit signal_create_developers_list();
      break;
    }
    case 3: {
      emit signal_create_logs_list();
      break;
    }
  }
}

void GasGui::slot_draw_resource_tree(
    GasGui::resourece_tree_type resourece_tree) {
  ui->resource_tree->clear();
  resource_tree_ids.clear();
  for (const auto& [head_name, children] : resourece_tree) {
    auto head = new QTreeWidgetItem(ui->resource_tree, {head_name});
    for (const auto& [id, item] : children) {
      auto child = new QTreeWidgetItem(head, {item});
      resource_tree_ids.insert({child, id});
    }
  }
}

void GasGui::slot_draw_commits_list(GasGui::commits_list_type commit_list) {
  ui->commit_list->clear();
  commits_ids.clear();
  for (const auto& [id, data] : commit_list) {
    const auto& [date, message] = data;
    auto child = new QTreeWidgetItem(ui->commit_list, {date, message});
    commits_ids.insert({child, id});
  }
}

void GasGui::slot_draw_developers_list(GasGui::developers_list_type devs_list) {
  ui->commit_list->clear();
  for (const auto& [id, data] : devs_list) {
    const auto& [date, message] = data;
    auto child = new QTreeWidgetItem(ui->developer_list, {date, message});
    commits_ids.insert({child, id});
  }
}

void GasGui::slot_draw_logs_list(GasGui::logs_list_type log_list) {
  ui->logs_list->clear();
  for (const auto& [date, descr] : log_list) {
    new QTreeWidgetItem(ui->logs_list, {date, descr});
  }
}

void GasGui::slot_draw_resource_info(GasGui::resource_info_type info) {
  ui->resource_name_value->setText(QString::fromStdString(info.name));
  ui->resource_description_value->setText(
      QString::fromStdString(info.description));
  ui->resource_size_value->setValue(info.size);
  ui->resource_type_value->setText(QString::fromStdString(info.type));

  for (const auto& commit : info.commits) {
    new QTreeWidgetItem(ui->resource_commits,
                        {QString::fromStdString(commit.date),
                         QString::fromStdString(commit.description)});
  }
  for (const auto& dependency : info.dependencies) {
    new QListWidgetItem(QString::fromStdString(dependency),
                        ui->resource_dependency);
  }
}

void GasGui::slot_draw_commit_info(GasGui::commit_info_type info) {
  auto create_widget = [this](const auto& name, const auto& value) {
    auto box = new QGroupBox(ui->commit_info);
    auto layout = new QHBoxLayout(box);
    layout->addWidget(new QLabel(name, box));
    layout->addWidget(new QLabel(value, box));
    box->setLayout(layout);
    return box;
  };
  auto info_layout = ui->commit_info->layout();
  if (info_layout) delete info_layout;
  info_layout = new QVBoxLayout(ui->commit_info);
  for (const auto& [name, value] : info)
    info_layout->addWidget(create_widget(name, value));
}

void GasGui::slot_draw_developer_info(GasGui::developer_info_type info) {
  auto create_widget = [this](const auto& name, const auto& value) {
    auto box = new QGroupBox(ui->developer_info);
    auto layout = new QHBoxLayout(box);
    layout->addWidget(new QLabel(name, box));
    layout->addWidget(new QLabel(value, box));
    box->setLayout(layout);
    return box;
  };
  auto info_layout = ui->developer_info->layout();
  if (info_layout) delete info_layout;
  info_layout = new QVBoxLayout(ui->developer_info);
  for (const auto& [name, value] : info)
    info_layout->addWidget(create_widget(name, value));
}

void GasGui::slot_add_new_resource() {
  resource_create_info_type info;
  info.name = ui->add_resource_name_value->text().toStdString();
  info.description = ui->add_resource_description_value->text().toStdString();
  info.file_path = ui->add_resource_file_value->text().toStdString();
  info.type = ui->add_resource_type_value->text().toStdString();
  emit signal_add_new_resource(std::move(info));
}

void GasGui::slot_update_resource() {
  if (auto curr_resource = ui->resource_tree->currentItem()) {
    resource_update_info_type info;
    info.id = (resource_tree_ids[curr_resource]);
    info.name = ui->update_resource_name_vaue->text().toStdString();
    info.description =
        ui->update_resource_description_value->text().toStdString();
    info.file_path = ui->update_resource_file_value->text().toStdString();
    info.type = ui->update_resource_type_value->text().toStdString();
    emit signal_update_resource(std::move(info));
  }
}

void GasGui::slot_download_resource() {
  emit signal_download_resource(
      ui->download_resource_path_value->text().toStdString());
}

void GasGui::slot_delete_resource() {
  if (auto curr_resource = ui->resource_tree->currentItem())
    emit signal_delete_resource(resource_tree_ids[curr_resource]);
}

void GasGui::slot_resource_click(QTreeWidgetItem* item) {
  if (auto result = resource_tree_ids.find(item);
      result != std::end(resource_tree_ids))
    emit signal_create_resource_info(result->second);
}

void GasGui::slot_commit_click(QTreeWidgetItem* item) {
  if (auto result = commits_ids.find(item); result != std::end(commits_ids))
    emit signal_create_commit_info(result->second);
}

void GasGui::slot_prepare_for_new_connector_settings() {
  ui->connection_tabs->setCurrentIndex(0);
  ui->connection_name_value->clear();
  ui->host_value->clear();
  ui->port_value->clear();
  ui->db_name_value->clear();
  ui->username_value->clear();
  ui->password_value->clear();
}

void GasGui::slot_request_connector_settings() {
  auto caller = static_cast<QPushButton*>(sender());
  for (const auto& setting_button : connections_settings_ids)
    if (setting_button.first != caller) setting_button.first->setChecked(false);
  ui->connection_tabs->setCurrentIndex(1);
  emit signal_request_connector_settings(
      connections_settings_ids[static_cast<QPushButton*>(sender())]);
}

void GasGui::slot_show_connector_settings(connector_info info) {
  ui->connection_tabs->setCurrentIndex(1);
  ui->info_connection_name_value->setText(QString::fromStdString(info.name));
  ui->info_connection_host_value->setText(QString::fromStdString(info.host));
  ui->info_connection_port_value->setValue(info.port);
  ui->info_connection_db_name_value->setText(
      QString::fromStdString(info.db_name));
  ui->info_connection_username_value->setText(
      QString::fromStdString(info.username));
}

void GasGui::slot_submit_connector_creation() {
  connector_info info;
  info.name = ui->connection_name_value->text().toStdString();
  info.host = ui->host_value->text().toStdString();
  info.port = ui->port_value->value();
  info.db_name = ui->db_name_value->text().toStdString();
  info.username = ui->username_value->text().toStdString();
  info.password = ui->password_value->text().toStdString();
  emit signal_create_connector(std::move(info));
}

void GasGui::slot_create_connector(connector_info info) {
  auto name = QString::fromStdString(info.name);
  auto bar_button = new QPushButton(name, ui->storages_box);
  bar_button->setCheckable(true);
  ui->storages_box->layout()->addWidget(bar_button);
  connections_bar_ids.insert({bar_button, info.id});
  connect(bar_button, &QPushButton::clicked, this,
          &GasGui::slot_connector_state_change);

  auto settings_button = new QPushButton(name, ui->connections);
  settings_button->setCheckable(true);
  ui->connections->layout()->addWidget(settings_button);
  connections_settings_ids.insert({settings_button, info.id});
  connect(settings_button, &QPushButton::clicked, this,
          &GasGui::slot_request_connector_settings);
}

void GasGui::slot_remove_connector() {}

void GasGui::log_message(std::string msg) {
  ui->app_logs->appendPlainText(
      QString("[message]: %1\n").arg(QString::fromStdString(msg)));
}

void GasGui::log_warning(std::string msg) {
  ui->app_logs->appendPlainText(
      QString("<font color = yellow>[warning]: %1</font>\n")
          .arg(QString::fromStdString(msg)));
}

void GasGui::log_error(std::string msg) {
  ui->app_logs->appendPlainText(
      QString("<font color = red>[error]: %1</font>\n")
          .arg(QString::fromStdString(msg)));
}
