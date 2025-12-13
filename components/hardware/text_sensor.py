import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID

hardware_ns = cg.esphome_ns.namespace("hardware")
HardwareTextSensor = hardware_ns.class_(
    "HardwareTextSensor", cg.Component
)

CONF_PRODUCT = "product"
CONF_REVISION = "revision"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(HardwareTextSensor),
        cv.Optional(CONF_PRODUCT): text_sensor.text_sensor_schema(
            HardwareTextSensor,
        ).extend(),
        cv.Optional(CONF_REVISION): text_sensor.text_sensor_schema(
            HardwareTextSensor,
        ).extend(),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if CONF_PRODUCT in config:
        sens = await text_sensor.new_text_sensor(config[CONF_PRODUCT])
        cg.add(var.set_product(sens))

    if CONF_REVISION in config:
        sens = await text_sensor.new_text_sensor(config[CONF_REVISION])
        cg.add(var.set_revision(sens))
