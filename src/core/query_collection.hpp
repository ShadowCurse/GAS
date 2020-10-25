#ifndef GAS_SRC_CORE_QUERIES_HPP_
#define GAS_SRC_CORE_QUERIES_HPP_

#include <array>
#include <utility>
#include <stdexcept>

using namespace std::string_view_literals;

template<typename Key, typename Value, std::size_t Size>
struct ConstExprMap {
  std::array<std::pair<Key, Value>, Size> data;
  [[nodiscard]] constexpr Value operator[](const Key &key) const {
    if (const auto result =
          std::find_if(std::begin(data), std::end(data), [&key](const auto &value) { return value.first = key; });
        result != std::end(data)) {
      return result->second;
    } else {
      throw std::range_error("Value not found");
    }
  }
};

static constexpr ConstExprMap<std::string_view, std::string_view, 10> query_collection =
    {std::pair{"add_project"sv, ""sv},
     {"delete_project", ""}};

#endif //GAS_SRC_CORE_QUERIES_HPP_
