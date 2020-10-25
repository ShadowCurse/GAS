#ifndef GAMEASSETSDB_SRC_DATABASE_DB_CONNECTOR_HPP_
#define GAMEASSETSDB_SRC_DATABASE_DB_CONNECTOR_HPP_

#include <string>
#include <sstream>
#include <utility>
#include <pqxx/pqxx>
#include <pqxx/notification>
#include <iostream>

namespace gas {

class Settings {
 public:
  Settings() = default;

  auto host(std::string host) -> Settings & {
    host_ = std::move(host);
    return *this;
  }
  auto port(size_t port) -> Settings & {
    port_ = port;
    return *this;
  }
  auto db_name(std::string db_name) -> Settings & {
    db_name_ = std::move(db_name);
    return *this;
  }
  auto username(std::string username) -> Settings & {
    username_ = std::move(username);
    return *this;
  }
  auto password(std::string password) -> Settings & {
    password_ = std::move(password);
    return *this;
  }

  auto to_string() const -> std::string {
    std::stringstream ss{};
    ss << " host=" << host_
       << " port=" << std::to_string(port_)
       << " dbname=" << db_name_
       << " user=" << username_
       << " password=" << password_;
    return ss.str();
  }

 private:
  std::string host_{};
  size_t port_{};
  std::string db_name_{};
  std::string username_{};
  std::string password_{};
};

class Notifier final : public pqxx::notification_receiver {
 public:
  Notifier(pqxx::connection &connector, const std::string &channel) : pqxx::notification_receiver(connector,
                                                                                                  channel) {}
  void operator()(std::string const &payload, int backend_pid) final {}
};

class Connector {
 public:
  Connector() = default;
  explicit Connector(Settings settings) : settings_(std::move(settings)) {}
  void set_settings(Settings settings) { settings_ = std::move(settings); }
  bool connect() {
    try {
      connection_ = std::make_unique<pqxx::connection>(settings_.to_string());
    }
    catch (std::exception const &e) {
      return false;
    }
    return true;
  }
  bool connect(const Settings &settings) {
    settings_ = settings;
    return connect();
  }
  void add_notifier(const std::string &channel) {
    notifiers_.emplace_back(*connection_, channel);
  }
  [[nodiscard]] auto exec(const std::string &sql) const -> std::optional<pqxx::result> {
    pqxx::result result;
    try {
      pqxx::work work(*connection_);
      result = work.exec(sql);
    } catch (std::exception const &e) {
      return std::nullopt;
    }
    return result;
  }

 private:
  Settings settings_{};
  std::unique_ptr<pqxx::connection> connection_;
  std::vector<Notifier> notifiers_;
};

} // namespace gas

#endif //GAMEASSETSDB_SRC_DATABASE_DB_CONNECTOR_HPP_
