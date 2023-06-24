/**
 * @file gfxscreen.h
 * @author Louis Bertrand (louis@bertrandtech.ca)
 * @brief Drives the actual Core2 display using M5Stack API
 * @version 0.1
 * @date 2023-05-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef BGEIGIECAST_CORE2_SCREEN_H
#define BGEIGIECAST_CORE2_SCREEN_H

#include <M5Core2.h>

class GFXScreen {
 public:
  /**
   * Create an instance of the TFT screen
   */
  GFXScreen();
  virtual ~GFXScreen() = default;

  void init();
  void clear();
  void off();
  void drawsplash();
  void core2Brightness(uint8_t lvl, bool overdrive = false);

 private:
  // size of the screen M5Stack Core2
  static constexpr int width = 320;
  static constexpr int height = 240;
  uint32_t _refresh_period_ms;

  // Dim then blank settings; tweak to taste (will become a menu setting)
  static constexpr uint8_t LEVEL_BRIGHT = 35;  // max brightness = 36
  static constexpr uint8_t LEVEL_DIMMED = 10;
  static constexpr uint8_t LEVEL_BLANKED = 0;
  static constexpr uint32_t DELAY_DIMMING_DEFAULT = 2*60*1000;  // ms before dimming screen
  static constexpr uint32_t DELAY_BLANKING_DEFAULT = 3*60*1000;  // ms before blanking screen


};

#endif //BGEIGIECAST_CORE2_SCREEN_H
