#ifndef GUIDER_CUSTOMER_FONTS_H
#define GUIDER_CUSTOMER_FONTS_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "lv_font.h"

#ifdef USE_LVGL

LV_FONT_DECLARE(JetBrainsMono);
LV_FONT_DECLARE(TimesNewRoman);

#define background_img_path "0:/background.bin"
#define _background_480x320 *background_img_path
#define _background_alpha_480x320 *background_img_path

#define lv_font_JetBrainsMono_12 JetBrainsMono
#define lv_font_TimesNewRoman_12 TimesNewRoman
#endif

#ifdef __cplusplus
}
#endif
#endif