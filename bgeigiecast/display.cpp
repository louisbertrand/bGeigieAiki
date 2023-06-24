/**
 * @file display.cpp
 * @author Louis Bertrand (louis@bertrandtech.ca)
 * @brief [Core2] Drives the display based on controller state and device reports
 * @version 0.1
 * @date 2023-05-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "display.h"  // based on "mode_led.h"
#include "user_config.h"
#include "debugger.h"
#include "identifiers.h"
#include "sm_c_state.h"
#include "bluetooth_reporter.h"
#include "api_connector.h"

Display::Display(LocalStorage& config) :
    GFXScreen(),
    Supervisor(),
    _config(config)
{}

void Display::loop() {
  /// @todo Manage the dimming and blanking behaviour
}

bool Display::activate() {
  init();
  return true;
}

void Display::handle_report(const Report& report) {
  auto& worker_stats = report.get_worker_stats();
  auto& handler_stats = report.get_handler_stats();

  // Switch controller system state
  switch(handler_stats.at(k_handler_controller_handler).status) {
    case ControllerState::k_state_InitializeState:
    case ControllerState::k_state_InitReadingState:
      /// Initializing, show startup splash screen
      drawsplash();
      break;
    case ControllerState::k_state_PostInitializeState:
      /// Post initializing, blink every 0.8 second ( this lasts for 3 seconds total )
      //set_values(mode_color_init, 1.5, 15);
      break;
    case ControllerState::k_state_ConfigurationModeState:
      switch(worker_stats.at(k_worker_configuration_server).active_state) {
        case WorkerStatus::e_state_active:
          /// Configuration mode - up and running. no blink
          //set_values(mode_color_config);
          break;
        case WorkerStatus::e_state_activating_failed:
        default:
          /// Configuration mode - connecting to wifi / settings up access point. blink
          //set_values(mode_color_config, 1, 50);
          break;
      }
      break;
    case ControllerState::k_state_MobileModeState:
      switch(handler_stats.at(k_handler_bluetooth_reporter).status) {
        case BluetoothReporter::e_handler_clients_available:
          /// Mobile mode - Clients connected
          // set_values(mode_color_mobile);
          break;
        case BluetoothReporter::e_handler_idle:
        case BluetoothReporter::e_handler_no_clients:
        default:
          /// Mobile mode - No clients connected
          // set_values(mode_color_mobile, 0.33, 5);
          break;
      }
      break;
    case ControllerState::k_state_FixedModeState:
      if(handler_stats.at(k_handler_api_reporter).active_state == HandlerStatus::e_state_activating_failed) {
        /// Fixed mode - Connecting to wifi, blink connected
        // set_values(mode_color_fixed_connected, 1, 25);
        break;
      }
      switch(handler_stats.at(k_handler_api_reporter).status) {
        case ApiReporter::e_api_reporter_error_not_connected:
          /// Fixed mode - Lost connection to wifi, blink connected again
          // set_values(mode_color_fixed_connected, 1, 25);
          break;
        case ApiReporter::e_api_reporter_error_invalid_reading:
          /// Fixed mode - Invalid reading (no good gps) TODO: add additional check for gps
          // set_values(mode_color_fixed_hard_error, 0.33, 15);
          break;
        case ApiReporter::e_api_reporter_error_remote_not_available:
        case ApiReporter::e_api_reporter_error_server_rejected_post:
          /// Fixed mode - Remote is not available
          // set_values(mode_color_fixed_soft_error);
          break;
        case ApiReporter::e_api_reporter_send_success:
        default:
          /// Fixed mode - All good and connected
          // set_values(mode_color_fixed_connected);
          break;
      }
      break;
    case ControllerState::k_state_ResetState:
      /// Reset - Display red because its least used TODO: custom color
      // set_values(mode_color_fixed_hard_error);
      break;
    default:
      break;
  }
}
