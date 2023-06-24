/// @todo Replace strcpy, sprintf with strncpy, snprintf for bounded string creation

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <HardwareSerial.h>

#include "reading.h"
#include "debugger.h"

#define EXPECTED_PARSE_RESULT_COUNT 15
#define VALID_BGEIGIE_ID(id) (id >= 1000 && id < 10000)
#define HOME_LOCATION_PRECISION_KM 0.2

/**
 * Convert degree minutes to decimal degree
 * @param dm
 * @return
 */
double dm_to_dd(double dm) {
  double degree = static_cast<int>(dm / 100);
  double minutes = dm - (degree * 100);
  return degree + minutes / 60;
}

/**
 * Calculate distance using haversine formula
 * @param lon1
 * @param lat1
 * @param lon2
 * @param lat2
 * @return distance in km
 */
double calc_distance(double lon1, double lat1, double lon2, double lat2) {
  //This portion converts the current and destination GPS coords from decDegrees to Radians
  lon1 *= (PI / 180);
  lon2 *= (PI / 180);
  lat1 *= (PI / 180);
  lat2 *= (PI / 180);

  //This portion calculates the differences for the Radian latitudes and longitudes and saves them to variables
  double dlon = lon2 - lon1;
  double dlat = lat2 - lat1;

  //This portion is the Haversine Formula for distance between two points. Returned value is in KM
  double a = (sq(sin(dlat / 2))) + cos(lat1) * cos(lat2) * (sq(sin(dlon / 2)));
  double e = 2 * atan2(sqrt(a), sqrt(1 - a));
  return 6371.00 * e;
}

Reading::Reading() :
    _reading_str(""),
    _status(0x0),
    _average_of(0),
    _device_id(0),
    _iso_timestr(""),
    _cpm(0),
    _cpb(0),
    _total_count(0),
    _latitude(),
    _longitude(),
    _altitude(),
    _sat_count(),
    _precision(0) {
}

Reading::Reading(const char* reading_str) :
    _reading_str(""),
    _status(0x0),
    _average_of(0),
    _device_id(0),
    _iso_timestr(""),
    _cpm(0),
    _cpb(0),
    _total_count(0),
    _latitude(),
    _longitude(),
    _altitude(),
    _sat_count(),
    _precision(0) {
  strcpy(_reading_str, reading_str);
  parse_values();
}

Reading::Reading(const Reading& copy) :
    _reading_str(""),
    _status(copy._status),
    _average_of(copy._average_of),
    _device_id(copy._device_id),
    _iso_timestr(""),
    _cpm(copy._cpm),
    _cpb(copy._cpb),
    _total_count(copy._total_count),
    _latitude(copy._latitude),
    _longitude(copy._longitude),
    _altitude(copy._altitude),
    _sat_count(copy._sat_count),
    _precision(copy._precision) {
  strcpy(_reading_str, copy._reading_str);
  strcpy(_iso_timestr, copy._iso_timestr);
}

Reading& Reading::operator=(const char* reading_str) {
  reset();
  strcpy(_reading_str, reading_str);
  parse_values();
  return *this;
}

Reading& Reading::operator=(const Reading& other) {
  if(&other != this) {
    _status = other._status;
    _average_of = other._average_of;
    _device_id = other._device_id;
    _cpm = other._cpm;
    _cpb = other._cpb;
    _total_count = other._total_count;
    _latitude = other._latitude;
    _longitude = other._longitude;
    _altitude = other._altitude;
    _sat_count = other._sat_count;
    _precision = other._precision;
    strcpy(_reading_str, other._reading_str);
    strcpy(_iso_timestr, other._iso_timestr);
  }
  return *this;
}

Reading& Reading::operator+=(const Reading& o) {
  if(!(o._status & k_reading_valid) || o._average_of == 0) {
    // Do nothing with the other, not valid or empty
    return *this;
  }
  if(_average_of == 0) {
    // Assign other to this
    return operator=(o);
  }
  // Else, merge other with this

  // Use latest datetime and total count
  strcpy(_iso_timestr, o._iso_timestr);
  _total_count = o._total_count;

  // Maybe do something smarter with the validity...?
  _status |= o._status & k_reading_parsed;

  uint16_t o_cpm = o._cpm, o_cpb = o._cpb;

  if(!(_status & k_reading_sensor_ok) && o._status & k_reading_sensor_ok) {
    _status |= k_reading_sensor_ok;
    _cpm = o_cpm;
    _cpb = o_cpb;
  } else if(_status & k_reading_sensor_ok && !(o._status & k_reading_sensor_ok)) {
    o_cpm = _cpm;
    o_cpb = _cpb;
  }

  // Sensor data
  _cpm = ((_cpm * _average_of) + (o_cpm * o._average_of)) / (_average_of + o._average_of);
  _cpb = ((_cpb * _average_of) + (o_cpb * o._average_of)) / (_average_of + o._average_of);

  // Use latest gps location
  if(o._status & k_reading_gps_ok) {
    _status |= k_reading_gps_ok;
    _latitude = o._latitude;
    _longitude = o._longitude;
    _altitude = o._altitude;
  }

  // Update count of readings merged with this
  _average_of += o._average_of;

  return *this;
}

bool Reading::as_json(char* out) {
  if(!valid_reading()) {
    return false;
  }

  sprintf(
      out,
      "{\"captured_at\":\"%s\","
      "\"device_id\":%d,"
      "\"value\":%d,"
      "\"unit\":\"cpm\","
      "\"longitude\":%.5f,"
      "\"latitude\":%.5f}\n",
      _iso_timestr,
      get_fixed_device_id(),
      _cpm,
      _longitude,
      _latitude
  );
  return true;
}

void Reading::reset() {
  _average_of = 0;
  _status = 0;
}

void Reading::apply_home_location(double home_lat, double home_long) {
  if(calc_distance(_longitude, _latitude, home_long, home_lat) < HOME_LOCATION_PRECISION_KM) {
    DEBUG_PRINTF("Gps in home location, setting reading location to %.5f , %.5f\n", home_lat, home_long);
    _latitude = home_lat;
    _longitude = home_long;
  } else {
    DEBUG_PRINTLN("Gps not in home location");
    _status &= ~(k_reading_gps_ok);
  }
}

void Reading::parse_values() {
  reset();
  double lat_dm = 0, long_dm = 0;
  char n_or_s, w_or_e, sensor_status, gps_status;
  int16_t checksum;

  int parse_result = sscanf(
      _reading_str,
      "$BNRDD,%hu,%[^,],%hu,%hu,%hu,%c,%lf,%c,%lf,%c,%lf,%c,%d,%f*%hx",
      &_device_id,
      _iso_timestr,
      &_cpm,
      &_cpb,
      &_total_count,
      &sensor_status,
      &lat_dm,
      &n_or_s,
      &long_dm,
      &w_or_e,
      &_altitude,
      &gps_status,
      &_sat_count,
      &_precision,
      &checksum
  );

  if(parse_result == EXPECTED_PARSE_RESULT_COUNT && VALID_BGEIGIE_ID(_device_id)) { // 15 values to be parsed
    _status |= k_reading_parsed;

    // TODO Validate checksum?
    if(checksum > 0) {
      _average_of = 1;
      _status |= k_reading_valid;
    }

    if(sensor_status == 'A') {
      _status |= k_reading_sensor_ok;
    }
    if(gps_status == 'A') {
      _status |= k_reading_gps_ok;

      _latitude = dm_to_dd(lat_dm);
      _longitude = dm_to_dd(long_dm);

      if(n_or_s == 'S') { _latitude *= -1; }
      if(w_or_e == 'W') { _longitude *= -1; }
    }
  }
}

bool Reading::valid_reading() const {
  return _average_of > 0 && _status & k_reading_sensor_ok && _status & k_reading_gps_ok;
}

const char* Reading::get_reading_str() const {
  return _reading_str;
}

uint8_t Reading::get_status() const {
  return _status;
}

uint16_t Reading::get_device_id() const {
  return _device_id;
}

uint32_t Reading::get_fixed_device_id() const {
  return 60000 + _device_id;
}

const char* Reading::get_iso_timestr() const {
  return _iso_timestr;
}

uint16_t Reading::get_cpm() const {
  return _cpm;
}

uint16_t Reading::get_cpb() const {
  return _cpb;
}

uint16_t Reading::get_total_count() const {
  return _total_count;
}

double Reading::get_latitude() const {
  return _latitude;
}

double Reading::get_longitude() const {
  return _longitude;
}

double Reading::get_altitude() const {
  return _altitude;
}

int Reading::get_sat_count() const {
  return _sat_count;
}

float Reading::get_precision() const {
  return _precision;
}

