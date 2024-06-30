import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor
from esphome.const import (
    CONF_HUMIDITY,
    CONF_ID,
    CONF_CS_PIN,
    CONF_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_PERCENT,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_HUMIDITY,
    CONF_NAME,
)
from esphome.cpp_helpers import gpio_pin_expression

CONF_SENSORS = "sensors"
CONF_SENSOR0 = "sensor_0"
CONF_SENSOR1 = "sensor_1"
CONF_SENSOR2 = "sensor_2"
CONF_SENSOR3 = "sensor_3"
CONF_SENSOR4 = "sensor_4"
CONF_SENSOR5 = "sensor_5"
CONF_CE_PIN = "ce_pin"
CONF_CHANNEL = "channel"
CONF_SENDER_ADDRESS = "sender_address"

esp_nrf24l01_receiver_ns = cg.esphome_ns.namespace("esp_nrf24l01_receiver")
ESP_NRF24L01_Receiver = esp_nrf24l01_receiver_ns.class_("ESP_NRF24L01_Receiver", cg.Component)

NRF_SENSOR_SCHEMA = cv.Schema(
            {
                cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                    unit_of_measurement=UNIT_CELSIUS,
                    accuracy_decimals=1,
                    device_class=DEVICE_CLASS_TEMPERATURE,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
                    unit_of_measurement=UNIT_PERCENT,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_HUMIDITY,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Required(CONF_SENDER_ADDRESS): cv.string_strict,
            }
        )

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ESP_NRF24L01_Receiver),
        cv.Required(CONF_CS_PIN): pins.internal_gpio_input_pin_schema,
        cv.Required(CONF_CE_PIN): pins.internal_gpio_input_pin_schema,
        cv.Required(CONF_NAME): cv.string_strict,
        cv.Required(CONF_CHANNEL): cv.int_,
        cv.Required(CONF_SENSORS): cv.Schema(
            {
                cv.Optional(CONF_SENSOR0): NRF_SENSOR_SCHEMA,
                cv.Optional(CONF_SENSOR1): NRF_SENSOR_SCHEMA,
                cv.Optional(CONF_SENSOR2): NRF_SENSOR_SCHEMA,
                cv.Optional(CONF_SENSOR3): NRF_SENSOR_SCHEMA,
                cv.Optional(CONF_SENSOR4): NRF_SENSOR_SCHEMA,
                cv.Optional(CONF_SENSOR5): NRF_SENSOR_SCHEMA,
            }
        ),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cs_pin = await gpio_pin_expression(config[CONF_CS_PIN])
    cg.add(var.set_cs_pin(cs_pin))
    
    ce_pin = await gpio_pin_expression(config[CONF_CE_PIN])
    cg.add(var.set_ce_pin(ce_pin))

    cg.add(var.set_channel(config[CONF_CHANNEL]))

    for sensor_number, sensor_name in enumerate(config[CONF_SENSORS]):
        sensor_config = config[CONF_SENSORS][sensor_name]
        cg.add(var.set_sensor_address(sensor_config["sender_address"], sensor_number))
        sens = await sensor.new_sensor(sensor_config["temperature"])
        cg.add(var.set_temperature_sensor(sens, sensor_number))
        sens = await sensor.new_sensor(sensor_config["humidity"])
        cg.add(var.set_humidity_sensor(sens, sensor_number))
