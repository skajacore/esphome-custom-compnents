#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace lvgl_component {

class LVGLComponent : public Component {
 private:
  uint16_t l, w, x, y;
  uint16_t min, max, val;

  HighFrequencyLoopRequester high_freq_;
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
};


}  // namespace lvgl_component
}  // namespace esphome
