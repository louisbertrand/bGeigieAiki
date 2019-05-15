#ifndef BGEIGIECAST_BGEIGIE_CONNECTOR_H
#define BGEIGIECAST_BGEIGIE_CONNECTOR_H

#include <Arduino.h>

#include "reading.h"

/**
 * Connect the system to the bGeigieNano to read sensor data
 */
class BGeigieConnector {
 public:
  explicit BGeigieConnector(Stream& _serial_connection);
  virtual ~BGeigieConnector() = default;

  /**
   * Get a reading from serial
   * @param out: output for the reading
   * @return: true if a reading was read
   */
  bool get_reading(Reading** out);

 private:
  Stream& _serial_connection;
  String _buffer;
};

#endif //BGEIGIECAST_BGEIGIE_CONNECTOR_H
