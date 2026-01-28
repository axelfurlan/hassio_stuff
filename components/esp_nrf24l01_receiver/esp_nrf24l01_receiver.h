#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include <RF24.h>

namespace esphome {
namespace esp_nrf24l01_receiver {

struct NrfSensorDataStruct {
  float temperature;
  float humidity;
};

typedef struct{
    char address[6];
    bool is_set = false;
} NRF24_address;

class ESP_NRF24L01_Receiver : public Component {
public:
    void set_cs_pin(InternalGPIOPin *pin) { cs_pin_ = pin; }
    void set_ce_pin(InternalGPIOPin *pin) { ce_pin_ = pin; }
    void set_temperature_sensor(sensor::Sensor *temperature_sensor, unsigned int sensor_number) { temperature_sensors_[sensor_number] = temperature_sensor; }
    void set_humidity_sensor(sensor::Sensor *humidity_sensor, unsigned int sensor_number) { humidity_sensors_[sensor_number] = humidity_sensor; }
    void set_last_packet_sensor(sensor::Sensor *last_packet_sensor, unsigned int sensor_number) { last_packet_sensors_[sensor_number] = last_packet_sensor; }
    void set_sensor_address(char const *address, unsigned int sensor_number) { strcpy(TX_addresses[sensor_number].address, address); TX_addresses[sensor_number].is_set = true; }
    void set_channel(int channel) { channel_ = (uint8_t)channel; }
    
    void setup() override;
    void loop() override;
    void dump_config() override;
    float get_setup_priority() const override;

private:
    InternalGPIOPin *cs_pin_;
    InternalGPIOPin *ce_pin_;
    RF24 radio;
    NrfSensorDataStruct NrfSensorData;
    uint8_t pipe_;
    uint8_t channel_;
    NRF24_address TX_addresses[6];
    sensor::Sensor *temperature_sensors_[6] = {nullptr};
    sensor::Sensor *humidity_sensors_[6] = {nullptr};
    sensor::Sensor *last_packet_sensors_[6] = {nullptr};
};

}  // namespace esp_nrf24l01_receiver
}  // namespace esphome
