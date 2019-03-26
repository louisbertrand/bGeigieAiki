#ifndef BGEIGIE_POINTCAST_CONTROLLER_HPP
#define BGEIGIE_POINTCAST_CONTROLLER_HPP

#include <button.h>
#include <button_observer.h>

#include "state_machine/context.h"
#include "web_server/web_server.h"
#include "connectors/bgeigie_connector.h"
#include "connectors/i_api_connector.h"
#include "connectors/i_bluetooth_connector.h"
#include "state_led.h"

/**
 * Main controller for the system, implements state machine to run
 */
class Controller : public Context, public ButtonObserver {
 public:
  Controller(IEspConfig& config, Stream& bGegie_connection, IApiConnector& api_connector, IBluetoohConnector& bluetooth_connector);
  virtual ~Controller() = default;

  /**
   * Set initial state for the state machine,
   */
  void setup_state_machine();

  /**
   * Initialize the controller and all of its components
   */
  void initialize();

  /**
   * Tries to read new serial input from the bGeigie, if there is, it will report to bluetooth / api
   * @param report_bluetooth: If true, it will command the bluetooth component to process the report
   * @param report_api: If true, it will command the api component to process the report
   */
  void process_possible_bgeigie_readings(bool report_bluetooth, bool report_api);

  /**
   * Callback for the button
   */
  void on_button_pressed(Button* button, uint32_t millis) override;

  void set_state(AbstractState* state) override;

  // Getters
  IEspConfig& get_config();
  ConfigWebServer& get_ap_server();
  IApiConnector& get_api_connector();
  StateLED& get_state_led();
 private:
  IEspConfig& _config;
  IApiConnector& _api_connector;
  IBluetoohConnector& _bluetooth;
  ConfigWebServer _ap_server;
  Button _mode_button;
  StateLED _state_led;
  BGeigieConnector _bgeigie_connector;


};

#endif //BGEIGIE_POINTCAST_CONTROLLER_HPP
