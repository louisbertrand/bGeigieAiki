#ifndef BGEIGIECAST_STATE_LED_H
#define BGEIGIECAST_STATE_LED_H

#include "rgb_led.h"
#include "i_esp_config.h"

/**
 * Controls the LED to visualize the current state
 */
class StateLED : private RGBLed{
 public:
  typedef enum {
    off,
    init,
    config,
    mobile,
    fixed_connecting,
    fixed_active,
    fixed_error,
    COUNT
  } StateColor;


  explicit StateLED(IEspConfig& config);
  virtual ~StateLED() = default;

  void set_color(StateColor color);

  void blink(StateColor color, double frequency, double percentage_on = 50);

 private:
  typedef struct {
    RGB normal;
    RGB color_blind;
  } ColorType;

  uint8_t get_intensity() const override;

  const IEspConfig& _config;
  bool _blink_state;
  const ColorType _colorTypes[StateColor::COUNT];
};

#endif //BGEIGIECAST_STATE_LED_H
