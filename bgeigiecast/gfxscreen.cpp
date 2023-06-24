/**
 * @file gfxscreen.cpp
 * @author Louis Bertrand (louis@bertrandtech.ca)
 * @brief Drives the actual Core2 display using M5Stack API
 * @version 0.1
 * @date 2023-05-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "gfxscreen.h"

GFXScreen::GFXScreen() {

}

void GFXScreen::init() {
  // Initialize TFT (may already be done by M5)
  off();
}

void GFXScreen::off() {
  // Clear the graphics screen
  M5.Lcd.clear();
  core2Brightness(LEVEL_BLANKED);
}

/**
 * @brief Draw the boot-up welcome screen
 * 
 */
void GFXScreen::drawsplash() {
  /// @todo drawsplash should display the device name, ID, F/W version and copyright
  clear();
  // Display safecast copyright
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.drawString("SAFECAST", 230, 215, 1);
  M5.Lcd.setTextColor(TFT_ORANGE, TFT_BLACK);
  M5.Lcd.drawString("2023", 285, 215, 1);
}

/**
 * @todo Make set/get_intensity the dimming and blanking function calls
 */
// void GFXScreen::set_intensity(uint8_t intensity) {
//   config_intensity = intensity;
// }

// uint8_t GFXScreen::get_intensity() const {
//   return config_intensity;
// }


//setup brightness by Rob Oudendijk 2023-03-13
void GFXScreen::core2Brightness(uint8_t lvl, bool overdrive) {
  // The backlight brightness is in steps of 25 in AXP192.cpp
  // calculation in SetDCVoltage: ( (voltage - 700) / 25 )
  // 2325 is the minimum "I can just about see a glow in a dark room" level of brightness.
  // 2800 is the value set by the AXP library as "standard" bright backlight.
  int v = lvl * 25 + 2300;

  // Clamp to safe values.
  if (v < 2300) v = 2300;
  if (overdrive) {
    if (v > 3200) v = 3200; // maximum of 3.2 volts, 3200 (uint8_t lvl  = 36) absolute max!
  } else {
    if (v > 2800) v = 2800; // maximum of 2.8 volts, 2800 (uint8_t lvl  = 20)
  }

  // Minimum brightness means turn off the LCD backlight.
  if (v == 2300) {
    // LED set to minimum brightness? Turn off.
    M5.Axp.SetDCDC3(false);
    return;
  } else {
    // Ensure backlight is on. (magic name = DCDC3)
    M5.Axp.SetDCDC3(true);
  }

  // Set the LCD backlight voltage. (magic number = 2)
  M5.Axp.SetDCVoltage(2, v);
}

void GFXScreen::clear() {
  // Clear display
  M5.Lcd.clear();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextDatum(BL_DATUM);  // By default, text x,y is bottom left corner
  core2Brightness(LEVEL_BRIGHT);
 
  // Buttons defined in M5Core2.h

  /// @todo Move hot zone init to a more appropriate function
  // Hot zones start 40px above top of visible display
  // See discussion in M5Button.h and M5Touch.h
  M5.BtnA.set( 10, -40, 80, 70);
  M5.BtnB.set(120, -40, 80, 70);
  M5.BtnC.set(230, -40, 80, 70);
}
