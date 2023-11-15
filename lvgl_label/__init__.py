import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome import automation
import enum

lvgl_label_ns = cg.esphome_ns.namespace('lvgl_label')
LVGLLabel = lvgl_label_ns.class_('LVGLLabel', cg.Component)

# Actions
TextSetAction = lvgl_label_ns.class_("TextSetAction", automation.Action)

LV_Align = lvgl_label_ns.enum("LVGLLabel::LV_ALIGN")
LV_ALIGN = {
    "LEFT": LV_Align.LEFT,
    "CENTER": LV_Align.CENTER,
    "RIGHT": LV_Align.RIGHT,
}
validate_align = cv.enum(LV_ALIGN, upper=True)

LV_Label_Long = lvgl_label_ns.enum("LV_LABEL_LONG")
LV_LABEL_LONG = {
    "EXPAND": LV_Label_Long.EXPAND,
    "BREAK": LV_Label_Long.BREAK,
    "DOT": LV_Label_Long.DOT,
    "SCROLL": LV_Label_Long.SCROLL,
    "CIRCULAR": LV_Label_Long.CIRCULAR,
    "CROP": LV_Label_Long.CROP,
}
validate_label_long = cv.enum(LV_LABEL_LONG, upper=True)

CONF_TEXTSIZE = 'text-size'
CONF_ALIGN = 'align'

POSITION_SCHEMA = cv.Schema({
    cv.Required('x'): cv.int_,
    cv.Required('y'): cv.int_,
})

SIZE_SCHEMA = cv.Schema({
    cv.Required('w'): cv.int_,
    cv.Required('h'): cv.int_,
})

LABEL_SCHEMA = cv.Schema({
    cv.Required('text'): cv.templatable(cv.string),
    cv.Required('text-size'): cv.int_,
    cv.Required('position'): POSITION_SCHEMA,
    cv.Required('size'): SIZE_SCHEMA,
    cv.Optional('align'): cv.templatable(validate_align),
    cv.Optional('label-long'): cv.templatable(validate_label_long),
})

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(LVGLLabel),
    cv.Required('label'): LABEL_SCHEMA,
}).extend(cv.COMPONENT_SCHEMA)

def lvgl_label_to_code(config, condition_id, template_arg, args):
    var = cg.new_Pvariable(condition_id, template_arg)
    yield cg.register_parented(var, config[CONF_ID])
    template_ = yield cg.templatable(labelConfig['text'], args, cv.string)
    cg.add(var.setText(template_.c_str()))
    yield var

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    labelConfig = config['label']

    sizeConfig = labelConfig['size']
    cg.add(var.setSize(sizeConfig['w'],sizeConfig['h']))
    
    posConfig = labelConfig['position']
    cg.add(var.setPosition(posConfig['x'],posConfig['y']))

    template_ = await cg.templatable(labelConfig['text'], {}, cg.std_string)
    cg.add(var.setText(template_))

    if 'align' in labelConfig:
        cg.add(var.setAlign(labelConfig['align']))
    
    if 'label-long' in labelConfig:
        cg.add(var.setLabelLong(labelConfig['label-long']))

OPERATION_BASE_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_ID): cv.use_id(LVGLLabel),
    }
)

@automation.register_action(
    "text.set",
    TextSetAction,
    OPERATION_BASE_SCHEMA.extend(
        {
            cv.Required('text'): cv.templatable(cv.string_strict),
        }
    ),
)
async def text_set_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = await cg.templatable(labelConfig['text'], args, cg.std_string)
    cg.add(var.setText(template_))
    return var
