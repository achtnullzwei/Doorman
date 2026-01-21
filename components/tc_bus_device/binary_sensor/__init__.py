import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID, CONF_ICON, CONF_TYPE
from .. import tc_bus_ns, TCBusDeviceComponent, CONF_TC_BUS_DEVICE_ID
from ...tc_bus import TELEGRAM_TYPES

DeviceTelegramListenerBinarySensor = tc_bus_ns.class_(
    "DeviceTelegramListenerBinarySensor",
    binary_sensor.BinarySensor,
    cg.Component
)

CONF_ADDRESS = "address"
CONF_PAYLOAD = "payload"

CONF_NAME = "name"
CONF_AUTO_OFF = "auto_off"

DEPENDENCIES = ["tc_bus"]

def validate(config):
    config = config.copy()

    return config

CONFIG_SCHEMA = cv.All(
    binary_sensor.binary_sensor_schema(DeviceTelegramListenerBinarySensor).extend(
        {
            cv.GenerateID(): cv.declare_id(DeviceTelegramListenerBinarySensor),
            cv.GenerateID(CONF_TC_BUS_DEVICE_ID): cv.use_id(TCBusDeviceComponent),
            cv.Required(CONF_TYPE): cv.enum(TELEGRAM_TYPES, upper=False),
            cv.Optional(CONF_ADDRESS): cv.templatable(cv.hex_uint8_t),
            cv.Optional(CONF_PAYLOAD): cv.templatable(cv.hex_uint32_t),
            cv.Optional(CONF_ICON, default="mdi:doorbell"): cv.icon,
            cv.Optional(CONF_NAME, default="Doorbell"): cv.string,
            cv.Optional(CONF_AUTO_OFF, default="3s"): cv.positive_time_period_milliseconds
        }
    ),
    validate,
)

async def new_binary_sensor(config, *args):
    var = cg.new_Pvariable(config[CONF_ID], *args)
    await binary_sensor.register_binary_sensor(var, config)
    return var

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await binary_sensor.register_binary_sensor(var, config)

    cg.add(var.set_type(config[CONF_TYPE]))

    if CONF_ADDRESS in config:
        telegram_address = await cg.templatable(config[CONF_ADDRESS], [], cg.uint8)
        cg.add(var.set_address(telegram_address))

    if CONF_PAYLOAD in config:
        telegram_payload = await cg.templatable(config[CONF_PAYLOAD], [], cg.uint32)
        cg.add(var.set_payload(telegram_payload))
    
    cg.add(var.set_auto_off(config[CONF_AUTO_OFF]))

    tc_bus_device = await cg.get_variable(config[CONF_TC_BUS_DEVICE_ID])
    cg.add(tc_bus_device.register_listener(var))
