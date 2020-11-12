#ifndef GAS_SRC_CORE_STORAGE_HPP_
#define GAS_SRC_CORE_STORAGE_HPP_

#include "cache.hpp"
#include "connector.hpp"
#include "tables.hpp"

namespace gas {

using StorageCache =
    Cache<Log, Developer, ResourceType, Resource, Dependency, Commit>;

class StorageUnit {
 public:
  using storage_id = uint32_t;
  StorageUnit(storage_id id, Settings settings)
      : id_(id), connector_(std::move(settings)) {}

  [[nodiscard]] auto connect() -> bool { return connector_.connect(); }
  auto disconnect() -> void { connector_.disconnect(); }
  [[nodiscard]] auto connected() const -> bool {
    return connector_.connected();
  }

  template <typename T>
  [[nodiscard]] auto create_view() {
    return cache_.get_cache_view<T>();
  }
  [[nodiscard]] constexpr inline auto id() const { return id_; }
  auto update() -> void {
    if (connector_.connected()) {
      if (auto result = connector_.exec(Developer::get_all())) {
        cache_.put(result->cast<Developer>());
      }
    }
  }

 private:
  storage_id id_;
  Connector connector_;
  StorageCache cache_;
};

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
  explicit View(const std::vector<std::unique_ptr<StorageUnit>>& storages) {
    for (const auto& storage : storages) {
      views_.emplace_back(storage->create_view<T>());
    }
  }

  auto begin() { return Iterator::create_begin(&views_); }
  auto end() { return Iterator::create_end(&views_); }

 private:
  std::vector<CacheView<T>> views_;
};

class Storage {
 public:
  Storage() = default;

  [[nodiscard]] auto add_storage(Settings settings) -> StorageUnit::storage_id {
    ++id_counter;
    storages_.emplace_back(
        std::make_unique<StorageUnit>(id_counter, std::move(settings)));
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
  template <typename T>
  [[nodiscard]] constexpr auto add_view() const {
    return View<T>{storages_};
  };

  auto update() -> void {
    for (const auto& storage : storages_) storage->update();
  }

 private:
  StorageUnit::storage_id id_counter{0};
  std::vector<std::unique_ptr<StorageUnit>> storages_;
};

}  // namespace gas

#endif  // GAS_SRC_CORE_STORAGE_HPP_
