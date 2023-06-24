#ifndef BGEIGIECAST_READING_H
#define BGEIGIECAST_READING_H

#include <stdint.h>

#define READING_STR_MAX 100

constexpr uint8_t k_reading_parsed = 0x1u<<0u;
constexpr uint8_t k_reading_sensor_ok = 0x1u<<1u;
constexpr uint8_t k_reading_gps_ok = 0x1u<<2u;
constexpr uint8_t k_reading_valid = 0x1u<<3u;

/**
 * Container for a reading from the bGeigie, with some extra functions
 */
class Reading {
 public:
  /**
   * Create a new empty reading
   */
  Reading();

  /**
   * Create a new reading from string
   * @param reading_str: the reading from the bgeigie connection, it will parse right away
   */
  explicit Reading(const char* reading_str);
  virtual ~Reading() = default;
  Reading(const Reading& copy);
  Reading& operator=(const char* reading_str);
  Reading& operator=(const Reading& other);

  /**
   * Merge another reading with this one, takes the averages of all
   * @param o
   */
  Reading& operator+=(const Reading& o);

  /**
   * Get this reading as a json object in string
   * @param out: output param
   * @param fixed: if the device is in fixed mode, it will add 60000 to the device id
   * @return: succes / not
   * @todo Parameter "fixed" is not implemented, is it needed?
   */
  bool as_json(char* out = nullptr);

  /**
   * Clear this reading
   */
  void reset();

  /**
   * Check if the reading is 100% valid (parsed, sensor and gps OK)
   * @return true if valid
   */
  bool valid_reading() const;

  /**
   * Apply home location. will invalidate the gps status if its out of range, or set the home lat / long if in range.
   * @param home_lat: latitude
   * @param home_long: longitude
   */
  void apply_home_location(double home_lat, double home_long);

  const char* get_reading_str() const;
  uint8_t get_status() const;
  uint16_t get_device_id() const;
  uint32_t get_fixed_device_id() const;
  const char* get_iso_timestr() const;
  uint16_t get_cpm() const;
  uint16_t get_cpb() const;
  uint16_t get_total_count() const;
  double get_latitude() const;
  double get_longitude() const;
  double get_altitude() const;
  int get_sat_count() const;
  float get_precision() const;

 private:
  /**
   * Parse values from the reading_str
   */
  void parse_values();

  char _reading_str[READING_STR_MAX];
  uint8_t _status;
  uint16_t _average_of;

  // Reading content
  uint16_t _device_id;
  char _iso_timestr[30];
  uint16_t _cpm;
  uint16_t _cpb;
  uint16_t _total_count;
  double _latitude;
  double _longitude;
  double _altitude;
  int _sat_count;
  float _precision;

};

#endif //BGEIGIECAST_READING_H
