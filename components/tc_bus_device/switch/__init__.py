import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv
from esphome.const import (
    ENTITY_CATEGORY_CONFIG,
    DEVICE_CLASS_SWITCH
)
from .. import CONF_TC_BUS_DEVICE_ID, TCBusDeviceComponent, tc_bus_ns

ForceLongDoorOpenerProtocolSwitch = tc_bus_ns.class_("ForceLongDoorOpenerProtocolSwitch", switch.Switch, cg.Component)

CONF_FORCE_LONG_DOOR_OPENER_PROTOCOL = "force_long_door_opener_protocol"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_TC_BUS_DEVICE_ID): cv.use_id(TCBusDeviceComponent),
        cv.Optional(CONF_FORCE_LONG_DOOR_OPENER_PROTOCOL): switch.switch_schema(
            ForceLongDoorOpenerProtocolSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            icon="mdi:flash-alert"
        ),
    }
)

async def to_code(config):
    tc_bus_device_component = await cg.get_variable(config[CONF_TC_BUS_DEVICE_ID])

    if force_long_door_opener_protocol := config.get(CONF_FORCE_LONG_DOOR_OPENER_PROTOCOL):
        s = await switch.new_switch(force_long_door_opener_protocol)
        await cg.register_parented(s, config[CONF_TC_BUS_DEVICE_ID])
        cg.add(tc_bus_device_component.set_force_long_door_opener_protocol_switch(s))