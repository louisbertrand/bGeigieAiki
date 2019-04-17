#ifndef BGEIGIE_POINTCAST_REPORTER_H
#define BGEIGIE_POINTCAST_REPORTER_H

#include <connectors/i_bluetooth_connector.h>
#include <connectors/i_api_connector.h>
#include <Stream.h>
#include <connectors/bgeigie_connector.h>
#include "state_machine/sm_context.h"

/**
 * The reporter will receive instructions to get and report readings from the bgeigie to the api / bluetooth
 */
class Reporter : public Context {
 public:
  Reporter(IEspConfig& config, Stream& bgeigie_connection, IApiConnector& api_connector, IBluetoohConnector& bluetooth_connector);
  virtual ~Reporter() = default;

  /**
   * Set initial state for the state machine,
   */
  void setup_state_machine() override;

 private:
  BGeigieConnector _geigie_connector;
  IEspConfig& _config;
  IApiConnector& _api_connector;
  IBluetoohConnector& _bluetooth;

  Reading _last_reading;

};

#endif //BGEIGIE_POINTCAST_REPORTER_H
