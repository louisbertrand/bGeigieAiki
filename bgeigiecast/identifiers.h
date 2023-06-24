#ifndef BGEIGIECAST_IDENTIFIERS_H_
#define BGEIGIECAST_IDENTIFIERS_H_

enum DataWorkers {
  k_worker_bgeigie_connector = 0, // Cast: receive data from Nano
  k_worker_configuration_server,
  k_worker_wifi_access_point,
  k_worker_controller_state_changer,
  k_worker_data_collector  // Zen: collects data from sensor and GPS
};

enum DataHandlers {
  k_handler_controller_handler = 0,
  k_handler_storage_handler,
  k_handler_bluetooth_reporter,
  k_handler_api_reporter,
};


#endif //BGEIGIECAST_IDENTIFIERS_H_
