#ifndef GAS_SRC_CORE_STORAGE_HPP_
#define GAS_SRC_CORE_STORAGE_HPP_

#include <filesystem>

#include "cache.hpp"
#include "connector.hpp"
#include "tables.hpp"

namespace gas {

using StorageCache =
    StorageCache_T<Log, User, ResourceType, Resource, Dependency, Commit>;

template <typename... Types>
class StorageUnit_T {
 public:
  using storage_id = uint32_t;

 public:
  StorageUnit_T(storage_id id, Settings settings)
      : id_(id), connector_(std::move(settings)) {}

  [[nodiscard]] auto connect() -> bool {
    connector_.add_notifier("gas_channel", [this](const std::string& payload) {
      notification_callback(payload);
    });
    connector_.enable_notifications();
    return connector_.connect() && connector_.notifications_enabled();
  }
  auto disconnect() -> void { connector_.disconnect(); }
  [[nodiscard]] auto connected() const -> bool {
    return connector_.connected();
  }
  auto get_settings() const { return connector_.get_settings(); }

  template <typename T>
  [[nodiscard]] auto create_view() {
    return cache_.get_cache_view<T>();
  }
  [[nodiscard]] constexpr inline auto id() const { return id_; }

  template <typename T>
  auto update() -> bool {
    if (connector_.connected()) {
      if (auto result = connector_.exec(T::select_all())) {
        cache_.update<T>(result->template cast<T>());
        return true;
      }
    }
    return false;
  }
  auto update_all() -> bool {
    if (connector_.connected()) {
      return (update<Types>() && ...);
    }
    return false;
  }

  template <typename T>
  auto insert(const T& data) -> bool {
    if (connector_.connected()) {
      cache_.wait_for_update_state<T>();
      return connector_.exec(data.insert_query()).has_value();
    }
    return false;
  }
  template <typename T>
  auto update(const T& data) -> bool {
    if (connector_.connected()) {
      cache_.wait_for_update_state<T>();
      return connector_.exec(data.update_query()).has_value();
    }
    return false;
  }
  template <typename T>
  auto remove(const T& data) -> bool {
    if (connector_.connected()) {
      cache_.wait_for_update_state<T>();
      return connector_.exec(data.remove_query()).has_value();
    }
    return false;
  }

  template <typename T>
  auto search(StorageCache::search_fun<T> fun) -> std::optional<T> {
    return cache_.search<T>(fun);
  }

  template <typename T>
  auto search_all(StorageCache::search_fun<T> fun) -> std::vector<T> {
    return cache_.search_all<T>(fun);
  }

  auto insert_user(std::string_view username, std::string_view password,
                  std::string_view email, std::string_view description)
      -> bool {
    if (connector_.exec(User::insert(username, password, email, description)))
      return true;
    return false;
  }

  auto search_user(std::string_view username, std::string_view password)
      -> bool {
    if (auto result = connector_.exec(User::search(username, password)))
        if ((*result).size() != 0) return true;
    return false;
  }

  auto create_resource(Resource& resource, std::string_view file_path) -> bool {
    if (not std::filesystem::exists(file_path)) return false;
    auto size = std::filesystem::file_size(file_path);
    auto lo_oid = connector_.upload_large_object(file_path);
    resource.data = lo_oid;
    resource.size = size;
    return insert(resource);
  }
  auto remove_resource(const Resource& resource) -> bool {
    connector_.remove_lo(resource.data);
    return remove(resource);
  }
  auto update_resource(Resource& resource, std::string_view file_path) -> bool {
    if (not std::filesystem::exists(file_path)) return false;
    connector_.remove_lo(resource.data);
    auto size = std::filesystem::file_size(file_path);
    auto lo_oid = connector_.upload_large_object(file_path);
    resource.data = lo_oid;
    resource.size = size;
    return update(resource);
  }
  auto download_resource(const Resource& resource, std::string_view file_path) {
    connector_.download_large_object(resource.data, file_path);
  }

 private:
  auto notification_callback(const std::string& payload) -> void {
    if (callback_map.contains(payload)) callback_map[payload]();
  }

  storage_id id_;
  Connector connector_;
  StorageCache cache_;

  std::map<std::string_view, std::function<void()>> callback_map = {
      {"log", [this] { update<Log>(); }},
      {"developer", [this] { update<User>(); }},
      {"resourcetype", [this] { update<ResourceType>(); }},
      {"resource", [this] { update<Resource>(); }},
      {"dependency", [this] { update<Dependency>(); }},
      {"commit", [this] { update<Commit>(); }},
  };
};

using StorageUnit =
    StorageUnit_T<Log, User, ResourceType, Resource, Dependency, Commit>;

template <typename T>
class View {
 private:
  class Iterator {
   public:
    using type = T;
    using data_type = CacheView<type>;
    using data_iterator = typename data_type::iterator;
    using view = std::map<StorageUnit::storage_id, data_type>;
    using view_iterator = typename view::iterator;
    friend class View<T>;

   public:
    explicit Iterator(view* views, view_iterator view_iterator,
                      data_iterator data_iterator)
        : views_{views},
          current_view_iterator_{view_iterator},
          current_data_iterator_{data_iterator} {}

    static auto create_begin(view* views) {
      auto views_iter = std::begin(*views);
      while (views_iter != std::end(*views) && std::empty((*views_iter).second)) {
        ++views_iter;
      }
      data_iterator di{};
      if (views_iter != std::end(*views)) {
        di = std::begin((*views_iter).second);
      }
      return Iterator{views, views_iter, di};
    }

    static auto create_end(view* views) {
      return Iterator{views, std::end(*views), {}};
    }

    friend auto operator==(const Iterator& first, const  Iterator& second) -> bool {
      return first.current_view_iterator_ == second.current_view_iterator_;
    }
    friend auto operator!=(const Iterator& first, const Iterator& second) -> bool {
      return !(first == second);
    }

    auto operator++() -> Iterator& {
      if (current_view_iterator_ != std::end(*views_)) {
        if (current_data_iterator_ !=
            std::prev(std::end((*current_view_iterator_).second))) {
          ++current_data_iterator_;
        } else {
          while (++current_view_iterator_ != std::end(*views_) &&
                 std::empty((*current_view_iterator_).second)) {
          }
          if (current_view_iterator_ != std::end(*views_)) {
            current_data_iterator_ =
                std::begin((*current_view_iterator_).second);
          }
        }
      }
      return *this;
    }

    auto operator++(int) -> Iterator {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    auto operator*() const -> std::pair<StorageUnit::storage_id, T&> {
      return {(*current_view_iterator_).first, *current_data_iterator_};
    }

   private:
    data_iterator current_data_iterator_{};
    view_iterator current_view_iterator_{};
    view* views_{nullptr};
  };

 public:
  explicit View(const std::map<StorageUnit::storage_id,
                               std::shared_ptr<StorageUnit>>& storages) {
    for (const auto& [id, storage] : storages) {
      views_.insert({id, storage->create_view<T>()});
    }
  }

  // TODO make const
  [[nodiscard]] constexpr auto begin() {
    return Iterator::create_begin(&views_);
  }
  [[nodiscard]] constexpr auto end() { return Iterator::create_end(&views_); }
  [[nodiscard]] constexpr auto size() const {
    return std::accumulate(
        std::begin(views_), std::end(views_), 0,
        [](const auto& size, const auto& view) { return size + view.second.size(); });
  }

 private:
  std::map<StorageUnit::storage_id, CacheView<T>> views_;
};

class Storage {
 public:
  Storage() = default;

  [[nodiscard]] auto add_storage(Settings settings) -> StorageUnit::storage_id {
    ++id_counter;
    storages_.insert({id_counter, std::make_shared<StorageUnit>(
                                      id_counter, std::move(settings))});
    return id_counter;
  }
  auto remove_storage(StorageUnit::storage_id id) -> void {
    storages_.erase(id);
  }
  [[nodiscard]] auto connect_storage(StorageUnit::storage_id id) -> bool {
    if (storages_.contains(id) && storages_[id]->connect()) {
      storages_[id]->update_all();
      return true;
    }
    return false;
  }
  auto disconnect_storage(StorageUnit::storage_id id) -> void {
    if (storages_.contains(id)) storages_[id]->disconnect();
  }
  auto get_storage(StorageUnit::storage_id id)
      -> std::optional<std::shared_ptr<StorageUnit>> {
    if (storages_.contains(id)) return storages_[id];
    return std::nullopt;
  }
  template <typename T>
  [[nodiscard]] auto create_view() const {
    for (const auto& [id, storage] : storages_) storage->update<T>();
    return View<T>{storages_};
  };
  template <typename T>
  [[nodiscard]] auto create_view(StorageUnit::storage_id id) {
    if (storages_.contains(id)) return storages_[id]->create_view<T>();
  };

  auto update() -> void {
    for (const auto& [id, storage] : storages_) storage->update_all();
  }

 private:
  StorageUnit::storage_id id_counter{0};
  std::map<StorageUnit::storage_id, std::shared_ptr<StorageUnit>> storages_;
};

}  // namespace gas

#endif  // GAS_SRC_CORE_STORAGE_HPP_
