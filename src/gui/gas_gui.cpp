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

  // login page
  connect(ui->first_connection_button, &QPushButton::clicked, this,
          &GasGui::slot_starting_connect);
  connect(ui->add_new_user, &QPushButton::clicked,
          [&]() { ui->user_tabs->setCurrentIndex(1); });
  connect(ui->create_new_user_back, &QPushButton::clicked,
          [&]() { ui->user_tabs->setCurrentIndex(0); });
  connect(ui->create_new_user_button, &QPushButton::clicked, this,
          &GasGui::slot_starting_create_user);
  connect(ui->user_login, &QPushButton::clicked, this,
          &GasGui::slot_starting_login);
  connect(ui->change_user_button, &QPushButton::clicked,
          [&]() { ui->main_window_tabs->setCurrentIndex(1); });

  // logs panel
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
          &GasGui::slot_remove_resource);

  // controls for showing resources, commits, logs ...
  connect(ui->resource_tree, &QTreeWidget::itemClicked, this,
          &GasGui::slot_resource_click);
  connect(ui->commit_list, &QTreeWidget::itemClicked, this,
          &GasGui::slot_commit_click);
  connect(ui->users_list, &QTreeWidget::itemClicked, this,
          &GasGui::slot_user_click);

  // settings
  connect(ui->add_connection_button, &QPushButton::clicked, this,
          &GasGui::slot_prepare_for_new_connector_settings);
  connect(ui->connection_add_button, &QPushButton::clicked, this,
          &GasGui::slot_request_connector_creation);
  connect(ui->remove_connection_button, &QPushButton::clicked, this,
          &GasGui::slot_request_connector_remove);
}

GasGui::~GasGui() { delete ui; }

// public slots

void GasGui::slot_starting_connection_state_change(bool status) {
  ui->first_connection_status->setChecked(status);
  if (!status) log_error("Could not connect to database");
}

void GasGui::slot_accept_user() {
  ui->main_window_tabs->setCurrentIndex(0);
  ui->tabWidget->setCurrentIndex(0);
}

void GasGui::slot_deny_user() {
  emit log_error("invalid user");
}

void GasGui::slot_draw_resource_tree(
    GasGui::resource_tree_type resourece_tree) {
  ui->resource_tree->clear();
  resource_tree_ids.clear();
  for (const auto& [head_name, children] : resourece_tree) {
    auto head = new QTreeWidgetItem(ui->resource_tree,
                                    {QString::fromStdString(head_name)});
    for (const auto& [id, item_name] : children) {
      auto child =
          new QTreeWidgetItem(head, {QString::fromStdString(item_name)});
      resource_tree_ids.insert({child, id});
    }
  }
}

void GasGui::slot_draw_commits_list(GasGui::commits_list_type commit_list) {
  ui->commit_list->clear();
  commits_ids.clear();
  for (const auto& [id, date, message] : commit_list) {
    auto child = new QTreeWidgetItem(
        ui->commit_list,
        {QString::fromStdString(date), QString::fromStdString(message)});
    commits_ids.insert({child, id});
  }
}

void GasGui::slot_draw_users_list(GasGui::users_list_type users_list) {
  ui->users_list->clear();
  for (const auto& user : users_list) {
    auto child = new QTreeWidgetItem(ui->users_list,
                                     {QString::fromStdString(user.name)});
    users_ids.insert({child, user.id});
  }
}

void GasGui::slot_draw_logs_list(GasGui::logs_list_type log_list) {
  ui->logs_list->clear();
  for (const auto& [date, message] : log_list) {
    new QTreeWidgetItem(ui->logs_list, {QString::fromStdString(date),
                                        QString::fromStdString(message)});
  }
}

void GasGui::slot_draw_resource_info(GasGui::resource_info_type info) {
  ui->resource_name_value->setText(QString::fromStdString(info.name));
  ui->resource_description_value->setText(
      QString::fromStdString(info.description));
  ui->resource_size_value->setValue(info.size);
  ui->resource_type_value->setText(QString::fromStdString(info.type));

  ui->update_resource_name_vaue->setText(QString::fromStdString(info.name));
  ui->update_resource_description_value->setText(
      QString::fromStdString(info.description));
  ui->update_resource_type_value->setText(QString::fromStdString(info.type));

  ui->resource_commits->clear();
  for (const auto& commit : info.commits) {
    new QTreeWidgetItem(ui->resource_commits,
                        {QString::fromStdString(commit.date),
                         QString::fromStdString(commit.message)});
  }
  ui->resource_dependency->clear();
  for (const auto& dependency : info.dependencies) {
    new QListWidgetItem(QString::fromStdString(dependency),
                        ui->resource_dependency);
  }
}

void GasGui::slot_draw_commit_info(GasGui::commit_info_type info) {
  ui->commit_username_value->setText(QString::fromStdString(info.username));
  ui->commit_resource_name_value->setText(
      QString::fromStdString(info.resource_name));
  ui->commit_data_value->setText(QString::fromStdString(info.date));
  ui->commit_message_value->setText(QString::fromStdString(info.message));
}

void GasGui::slot_draw_users_info(GasGui::user_info_type info) {
  ui->users_name_value->setText(QString::fromStdString(info.name));
  ui->users_email_value->setText(QString::fromStdString(info.email));
  ui->users_descrription_value->setText(
      QString::fromStdString(info.description));
}

void GasGui::slot_show_connector_settings(connector_info info) {
  ui->connection_tabs->setCurrentIndex(1);
  ui->info_connection_host_value->setText(QString::fromStdString(info.host));
  ui->info_connection_port_value->setValue(info.port);
  ui->info_connection_db_name_value->setText(
      QString::fromStdString(info.db_name));
  ui->info_connection_username_value->setText(
      QString::fromStdString(info.username));
}

void GasGui::slot_create_connector(connector_info info) {
  auto name = QString::fromStdString(info.name);
  auto bar_button = new QPushButton(name, ui->storages_box);
  bar_button->setCheckable(true);
  bar_button->setChecked(true);
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

void GasGui::slot_remove_connector(id_type id) {
  auto bar_widget = std::find_if(
      std::begin(connections_bar_ids), std::end(connections_bar_ids),
      [&](const auto& pair) { return pair.second == id; });
  auto settings_widget = std::find_if(
      std::begin(connections_settings_ids), std::end(connections_settings_ids),
      [&](const auto& pair) { return pair.second == id; });
  if (bar_widget == std::end(connections_bar_ids) ||
      settings_widget == std::end(connections_settings_ids)) {
    log_error("trying to delete connector that does not exists: " +
              std::to_string(id));
  }
  ui->storages_bar->layout()->removeWidget(bar_widget->first);
  delete bar_widget->first;
  ui->connections->layout()->removeWidget(settings_widget->first);
  delete settings_widget->first;
  connections_bar_ids.erase(bar_widget);
  connections_settings_ids.erase(settings_widget);
}

void GasGui::log_message(std::string msg) {
  ui->app_logs->appendPlainText(
      QString("[message]: %1\n").arg(QString::fromStdString(msg)));
}

void GasGui::log_warning(std::string msg) {
  ui->app_logs->appendHtml(
      QString("<font color = #ffec17>[warning]: %1</font>\n")
          .arg(QString::fromStdString(msg)));
}

void GasGui::log_error(std::string msg) {
  ui->app_logs->appendHtml(QString("<font color = red>[error]: %1</font>\n")
                               .arg(QString::fromStdString(msg)));
}

// private slots

void GasGui::slot_starting_connect() {
  connector_info info;
  info.name = ui->first_connectuin_name_value->text().toStdString();
  info.host = ui->first_host_value->text().toStdString();
  info.port = ui->first_port_value->value();
  info.db_name = ui->first_db_name_value->text().toStdString();
  info.username = ui->first_username_value->text().toStdString();
  info.password = ui->first_password_value->text().toStdString();
  emit signal_first_connect(std::move(info));
}

void GasGui::slot_starting_login() {
  if (!ui->first_connection_status->isChecked()) {
    log_warning("can not login before connecting to database");
    return;
  }
  login_info info;
  info.username = ui->login_user_name_value->text().toStdString();
  info.password = ui->login_password_value->text().toStdString();
  emit signal_first_login(std::move(info));
}

void GasGui::slot_starting_create_user() {
  if (!ui->first_connection_status->isChecked()) {
    log_warning("can not create user before connecting to database");
    return;
  }
  create_user_info info;
  info.username = ui->user_name_value->text().toStdString();
  info.password = ui->user_password_value->text().toStdString();
  info.email = ui->user_email_value->text().toStdString();
  info.description = ui->user_description_value->text().toStdString();
  emit signal_first_create_user(std::move(info));
}

void GasGui::slot_connector_state_change() {
  auto caller = dynamic_cast<QPushButton*>(sender());
  auto id = connections_settings_ids[caller];
  emit signal_connector_state_change(id, caller->isChecked());
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
      emit signal_create_users_list();
      break;
    }
    case 3: {
      emit signal_create_logs_list();
      break;
    }
    default: {
      break;
    }
  }
}

void GasGui::slot_add_new_resource() {
  resource_create_type info;
  for (const auto& pair : connections_bar_ids) {
    if (pair.first->text() == ui->add_resource_connector_name_value->text())
      info.connector_id = pair.second;
  }
  info.name = ui->add_resource_name_value->text().toStdString();
  info.description = ui->add_resource_description_value->text().toStdString();
  info.file_path = ui->add_resource_file_value->text().toStdString();
  info.type = ui->add_resource_type_value->text().toStdString();
  emit signal_add_new_resource(std::move(info));
}

void GasGui::slot_update_resource() {
  if (auto curr_resource = ui->resource_tree->currentItem()) {
    resource_update_type info;
    info.id = (resource_tree_ids[curr_resource]);
    info.name = ui->update_resource_name_vaue->text().toStdString();
    info.description =
        ui->update_resource_description_value->text().toStdString();
    info.file_path = ui->update_resource_file_value->text().toStdString();
    info.type = ui->update_resource_type_value->text().toStdString();
    info.commit_message = ui->update_resource_commit_message_value->text().toStdString();
    emit signal_update_resource(std::move(info));
  }
}

void GasGui::slot_download_resource() {
  if (auto curr_resource = ui->resource_tree->currentItem())
    emit signal_download_resource(
        {resource_tree_ids[curr_resource],
         ui->download_resource_path_value->text().toStdString()});
}

void GasGui::slot_remove_resource() {
  if (auto curr_resource = ui->resource_tree->currentItem())
    emit signal_remove_resource(resource_tree_ids[curr_resource]);
}

void GasGui::slot_resource_click(QTreeWidgetItem* item) {
  if (auto result = resource_tree_ids.find(item);
      result != std::end(resource_tree_ids))
    emit signal_get_resource_info(result->second);
}

void GasGui::slot_commit_click(QTreeWidgetItem* item) {
  if (auto result = commits_ids.find(item); result != std::end(commits_ids))
    emit signal_get_commit_info(result->second);
}

void GasGui::slot_user_click(QTreeWidgetItem* item) {
  if (auto result = users_ids.find(item); result != std::end(users_ids))
    emit signal_get_user_info(result->second);
}

void GasGui::slot_prepare_for_new_connector_settings() {
  for (const auto& connector : connections_settings_ids)
    connector.first->setChecked(false);
  ui->connection_tabs->setCurrentIndex(0);
  ui->connection_name_value->clear();
  ui->host_value->clear();
  ui->port_value->clear();
  ui->db_name_value->clear();
  ui->username_value->clear();
  ui->password_value->clear();
}

void GasGui::slot_request_connector_settings() {
  auto caller = dynamic_cast<QPushButton*>(sender());
  if (!caller->isChecked()) {
    caller->setChecked(true);
    return;
  }
  for (const auto& setting_button : connections_settings_ids)
    if (setting_button.first != caller) setting_button.first->setChecked(false);
  ui->connection_tabs->setCurrentIndex(1);
  emit signal_request_connector_settings(connections_settings_ids[caller]);
}

void GasGui::slot_request_connector_creation() {
  auto connector_name = ui->connection_name_value->text();
  for (const auto& pair: connections_bar_ids) {
    if (pair.first->text() == connector_name)
      log_error("connector with such name already exists");
  }
  connector_info info;
  info.name = connector_name.toStdString();
  info.host = ui->host_value->text().toStdString();
  info.port = ui->port_value->value();
  info.db_name = ui->db_name_value->text().toStdString();
  info.username = ui->username_value->text().toStdString();
  info.password = ui->password_value->text().toStdString();
  emit signal_request_connector_create(std::move(info));
}

void GasGui::slot_request_connector_remove() {
  for (const auto& setting_button : connections_settings_ids)
    if (setting_button.first->isChecked()) {
      emit signal_request_connector_remove(connections_settings_ids[setting_button.first]);
    }
}
