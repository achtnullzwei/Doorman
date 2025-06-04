from esphome import automation
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, lock, sensor, binary_sensor, event, button, switch, climate, fan
from esphome.const import PLATFORM_ESP32, CONF_ID, CONF_TRIGGER_ID, CONF_LIGHT, CONF_SENSOR, CONF_BINARY_SENSOR, CONF_BUTTON, CONF_EVENT
from esphome.core import ID
from esphome.components.esp32 import add_idf_component
from .. import homekit_base

AUTO_LOAD = ["homekit_base"]
DEPENDENCIES = ['esp32', 'network', 'homekit_base']
CODEOWNERS = ["@rednblkx"]

homekit_ns = homekit_base.homekit_ns
HomeKitBaseComponent = homekit_base.HomeKitBaseComponent

TemperatureUnits = homekit_ns.enum("TemperatureUnits")
AInfo = homekit_ns.enum("AInfo")
HKFinish = homekit_ns.enum("HKFinish")
HAPAccessory = homekit_ns.class_('HAPAccessory', cg.Component)
LightEntity = homekit_ns.class_('LightEntity')
SensorEntity = homekit_ns.class_('SensorEntity')
BinarySensorEntity = homekit_ns.class_('BinarySensorEntity')
EventEntity = homekit_ns.class_('EventEntity')
ButtonEntity = homekit_ns.class_('ButtonEntity')
SwitchEntity = homekit_ns.class_('SwitchEntity')
ClimateEntity = homekit_ns.class_('ClimateEntity')
LockEntity = homekit_ns.class_('LockEntity')
FanEntity = homekit_ns.class_('FanEntity')

OnHKIdentifyTrigger = homekit_ns.class_(
    "HKIdentifyTrigger", automation.Trigger.template()
)

CONF_IDENTIFY = "on_identify"
CONF_HK_SUCCESS = "on_hk_success"
CONF_HK_FAIL = "on_hk_fail"

CONF_LOCK = "lock"
CONF_FAN = "fan"
CONF_SWITCH = "switch"
CONF_CLIMATE = "climate"
CONF_META = "meta"
CONF_TEMP_UNITS = "temp_units"

CONF_HOMEKIT_BASE = "homekit_base_id"

TEMP_UNITS = {
    "CELSIUS": TemperatureUnits.CELSIUS,
    "FAHRENHEIT": TemperatureUnits.FAHRENHEIT
}

ACC_INFO = {
    "name": AInfo.NAME,
    "model": AInfo.MODEL,
    "manufacturer": AInfo.MANUFACTURER,
    "serial_number": AInfo.SN,
    "fw_rev": AInfo.FW_REV,
}

HK_HW_FINISH = {
    "TAN": HKFinish.TAN,
    "GOLD": HKFinish.GOLD,
    "SILVER": HKFinish.SILVER,
    "BLACK": HKFinish.BLACK
}

ACCESSORY_INFORMATION = {
    cv.Optional(i): cv.string for i in ACC_INFO
}

CONFIG_SCHEMA = cv.All(cv.Schema({
    cv.GenerateID() : cv.declare_id(HAPAccessory),
    cv.GenerateID(CONF_HOMEKIT_BASE): cv.use_id(
        homekit_base.HomeKitBaseComponent
    ),
    cv.Optional(CONF_LIGHT): cv.ensure_list({
        cv.Required(CONF_ID): cv.use_id(light.LightState),
        cv.Optional(CONF_META) : ACCESSORY_INFORMATION,
        cv.Optional(CONF_IDENTIFY): automation.validate_automation({
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OnHKIdentifyTrigger),
        }),
    }),
    cv.Optional(CONF_LOCK): cv.ensure_list({
        cv.Required(CONF_ID): cv.use_id(lock.Lock),
        cv.Optional(CONF_META) : ACCESSORY_INFORMATION,
        cv.Optional(CONF_IDENTIFY): automation.validate_automation({
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OnHKIdentifyTrigger),
        }),
    }),
    cv.Optional(CONF_SENSOR): cv.ensure_list({
        cv.Required(CONF_ID): cv.use_id(sensor.Sensor),
        cv.Optional(CONF_TEMP_UNITS, default="CELSIUS") : cv.enum(TEMP_UNITS),
        cv.Optional(CONF_META) : ACCESSORY_INFORMATION,
        cv.Optional(CONF_IDENTIFY): automation.validate_automation({
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OnHKIdentifyTrigger),
        }),
    }),
    cv.Optional(CONF_BINARY_SENSOR): cv.ensure_list({
        cv.Required(CONF_ID): cv.use_id(binary_sensor.BinarySensor),
        cv.Optional(CONF_META) : ACCESSORY_INFORMATION,
        cv.Optional(CONF_IDENTIFY): automation.validate_automation({
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OnHKIdentifyTrigger),
        }),
    }),
    cv.Optional(CONF_EVENT): cv.ensure_list({
        cv.Required(CONF_ID): cv.use_id(event.Event),
        cv.Optional(CONF_META) : ACCESSORY_INFORMATION,
        cv.Optional(CONF_IDENTIFY): automation.validate_automation({
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OnHKIdentifyTrigger),
        }),
    }),
    cv.Optional(CONF_BUTTON): cv.ensure_list({
        cv.Required(CONF_ID): cv.use_id(button.Button),
        cv.Optional(CONF_META) : ACCESSORY_INFORMATION,
        cv.Optional(CONF_IDENTIFY): automation.validate_automation({
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OnHKIdentifyTrigger),
        }),
    }),
    cv.Optional(CONF_FAN): cv.ensure_list({
        cv.Required(CONF_ID): cv.use_id(fan.Fan),
        cv.Optional(CONF_META) : ACCESSORY_INFORMATION,
        cv.Optional(CONF_IDENTIFY): automation.validate_automation({
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OnHKIdentifyTrigger),
        }),
    }),
    cv.Optional(CONF_SWITCH): cv.ensure_list({
        cv.Required(CONF_ID): cv.use_id(switch.Switch),
        cv.Optional(CONF_META) : ACCESSORY_INFORMATION,
        cv.Optional(CONF_IDENTIFY): automation.validate_automation({
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OnHKIdentifyTrigger),
        }),
    }),
    cv.Optional(CONF_CLIMATE): cv.ensure_list({
        cv.Required(CONF_ID): cv.use_id(climate.Climate),
        cv.Optional(CONF_META) : ACCESSORY_INFORMATION,
        cv.Optional(CONF_IDENTIFY): automation.validate_automation({
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OnHKIdentifyTrigger),
        }),
    }),
}).extend(cv.COMPONENT_SCHEMA),
cv.only_on([PLATFORM_ESP32]),
cv.only_with_esp_idf)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    homekit_base_component = await cg.get_variable(config[CONF_HOMEKIT_BASE])
    cg.add(var.set_base_component(homekit_base_component))

    if 'light' in config:
        for l in config["light"]:
            light_entity = cg.Pvariable(ID(f"{l['id'].id}_hk_light_entity", type=LightEntity), var.add_light(await cg.get_variable(l['id'])))
            
            if "meta" in l:
                info_temp = []
                for m in l["meta"]:
                    info_temp.append([ACC_INFO[m], l["meta"][m]])
                cg.add(light_entity.set_meta(info_temp))

            for conf in l.get(CONF_IDENTIFY, []):
                trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
                cg.add(light_entity.register_on_identify_trigger(trigger))
                await automation.build_automation(trigger, [], conf)

    if 'sensor' in config:
        for l in config["sensor"]:
            sensor_entity = cg.Pvariable(ID(f"{l['id'].id}_hk_sensor_entity", type=SensorEntity), var.add_sensor(await cg.get_variable(l['id']), l['temp_units']))
            
            if "meta" in l:
                info_temp = []
                for m in l["meta"]:
                    info_temp.append([ACC_INFO[m], l["meta"][m]])
                cg.add(sensor_entity.set_meta(info_temp))

            for conf in l.get(CONF_IDENTIFY, []):
                trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
                cg.add(sensor_entity.register_on_identify_trigger(trigger))
                await automation.build_automation(trigger, [], conf)

    if 'binary_sensor' in config:
        for l in config["binary_sensor"]:
            binary_sensor_entity = cg.Pvariable(ID(f"{l['id'].id}_hk_binary_sensor_entity", type=BinarySensorEntity), var.add_binary_sensor(await cg.get_variable(l['id'])))
            
            if "meta" in l:
                info_temp = []
                for m in l["meta"]:
                    info_temp.append([ACC_INFO[m], l["meta"][m]])
                cg.add(binary_sensor_entity.set_meta(info_temp))

            for conf in l.get(CONF_IDENTIFY, []):
                trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
                cg.add(binary_sensor_entity.register_on_identify_trigger(trigger))
                await automation.build_automation(trigger, [], conf)

    if 'event' in config:
        for l in config["event"]:
            event_entity = cg.Pvariable(ID(f"{l['id'].id}_hk_event_entity", type=EventEntity), var.add_event(await cg.get_variable(l['id'])))
            
            if "meta" in l:
                info_temp = []
                for m in l["meta"]:
                    info_temp.append([ACC_INFO[m], l["meta"][m]])
                cg.add(event_entity.set_meta(info_temp))

            for conf in l.get(CONF_IDENTIFY, []):
                trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
                cg.add(event_entity.register_on_identify_trigger(trigger))
                await automation.build_automation(trigger, [], conf)

    if 'button' in config:
        for l in config["button"]:
            button_entity = cg.Pvariable(ID(f"{l['id'].id}_hk_button_entity", type=ButtonEntity), var.add_button(await cg.get_variable(l['id'])))
            
            if "meta" in l:
                info_temp = []
                for m in l["meta"]:
                    info_temp.append([ACC_INFO[m], l["meta"][m]])
                cg.add(button_entity.set_meta(info_temp))

            for conf in l.get(CONF_IDENTIFY, []):
                trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
                cg.add(button_entity.register_on_identify_trigger(trigger))
                await automation.build_automation(trigger, [], conf)

    if 'lock' in config:
        for l in config["lock"]:
            lock_entity = cg.Pvariable(ID(f"{l['id'].id}_hk_lock_entity", type=LockEntity), var.add_lock(await cg.get_variable(l['id'])))

            if "meta" in l:
                info_temp = []
                for m in l["meta"]:
                    info_temp.append([ACC_INFO[m], l["meta"][m]])
                cg.add(lock_entity.set_meta(info_temp))

            for conf in l.get(CONF_IDENTIFY, []):
                trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
                cg.add(lock_entity.register_on_identify_trigger(trigger))
                await automation.build_automation(trigger, [], conf)

    if "fan" in config:
        for l in config["fan"]:
            fan_entity = cg.Pvariable(ID(f"{l['id'].id}_hk_fan_entity", type=FanEntity), var.add_fan(await cg.get_variable(l['id'])))
            
            if "meta" in l:
                info_temp = []
                for m in l["meta"]:
                    info_temp.append([ACC_INFO[m], l["meta"][m]])
                cg.add(fan_entity.set_meta(info_temp))

            for conf in l.get(CONF_IDENTIFY, []):
                trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
                cg.add(fan_entity.register_on_identify_trigger(trigger))
                await automation.build_automation(trigger, [], conf)

    if "switch" in config:
        for l in config["switch"]:
            switch_entity = cg.Pvariable(ID(f"{l['id'].id}_hk_switch_entity", type=SwitchEntity), var.add_switch(await cg.get_variable(l['id'])))
            
            if "meta" in l:
                info_temp = []
                for m in l["meta"]:
                    info_temp.append([ACC_INFO[m], l["meta"][m]])
                cg.add(switch_entity.set_meta(info_temp))
            
            for conf in l.get(CONF_IDENTIFY, []):
                trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
                cg.add(switch_entity.register_on_identify_trigger(trigger))
                await automation.build_automation(trigger, [], conf)

    if "climate" in config:
        for l in config["climate"]:
            climate_entity = cg.Pvariable(ID(f"{l['id'].id}_hk_climate_entity", type=ClimateEntity), var.add_climate(await cg.get_variable(l['id'])))
            
            if "meta" in l:
                info_temp = []
                for m in l["meta"]:
                    info_temp.append([ACC_INFO[m], l["meta"][m]])
                cg.add(climate_entity.set_meta(info_temp))

            for conf in l.get(CONF_IDENTIFY, []):
                trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID])
                cg.add(climate_entity.register_on_identify_trigger(trigger))
                await automation.build_automation(trigger, [], conf)
