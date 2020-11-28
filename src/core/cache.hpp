#ifndef GAS_SRC_UTILITY_CACHE_HPP_
#define GAS_SRC_UTILITY_CACHE_HPP_

#include <memory>
#include <mutex>
#include <vector>

namespace gas {

template <typename T>
struct CacheUnit {
 public:
  using search_fun = std::function<bool(const T&)>;
 protected:
  virtual ~CacheUnit() = default;
  std::vector<T> data;
  mutable std::mutex mutex;
};

template <typename T>
class CacheView {
 public:
  using data = std::vector<T>;
  using iterator = typename data::iterator;

 public:
  explicit constexpr CacheView(data& cache_data) : cache_data_(cache_data) {}

  [[nodiscard]] constexpr auto begin() const { return std::begin(cache_data_); }
  [[nodiscard]] constexpr auto end() const { return std::end(cache_data_); }
  [[nodiscard]] constexpr auto size() const { return cache_data_.size(); }
  [[nodiscard]] constexpr auto empty() const { return cache_data_.empty(); }

 private:
  data& cache_data_;
};

template <typename... Types>
class StorageCache_T final : public CacheUnit<Types>... {
 public:
  template <typename T>
  using search_fun = typename CacheUnit<T>::search_fun;

 public:
  constexpr StorageCache_T() = default;
  ~StorageCache_T() final = default;

  template <typename T>
  constexpr auto put(std::vector<T> data) -> void {
    std::lock_guard lk(CacheUnit<T>::mutex);
    CacheUnit<T>::data = std::move(data);
  }
  template <typename T>
  [[nodiscard]] constexpr auto search(search_fun<T> fun) const
      -> std::optional<const std::reference_wrapper<const T>> {
    std::lock_guard lk(CacheUnit<T>::mutex);
    if (auto result = std::find_if(std::begin(CacheUnit<T>::data),
                                   std::end(CacheUnit<T>::data), fun);
        result != std::end(CacheUnit<T>::data))
      return *result;
    return std::nullopt;
  }
  template <typename T>
  [[nodiscard]] constexpr auto get_cache_view() -> CacheView<T> {
    return CacheView<T>{CacheUnit<T>::data};
  }
};

}  // namespace gas

#endif  // GAS_SRC_UTILITY_CACHE_HPP_
