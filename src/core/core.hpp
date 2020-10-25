#ifndef GAS_SRC_CORE_CORE_HPP_
#define GAS_SRC_CORE_CORE_HPP_

#include "database/db_connector.hpp"

namespace gas {

#define ADD_DELETE(name) \
  auto add_##name() -> bool; \
  auto delete_##name() -> bool;

class Core {
  Core() = default;

  auto set_connection_settings(Settings settings) -> void;
  auto connect() -> bool;

  ADD_DELETE(project)
  ADD_DELETE(developer)
  ADD_DELETE(developer_info)
  ADD_DELETE(role)
  ADD_DELETE(commit)
  ADD_DELETE(resource_type)
  ADD_DELETE(dependency)
  ADD_DELETE(resource)

  auto get_resource_tree();
  auto get_commit_history();
  auto get_commit_history(std::string_view resource);
  auto upload_resource(std::string_view name);
  auto update_resource_info();
  auto commit_resource();

  auto on_commit_update();
  auto on_resource_update();

 private:
  Connector connector_;
};

} // namespace gas

#endif //GAS_SRC_CORE_CORE_HPP_
