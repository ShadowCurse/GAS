#ifndef GAS_SRC_QUERIES_QUERIES_HPP_
#define GAS_SRC_QUERIES_QUERIES_HPP_

#include <concepts>
#include <string>
#include <functional>
#include <numeric>

using namespace std::literals::string_literals;

namespace gas {

template<typename T>
concept is_table = requires(T t) {{ std::invoke(t) } -> std::same_as<std::string>; }
    && std::is_class_v<typename T::table_class>;

template<typename T>
concept is_column = std::is_class_v<typename T::parent_table>;

template<typename Column, typename Table>
concept is_column_of_table = is_table<Table> && is_column<Column>
    && std::is_same_v<typename Table::table_class, typename Column::parent_table>;

#define PREDICATE(name, operation)     \
  template <typename T> \
  struct name {\
    using is_pred = name;\
    explicit name(T value) : value_(value) {}\
    [[nodiscard]] inline auto operator()(const std::string &str) const  -> std::string {\
      if constexpr (std::is_convertible_v<T, std::string>)\
        return str + " " + #operation + " " + value_;\
      else\
        return str + " " + #operation + " " + std::to_string(value_);\
    }\
   private:\
    T value_;\
  };
PREDICATE(eq, =)
PREDICATE(gt, >)
PREDICATE(ge, >=)
PREDICATE(lt, <)
PREDICATE(le, <=)
template<typename T>
struct between {
  using is_pred = between;
  explicit between(T min, T max) : min_(min), max_(max) {}
  [[nodiscard]] inline auto operator()(const std::string &str) const -> std::string {
    if constexpr (std::is_convertible_v<T, std::string>)
      return str + " between " + min_ and max_;
    else
      return str + " between " + std::to_string(min_) + " and " + std::to_string(max_);
  }
 private:
  T min_;
  T max_;
};

template<typename T>
concept is_predicate = std::is_class_v<typename T::is_pred>;

#define LOGICAL(name) \
  struct name {       \
    using is_logical = name; \
    explicit name() {}\
    [[nodiscard]] inline auto operator()() const  -> std::string { return #name; }\
  };

namespace logical {
LOGICAL(AND)
LOGICAL(OR)
}

template<typename T>
concept is_logical = std::is_class_v<typename T::is_logical>;

template<is_table Table>
struct columns {
  explicit columns(is_column_of_table<Table> auto ... columns) : columns_{columns ...} {}
  void add(is_column_of_table<Table> auto column) {
    columns_.emplace_back(column);
  }
  [[nodiscard]] inline auto operator()() const -> std::string {
    return std::accumulate(std::next(std::begin(columns_)),
                           std::end(columns_),
                           std::invoke(*std::begin(columns_)),
                           [](const auto &item1, const auto &item2) { return item1 + ", " + std::invoke(item2); });
  }
 private:
  std::vector<std::function<std::string()>> columns_;
};

template<is_table Table>
struct condition {
  condition(is_column_of_table<Table> auto column, is_predicate auto predicate) : result_(std::invoke(predicate,
                                                                                                      std::invoke(column))) {}
  [[nodiscard]] inline auto operator()() const -> std::string { return result_; }
 private:
  std::string result_;
};

template<is_table Table>
struct where {
  where() = default;
  where(is_logical auto logic, std::vector<condition<Table>> conditions) {
    result_ = std::accumulate(std::next(std::begin(conditions)),
                              std::end(conditions),
                              std::invoke(*std::begin(conditions)),
                              [&](const auto &item1, const auto &item2) {
                                return item1 + " " + std::invoke(logic) + " " + std::invoke(item2);
                              });
  }
  [[nodiscard]] inline auto operator()() const -> std::string { return result_; }
 private:
  std::string result_{};
};

template<is_table ... Tables>
struct group_by {
  explicit group_by(columns<Tables...> columns) { result_ = " group by "s + columns.combine(); }
  [[nodiscard]] inline auto operator()() const -> std::string { return result_; }
 private:
  std::string result_{};
};

template<is_table ... Tables>
struct select {
  explicit select(columns<Tables> ... columns) {
    result_ = "select "s + ((std::invoke(columns) + ", "s) + ...) + " from " + ((std::invoke(Tables{}) + ", "s) + ...);
  }
  [[nodiscard]] inline auto operator()() const -> std::string {
    return result_;
    // query<ColTypes ...>(result_);
  }
  auto &where(where<Tables> ... where) {
    result_ += " where "s + (std::invoke(where) + ...);
    return *this;
  }
  template<is_table ... T>
  auto &group_by(group_by<T> ... group_by) {
    result_ += std::invoke(group_by ...);
    return *this;
  }
 private:
  std::string result_{};
};

//template<is_column Column>
//struct con {
//  con(is_predicate auto predicate) : result_(std::invoke(predicate, std::invoke(Column{}))) {}
//  [[nodiscard]] inline auto operator()() const -> std::string { return result_; }
// private:
//  std::string result_;
//};
//
//template <is_column ... Columns>
//struct sel {
//  sel() = default;
//  template<is_logical logical, is_predicate ... Predicates>
//  auto where(Predicates ... predicates) { return (std::invoke(con<Columns>(predicates)) + ...); }
//};

template<is_table Table>
struct update {
  explicit update() {
    result_ = "update " + std::invoke(Table{});
  }
  [[nodiscard]] inline auto operator()() const -> std::string {
    return result_;
  }
  template<is_column_of_table<Table> Column>
  auto &set(typename Column::field_type value) {
    if constexpr (std::is_convertible_v<typename Column::field_type, std::string>)
      result_ += " set " + std::invoke(Column{}) + " = " + value;
    else
      result_ += " set " + std::invoke(Column{}) + " = " + std::to_string(value);
    return *this;
  }
  auto &where(where<Table> where) {
    result_ += " where "s + std::invoke(where);
    return *this;
  }
 private:
  std::string result_;
};

//template<typename ... Types>
//struct query {
//  query(std::string sql);
//};

}; // namespace gas

#endif //GAS_SRC_QUERIES_QUERIES_HPP_
