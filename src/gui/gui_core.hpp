#ifndef RESOURCE_TREE_H
#define RESOURCE_TREE_H

#include <QObject>

#include "gas_gui.hpp"

class GuiCore final : public QObject {
  Q_OBJECT
 public:
  GuiCore() : QObject() {
    // create resourece tree
//    connect(&gui, &GasGui::signal_create_resource_tree, this,
//            &GuiCore::slot_create_resource_tree);
//    connect(this, &GuiCore::signal_draw_resource_tree, &gui,
//            &GasGui::slot_draw_resource_tree);
//    // get resource info
//    connect(&gui, &GasGui::signal_create_resource_info, this,
//            &GuiCore::slot_create_resource_info);
//    connect(this, &GuiCore::signal_draw_resource_info, &gui,
//            &GasGui::slot_draw_resource_info);
//    // create commits list
//    connect(&gui, &GasGui::signal_create_commits_list, this,
//            &GuiCore::slot_create_commits_list);
//    connect(this, &GuiCore::signal_draw_commit_list, &gui,
//            &GasGui::slot_draw_commits_list);
//    // get commit info
//    connect(&gui, &GasGui::signal_create_commit_info, this,
//            &GuiCore::slot_create_commit_info);
//    connect(this, &GuiCore::signal_draw_commit_info, &gui,
//            &GasGui::slot_draw_commit_info);
  }

  ~GuiCore() final = default;

  auto show_gui() -> void { gui.show(); }

 public slots:

  void slot_add_storage() {}
  void slot_remove_storage() {}
  void slot_connect_storage() {}
  void slot_disconnect_storage() {}

  void slot_create_resource() {}
  void slot_update_resource() {}
  void slot_download_resource() {}
  void slot_remove_resource() {}

  void slot_create_resource_tree() {
    // if !connectors.empty() {
    //  auto resourece_type_view = View<ResourceType>();
    //  for (const auto& type: resource_type_view)
    //      tree.add_head_node(type);
    //  auto resourece_view = View<Resource>();
    //  for (const auto& res: resource_view)
    //      tree[res.type].add_node(res);
    //}

    std::vector<std::pair<QString, std::vector<std::pair<int, QString>>>> tree =
        {{"Textures", {{1, "Texture1"}, {2, "Texture2"}}},
         {"Meshes", {{3, "M1"}, {4, "M2"}}}};
    emit signal_draw_resource_tree(std::move(tree));
  }
  void slot_create_commits_list() {
    emit signal_draw_commit_list(
        {{1, {"name", "Pog"}}, {2, {"description", "pog texture"}}});
  }
  void slot_create_logs_list() {}

  // called when user clicks on particularr resource
  // presents resource info on scrollable widget and fills commits and
  // dependencies info in tabwidget
  void slot_create_resource_info(int id) {
    if (id == 1)
      emit signal_draw_resource_info(
          {{"name", "Pog"}, {"description", "pog texture"}});
    else if (id == 2)
      emit signal_draw_resource_info(
          {{"name", "Pog2"}, {"description", "pog texture2"}});
  }

  void slot_create_commit_info(int id) {
    if (id == 1)
      emit signal_draw_commit_info(
          {{"name", "Pog"}, {"description", "pog texture"}});
    else if (id == 2)
      emit signal_draw_commit_info(
          {{"name", "Pog2"}, {"description", "pog texture2"}});
  }

 signals:

  void signal_draw_resource_tree(
      std::vector<std::pair<QString, std::vector<std::pair<int, QString>>>>
          resourece_tree);
  void signal_draw_resource_info(std::vector<std::pair<QString, QString>> info);

  void signal_draw_commit_list(std::vector<std::pair<int, std::pair<QString, QString>>> commit_list);
  void signal_draw_commit_info(std::vector<std::pair<QString, QString>> info);

 private:
  GasGui gui;
  // gas::Core core;
  // ...
};

#endif  // RESOURCE_TREE_H
