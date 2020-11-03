#ifndef GAS_SRC_UTILITY_DB_CONNECTOR_HPP_
#define GAS_SRC_UTILITY_DB_CONNECTOR_HPP_

// TODO think about spdlog
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <utility>

#include "notification.hpp"
#include "query.hpp"
#include "settings.hpp"

namespace gas {

template <typename... Types>
class Result {
 public:
  explicit Result(const pqxx::result &result) : result_(result) {}
  [[nodiscard]] auto size() const { return result_.size(); }

  template <typename T>
  requires std::is_constructible_v<T, std::tuple<Types...>> auto cast()
      -> std::vector<T> {
    std::vector<T> values;
    try {
      auto iter = result_.iter<Types...>();
      for (auto pack : iter) {
        values.emplace_back(pack);
      }
    } catch (std::exception const &e) {
      std::cerr << "Caught exception: " << e.what() << '\n';
    }
    return values;
  }

 private:
  pqxx::result result_;
};

class Connector {
 public:
  using notification_callback = NotificationSystem::callback_fn;

 public:
  explicit Connector(Settings settings)
      : settings_(settings), notification_system_(std::move(settings)) {}

  auto set_settings(Settings settings) -> void {
    settings_ = std::move(settings);
  }
  [[nodiscard]] auto get_settings() const -> const Settings & {
    return settings_;
  }

  auto connect() -> bool {
    try {
      connection_ = std::make_unique<pqxx::connection>(settings_.to_string());
    } catch (std::exception const &e) {
      return false;
    }
    return true;
  }
  auto connect(const Settings &settings) -> bool {
    settings_ = settings;
    return connect();
  }
  auto add_notifier(const std::string &channel,
                    const notification_callback &callback) -> void {
    notification_system_.add_notifier(channel, callback);
  }
  auto delete_notifier(const std::string &channel) -> void {
    notification_system_.delete_notifier(channel);
  }
  auto enable_notifications() -> void { notification_system_.enable(); }
  auto disable_notifications() -> void { notification_system_.disable(); }

  template <typename... T>
  auto exec(Query<T...> query) -> std::optional<Result<T...>> {
    pqxx::result result;
    try {
      pqxx::work work(*connection_);
      result = work.exec(query.sql);
      //      work.commit();
    } catch (std::exception const &e) {
      std::cerr << "Caught exception: " << e.what() << '\n';
      return std::nullopt;
    }
    return Result<T...>(result);
  }

 private:
  Settings settings_;
  std::unique_ptr<pqxx::connection> connection_;
  NotificationSystem notification_system_;
};

}  // namespace gas

#endif  // GAS_SRC_UTILITY_DB_CONNECTOR_HPP_
