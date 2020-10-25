#ifndef GAMEASSETSDB_SRC_CORE_CORE_HPP_
#define GAMEASSETSDB_SRC_CORE_CORE_HPP_

class core {

  auto get_resource_tree();
  auto get_commit_history();
  auto upload_resource();
  auto update_resource();
  auto commit_resource();
  auto on_commit_update();
  auto on_resource_update();

};

#endif //GAMEASSETSDB_SRC_CORE_CORE_HPP_
