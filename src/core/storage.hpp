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
  auto disconnect() -> void {
    connector_.disconnect();
  }
  [[nodiscard]] auto connected() const -> bool {
    return connector_.connected();
  }

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
  auto search(StorageCache::search_fun<T> fun) {
    return cache_.search<T>(fun);
  }

  auto create_resource(Resource& resource, std::string_view file_path) {
    auto size = std::filesystem::file_size(file_path);
    auto lo_oid = connector_.upload_large_object(file_path);
    resource.data = lo_oid;
    resource.size = size;
    insert(resource);
  }
  auto remove_resource(const Resource& resource) {
    connector_.remove_lo(resource.data);
    remove(resource);
  }
  auto upload_resource(Resource& resource, std::string_view file_path) {
    remove_resource(resource);
    auto size = std::filesystem::file_size(file_path);
    auto lo_oid = connector_.upload_large_object(file_path);
    resource.data = lo_oid;
    resource.size = size;
    update(resource);
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
    using view = std::vector<data_type>;
    using view_iterator = typename view::iterator;
    friend class View<T>;

   public:
    explicit Iterator(view* views, view_iterator view_iterator,
                      data_iterator data_iterator)
        : views_{views},
          current_view_iterator_{view_iterator},
          current_data_iterator_{data_iterator} {}

    static auto create_begin(view* views) {
      auto views_iter = views->begin();
      while (views_iter != views->end() && (*views_iter).empty()) {
        ++views_iter;
      }
      data_iterator di;
      if (views_iter != views->end()) {
        di = std::begin(*views_iter);
      }
      return Iterator{views, views_iter, di};
    }

    static auto create_end(view* views) {
      return Iterator{views, views->end(), {}};
    }

    auto operator==(Iterator& other) const -> bool {
      return current_view_iterator_ == other.current_view_iterator_;
    }
    auto operator!=(Iterator& other) -> bool { return !(*this == other); }

    auto operator++() -> Iterator& {
      if (current_view_iterator_ != std::end(*views_)) {
        if (current_data_iterator_ != std::end(*current_view_iterator_)) {
          ++current_data_iterator_;
        } else {
          while (++current_view_iterator_ != std::end(*views_) &&
                 current_view_iterator_->empty()) {
          }
          if (current_view_iterator_ != std::end(*views_)) {
            current_data_iterator_ = std::begin(*current_view_iterator_);
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

    auto operator*() const -> T& { return *current_data_iterator_; }

   private:
    data_iterator current_data_iterator_{};
    view_iterator current_view_iterator_{};
    view* views_{nullptr};
  };

 public:
  explicit View(const std::vector<std::shared_ptr<StorageUnit>>& storages) {
    for (const auto& storage : storages) {
      views_.emplace_back(storage->create_view<T>());
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
        [](const auto& size, const auto& view) { return size + view.size(); });
  }

 private:
  std::vector<CacheView<T>> views_;
};

class Storage {
 public:
  Storage() = default;

  [[nodiscard]] auto add_storage(Settings settings) -> StorageUnit::storage_id {
    ++id_counter;
    storages_.emplace_back(
        std::make_shared<StorageUnit>(id_counter, std::move(settings)));
    return id_counter;
  }
  auto remove_storage(StorageUnit::storage_id id) -> void {
    std::erase_if(storages_,
                  [&id](const auto& storage) { return storage->id(); });
  }
  [[nodiscard]] auto connect_storage(StorageUnit::storage_id id) -> bool {
    if (auto result = std::find_if(
            std::begin(storages_), std::end(storages_),
            [&id](const auto& storage) { return storage->id() == id; });
        result != std::end(storages_))
      return (*result)->connect();
    return false;
  }
  auto disconnect_storage(StorageUnit::storage_id id) -> void {
    if (auto result = std::find_if(
            std::begin(storages_), std::end(storages_),
            [&id](const auto& storage) { return storage->id() == id; });
        result != std::end(storages_))
      (*result)->disconnect();
  }
  auto get_storage(StorageUnit::storage_id id)
      -> std::optional<std::shared_ptr<StorageUnit>> {
    if (auto result = std::find_if(
            std::begin(storages_), std::end(storages_),
            [&id](const auto& storage) { return storage->id() == id; });
        result != std::end(storages_))
      return (*result);
    return std::nullopt;
  }
  template <typename T>
  [[nodiscard]] constexpr auto create_view() const {
    for (const auto& storage : storages_) storage->update<T>();
    return View<T>{storages_};
  };

  auto update() -> void {
    for (const auto& storage : storages_) storage->update_all();
  }

 private:
  StorageUnit::storage_id id_counter{0};
  std::vector<std::shared_ptr<StorageUnit>> storages_;
};

}  // namespace gas

#endif  // GAS_SRC_CORE_STORAGE_HPP_
