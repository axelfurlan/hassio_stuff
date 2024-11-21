#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include <ADS1X15.h>

#define BUTTON_CH D1
#define BUTTON_UP D7
#define BUTTON_STOP D6
#define BUTTON_DOWN D5

#define ADS_SDA D2
#define ADS_SCL D3

namespace esphome
{
    namespace somphy_shades_controller
    {
        class Somphy_Shades_Controller : public Component, public Cover
        {
        public:
            void setup() override;
            void dump_config() override;
            CoverTraits get_traits() override;
            void control(const CoverCall &call) override;

        private:
            void simulatePushbuttonPress(uint8_t push_button);
            void initializePushButton(uint8_t push_button);
            void readChannelLEDs();
            int GetCurrentChannel();
            bool SelectDeisredChannel(int desired_channel);

            ADS1115 ADS;
            bool led_0;
            bool led_1;
            bool led_2;
            bool led_3;
            bool led_all;
        };
    } // namespace somphy_shades_controller
} // namespace esphome
