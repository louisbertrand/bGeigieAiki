

/* Adapted from bGeigieNanoKit/NanoSetup.cpp
   Simple library to handle nano configuration from file and EEPROM
   Copyright (c) 2012, Lionel Bergeret
   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef BGEIGIECAST_ESP_CONFIG_H
#define BGEIGIECAST_ESP_CONFIG_H

#include <M5Core2.h> // #include <M5Stack.h>

#include <Preferences.h>

#include <Handler.hpp>
#include <config.hpp>  // adapted from bGeigieZen

#define CONFIG_VAL_MAX 32

/* From bGeigieZen/include/setup.hpp */
const char SETUP_NAMESPACE[] = "bgzen-data";
const char SETUP_KEY_CONFIG[] = "config";
const char SETUP_KEY_DOSE[] = "dose";

// geiger dose
// write every hours (eeprom ~ 100000 cycles -> ~ 11 years)
const uint32_t BNRDD_EEPROM_DOSE_WRITETIME = 3600;

enum GeigieType { GEIGIE_TYPE_B = 0, GEIGIE_TYPE_X };

enum GeigieMode { GEIGIE_MODE_USVH = 0, GEIGIE_MODE_BQM2 };

enum SensorType {
  SENSOR_TYPE_LND7317 = 0,
  SENSOR_TYPE_LND712,
};

enum SensorShield {
  SENSOR_SHIELD_NONE = 0,
  SENSOR_SHIELD_ALPHA,
  SENSOR_SHIELD_ALPHABETA,
};

enum SensorMode {
  SENSOR_MODE_AIR = 0,
  SENSOR_MODE_SURFACE = 1,
  SENSOR_MODE_PLANE = 2
};

struct DoseData {
  uint32_t total_count = 0;
  uint32_t total_time = 0;
};

struct ConfigData {
  ConfigData() { strcpy(country_code, SETUP_DEFAULT_COUNTRY_CODE); }
  uint8_t type = GEIGIE_TYPE_B;     // 0 for bGeigie, 1 for xGeigie
  uint8_t mode = GEIGIE_MODE_USVH;  // 0 for uSv/h, 1 for Bq/m2
  char user_name[SETUP_USERNAME_MAXLEN + 1] = {0};
  uint32_t device_id = SETUP_DEFAULT_DEVICE_ID;
  uint8_t cpm_window = SETUP_DEFAULT_CPM_WINDOW;
  float cpm2ush_divider = SETUP_DEFAULT_USH_DIVIDER;
  float cpm2bqm2_factor = SETUP_DEFAULT_BQM2_FACTOR;
  uint32_t alarm_level = SETUP_DEFAULT_ALARM_LEVEL;  // in CPM
  uint8_t timezone = SETUP_DEFAULT_TIMEZONE;         // in hours
  char country_code[sizeof(SETUP_DEFAULT_COUNTRY_CODE)];
  uint8_t sensor_type = SENSOR_TYPE_LND7317;
  uint8_t sensor_shield = SENSOR_SHIELD_NONE;
  uint32_t sensor_height = SETUP_DEFAULT_SENSOR_HEIGHT;  // in cm
  uint8_t sensor_mode = SENSOR_MODE_AIR;

  void print() const {
    Serial.print("Type: "); Serial.println(type);
    Serial.print("Mode: "); Serial.println(mode);
    Serial.print("Username: "); Serial.println(user_name);
    Serial.print("Device_id: "); Serial.println(device_id);
    Serial.print("CPM window: "); Serial.println(cpm_window);
    Serial.print("CPM factor: "); Serial.println(cpm2ush_divider);
    Serial.print("BQM2 factor: "); Serial.println(cpm2bqm2_factor);
    Serial.print("Alarm level: "); Serial.println(alarm_level);
    Serial.print("Timezone: "); Serial.println(timezone);
    Serial.print("Country code: "); Serial.println(country_code);
    Serial.print("Sensor type: "); Serial.println(sensor_type);
    Serial.print("Sensor shield: "); Serial.println(sensor_shield);
    Serial.print("Sensor height: "); Serial.println(sensor_height);
    Serial.print("Sensor mode: "); Serial.println(sensor_mode);
  }
};
/* End of from bGeigieZen/include/setup.hpp */


/**
 * Configurations for the ESP32, stored in the flash memory
 */
class LocalStorage : public Handler{
 public:
  LocalStorage();
  virtual ~LocalStorage() = default;

  /**
   * Reset settings to default (defined in user_config)
   */
  void reset_defaults();

  /**
   * Read device configuration for file on uSD card (from bGeigieZen/setup.cpp)
   */
  bool load_from_file(const char *setup_filename);

  // Getters and setters
  virtual uint16_t get_device_id() const final;
  virtual const char* get_ap_password() const final;
  virtual const char* get_wifi_ssid() const final;
  virtual const char* get_wifi_password() const final;
  virtual const char* get_api_key() const final;
  virtual bool get_use_dev() const final;
  virtual bool is_led_color_blind() const final;
  virtual uint8_t get_led_color_intensity() const final;
  virtual int8_t get_saved_state() const final;

  virtual bool get_use_home_location() const final;
  virtual double get_home_longitude() const final;
  virtual double get_home_latitude() const final;
  virtual double get_last_longitude() const final;
  virtual double get_last_latitude() const final;

  virtual void set_device_id(uint16_t device_id, bool force);
  virtual void set_ap_password(const char* ap_password, bool force);
  virtual void set_wifi_ssid(const char* wifi_ssid, bool force);
  virtual void set_wifi_password(const char* wifi_password, bool force);
  virtual void set_api_key(const char* api_key, bool force);
  virtual void set_use_dev(bool use_dev, bool force);
  virtual void set_saved_state(uint8_t saved_state, bool force);
  virtual void set_use_home_location(bool use_home_location, bool force);
  virtual void set_home_longitude(double home_longitude, bool force);
  virtual void set_home_latitude(double home_latitude, bool force);
  virtual void set_last_longitude(double last_longitude, bool force);
  virtual void set_last_latitude(double last_latitude, bool force);

 protected:
  virtual bool clear();

  /**
   * Read all settings
   */
  bool activate(bool) override;
  int8_t handle_produced_work(const worker_status_t& worker_reports) override;
 
 private:
  Preferences _memory;
  ConfigData _config;   // configuration data
  DoseData _dose;       // dose data

  // Device
  uint16_t _device_id;

  // Access point config (for web _ap_server)
  char _ap_password[CONFIG_VAL_MAX];

  // Wifi config (to connect to the internet)
  char _wifi_ssid[CONFIG_VAL_MAX];
  char _wifi_password[CONFIG_VAL_MAX];

  // API config (to connect to the API)
  char _api_key[CONFIG_VAL_MAX];
  bool _use_dev;

  // RGB LED config
  bool _led_color_blind;
  uint8_t _led_color_intensity;

  // Flag if fixed or mobile mode is started after init
  int8_t _saved_state;

  // Home location configs
  bool _use_home_location;
  double _home_longitude;
  double _home_latitude;

  double _last_longitude;
  double _last_latitude;
};

#endif //BGEIGIECAST_ESP_CONFIG_H
