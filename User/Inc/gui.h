//
// Created by ALIEN on 2023/7/26.
//

#ifndef TOOL_GUI_H
#define TOOL_GUI_H

#include "lcd.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"
#include "lv_bmp/lv_bmp.h"
#include "gui_guider.h"
#include "events_init.h"

extern lv_ui *gui;

void gui_init();

#endif //TOOL_GUI_H
