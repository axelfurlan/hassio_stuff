import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import cover
from esphome.const import (
    CONF_CLOSE_ACTION,
    CONF_ID,
    CONF_OPEN_ACTION,
    CONF_STOP_ACTION,
    CONF_NAME,
)
from esphome.cpp_helpers import gpio_pin_expression

CONF_SHADES = "shades"
CONF_SHADE0 = "shade_0"
CONF_SHADE1 = "shade_1"
CONF_SHADE2 = "shade_2"
CONF_SHADE3 = "shade_3"
CONF_SHADE_ALL = "shade_all"

somphy_shades_controller_ns = cg.esphome_ns.namespace("somphy_shades_controller")
Somphy_Shades_Controller = somphy_shades_controller_ns.class_("Somphy_Shades_Controller", cg.Component)

DEDICATED_COVER_SCHEMA = cover.COVER_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(Somphy_Shades_Controller),
        cv.Required(CONF_STOP_ACTION): automation.validate_automation(single=True),
        cv.Required(CONF_OPEN_ACTION): automation.validate_automation(single=True),
        cv.Required(CONF_CLOSE_ACTION): automation.validate_automation(single=True),
    }
).extend(cv.COMPONENT_SCHEMA)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Somphy_Shades_Controller),
        cv.Required(CONF_NAME): cv.string_strict,
        cv.Required(CONF_SHADES): cv.Schema(
            {
                cv.Optional(CONF_SHADE0): DEDICATED_COVER_SCHEMA,
                cv.Optional(CONF_SHADE1): DEDICATED_COVER_SCHEMA,
                cv.Optional(CONF_SHADE2): DEDICATED_COVER_SCHEMA,
                cv.Optional(CONF_SHADE3): DEDICATED_COVER_SCHEMA,
                cv.Optional(CONF_SHADE_ALL): DEDICATED_COVER_SCHEMA,
            }
        ),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    for sensor_number, sensor_name in enumerate(config[CONF_SHADES]):
        sensor_config = config[CONF_SHADES][sensor_name]
        # cg.add(var.set_sensor_address(sensor_config["sender_address"], sensor_number))
        # sens = await sensor.new_sensor(sensor_config["temperature"])
        # cg.add(var.set_temperature_sensor(sens, sensor_number))
        # sens = await sensor.new_sensor(sensor_config["humidity"])
        # cg.add(var.set_humidity_sensor(sens, sensor_number))
