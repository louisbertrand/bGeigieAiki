/**
 * @file display.h
 * @author Louis Bertrand (louis@bertrandtech.ca)
 * @brief [Core2] Drives the display based on controller state and device reports
 * @version 0.1
 * @date 2023-05-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef BGEIGIECAST_STATE_DISPLAY_H
#define BGEIGIECAST_STATE_DISPLAY_H

#include "gfxscreen.h"  // replaces bGeigieCast rgb_led
#include "local_storage.h"

#include <Supervisor.hpp>

/**
 * Controls the LED to visualize the current mode
 */
class Display : private GFXScreen, public Supervisor {
 public:

  explicit Display(LocalStorage& config);
  virtual ~Display() = default;

  void loop();

  void handle_report(const Report& report) override;

  bool activate() override;
 private:

  const LocalStorage& _config;
};

#endif //BGEIGIECAST_STATE_DISPLAY_H
