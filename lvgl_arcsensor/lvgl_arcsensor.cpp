#include "esphome/core/log.h"
#include "lvgl_arcsensor.h"

namespace esphome {
namespace lvgl_arcsensor {

static const char *TAG = "LVGLArcSensor.sensor";

static void lvgl_event_cb(lv_event_t *event);

lv_obj_t *obj = NULL;

// constructor

lv_obj_t *LVGLArcSensor::drawArc()
{
  if (obj == NULL){
    obj = lv_arc_create(lv_scr_act());
  }

  lv_obj_set_width( obj, w);
  lv_obj_set_height( obj, h);
  lv_obj_set_align( obj, LV_ALIGN_CENTER );
  lv_arc_set_range(obj, low, high);
  lv_arc_set_value(obj, val);

  // Set Callback
  lv_obj_add_event_cb(obj, lvgl_event_cb, LV_EVENT_VALUE_CHANGED, (void *)(this));
  enable_cb = 1;

  return obj;
}

void LVGLArcSensor::setup() {
    drawArc();
}

void LVGLArcSensor::loop() {
  
}

void LVGLArcSensor::update() {

}
void LVGLArcSensor::setValue(float _val) {
  val = _val;
  if (obj != NULL){
    lv_arc_set_value(obj, val);
  }
}
void LVGLArcSensor::setCentered(bool _val) {
  centered = _val;
  if (val && obj != NULL){
    lv_obj_set_align( obj, LV_ALIGN_CENTER);
  }
}
void LVGLArcSensor::setPosition(int16_t _x, int16_t _y) {
  x = _x;
  y = _y;
  if (obj != NULL){
    lv_obj_set_pos( obj, x, y );
  }
}
void LVGLArcSensor::setSize(int16_t _w, int16_t _h){
  w = _w;
  h = _h;
  if (obj != NULL){
    lv_obj_set_size( obj, w, h );
  }
}
void LVGLArcSensor::setRange(float _low, float _high){
  low = _low;
  high = _high;
  if (obj != NULL){
    lv_arc_set_range( obj, low, high);
  }
}
void LVGLArcSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Empty custom sensor");
}

static void lvgl_event_cb(lv_event_t *event)
{
    lv_obj_t *target = lv_event_get_target(event);
    LVGLArcSensor * call = (LVGLArcSensor *)(event->user_data);

    call->publish_state(lv_arc_get_value(target));
}
} //namespace empty_sensor
} //namespace esphome
