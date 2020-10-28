#ifndef GAS_SRC_CORE_CORE_HPP_
#define GAS_SRC_CORE_CORE_HPP_

#include "database/db_connector.hpp"

namespace gas {

class Core {
 public:
  using callback_fn = Notifier::callback_fn;

 public:
  Core() = default;

  auto set_connection_settings(Settings settings) -> void;
  auto connect() -> bool;

  auto add_project() -> bool;
  auto get_project() -> bool;
  auto delete_project() -> bool;

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
