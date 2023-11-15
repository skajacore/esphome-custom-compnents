import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (CONF_ID, UNIT_CELSIUS, ICON_THERMOMETER)

lvgl_arcsensor_ns = cg.esphome_ns.namespace('lvgl_arcsensor')

LVGLArcSensor = lvgl_arcsensor_ns.class_('LVGLArcSensor', cg.PollingComponent)

CONF_ARCSNSR = 'arcsensor'

CONF_SIZE = 'size'
CONF_POS = 'position'
CONF_X = 'x'
CONF_Y = 'y'
CONF_W = 'w'
CONF_H = 'h'
CONF_LOW = 'low'
CONF_HIGH = 'high'
CONF_VAL = 'val'
CONF_CENTERED = 'centered'

POSITION_SCHEMA = cv.Schema({
    cv.Required(CONF_X): cv.int_,
    cv.Required(CONF_Y): cv.int_,
})

SIZE_SCHEMA = cv.Schema({
    cv.Required(CONF_W): cv.int_,
    cv.Required(CONF_H): cv.int_,
})

ARC_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_LOW): cv.float_,
        cv.Required(CONF_HIGH): cv.float_,
        cv.Required(CONF_VAL): cv.float_,
        cv.Required(CONF_SIZE): SIZE_SCHEMA,
        cv.Optional(CONF_CENTERED, default=False): cv.boolean,
        cv.Optional(CONF_POS, default=({CONF_X: 0, CONF_Y: 0})): POSITION_SCHEMA,
    }
)

CONFIG_SCHEMA = sensor.sensor_schema(unit_of_measurement=UNIT_CELSIUS, icon=ICON_THERMOMETER, accuracy_decimals=1).extend(
    {
        cv.GenerateID(): cv.declare_id(LVGLArcSensor),
        cv.Required(CONF_ARCSNSR): ARC_SCHEMA,
    }
).extend(cv.polling_component_schema('60s'))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)
    
    arcSensorConfig = config[CONF_ARCSNSR]

    sizeConfig = arcSensorConfig[CONF_SIZE]
    cg.add(var.setSize(sizeConfig[CONF_W],sizeConfig[CONF_H]))
    
    posConfig = arcSensorConfig[CONF_POS]
    cg.add(var.setPosition(posConfig[CONF_X],posConfig[CONF_Y]))

    cg.add(var.setRange(arcSensorConfig[CONF_LOW],arcSensorConfig[CONF_HIGH]))
    cg.add(var.setValue(arcSensorConfig[CONF_VAL]))
    
    cg.add(var.setCentered(arcSensorConfig[CONF_CENTERED]))
    
    #x = yield cg.get_variable(config[CONF_X]);
    #y = yield cg.get_variable(config[CONF_Y]);
    #w = yield cg.get_variable(config[CONF_W]);
    #h = yield cg.get_variable(config[CONF_H]);
    #low = yield cg.get_variable(config[CONF_LOW]);
    #high = yield cg.get_variable(config[CONF_HIGH]);
    #val = yield cg.get_variable(config[CONF_VAL]);
    
