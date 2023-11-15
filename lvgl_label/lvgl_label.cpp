#include "esphome/core/log.h"
#include "lvgl_label.h"

namespace esphome {
namespace lvgl_label {

static const char *TAG = "lvgl_label.component";

void LVGLLabel::setSize(int16_t _w, int16_t _h){
  w = _w;
  h = _h;

  if (obj != NULL){
    lv_obj_set_size( obj, w, h );
  }
}
void LVGLLabel::setPosition(int16_t _x, int16_t _y){
  x = _x;
  y = _y;
  if (obj != NULL){
    lv_obj_set_pos( obj, x, y );
  }
}

void LVGLLabel::setText(std::string _text){
  text = _text;
  if (obj != NULL){
    lv_label_set_text(obj, text.c_str());
  }
}
void LVGLLabel::setText(std::function<std::string()> &&lambda){
  text = lambda();
  if (obj != NULL){
    lv_label_set_text(obj, text.c_str());
  }
}

void LVGLLabel::setTextSize(int16_t _size){
}

void LVGLLabel::setAlign(LV_Align _align){
  align = _align;
  if (obj != NULL){
    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
  }
}
void LVGLLabel::setLongMode(LV_Label_Long _longmode){
  longmode = _longmode;
  if (obj != NULL){
    lv_label_set_long_mode(obj,longmode); 
  }
}
void LVGLLabel::setup() {
  if (obj == NULL){
    obj = lv_label_create(lv_scr_act());
  }
  lv_obj_set_pos(obj, x, y);
  lv_obj_set_size( obj, w, h);
  lv_obj_set_align( obj, align );
  lv_label_set_text(obj, "");
  lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_text_font(&style, &lv_font_montserrat_22); // <--- you have to enable other font sizes in menuconfig
  lv_obj_add_style(obj, &style, 0);
  // Set Callback
  //lv_obj_add_event_cb(obj, lvgl_event_cb, LV_EVENT_VALUE_CHANGED, (void *)(this));
  //enable_cb = 1;
}

void LVGLLabel::loop() {

}

void LVGLLabel::dump_config(){
    ESP_LOGCONFIG(TAG, "LVGLLabel");
}


}  // namespace lvgl_label
}  // namespace esphome
