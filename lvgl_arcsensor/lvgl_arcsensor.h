#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "lvgl.h"

namespace esphome {
namespace lvgl_arcsensor {

class LVGLArcSensor : public sensor::Sensor, public PollingComponent {

  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
  int low;
  int high;
  int val;
  bool centered;

  public:
  int enable_cb;
  lv_obj_t* drawArc();
  void setup() override;
  void loop() override;
  void update() override;
  void dump_config() override;
  void setValue(float val);
  void setCentered(bool val);
  void setPosition(int16_t x, int16_t y);
  void setSize(int16_t w, int16_t h);
  void setRange(float low, float high);
};

} //namespace empty_sensor
} //namespace esphome
