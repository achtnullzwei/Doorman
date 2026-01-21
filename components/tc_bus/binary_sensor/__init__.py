import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID, CONF_ICON
from .. import tc_bus_ns, TCBusComponent, CONF_TC_BUS_ID, TELEGRAM_TYPES

BusTelegramListenerBinarySensor = tc_bus_ns.class_("BusTelegramListenerBinarySensor", binary_sensor.BinarySensor, cg.Component)

CONF_TELEGRAM = "telegram"

CONF_TYPE = "type"
CONF_ADDRESS = "address"
CONF_PAYLOAD = "payload"
CONF_SERIAL_NUMBER = "serial_number"

CONF_NAME = "name"
CONF_AUTO_OFF = "auto_off"

DEPENDENCIES = ["tc_bus"]

def validate(config):
    config = config.copy()

    has_telegram = CONF_TELEGRAM in config
    has_type = CONF_TYPE in config
    has_address = CONF_ADDRESS in config
    has_payload = CONF_PAYLOAD in config
    has_serial_number = CONF_SERIAL_NUMBER in config

    if not (has_telegram or has_type):
        raise cv.Invalid("You need to set either TELEGRAM or TYPE.")

    if has_telegram:
        if has_type or has_address or has_payload or has_serial_number:
            raise cv.Invalid("You can either set TELEGRAM or TYPE and ADDRESS, PAYLOAD and SERIAL_NUMBER.")
    else:
        if not has_type:
            raise cv.Invalid("You need to set TYPE.")

    return config

CONFIG_SCHEMA = cv.All(
    binary_sensor.binary_sensor_schema(BusTelegramListenerBinarySensor).extend(
        {
            cv.GenerateID(): cv.declare_id(BusTelegramListenerBinarySensor),
            cv.GenerateID(CONF_TC_BUS_ID): cv.use_id(TCBusComponent),
            cv.Optional(CONF_TELEGRAM): cv.hex_uint32_t,
            cv.Optional(CONF_TYPE): cv.enum(TELEGRAM_TYPES, upper=False),
            cv.Optional(CONF_ADDRESS): cv.templatable(cv.hex_uint8_t),
            cv.Optional(CONF_PAYLOAD): cv.templatable(cv.hex_uint32_t),
            cv.Optional(CONF_SERIAL_NUMBER): cv.templatable(cv.hex_uint32_t),
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

    if CONF_TELEGRAM in config:
        telegram = await cg.templatable(config[CONF_TELEGRAM], [], cg.uint32)
        cg.add(var.set_telegram(telegram))

    if CONF_TYPE in config:
        cg.add(var.set_type(config[CONF_TYPE]))

    if CONF_ADDRESS in config:
        telegram_address = await cg.templatable(config[CONF_ADDRESS], [], cg.uint8)
        cg.add(var.set_address(telegram_address))

    if CONF_PAYLOAD in config:
        telegram_payload = await cg.templatable(config[CONF_PAYLOAD], [], cg.uint32)
        cg.add(var.set_payload(telegram_payload))

    if CONF_SERIAL_NUMBER in config:
        telegram_serial_number = await cg.templatable(config[CONF_SERIAL_NUMBER], [], cg.uint32)
        cg.add(var.set_serial_number(telegram_serial_number))

    cg.add(var.set_auto_off(config[CONF_AUTO_OFF]))

    tc_bus = await cg.get_variable(config[CONF_TC_BUS_ID])
    cg.add(tc_bus.register_listener(var))
