#include <Arduino.h>

#include "bluetooth_reporter.h"
#include "debugger.h"
#include "identifiers.h"

BluetoothReporter::BluetoothReporter(LocalStorage& config)
    : Handler(k_handler_bluetooth_reporter), config(config), pServer(nullptr), pDataRXCharacteristic(nullptr) {
}

bool BluetoothReporter::activate(bool) {
  if(!config.get_device_id()) {
    DEBUG_PRINTLN("Cannot initialize bluetooth without device id");
    return false;
  }

  char deviceName[16];
  sprintf(deviceName, "bGeigie%d", config.get_device_id());
  BLEDevice::init(deviceName);
  DEBUG_PRINT("Bluetooth initialized, device: ");
  DEBUG_PRINTLN(deviceName);

  if(!pServer) {
    pServer = BLEDevice::createServer();

    create_ble_profile_service(pServer);
    create_ble_device_service(pServer);
    create_ble_data_service(pServer);

    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_PROFILE_UUID);
    pAdvertising->addServiceUUID(SERVICE_DEVICE_UUID);
    pAdvertising->addServiceUUID(SERVICE_DATA_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
  }

  return BLEDevice::getInitialized();
}

void BluetoothReporter::deactivate() {
  BLEDevice::deinit();
}

int8_t BluetoothReporter::handle_produced_work(const worker_status_t& worker_reports) {
  const auto& reading_stat = worker_reports.at(k_worker_bgeigie_connector);
  if(!reading_stat.is_fresh()) {
    return pServer->getConnectedCount() > 0 ? e_handler_clients_available : Status::e_handler_idle;
  }
  // Fresh reading is produced
  const auto& reading = reading_stat.get<Reading>();
  return send_reading(reading) ? Status::e_handler_clients_available : Status::e_handler_no_clients;
}

void BluetoothReporter::create_ble_profile_service(BLEServer* pServer) {
  BLEService* pProfileService = pServer->createService(SERVICE_PROFILE_UUID);
  BLECharacteristic* pProfileNameCharacteristic = pProfileService->createCharacteristic(
      CHARACTERISTIC_PROFILE_NAME_UUID,
      BLECharacteristic::PROPERTY_READ
  );
  /*BLECharacteristic* pProfileAppearanceCharacteristic = */pProfileService->createCharacteristic(
      CHARACTERISTIC_PROFILE_APPEARANCE_UUID,
      BLECharacteristic::PROPERTY_READ
  );

  pProfileNameCharacteristic->setValue(BLE_PROFILE_NAME);

  pProfileService->start();
}

void BluetoothReporter::create_ble_device_service(BLEServer* pServer) {
  BLEService* pDeviceService = pServer->createService(SERVICE_DEVICE_UUID);

  // Manufacturer name
  BLECharacteristic* pDeviceManufacturerCharacteristic = pDeviceService->createCharacteristic(
      CHARACTERISTIC_DEVICE_MANUFACTURER_UUID,
      BLECharacteristic::PROPERTY_READ
  );
  BLEDescriptor* pDescriptorManufacturer = new BLEDescriptor((uint16_t) 0x2901);
  pDescriptorManufacturer->setValue("Manufacturer Name String");
  pDeviceManufacturerCharacteristic->addDescriptor(pDescriptorManufacturer);

  // Model number
  BLECharacteristic* pDeviceModelCharacteristic = pDeviceService->createCharacteristic(
      CHARACTERISTIC_DEVICE_MODEL_UUID,
      BLECharacteristic::PROPERTY_READ
  );
  BLEDescriptor* pDescriptorModel = new BLEDescriptor((uint16_t) 0x2901);
  pDescriptorModel->setValue("Model Number String");
  pDeviceModelCharacteristic->addDescriptor(pDescriptorModel);

  // Firmware revision
  BLECharacteristic* pDeviceFirmwareCharacteristic = pDeviceService->createCharacteristic(
      CHARACTERISTIC_DEVICE_FIRMWARE_UUID,
      BLECharacteristic::PROPERTY_READ
  );
  BLEDescriptor* pDescriptorFirmware = new BLEDescriptor((uint16_t) 0x2901);
  pDescriptorFirmware->setValue("Firmware Revision String");
  pDeviceFirmwareCharacteristic->addDescriptor(pDescriptorFirmware);

  // Hardware revision
  BLECharacteristic* pDeviceRevisionCharacteristic = pDeviceService->createCharacteristic(
      CHARACTERISTIC_DEVICE_REVISION_UUID,
      BLECharacteristic::PROPERTY_READ
  );
  BLEDescriptor* pDescriptorHardware = new BLEDescriptor((uint16_t) 0x2901);
  pDescriptorHardware->setValue("Hardware Revision String");
  pDeviceRevisionCharacteristic->addDescriptor(pDescriptorHardware);

  pDeviceManufacturerCharacteristic->setValue(BLE_DEVICE_INFO_MANUFACTURER);
  pDeviceModelCharacteristic->setValue(BLE_DEVICE_INFO_MODEL);
  pDeviceFirmwareCharacteristic->setValue(BLE_DEVICE_INFO_FIRMWARE_REVISION);
  pDeviceRevisionCharacteristic->setValue(BLE_DEVICE_INFO_HARDWARE_REVISION);

  pDeviceService->start();
}

void BluetoothReporter::create_ble_data_service(BLEServer* pServer) {
  BLEService* pDataService = pServer->createService(SERVICE_DATA_UUID);

  // DB Addr
  BLECharacteristic* pDataDBAddrCharacteristic = pDataService->createCharacteristic(
      CHARACTERISTIC_DATA_BDADDR_UUID,
      BLECharacteristic::PROPERTY_READ
  );
  BLEDescriptor* pDescriptorAddr = new BLEDescriptor((uint16_t) 0x2901);
  pDescriptorAddr->setValue("DB-Addr");
  pDataDBAddrCharacteristic->addDescriptor(pDescriptorAddr);

  // Baudrate
  BLECharacteristic* pDataBaudCharacteristic = pDataService->createCharacteristic(
      CHARACTERISTIC_DATA_BAUD_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  BLEDescriptor* pDescriptorBaud = new BLEDescriptor((uint16_t) 0x2901);
  pDescriptorBaud->setValue("Baudrate");
  pDataBaudCharacteristic->addDescriptor(pDescriptorBaud);

  // RX
  pDataRXCharacteristic = pDataService->createCharacteristic(
      CHARACTERISTIC_DATA_RX_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  BLEDescriptor* pDescriptorRX = new BLEDescriptor((uint16_t) 0x2901);
  pDescriptorRX->setValue("RX");
  pDataRXCharacteristic->addDescriptor(pDescriptorRX);
  BLEDescriptor* pDescriptorRXNotifications = new BLEDescriptor((uint16_t) 0x2902);
  pDescriptorRXNotifications->setValue("Notifications and indications disabled");
  pDataRXCharacteristic->addDescriptor(pDescriptorRXNotifications);

  // TX
  BLECharacteristic* pDataTXCharacteristic = pDataService->createCharacteristic(
      CHARACTERISTIC_DATA_TX_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
  );
  BLEDescriptor* pDescriptorTX = new BLEDescriptor((uint16_t) 0x2901);
  pDescriptorTX->setValue("TX");
  pDataTXCharacteristic->addDescriptor(pDescriptorTX);

  pDataDBAddrCharacteristic->setValue(addr, BLE_DATA_ADDR_SIZE);

  pDataService->start();
}

bool BluetoothReporter::send_reading(const Reading& reading) const {
  if(pServer->getConnectedCount() == 0) {
    // No clients to send data to
    return false;
  }
  DEBUG_PRINTLN("Sending reading over Bluetooth");
  const char* reading_str = reading.get_reading_str();
  size_t size = strlen(reading_str);

  int segment = 0;
  const static uint8_t max_segment_size = 20; // Max that can be send over bluetooth
  do {
    ++segment;
    uint8_t segment_size = segment * max_segment_size > size ? size % max_segment_size : max_segment_size;
    char to_send[segment_size];
    strncpy(to_send, reading_str + ((segment - 1) * max_segment_size), segment_size);

    pDataRXCharacteristic->setValue((uint8_t*) to_send, segment_size);
    pDataRXCharacteristic->notify();
  } while(segment * max_segment_size < size);
  return true;
}
