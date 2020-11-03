#include "core.hpp"

#include <utility>

namespace gas {

auto Core::set_connection_settings(Settings settings) -> void {
  connector_->set_settings(std::move(settings));
}
auto Core::connect() -> bool { return connector_->connect(); }

}  // namespace gas
