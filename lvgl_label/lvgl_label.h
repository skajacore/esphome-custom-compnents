#pragma once

#include "esphome/core/component.h"
#include "lvgl.h"

namespace esphome {
namespace lvgl_label {

enum LV_Align { LEFT, CENTER, RIGHT };
enum LV_Label_Long { EXPAND, BREAK, DOT, SCROLL, CIRCULAR, CROP };

class LVGLLabel : public Component {
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
  int low;
  int high;
  int val;
  LV_Align align;
  LV_Label_Long longmode;
  bool centered;
  std::string text;
  lv_obj_t * obj = NULL;

 public:
  void setSize(int16_t _w, int16_t _h);
  void setPosition(int16_t _x, int16_t _y);
  void setText(std::string _text);
  void setText(std::function<std::string()> &&lambda);
  void setTextSize(int16_t _size);
  void setAlign(LV_Align _align);
  void setLongMode(LV_Label_Long _longmode);
  void setup() override;
  void loop() override;
  void dump_config() override;
};


}  // namespace empty_component
}  // namespace esphome
