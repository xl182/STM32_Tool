//
// Created by ALIEN on 2023/7/26.
//

#include "gui.h"

lv_ui guider_ui;
lv_ui *gui = &guider_ui;

void gui_init() {
    lv_init();
    lv_port_disp_init();
    lv_port_fs_init();
    lv_bmp_init();
    lv_port_indev_init();

    setup_ui(gui);
    events_init(gui);
    lv_task_handler();
}

