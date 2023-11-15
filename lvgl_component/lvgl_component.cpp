
#include "esphome.h"
#include "lvgl.h"
#include "TFT_eSPI.h"
#include "bootlogo.h"

#include "esphome/core/log.h"
#include "lvgl_component.h"

#include "esphome/core/component.h"
#include "esphome/components/number/number.h"

#include <Wire.h>
#include "FT6336U.h"

#ifndef TFT_INVERT_COLORS
#define TFT_INVERT_COLORS 0
#endif

#define RST_PIN (TOUCH_RST) // -1 if pin is connected to VCC else set pin number

namespace esphome {
namespace lvgl_component {

  FT6336U* ft6336u_touch;
  static inline void FT6336U_drv_init();

  const size_t buf_pix_count = LV_HOR_RES_MAX * LV_VER_RES_MAX / 5;

  static lv_disp_draw_buf_t disp_buf;
  static lv_color_t buf[buf_pix_count];
  lv_style_t switch_style;

  /* LVGL callbacks - Needs to be accessible from C library */
  void IRAM_ATTR my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
  void IRAM_ATTR cap_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
  void IRAM_ATTR gui_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

  TFT_eSPI tft;

  /* Update the TFT - Needs to be accessible from C library */
  void IRAM_ATTR gui_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
  {
    size_t len = lv_area_get_size(area);

    /* Update TFT */
    tft.startWrite();                                      /* Start new TFT transaction */
    tft.setWindow(area->x1, area->y1, area->x2, area->y2); /* set the working window */
#ifdef USE_DMA_TO_TFT
    tft.pushPixelsDMA((uint16_t *)color_p, len); /* Write words at once */
#else
    tft.pushPixels((uint16_t *)color_p, len); /* Write words at once */
#endif
    tft.endWrite(); /* terminate TFT transaction */

    /* Tell lvgl that flushing is done */
    lv_disp_flush_ready(disp);
  }

  /*Read the touchpad - Needs to be accessible from C library */
  void IRAM_ATTR my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
  {
#ifdef TOUCH_CS
    uint16_t touchX, touchY;

    bool touched = tft.getTouch(&touchX, &touchY, 600);

    if (!touched)
    {
      data->state = LV_INDEV_STATE_REL;
    }
    else
    {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;

      // Serial.print("Data x");
      // Serial.print(touchX);

      // Serial.print(" - y");
      // Serial.println(touchY);
    }
#else
    data->state = LV_INDEV_STATE_REL;
#endif
  }

  void IRAM_ATTR cap_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
  {
    if (ft6336u_touch->read_touch_number() == 1)
    {
      // if (hasp_sleep_state != HASP_SLEEP_OFF)
      //   hasp_update_sleep_state(); // update Idle

      data->point.x = ft6336u_touch->read_touch1_x();
      data->point.y = ft6336u_touch->read_touch1_y();
      data->state = LV_INDEV_STATE_PR;
    }
    else
    {
      data->state = LV_INDEV_STATE_REL;
    }
  }

  static inline void FT6336U_drv_init()
  {
      // LOG_INFO(TAG_DRVR, F("Touch SDA     : %d"), TOUCH_SDA);
      // LOG_INFO(TAG_DRVR, F("Touch SCL     : %d"), TOUCH_SCL);
      // LOG_INFO(TAG_DRVR, F("Touch freq.   : %d"), TOUCH_FREQUENCY);
      // LOG_INFO(TAG_DRVR, F("Touch address : %x"), I2C_ADDR_FT6336U);

      ft6336u_touch = new FT6336U(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_IRQ);
      ft6336u_touch->begin();

      // From:  M5Core2/src/M5Touch.cpp
      // By default, the FT6336 will pulse the INT line for every touch
      // event. But because it shares the Wire1 TwoWire/I2C with other
      // devices, we cannot easily create an interrupt service routine to
      // handle these events. So instead, we set the INT wire to polled mode,
      // so it simply goes low as long as there is at least one valid touch.
      // ft6336u_touch->writeByte(0xA4, 0x00);
      Wire1.beginTransmission(I2C_ADDR_FT6336U);
      Wire1.write(0xA4); // address
      Wire1.write(0x00); // data
      Wire1.endTransmission();

      // touch_scan(Wire1);

      if(ft6336u_touch->read_chip_id() != 0) {
          printf("FT6336U touch driver started chipid: %d", ft6336u_touch->read_chip_id());
      } else {
          printf("FT6336U touch driver failed to start");
      }
  }
  static const char *TAG = "lvgl_arc.component";

  void tft_splashscreen()
  {
    uint8_t fg[] = logoFgColor;
    uint8_t bg[] = logoBgColor;
    lv_color_t fgColor = lv_color_make(fg[0], fg[1], fg[2]);
    lv_color_t bgColor = lv_color_make(bg[0], bg[1], bg[2]);

    tft.fillScreen(bgColor.full);
    int x = (tft.width() - logoWidth) / 2;
    int y = (tft.height() - logoHeight) / 2;
    tft.drawXBitmap(x, y, logoImage, logoWidth, logoHeight, fgColor.full);
  }

  void tft_setup()
  {
    // This will be called once to set up the component
    // think of it as the setup() call in Arduino
    tft.begin();
    tft.invertDisplay(TFT_INVERT_COLORS);
    tft.setSwapBytes(true); /* set endianess */
    tft.setRotation(TFT_ROTATION);
    tft_splashscreen();

#ifdef TOUCH_CS
    uint16_t calData[5] = {TOUCH_CAL_DATA};
    tft.setTouch(calData);
    #else
            FT6336U_drv_init();
#endif

    delay(250);
  }

  void LVGLComponent::setup() {
      // This will be called once to set up the component
      // think of it as the setup() call in Arduino
      tft_setup();
      lv_init();

#if USE_LV_LOG != 0
      lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

      lv_disp_draw_buf_init(&disp_buf, buf, NULL, buf_pix_count);

      /*Initialize the display*/
      static lv_disp_drv_t disp_drv;
      lv_disp_drv_init(&disp_drv);
      disp_drv.hor_res = TFT_WIDTH;
      disp_drv.ver_res = TFT_HEIGHT;
      disp_drv.flush_cb = gui_flush_cb;
      disp_drv.draw_buf = &disp_buf;
      lv_disp_drv_register(&disp_drv);

      /*Initialize the input device driver*/
      static lv_indev_drv_t indev_drv;
      lv_indev_drv_init(&indev_drv);
      indev_drv.type = LV_INDEV_TYPE_POINTER;
      indev_drv.read_cb = cap_touchpad_read;
      lv_indev_drv_register(&indev_drv);

      // Make unchecked checkboxes darker grey
      lv_style_init(&switch_style);
      lv_style_set_bg_color(&switch_style, lv_palette_main(LV_PALETTE_GREY));
      
      this->high_freq_.start(); // avoid 16 ms delay
    
  }

  void IRAM_ATTR LVGLComponent::loop() {
      lv_timer_handler(); // called by dispatch_loop
  }

void LVGLComponent::dump_config(){
    ESP_LOGCONFIG(TAG, "Empty component");
}


}  // namespace lvgl_component
}  // namespace esphome
