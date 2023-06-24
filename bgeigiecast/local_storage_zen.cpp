/// @brief This version for bGeigieZen on M5Stack Core2
#include "local_storage_zen.h"
#include "debugger.h"
#include "identifiers.h"
#include "reading.h"
#include "controller.h"
#include <cstring>

#define D_SAVED_STATE Controller::k_savable_MobileMode

const char* memory_name = "data";

// Keys for config
const char* key_device_id = "ap_ssid";
const char* key_ap_password = "ap_password";
const char* key_wifi_ssid = "wifi_ssid";
const char* key_wifi_password = "wifi_password";
const char* key_api_key = "api_key";
const char* key_use_dev = "use_dev";
// const char* key_led_color_blind = "led_color_blind";
// const char* key_led_color_intensity = "led_intensity";
const char* key_saved_state = "saved_state";
const char* key_use_home_location = "use_home_location";
const char* key_home_longtitude = "home_longtitude";
const char* key_home_latitude = "home_latitude";
const char* key_last_longtitude = "last_longtitude";
const char* key_last_latitude = "last_latitude";

// The states for the FSM used to parse the config file
enum LineParseState {
  LINE_START,
  LINE_PARSE_KEY,
  LINE_PARSE_DELIM,
  LINE_PARSE_VALUE,
  LINE_SKIP
};

LocalStorage::LocalStorage() :
    Handler(k_handler_storage_handler),
    _memory(),
    _device_id(0),
    _ap_password(""),
    _wifi_ssid(""),
    _wifi_password(""),
    _api_key(""),
    _use_dev(D_USE_DEV_SERVER),
    // _led_color_blind(D_LED_COLOR_BLIND),
    // _led_color_intensity(D_LED_COLOR_INTENSITY),
    _saved_state(D_SAVED_STATE),
    _use_home_location(false),
    _home_longitude(0),
    _home_latitude(0),
    _last_longitude(0),
    _last_latitude(0) {
}

void LocalStorage::reset_defaults() {
  if(clear()) {
    set_device_id(D_DEVICE_ID, true);
    set_ap_password(D_ACCESS_POINT_PASSWORD, true);
    set_wifi_ssid(D_WIFI_SSID, true);
    set_wifi_password(D_WIFI_PASSWORD, true);
    set_api_key(D_APIKEY, true);
    set_use_dev(D_USE_DEV_SERVER, true);
    // set_led_color_blind(D_LED_COLOR_BLIND, true);
    // set_led_color_intensity(D_LED_COLOR_INTENSITY, true);
    set_saved_state(D_SAVED_STATE, true);
    set_use_home_location(false, true);
    set_home_longitude(0, true);
    set_home_latitude(0, true);
    set_last_longitude(0, true);
    set_last_latitude(0, true);
  }
}

uint16_t LocalStorage::get_device_id() const {
  return _device_id;
}

const char* LocalStorage::get_ap_password() const {
  return _ap_password;
}

const char* LocalStorage::get_wifi_ssid() const {
  return _wifi_ssid;
}

const char* LocalStorage::get_wifi_password() const {
  return _wifi_password;
}

const char* LocalStorage::get_api_key() const {
  return _api_key;
}

int8_t LocalStorage::get_saved_state() const {
  return _saved_state;
}

bool LocalStorage::is_led_color_blind() const {
  return _led_color_blind;
}

uint8_t LocalStorage::get_led_color_intensity() const {
  return _led_color_intensity;
}

bool LocalStorage::get_use_dev() const {
  return _use_dev;
}

bool LocalStorage::get_use_home_location() const {
  return _use_home_location;
}

double LocalStorage::get_home_longitude() const {
  return _home_longitude;
}

double LocalStorage::get_home_latitude() const {
  return _home_latitude;
}

double LocalStorage::get_last_longitude() const {
  return _last_longitude;
}

double LocalStorage::get_last_latitude() const {
  return _last_latitude;
}

void LocalStorage::set_device_id(uint16_t device_id, bool force) {
  if(force || (device_id != _device_id)) {
    if(_memory.begin(memory_name)) {
      _device_id = device_id;
      _memory.putUShort(key_device_id, _device_id);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for device_id");
    }
  }
}

void LocalStorage::set_ap_password(const char* ap_password, bool force) {
  if(force || (ap_password != nullptr && strlen(ap_password) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_ap_password, ap_password);
      _memory.putString(key_ap_password, _ap_password);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for ap_password");
    }
  }
}

void LocalStorage::set_wifi_ssid(const char* wifi_ssid, bool force) {
  if(force || (wifi_ssid != nullptr && strlen(wifi_ssid) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_wifi_ssid, wifi_ssid);
      _memory.putString(key_wifi_ssid, _wifi_ssid);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for wifi_ssid");
    }
  }
}

void LocalStorage::set_wifi_password(const char* wifi_password, bool force) {
  if(force || (wifi_password != nullptr && strlen(wifi_password) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_wifi_password, wifi_password);
      _memory.putString(key_wifi_password, _wifi_password);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for wifi_password");
    }
  }
}

void LocalStorage::set_api_key(const char* api_key, bool force) {
  if(force || (api_key != nullptr && strlen(api_key) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_api_key, api_key);
      _memory.putString(key_api_key, _api_key);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for api_key");
    }
  }
}

void LocalStorage::set_use_dev(bool use_dev, bool force) {
  if(force || (use_dev != _use_dev)) {
    if(_memory.begin(memory_name)) {
      _use_dev = use_dev;
      _memory.putBool(key_use_dev, use_dev);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for use_dev");
    }
  }
}

void LocalStorage::set_saved_state(uint8_t saved_state, bool force) {
  if(force || (saved_state != _saved_state)) {
    if(_memory.begin(memory_name)) {
      _saved_state = saved_state;
      _memory.putChar(key_saved_state, saved_state);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for init_fixed");
    }
  }
}

/// @brief Changed because there was no corresponding key and stored value
/// @param use_home_location 
/// @param force 
void LocalStorage::set_use_home_location(bool use_home_location, bool force) {
  if(force || (use_home_location != _use_home_location)) {
    if(_memory.begin(memory_name)) {
      _use_home_location = use_home_location;
      _memory.putBool(key_use_home_location, use_home_location);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for key_use_home_location");
    }
  }
}

void LocalStorage::set_home_longitude(double home_longtitude, bool force) {
  if(_memory.begin(memory_name)) {
    _home_longitude = home_longtitude;
    _memory.putDouble(key_home_longtitude, home_longtitude);
    _memory.end();
  } else {
    DEBUG_PRINTLN("unable to save new value for key_home_longtitude");
  }
}

void LocalStorage::set_home_latitude(double home_latitude, bool force) {
  if(_memory.begin(memory_name)) {
    _home_latitude = home_latitude;
    _memory.putDouble(key_home_latitude, home_latitude);
    _memory.end();
  } else {
    DEBUG_PRINTLN("unable to save new value for key_home_latitude");
  }
}

void LocalStorage::set_last_longitude(double last_longitude, bool force) {
  if(_memory.begin(memory_name)) {
    _last_longitude = last_longitude;
    _memory.putDouble(key_last_longtitude, last_longitude);
    _memory.end();
  } else {
    DEBUG_PRINTLN("unable to save new value for key_last_longtitude");
  }
}

void LocalStorage::set_last_latitude(double last_latitude, bool force) {
  if(_memory.begin(memory_name)) {
    _last_latitude = last_latitude;
    _memory.putDouble(key_last_latitude, last_latitude);
    _memory.end();
  } else {
    DEBUG_PRINTLN("unable to save new value for key_last_latitude");
  }
}

bool LocalStorage::clear() {
  if(_memory.begin(memory_name)) {
    _memory.clear();
    _memory.end();
    return true;
  }
  return false;
}

/// @brief Activate local storage version for bGeigieZen
/// @param  dummy bool
/// @return always true (would crash if problem)
bool LocalStorage::activate(bool) {
  DEBUG_PRINTLN("Entered LocalStorage::activate");

  // Configuration
  DEBUG_PRINTF("Calling _memory.begin(%s, false)\n", SETUP_NAMESPACE);
  // 2nd arg (false) indicates that we want Read/Write operation
  _memory.begin(SETUP_NAMESPACE, false);
  DEBUG_PRINTF("Calling _memory.getBytesLength(%s),"
      "returned %d,"
      "sizeof(ConfigData) is %d\n", 
      SETUP_KEY_CONFIG, _memory.getBytesLength(SETUP_KEY_CONFIG), sizeof(ConfigData));

  if (_memory.getBytesLength(SETUP_KEY_CONFIG) == 0) {
    // This is the first time we set up the device
    _memory.putBytes(SETUP_KEY_CONFIG, &_config, sizeof(ConfigData));
  } else {
  DEBUG_PRINTLN("Calling _memory.getBytes(SETUP_KEY_CONFIG, &_config, sizeof(ConfigData))");
    _memory.getBytes(SETUP_KEY_CONFIG, &_config, sizeof(ConfigData));
  }

  if (_memory.getBytesLength(SETUP_KEY_DOSE) == 0) {
    // This is the first time we set up the device
    _memory.putBytes(SETUP_KEY_DOSE, &_dose, sizeof(DoseData));
  } else {
  DEBUG_PRINTLN("Calling _memory.getBytes(SETUP_KEY_DOSE, &_dose, sizeof(DoseData))");
    _memory.getBytes(SETUP_KEY_DOSE, &_dose, sizeof(DoseData));
  }

  _memory.end();
  DEBUG_PRINTLN("Returning true from LocalStorage::activate(bool)");
  return true;
}



/* OLD VERSION from bGeigieCast - NV store is from bGeigieZen
bool LocalStorage::activate(bool) {
  _memory.begin(memory_name, false); // 2nd arg means not read-only, therefore R/W
  // "config"
  if(_memory.getBytesLength(SETUP_KEY_CONFIG) == 0) {
    // First time run
    _memory.putBytes(SETUP_KEY_CONFIG, &_config, sizeof(ConfigData));
  }
  else {
    _memory.getBytes(SETUP_KEY_CONFIG, &_config, sizeof(ConfigData));
  }

  // "dose"
  if (_prefs.getBytesLength(SETUP_KEY_DOSE) == 0) {
    // This is the first time we set up the device
    _memory.putBytes(SETUP_KEY_DOSE, &_dose, sizeof(DoseData));
  }
  else {
    _memory.getBytes(SETUP_KEY_DOSE, &_dose, sizeof(DoseData));
  }

  _device_id = _memory.getUShort(key_device_id, D_DEVICE_ID);
  if(_memory.getString(key_ap_password, _ap_password, CONFIG_VAL_MAX) == 0) {
    strcpy(_ap_password, D_ACCESS_POINT_PASSWORD);
  }
  if(_memory.getString(key_wifi_ssid, _wifi_ssid, CONFIG_VAL_MAX) == 0) {
    strcpy(_wifi_ssid, D_WIFI_SSID);
  }
  if(_memory.getString(key_wifi_password, _wifi_password, CONFIG_VAL_MAX) == 0) {
    strcpy(_wifi_password, D_WIFI_PASSWORD);
  }
  if(_memory.getString(key_api_key, _api_key, CONFIG_VAL_MAX) == 0) {
    strcpy(_api_key, D_APIKEY);
  }
  _use_dev = _memory.getBool(key_use_dev, D_USE_DEV_SERVER);
  // _led_color_blind = _memory.getBool(key_led_color_blind, D_LED_COLOR_BLIND);
  // _led_color_intensity = _memory.getUChar(key_led_color_intensity, D_LED_COLOR_INTENSITY);
  _saved_state = _memory.getChar(key_saved_state, D_SAVED_STATE);
  _use_home_location = _memory.getBool(key_use_home_location, false);
  _home_longitude = _memory.getDouble(key_home_longtitude, 0);
  _home_latitude = _memory.getDouble(key_home_latitude, 0);
  _last_longitude = _memory.getDouble(key_last_longtitude, 0);
  _last_latitude = _memory.getDouble(key_last_latitude, 0);
  _memory.end();
  return true;
}
*/


int8_t LocalStorage::handle_produced_work(const worker_status_t& worker_reports) {
  // Get reading data to store
  const auto& reader = worker_reports.at(k_worker_bgeigie_connector);
  if(reader.is_fresh()) {
    const auto& reading = reader.get<Reading>();
    set_device_id(reading.get_device_id(), false);
    if(reading.get_status() & k_reading_gps_ok) {
      set_last_latitude(reading.get_latitude(), false);
      set_last_longitude(reading.get_longitude(), false);
    }
    return HandlerStatus::e_handler_data_handled;
  }
  return HandlerStatus::e_handler_idle;
}

bool LocalStorage::load_from_file(const char *setup_filename) {
  bool config_changed = false;
  char config_buffer[SETUP_FILE_PARSE_BUFFER_SIZE];
  char *key = config_buffer;
  char *value = config_buffer;

  // open the setup file
  auto setup_file = SD.open(setup_filename, FILE_READ);
  
  if (!setup_file)
    return false;

  // Process each config file lines
  char c;
  uint32_t pos = 0;
  LineParseState pstate = LINE_START;
  bool parsed_line = false;

  while (true) {
    c = (char)setup_file.read();
    if ((int8_t)c == -1)
      break;

    switch (pstate) {
      case LINE_START:
        if (c != ' ' && c != '\n' && c != '\r') {
          // found beginning of key
          config_buffer[pos] = c;
          pos++;  // pos == 1 after this line
          pstate = LINE_PARSE_KEY;
        }
        break;

      case LINE_PARSE_KEY:
        if (pos == SETUP_FILE_PARSE_BUFFER_SIZE) {
          pstate = LINE_SKIP;
        } else if (c == ' ') {
          // found end of the key, mark and go to the value
          config_buffer[pos] = '\0';  // mark the end of the string
          pos++;
          pstate = LINE_PARSE_DELIM;
        } else if (c == '=') {
          config_buffer[pos] = '\0';
          pos++;
          pstate = LINE_PARSE_DELIM;
        } else if (c == '\n' || c == '\r') {
          // premature end of line, skip
          pos = 0;
          pstate = LINE_START;
        } else {
          config_buffer[pos] = c;
          pos++;
        }
        break;

      case LINE_PARSE_DELIM:
        if (pos == SETUP_FILE_PARSE_BUFFER_SIZE) {
          pstate = LINE_SKIP;
        } else if (c == '\n' || c == '\r') {
          // we also accept an empty value
          value = config_buffer + pos;
          config_buffer[pos] = '\0';
          pos++;
          parsed_line = true;  // mark parsing as successful
          // parse next line
          pos = 0;
          pstate = LINE_START;
        } else if (c != ' ' && c != '=') {
          value = config_buffer + pos;
          config_buffer[pos] = c;
          pos++;
          pstate = LINE_PARSE_VALUE;
        }
        break;

      case LINE_PARSE_VALUE:
        if (pos == SETUP_FILE_PARSE_BUFFER_SIZE) {
          pstate = LINE_SKIP;
        } else if (c == '\n' || c == '\r' || c == ' ') {
          // end of value
          config_buffer[pos] = '\0';
          parsed_line = true;  // indicate successful parsing
          pos = 0;
          pstate = LINE_START;
        } else {
          config_buffer[pos] = c;
          pos++;
        }
        break;

      case LINE_SKIP:
        // wait until we find the end of line
        if (c == '\n' || c == '\r') {
          pos = 0;
          pstate = LINE_START;
        }
        break;
    }

    if (parsed_line) {
      parsed_line = false;

      //
      // Process matching keys
      //
      if (std::strcmp(key, "cpmf") == 0) {
        // Update cpm factor
        float factor = std::atof(value);
        if (_config.cpm2ush_divider != factor && factor > 0.0) {
          _config.cpm2ush_divider = factor;
          config_changed = true;
        }
      } else if (std::strcmp(key, "bqmf") == 0) {
        // Update bq/m2 factor
        float factor = std::atof(value);
        if (_config.cpm2bqm2_factor != factor && factor > 0.0) {
          _config.cpm2bqm2_factor = factor;
          config_changed = true;
        }
      } else if (std::strcmp(key, "nm") == 0) {
        // Update name in EEPROM
        if (std::strcmp(_config.user_name, value) != 0) {
          std::strncpy(_config.user_name, value, SETUP_USERNAME_MAXLEN);
          config_changed = true;
        }
      } else if (std::strcmp(key, "did") == 0) {
        // Update device id in persistent storage
        if (_config.device_id != (unsigned int)std::atoi(value)) {
          _config.device_id = std::atoi(value);
          config_changed = true;
        }
      } else if (std::strcmp(key, "gt") == 0) {
        // Update geiger type in persistent storage
        if (_config.type != std::atoi(value)) {
          _config.type = std::atoi(value);
          config_changed = true;
        }
      } else if (std::strcmp(key, "gm") == 0) {
        // Update geiger mode in persistent storage
        if (_config.mode != std::atoi(value)) {
          _config.mode = std::atoi(value);
          config_changed = true;
        }
      } else if (std::strcmp(key, "al") == 0) {
        // Update alarm threshold in persistent storage
        uint32_t new_alarm_level = std::atoi(value);
        if (_config.alarm_level != new_alarm_level) {
          _config.alarm_level = new_alarm_level;
          config_changed = true;
        }
      } else if (std::strcmp(key, "cn") == 0) {
        // Update country code in persistent storage
        if (std::strcmp(_config.country_code, value) != 0) {
          std::strncpy(_config.country_code, value,
                       sizeof(SETUP_DEFAULT_COUNTRY_CODE) - 1);
          config_changed = true;
        }
      } else if (std::strcmp(key, "tz") == 0) {
        // Update timezone in persistent storage
        uint8_t new_timezone = std::atoi(value);
        if (_config.timezone != new_timezone) {
          _config.timezone = new_timezone;
          config_changed = true;
        }
      } else if (std::strcmp(key, "st") == 0) {
        // Update sensor type in persistent storage
        uint8_t new_type = std::atoi(value);
        if (_config.sensor_type != new_type) {
          _config.sensor_type = new_type;
          config_changed = true;
        }
      } else if (std::strcmp(key, "ss") == 0) {
        // Update sensor shield in persistent storage
        uint8_t new_sensor_shield = std::atoi(value);
        if (_config.sensor_shield != new_sensor_shield) {
          _config.sensor_shield = new_sensor_shield;
          config_changed = true;
        }
      } else if (std::strcmp(key, "sh") == 0) {
        // Update sensor height in persistent storage
        uint32_t new_sensor_height = std::atoi(value);
        if (_config.sensor_height != new_sensor_height) {
          _config.sensor_height = new_sensor_height;
          config_changed = true;
        }
      } else if (std::strcmp(key, "sm") == 0) {
        // Update sensor mode in persistent storage
        if (_config.sensor_mode != std::atoi(value)) {
          _config.sensor_mode = std::atoi(value);
          config_changed = true;
        }
      } else if (std::strcmp(key, "dose") == 0) {
        // Reset total dose in persistent storage
        std::memset(&_dose, 0, sizeof(DoseData));
        _memory.putBytes(SETUP_KEY_DOSE, &_dose, sizeof(DoseData));
      }
    }
  }

  // close the setup file
  setup_file.close();

  if (config_changed) {
    // Configuration is changed
    _memory.putBytes(SETUP_KEY_CONFIG, &_config, sizeof(ConfigData));
  }

  return true;
}
