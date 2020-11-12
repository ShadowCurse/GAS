#ifndef GAS_SRC_CORE_CORE_HPP_
#define GAS_SRC_CORE_CORE_HPP_

#include "storage.hpp"

namespace gas {

class Core {
 public:
  Core() = default;

  auto add_storage(Settings settings) {
    return storage_.add_storage(std::move(settings));
  }
  auto remove_storage(StorageUnit::storage_id id) {
    storage_.remove_storage(id);
  }
  auto connect_storage(StorageUnit::storage_id id) {
    return storage_.connect_storage(id);
  }
  auto disconnect_storage(StorageUnit::storage_id id) {
    return storage_.disconnect_storage(id);
  }

  template <typename T>
  auto add_view() {
    return storage_.add_view<T>();
  }

  auto update_storage() -> void {
    storage_.update();
  }

 private:
  Storage storage_;
};

}  // namespace gas

#endif  // GAS_SRC_CORE_CORE_HPP_
