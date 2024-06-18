#include "esp_nrf24l01_receiver.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace esp_nrf24l01_receiver {

static const char *TAG = "esp_nrf24l01_receiver";

void ESP_NRF24L01_Receiver::setup() {
    ESP_LOGCONFIG(TAG, "Setting up pins...");
    cs_pin_->setup();
    ce_pin_->setup();

    ESP_LOGCONFIG(TAG, "Setting up NRF24...");
    radio.begin(ce_pin_->get_pin(), cs_pin_->get_pin());
    radio.setDataRate( RF24_250KBPS );
    radio.setPALevel(RF24_PA_HIGH);
    radio.setAutoAck(true);
    radio.setPayloadSize(sizeof(NrfSensorData));
    for (uint8_t i = 0; i < 6; ++i) {
        if(TX_addresses[i].is_set) {
            radio.openReadingPipe(i, (byte*)(TX_addresses[i].address));
        }
    }
    radio.startListening();
}

void ESP_NRF24L01_Receiver::loop() {
    if (radio.available(&pipe)) {              // is there a payload? get the pipe number that recieved it
        uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
        radio.read(&NrfSensorData, bytes);             // fetch payload from FIFO
        ESP_LOGD(TAG, "Got Temperature=%.1f°C Humidity=%.1f%%", NrfSensorData.temperature, NrfSensorData.humidity);
        
        if (temperature_sensors_[pipe] != nullptr) {
            temperature_sensors_[pipe]->publish_state(NrfSensorData.temperature);
        }
        if (humidity_sensors_[pipe] != nullptr) {
            humidity_sensors_[pipe]->publish_state(NrfSensorData.humidity);
        }
        status_clear_warning();
    }
}

float ESP_NRF24L01_Receiver::get_setup_priority() const { return setup_priority::DATA; }

void ESP_NRF24L01_Receiver::dump_config() { ESP_LOGCONFIG(TAG, "ESP_NRF240L_Receiver sensor"); }

}  // namespace esp_nrf24l01_receiver
}  // namespace esphome
