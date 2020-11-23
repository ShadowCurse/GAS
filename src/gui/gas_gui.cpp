#include "gas_gui.hpp"

#include <QCheckBox>
#include <QFileSystemModel>
#include <QLabel>

#include "./ui_mainwindow.h"

GasGui::GasGui(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->resource_tree->setColumnCount(1);
  ui->commit_list->setColumnCount(2);
  ui->logs_list->setColumnCount(2);

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

  connect(ui->add_resource_button, &QPushButton::clicked, this,
          &GasGui::slot_add_new_resource);
  connect(ui->update_resource_button, &QPushButton::clicked, this,
          &GasGui::slot_update_resource);
  connect(ui->download_resource_button, &QPushButton::clicked, this,
          &GasGui::slot_download_resource);
  connect(ui->delete_resource_button, &QPushButton::clicked, this,
          &GasGui::slot_delete_resource);

  // controls for showing resources, commits, logs ...
  connect(ui->resource_tree, &QTreeWidget::itemClicked, this,
          &GasGui::slot_resource_click);
  connect(ui->commit_list, &QTreeWidget::itemClicked, this,
          &GasGui::slot_commit_click);

    //settings
  connect(ui->add_connection_button, &QPushButton::clicked, this,
          &GasGui::prepare_for_new_connection_settings);
}

GasGui::~GasGui() { delete ui; }

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

void GasGui::slot_draw_developers_list(GasGui::developers_list_type devs_list)
{
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

void GasGui::slot_draw_resource_info(GasGui::resourece_info_type info) {
  auto create_widget = [this](const auto& name, const auto& value) {
    auto box = new QGroupBox(ui->resource_info);
    auto layout = new QHBoxLayout(box);
    layout->addWidget(new QLabel(name, box));
    layout->addWidget(new QLabel(value, box));
    box->setLayout(layout);
    return box;
  };
  auto info_layout = ui->resource_info->layout();
  if (info_layout) delete info_layout;
  info_layout = new QVBoxLayout(ui->resource_info);
  for (const auto& [name, value] : info)
    info_layout->addWidget(create_widget(name, value));
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

void GasGui::slot_draw_developer_info(GasGui::developer_info_type info)
{
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

void GasGui::slot_resource_click(QTreeWidgetItem* item) {
  if (auto result = resource_tree_ids.find(item);
      result != std::end(resource_tree_ids))
    emit signal_create_resource_info(result->second);
}

void GasGui::slot_commit_click(QTreeWidgetItem* item) {
  if (auto result = commits_ids.find(item); result != std::end(commits_ids))
      emit signal_create_commit_info(result->second);
}

void GasGui::prepare_for_new_connection_settings()
{
    ui->Connectuin_name_value->clear();
    ui->Host_value->clear();
    ui->Port_value->clear();
    ui->DB_name_value->clear();
    ui->Username_value->clear();
    ui->Password_value->clear();
    ui->Connection_Action_button->setText("Add");
}
