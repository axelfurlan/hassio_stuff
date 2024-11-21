#include "somphy_shades_controller.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome
{
    namespace somphy_shades_controller
    {
        static const char *TAG = "somphy_shades_controller";

        void Somphy_Shades_Controller::setup()
        {
            ESP_LOGCONFIG(TAG, "Setting up pins...");
            initializePushButton(BUTTON_CH);
            initializePushButton(BUTTON_UP);
            initializePushButton(BUTTON_STOP);
            initializePushButton(BUTTON_DOWN);

            ESP_LOGCONFIG(TAG, "Setting up ADC...");
            Wire.begin(ADS_SDA, ADS_SCL);
            ADS.begin();

            led_0 = false;
            led_1 = false;
            led_2 = false;
            led_3 = false;
            led_all = false;
        }

        cover::CoverTraits Somphy_Shades_Controller::get_traits()
        {
            auto traits = cover::CoverTraits();
            traits.set_is_assumed_state(true);
            traits.set_supports_position(false);
            traits.set_supports_tilt(false);
            traits.set_supports_stop(true);
            return traits;
        }

        float Somphy_Shades_Controller::get_setup_priority() const
        {
            return setup_priority::DATA;
        }

        void Somphy_Shades_Controller::loop()
        {
          if (this->current_operation == cover::COVER_OPERATION_IDLE)
            return;
        }

        void Somphy_Shades_Controller::control(const cover::CoverCall &call)
        {
            // This will be called every time the user requests a state change.
            if (call.get_position().has_value())
            {
                SelectDeisredChannel(channel_);
                float pos = *call.get_position();
                if(pos > 0.95)
                {
                    simulatePushbuttonPress(BUTTON_DOWN);
                }
                if(pos < 0.05)
                {
                    simulatePushbuttonPress(BUTTON_UP);
                }
                // Publish new state
                this->position = pos;
                this->publish_state();
            }
            if (call.get_stop())
            {
                SelectDeisredChannel(channel_);
                simulatePushbuttonPress(BUTTON_STOP);
                this->publish_state();
            }
        }

        void Somphy_Shades_Controller::dump_config() { ESP_LOGCONFIG(TAG, "Somphy_Shades_Controller"); }

        void Somphy_Shades_Controller::simulatePushbuttonPress(uint8_t push_button)
        {
            pinMode(push_button, OUTPUT);
            digitalWrite(push_button, LOW);
            delay(100);
            pinMode(push_button, INPUT);
        }

        void Somphy_Shades_Controller::initializePushButton(uint8_t push_button)
        {
            digitalWrite(push_button, LOW);
            pinMode(push_button, INPUT);
        }

        void Somphy_Shades_Controller::readChannelLEDs()
        {
            ADS.setGain(0);
            int16_t val_0 = ADS.readADC(3);
            int16_t val_1 = ADS.readADC(2);
            int16_t val_2 = ADS.readADC(1);
            int16_t val_3 = ADS.readADC(0);

            led_0 = false;
            led_1 = false;
            led_2 = false;
            led_3 = false;
            led_all = false;

            led_0 = val_0 > 5000;
            led_1 = val_1 > 5000;
            led_2 = val_2 > 5000;
            led_3 = val_3 > 5000;
            led_all = led_0 && led_1 && led_2 && led_3;

            if (led_all)
            {
                led_0 = led_1 = led_2 = led_3 = false;
            }
        }

        int Somphy_Shades_Controller::GetCurrentChannel()
        {
            readChannelLEDs();
            if (led_0)
            {
                return 0;
            }
            if (led_1)
            {
                return 1;
            }
            if (led_2)
            {
                return 2;
            }
            if (led_3)
            {
                return 3;
            }
            if (led_all)
            {
                return 4;
            }
            return -1;
        }

        bool Somphy_Shades_Controller::SelectDeisredChannel(int desired_channel)
        {
            ESP_LOGCONFIG(TAG, "\t\tStart selecting desired channel\n");
            int current_channel = GetCurrentChannel();
            if (current_channel < 0)
            {
                simulatePushbuttonPress(BUTTON_CH);
                delay(100);
            }
            current_channel = GetCurrentChannel();
            if (current_channel < 0)
            {
                return false;
            }
            int channel_difference = desired_channel - current_channel;
            ESP_LOGCONFIG(TAG, "\t\tBefore - Des: %d\tCur: %d\tDif: %d\n", desired_channel, current_channel, channel_difference);
            if (channel_difference >= 0)
            { // press channel button channel_difference times
                for (int i = 0; i < channel_difference; ++i)
                {
                    simulatePushbuttonPress(BUTTON_CH);
                    delay(200);
                }
            }
            else
            { // press channel button 5 + channel_difference times
                for (int i = 0; i < 5 + channel_difference; ++i)
                {
                    simulatePushbuttonPress(BUTTON_CH);
                    delay(200);
                }
            }
            delay(500);
            current_channel = GetCurrentChannel();
            channel_difference = desired_channel - current_channel;
            ESP_LOGCONFIG(TAG, "\t\tAfter  - Des: %d\tCur: %d\tDif: %d\n", desired_channel, current_channel, channel_difference);
            if (current_channel >= 0 && current_channel == desired_channel)
            {
                return true;
            }
            return false;
        }
    } // namespace somphy_shades_controller
} // namespace esphome
