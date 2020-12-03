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
  auto get_storage_settings(StorageUnit::storage_id id) -> std::optional<Settings> {
    if (auto storage_unit = storage_.get_storage(id))
      return (*storage_unit)->get_settings();
    return std::nullopt;
  }
  template <typename T>
  auto create_view() {
    return storage_.create_view<T>();
  }
  template <typename T>
  auto create_view(StorageUnit::storage_id id) {
    return storage_.create_view<T>(id);
  }
  auto update() -> void { storage_.update(); }

  template <typename T>
  auto insert(StorageUnit::storage_id storage_id, const T& data) {
    if (auto storage_unit = storage_.get_storage(storage_id)) {
      (*storage_unit)->insert(data);
    }
  }
  template <typename T>
  auto update(StorageUnit::storage_id storage_id, const T& data) {
    if (auto storage_unit = storage_.get_storage(storage_id)) {
      (*storage_unit)->update(data);
    }
  }
  template <typename T>
  auto remove(StorageUnit::storage_id storage_id, const T& data) {
    if (auto storage_unit = storage_.get_storage(storage_id)) {
      (*storage_unit)->remove(data);
    }
  }

  template <typename T>
  auto search(StorageUnit::storage_id storage_id,
              StorageCache::search_fun<T> fun) -> std::optional<T> {
    if (auto storage_unit = storage_.get_storage(storage_id)) {
      return (*storage_unit)->search<T>(fun);
    }
    return std::nullopt;
  }
  template <typename T>
  auto search_all(StorageUnit::storage_id storage_id,
              StorageCache::search_fun<T> fun) -> std::vector<T> {
    if (auto storage_unit = storage_.get_storage(storage_id)) {
      return (*storage_unit)->search_all<T>(fun);
    }
    return {};
  }

  auto insert_user(StorageUnit::storage_id storage_id, std::string_view username, std::string_view password,
                   std::string_view email, std::string_view description) -> bool {
    if (auto storage_unit = storage_.get_storage(storage_id)) {
      return (*storage_unit)
          ->insert_user(username, password, email, description);
    }
    return false;
  }

  auto search_user(StorageUnit::storage_id storage_id,
                   std::string_view username, std::string_view password) {
    if (auto storage_unit = storage_.get_storage(storage_id)) {
      return (*storage_unit)->search_user(username, password);
    }
    return false;
  }

  auto create_resource(StorageUnit::storage_id storage_id, Resource& resource,
                       std::string_view file_path) {
    if (auto storage_unit = storage_.get_storage(storage_id)) {
      (*storage_unit)->create_resource(resource, file_path);
    }
  }
  auto remove_resource(StorageUnit::storage_id storage_id,
                       const Resource& resource) {
    if (auto storage_unit = storage_.get_storage(storage_id)) {
      (*storage_unit)->remove_resource(resource);
    }
  }
  auto update_resource(StorageUnit::storage_id storage_id, Resource& resource,
                       std::string_view file_path) {
    if (auto storage_unit = storage_.get_storage(storage_id)) {
      (*storage_unit)->update_resource(resource, file_path);
    }
  }
  auto download_resource(StorageUnit::storage_id storage_id,
                         const Resource& resource, std::string_view file_path) {
    if (auto storage_unit = storage_.get_storage(storage_id)) {
      (*storage_unit)->download_resource(resource, file_path);
    }
  }

 private:
  Storage storage_;
};

}  // namespace gas

#endif  // GAS_SRC_CORE_CORE_HPP_
