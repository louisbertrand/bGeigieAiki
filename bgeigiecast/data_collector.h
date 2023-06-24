#ifndef BGEIGIECAST_DATA_COLLECTOR_H
#define BGEIGIECAST_DATA_COLLECTOR_H

#include <Arduino.h>

#include <Worker.hpp>

#include "reading.h"

/**
 * Initialize the bGeigieZen hardware to read sensor data
 */
class DataCollector : public Worker<Reading> {
 public:
  explicit DataCollector();  // parameter(s) TBD, if any
  virtual ~DataCollector() = default;

 private:
  int8_t produce_data() override;

 private: // TBD
  // Stream& _serial_connection;
  String _buffer;
};

#endif //BGEIGIECAST_DATA_COLLECTOR_H
