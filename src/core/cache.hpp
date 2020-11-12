#ifndef GAS_SRC_UTILITY_CACHE_HPP_
#define GAS_SRC_UTILITY_CACHE_HPP_

#include <memory>
#include <mutex>
#include <vector>

namespace gas {

template <typename T>
struct CacheUnit {
  virtual ~CacheUnit() = default;
  std::vector<T> data;
  std::mutex mutex;
};

template <typename T>
class CacheView {
 public:
  using data = std::vector<T>;
  using iterator = typename data::iterator;

 public:
  explicit CacheView(std::vector<T>& cache_data)
      : cache_data_(cache_data) {}

  [[nodiscard]] constexpr auto begin() const { return std::begin(cache_data_); }
  [[nodiscard]] constexpr auto end() const { return std::end(cache_data_); }
  [[nodiscard]] constexpr auto empty() const { return cache_data_.empty(); }

 private:
  data& cache_data_;
};

template <typename... Types>
class Cache final : public CacheUnit<Types>... {
 public:
  Cache() = default;
  ~Cache() final = default;

  template <typename T>
  auto put(std::vector<T> data) {
    std::lock_guard lk(CacheUnit<T>::mutex);
    CacheUnit<T>::data = std::move(data);
  }
  template <typename T>
  auto get() const {
    return CacheUnit<T>::data;
  }
  template <typename T>
  [[nodiscard]] constexpr auto get_cache_view() -> CacheView<T> {
    return CacheView<T>{CacheUnit<T>::data};
  }
};

}  // namespace gas

#endif  // GAS_SRC_UTILITY_CACHE_HPP_
