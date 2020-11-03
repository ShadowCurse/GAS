#ifndef GAMEASSETSDB_SRC_UTILITY_DB_CONNECTOR_HPP_
#define GAMEASSETSDB_SRC_UTILITY_DB_CONNECTOR_HPP_

#include <iostream>
#include <pqxx/notification>
#include <pqxx/pqxx>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

#include "query.hpp"

namespace gas {

class Settings {
 public:
  Settings() = default;

  [[nodiscard]] auto host(std::string host) -> Settings & {
    host_ = std::move(host);
    return *this;
  }
  [[nodiscard]] auto port(size_t port) -> Settings & {
    port_ = port;
    return *this;
  }
  [[nodiscard]] auto db_name(std::string db_name) -> Settings & {
    db_name_ = std::move(db_name);
    return *this;
  }
  [[nodiscard]] auto username(std::string username) -> Settings & {
    username_ = std::move(username);
    return *this;
  }
  [[nodiscard]] auto password(std::string password) -> Settings & {
    password_ = std::move(password);
    return *this;
  }

  [[nodiscard]] auto to_string() const -> std::string {
    std::stringstream ss{};
    ss << " host=" << host_ << " port=" << std::to_string(port_)
       << " dbname=" << db_name_ << " user=" << username_
       << " password=" << password_;
    return ss.str();
  }

 private:
  std::string host_;
  size_t port_{};
  std::string db_name_;
  std::string username_;
  std::string password_;
};

class Notifier final : public pqxx::notification_receiver {
 public:
  using callback_fn = std::function<void(std::string const &)>;

 public:
  Notifier(pqxx::connection &connector, const std::string &channel,
           callback_fn callback)
      : pqxx::notification_receiver(connector, channel),
        connector_(connector),
        channel_(channel),
        callback_(std::move(callback)) {}
  ~Notifier() final = default;
  Notifier(const Notifier &other)
      : pqxx::notification_receiver(other.connector_, other.channel_),
        connector_(other.connector_),
        channel_(other.channel_),
        callback_(other.callback_) {}

  Notifier(Notifier &&other) noexcept
      : pqxx::notification_receiver(other.connector_, other.channel_),
        connector_(other.connector_),
        channel_(std::move(other.channel_)),
        callback_(std::move(other.callback_)) {}

  void operator()(std::string const &payload, int backend_pid) final {
    callback_(payload);
  }

 private:
  pqxx::connection &connector_;
  std::string channel_;
  callback_fn callback_;
};

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
  using notification_callback = Notifier::callback_fn;

 public:
  Connector() = default;
  ~Connector() {
    notifiers_.clear();
    connection_->close();
    notifier_thread_.join();
  }
  explicit Connector(Settings settings) : settings_(std::move(settings)) {}

  void set_settings(Settings settings) { settings_ = std::move(settings); }
  [[nodiscard]] auto get_settings() const -> const Settings & {
    return settings_;
  }

  bool connect() {
    try {
      connection_ = std::make_unique<pqxx::connection>(settings_.to_string());
    } catch (std::exception const &e) {
      return false;
    }
    return true;
  }
  bool connect(const Settings &settings) {
    settings_ = settings;
    return connect();
  }
  void add_notifier(const std::string &channel,
                    notification_callback callback) {
    notifiers_.emplace_back(*connection_, channel, std::move(callback));
  }

  void enable_notifications() {
    notifier_thread_ = std::jthread([&](std::stop_token stop_token) {
      while (!stop_token.stop_requested()) {
        std::cout << "Listening for notifications:\n";
        try {
          connection_->await_notification();
        } catch (std::exception const &e) {
          std::cerr << "Notifier exception: " << e.what() << '\n';
          return;
        }
      }
    });
  }

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
  std::vector<Notifier> notifiers_;
  std::jthread notifier_thread_;
};

}  // namespace gas

#endif  // GAMEASSETSDB_SRC_UTILITY_DB_CONNECTOR_HPP_
