#ifndef GAS_SRC_UTILITY_NOTIFICATION_HPP_
#define GAS_SRC_UTILITY_NOTIFICATION_HPP_

#include <functional>
#include <pqxx/notification>
#include <thread>

#include "settings.hpp"

namespace gas {

class NotificationSystem {
 public:
  using callback_fn = std::function<void(std::string const &)>;

  struct notifier_info {
    std::string channel;
    callback_fn callback;
  };

  struct notifier final : public pqxx::notification_receiver {
    notifier(pqxx::connection &connection, const notifier_info &info)
        : pqxx::notification_receiver(connection, info.channel),
          connection_(connection),
          info_(info) {}
    ~notifier() final = default;
    auto operator()(std::string const &payload, int backend_pid) -> void final {
      info_.callback(payload);
    }

   private:
    pqxx::connection &connection_;
    const notifier_info &info_;
  };

 public:
  explicit NotificationSystem(Settings settings)
      : settings_(std::move(settings)) {}
  ~NotificationSystem() { disable(); }

  auto add_notifier(const std::string &channel, const callback_fn &callback)
      -> void {
    notifiers_infos_.push_back({channel, callback});
  }
  auto delete_notifier(const std::string &channel) -> void {
    std::erase_if(notifiers_infos_, [&channel](const auto &item) {
      return item.channel == channel;
    });
  }
  [[nodiscard]] auto get_notifiers() -> const std::vector<notifier_info> & {
    return notifiers_infos_;
  }
  auto enable() -> bool {
    try {
      connection_ = std::make_unique<pqxx::connection>(settings_.to_string());
      if (!connection_->is_open()) return false;
    } catch (std::exception const &e) {
      return false;
    }

    for (const auto &info : notifiers_infos_)
      notifiers_.emplace_back(std::make_unique<notifier>(*connection_, info));

    notifier_thread_ = std::jthread([this](const std::stop_token &stop_token) {
      while (!stop_token.stop_requested()) {
        try {
          if (connection_) connection_->await_notification();
        } catch (std::exception const &e) {
          std::cerr << "Notifier exception: " << e.what() << '\n';
        }
      }
    });
    return true;
  }
  auto disable() -> bool {
    notifier_thread_.request_stop();
    if (connection_) connection_->close();
    notifiers_.clear();
    if (notifier_thread_.joinable()) notifier_thread_.join();
    return true;
  }
  auto is_enabled() -> bool { return !notifiers_.empty(); }

 private:
  Settings settings_;
  std::unique_ptr<pqxx::connection> connection_;
  std::vector<notifier_info> notifiers_infos_;
  std::vector<std::unique_ptr<notifier>> notifiers_;
  std::jthread notifier_thread_;
};

}  // namespace gas

#endif  // GAS_SRC_UTILITY_NOTIFICATION_HPP_
