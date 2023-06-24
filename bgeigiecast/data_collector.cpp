#include "data_collector.h"
#include "debugger.h"
#include "identifiers.h"

DataCollector::DataCollector() :
    Worker<Reading>(k_worker_data_collector, Reading(), 4000) // parameters and initializers TBD
    { 
}

int8_t DataCollector::produce_data() {
  // if hardware available 
  //   read GPS
  //   read CPM
  //   read IMU (for wake-on-shake)
  // edit this return statement as appropriate
  //      return data.get_status() & k_reading_parsed ? WorkerStatus::e_worker_data_read : WorkerStatus::e_worker_error;
  // 
  // }
  return WorkerStatus::e_worker_idle;
}
