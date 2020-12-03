#ifndef GAS_SRC_UTILITY_DB_CONNECTOR_HPP_
#define GAS_SRC_UTILITY_DB_CONNECTOR_HPP_

// TODO think about spdlog
#include <fstream>
#include <iostream>
#include <pqxx/largeobject>
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
  using oid = pqxx::oid;

 public:
  explicit Connector(Settings settings)
      : settings_(settings), notification_system_(std::move(settings)) {}
  ~Connector() { disconnect(); }

  auto set_settings(Settings settings) -> void {
    settings_ = std::move(settings);
  }
  [[nodiscard]] auto get_settings() const -> const Settings & {
    return settings_;
  }
  [[nodiscard]] auto connect() -> bool {
    if (!connection_) {
      try {
        connection_ = std::make_unique<pqxx::connection>(settings_.to_string());
        if (!connection_->is_open()) return false;
      } catch (std::exception const &e) {
        std::cerr << "Caught exception: " << e.what() << '\n';
        return false;
      }
    }
    return true;
  }
  auto disconnect() -> void {
    if (connection_) {
      notification_system_.disable();
      connection_.reset(nullptr);
    }
  }
  [[nodiscard]] auto connected() const -> bool {
    return connection_ && connection_->is_open();
  }
  auto add_notifier(const std::string &channel,
                    const notification_callback &callback) -> void {
    notification_system_.add_notifier(channel, callback);
  }
  auto delete_notifiers(std::string_view channel) -> void {
    notification_system_.delete_notifier(channel);
  }
  auto get_notifiers() { return notification_system_.get_notifiers(); }
  auto enable_notifications() -> bool { return notification_system_.enable(); }
  auto disable_notifications() -> bool {
    return notification_system_.disable();
  }
  auto notifications_enabled() -> bool {
    return notification_system_.is_enabled();
  }

  template <typename... T>
  auto exec(Query<T...> query) -> std::optional<Result<T...>> {
    if (query.sql.empty()) {
      return std::nullopt;
    }
    pqxx::result result;
    try {
      pqxx::work work(*connection_);
      result = work.exec(query.sql);
      work.commit();
    } catch (std::exception const &e) {
      std::cerr << "exec caught exception: " << e.what() << '\n';
      return std::nullopt;
    }
    return Result<T...>(result);
  }

  auto create_lo() -> oid {
    try {
      pqxx::work work(*connection_);
      pqxx::largeobject lo(work);
      work.commit();
      return lo.id();
    } catch (std::exception const &e) {
      std::cerr << "create_lo caught exception: " << e.what() << '\n';
    }
  }

  auto remove_lo(oid oid) -> void {
    try {
      pqxx::work work(*connection_);
      pqxx::largeobject lo(oid);
      lo.remove(work);
      work.commit();
    } catch (std::exception const &e) {
      std::cerr << "remove_lo caught exception: " << e.what() << '\n';
    }
  }

  auto upload_large_object(std::string_view file_path) -> oid {
    try {
      pqxx::work work(*connection_);
      pqxx::largeobjectaccess lo_new(work, file_path);
      work.commit();
      return lo_new.id();
    } catch (std::exception const &e) {
      std::cerr << "upload_lo caught exception: " << e.what() << '\n';
    }
  }

  auto download_large_object(uint oid, std::string_view file_path) -> void {
    try {
      pqxx::work work(*connection_);
      pqxx::largeobjectaccess object(work, oid);
      object.to_file(file_path);
    } catch (std::exception const &e) {
      std::cerr << "download_lo caught exception: " << e.what() << '\n';
    }
  }

 private:
  Settings settings_;
  std::unique_ptr<pqxx::connection> connection_;
  NotificationSystem notification_system_;
};

}  // namespace gas

#endif  // GAS_SRC_UTILITY_DB_CONNECTOR_HPP_
