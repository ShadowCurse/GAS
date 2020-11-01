#ifndef GAMEASSETSDB_SRC_UTILITY_QUERY_HPP_
#define GAMEASSETSDB_SRC_UTILITY_QUERY_HPP_

#include <concepts>
#include <functional>
#include <numeric>
#include <string>
#include <utility>

#include "table.hpp"

using namespace std::literals::string_literals;

namespace gas {

template <typename... T>
struct Query {
  explicit Query(std::string query) : sql(std::move(query)) {}
  std::string sql;
};

////////////////////////
/// currently unused ///
////////////////////////

#define PREDICATE(name, operation)                                     \
  template <typename T>                                                \
  struct name {                                                        \
    using is_pred = name;                                              \
    explicit name(T value) : value_(value) {}                          \
    [[nodiscard]] inline auto operator()(const std::string &str) const \
        -> std::string {                                               \
      if constexpr (std::is_convertible_v<T, std::string>)             \
        return str + " " + #operation + " " + value_;                  \
      else                                                             \
        return str + " " + #operation + " " + std::to_string(value_);  \
    }                                                                  \
                                                                       \
   private:                                                            \
    T value_;                                                          \
  };
PREDICATE(eq, =)
PREDICATE(gt, >)
PREDICATE(ge, >=)
PREDICATE(lt, <)
PREDICATE(le, <=)

template <typename T>
struct Between {
  using is_pred = Between;
  explicit Between(T min, T max) : min_(min), max_(max) {}
  [[nodiscard]] inline auto operator()(const std::string &str) const
      -> std::string {
    if constexpr (std::is_convertible_v<T, std::string>)
      return str + " between " + min_ and max_;
    else
      return str + " between " + std::to_string(min_) + " and " +
             std::to_string(max_);
  }

 private:
  T min_;
  T max_;
};

template <typename T>
concept is_predicate = std::is_class_v<typename T::is_pred>;

#define LOGICAL(name)                                             \
  struct name {                                                   \
    using is_logical = name;                                      \
    explicit name() {}                                            \
    [[nodiscard]] inline auto operator()() const -> std::string { \
      return #name;                                               \
    }                                                             \
  };

namespace logical {
LOGICAL(AND)
LOGICAL(OR)
}  // namespace logical

template <typename T>
concept is_logical = std::is_class_v<typename T::is_logical>;

template <is_table Table>
struct Columns {
  explicit Columns(is_column_of_table<Table> auto... columns)
      : columns_{columns...} {}
  void add(is_column_of_table<Table> auto column) {
    columns_.emplace_back(column);
  }
  [[nodiscard]] inline auto operator()() const -> std::string {
    return std::accumulate(std::next(std::begin(columns_)), std::end(columns_),
                           std::invoke(*std::begin(columns_)),
                           [](const auto &item1, const auto &item2) {
                             return item1 + ", " + std::invoke(item2);
                           });
  }

 private:
  std::vector<std::function<std::string()>> columns_;
};

template <is_table Table>
struct Condition {
  Condition(is_column_of_table<Table> auto column, is_predicate auto predicate)
      : result_(std::invoke(predicate, std::invoke(column))) {}
  [[nodiscard]] inline auto operator()() const -> std::string {
    return result_;
  }

 private:
  std::string result_;
};

template <is_table Table>
struct Where {
  Where() = default;
  Where(is_logical auto logic, std::vector<Condition<Table>> conditions) {
    result_ = std::accumulate(
        std::next(std::begin(conditions)), std::end(conditions),
        std::invoke(*std::begin(conditions)),
        [&](const auto &item1, const auto &item2) {
          return item1 + " " + std::invoke(logic) + " " + std::invoke(item2);
        });
  }
  [[nodiscard]] inline auto operator()() const -> std::string {
    return result_;
  }

 private:
  std::string result_{};
};

template <is_table... Tables>
struct GroupBy {
  explicit GroupBy(Columns<Tables...> columns) {
    result_ = " group by "s + columns.combine();
  }
  [[nodiscard]] inline auto operator()() const -> std::string {
    return result_;
  }

 private:
  std::string result_{};
};

template <is_table... Tables>
struct Select {
  explicit Select(Columns<Tables>... columns) {
    result_ = "select "s + ((std::invoke(columns) + ", "s) + ...) + " from " +
              ((std::invoke(Tables{}) + ", "s) + ...);
  }
  [[nodiscard]] inline auto operator()() const -> std::string {
    return result_;
    // query<ColTypes ...>(result_);
  }
  auto &where(Where<Tables>... where) {
    result_ += " where "s + (std::invoke(where) + ...);
    return *this;
  }
  template <is_table... T>
  auto &group_by(GroupBy<T>... group_by) {
    result_ += std::invoke(group_by...);
    return *this;
  }

 private:
  std::string result_{};
};

template <is_table Table>
struct Update {
  explicit Update() { result_ = "update " + std::invoke(Table{}); }
  [[nodiscard]] inline auto operator()() const -> std::string {
    return result_;
  }
  template <is_column_of_table<Table> Column>
  auto &set(typename Column::field_type value) {
    if constexpr (std::is_convertible_v<typename Column::field_type,
                                        std::string>)
      result_ += " set " + std::invoke(Column{}) + " = " + value;
    else
      result_ +=
          " set " + std::invoke(Column{}) + " = " + std::to_string(value);
    return *this;
  }
  auto &where(Where<Table> where) {
    result_ += " where "s + std::invoke(where);
    return *this;
  }

 private:
  std::string result_;
};

};  // namespace gas

#endif  // GAMEASSETSDB_SRC_UTILITY_QUERY_HPP_
