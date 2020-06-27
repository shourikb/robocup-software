#pragma once

#include <rclcpp/rclcpp.hpp>
#include <rj_msgs/msg/field_dimensions.hpp>
#include <rj_msgs/msg/game_settings.hpp>
#include <rj_msgs/srv/set_field_dimensions.hpp>
#include <rj_msgs/srv/set_game_settings.hpp>

namespace config_client {
using GameSettingsMsg = rj_msgs::msg::GameSettings;
using SetGameSettingsSrv = rj_msgs::srv::SetGameSettings;
using SetGameSettingsReq = SetGameSettingsSrv::Request;

using FieldDimensionsMsg = rj_msgs::msg::FieldDimensions;
using SetFieldDimensionsSrv = rj_msgs::srv::SetFieldDimensions;
using SetFieldDimensionsReq = SetFieldDimensionsSrv::Request;

// TODO(1520): Add callback functionality to ConfigClient.

/**
 * @brief Helper util that sets up the proper subscribers for
 * receiving config updates
 */
class ConfigClient {
public:
    ConfigClient(rclcpp::Node* node);

    [[nodiscard]] const GameSettingsMsg& gameSettings() const {
        return game_settings_.value();
    }

    [[nodiscard]] const FieldDimensionsMsg& fieldDimensions() const {
        return field_dimensions_.value();
    }

    /**
     * @brief Thread safe version of gameSettings(), returns
     * game_settings_ but uses a mutex.
     * @return
     */
    [[nodiscard]] const GameSettingsMsg& gameSettingsThreaded() const {
        std::lock_guard<std::mutex> guard{mutex_};
        return game_settings_.value();
    }

    /**
     * @brief Thread safe version of fieldDimensions(), returns
     * field_dimensions_ but uses a mutex.
     * @return
     */
    [[nodiscard]] const FieldDimensionsMsg& fieldDimensionsThreaded() const {
        std::lock_guard<std::mutex> guard{mutex_};
        return field_dimensions_.value();
    }

    /**
     * @brief Returns whether it is connected to the ConfigServer. All other
     * functions are invalid if this returns false.
     * @return
     */
    [[nodiscard]] bool connected() const;

    /**
     * @brief Thread safe version of connected().
     * @return
     */
    [[nodiscard]] bool connectedThreaded() const;

    /**
     * @brief Blocks until the config client is connected. Should only be called
     * from a different thread than the one that is spinning.
     */
    bool waitUntilConnected() const;

    /**
     * @brief Sends a service call to ConfigServer to update the GameSettings.
     * @param msg
     */
    void updateGameSettings(const GameSettingsMsg& msg);

    /**
     * @brief Sends a service call to ConfigServer to update the
     * FieldDimensions.
     * @param msg
     */
    void updateFieldDimensions(const FieldDimensionsMsg& msg);

private:
    rclcpp::Node* node_;

    rclcpp::Subscription<GameSettingsMsg>::SharedPtr game_settings_sub_;
    rclcpp::Client<SetGameSettingsSrv>::SharedPtr game_settings_client_;
    std::optional<GameSettingsMsg> game_settings_;

    rclcpp::Subscription<FieldDimensionsMsg>::SharedPtr field_dimensions_sub_;
    rclcpp::Client<SetFieldDimensionsSrv>::SharedPtr field_dimensions_client_;
    std::optional<FieldDimensionsMsg> field_dimensions_;

    mutable std::mutex mutex_;
};
}  // namespace config_client
