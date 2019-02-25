#ifndef BGEIGIE_POINTCAST_SERVER_H
#define BGEIGIE_POINTCAST_SERVER_H

#include <WiFi.h>

#include "esp_config.h"

/**
 * Class to host a web server for configuring the ESP32. Will set up an access
 * point based on user_config.h "Access point settings".
 */
class ConfigWebServer {
 public:
  ConfigWebServer(EspConfig& config);
  virtual ~ConfigWebServer() = default;

  /**
   * Initialize the web server, does nothing if it is already initialized.
   * @return true if initialization was success
   */
  bool initialize();

  /**
   * Stop the web server if it is running
   */
  void stop();

  /**
   * Check if the server is running
   * @return true if running
   */
  bool is_running();

  /**
   * Checks if there are requests and handles them
   */
  void handle_requests();
 private:
  WiFiServer server;
  EspConfig& config;

};

#endif //BGEIGIE_POINTCAST_SERVER_H
