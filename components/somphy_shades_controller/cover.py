import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import cover
from esphome.const import (
    CONF_ID,
)
from esphome.cpp_helpers import gpio_pin_expression

CONF_SHADE_CHANNEL = "shade_channel"

somphy_shades_controller_ns = cg.esphome_ns.namespace("somphy_shades_controller")
Somphy_Shades_Controller = somphy_shades_controller_ns.class_(
    "Somphy_Shades_Controller", cover.Cover, cg.Component)

CONFIG_SCHEMA = (
    cover.COVER_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(Somphy_Shades_Controller),
            cv.Required(CONF_SHADE_CHANNEL): cv.int_range(0, 5),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cover.register_cover(var, config)
    cg.add(var.set_channel(config[CONF_SHADE_CHANNEL]))