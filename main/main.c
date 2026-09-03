// main/main.c —— FoloToy AI Passport BSP 驱动参考示例:初始化 + 菜单 + 按键分发。
//
// 按键语义(全局统一):
//   上/下 短按   菜单中=移动选中项;演示页中=该页自定义
//   确定  短按   菜单中=进入选中项;演示页中=该页自定义
//   确定  长按   演示页中=返回菜单(由本文件统一拦截)
#include "bsp_i2c.h"
#include "bsp_display.h"
#include "bsp_button.h"
#include "bsp_audio.h"
#include "bsp_battery.h"
#include "bsp_pins.h"      // 错误日志里要打印 BSP_LCD_* 引脚号
#include "demo.h"
#include "ui_pixel.h"
#include "lvgl.h"
#include "driver/usb_serial_jtag.h"
#include "driver/usb_serial_jtag_vfs.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "main";

static const demo_entry_t DEMOS[] = {
    { "Display", demo_display_enter, demo_display_exit, demo_display_key },
    { "Button",  demo_button_enter,  demo_button_exit,  demo_button_key  },
    { "Audio",   demo_audio_enter,   demo_audio_exit,   demo_audio_key   },
    { "Battery", demo_battery_enter, demo_battery_exit, demo_battery_key },
    { "Wi-Fi",   demo_wifi_enter,    demo_wifi_exit,    demo_wifi_key    },
    { "TheGreatMe", demo_ble_enter,   demo_ble_exit,     demo_ble_key     },
    { "Low Power", demo_low_power_enter, demo_low_power_exit, demo_low_power_key },
};
#define DEMO_COUNT (sizeof(DEMOS) / sizeof(DEMOS[0]))

// 各外设初始化结果:失败的项在菜单里标 [FAIL] 且不允许进入。
static bool s_ok[DEMO_COUNT];

static lv_obj_t *s_menu_scr;
static lv_obj_t *s_cards[DEMO_COUNT];
static lv_obj_t *s_rows[DEMO_COUNT];
static lv_obj_t *s_mascot;
static int  s_sel;                 // 当前选中项
static int  s_active = -1;         // 当前所在演示页;-1 = 在菜单

// AI Passport 发布助手的只读抓屏协议。抓屏时复用 LVGL 已有的
// 20 行刷新缓冲，不常驻额外整屏副本，也不读写用户数据或设备设置。
#define SCREENSHOT_COMMAND "FAP_SCREENSHOT_V1"
#define SCREENSHOT_HEADER "FAP_SCREENSHOT_V1 240 320 RGB565LE 153600\n"

typedef enum {
    SCREENSHOT_IDLE,
    SCREENSHOT_CHECK,
    SCREENSHOT_WRITE,
} screenshot_mode_t;

static lv_display_t *s_display;
static screenshot_mode_t s_screenshot_mode;
static int s_screenshot_next_y;
static size_t s_screenshot_bytes;
static bool s_screenshot_ok;
static bool s_screenshot_serial_ready;

static bool screenshot_write(const void *data, size_t length) {
    return usb_serial_jtag_write_bytes(data, length, pdMS_TO_TICKS(2000)) == (int)length;
}

// LV_EVENT_FLUSH_START 在面板驱动交换 RGB565 高低字节之前发出，因此此处
// 看到的正是协议需要的 RGB565LE。强制整屏刷新后，现有缓冲会按
// y=0..319 分块到达；任何空洞、乱序或尺寸异常都会使本次抓屏失败。
static void screenshot_flush_event(lv_event_t *event) {
    if (s_screenshot_mode == SCREENSHOT_IDLE || !s_screenshot_ok) return;

    lv_display_t *display = lv_event_get_target(event);
    const lv_area_t *area = lv_event_get_param(event);
    lv_draw_buf_t *draw = lv_display_get_buf_active(display);
    if (!area || !draw || !draw->data ||
        area->x1 != 0 || area->x2 != BSP_LCD_W - 1 ||
        area->y1 != s_screenshot_next_y || area->y2 >= BSP_LCD_H ||
        draw->header.stride < BSP_LCD_W * sizeof(uint16_t)) {
        s_screenshot_ok = false;
        return;
    }

    const size_t row_bytes = BSP_LCD_W * sizeof(uint16_t);
    const int row_count = area->y2 - area->y1 + 1;
    if (s_screenshot_mode == SCREENSHOT_WRITE) {
        for (int row = 0; row < row_count; row++) {
            const uint8_t *pixels = draw->data + row * draw->header.stride;
            if (!screenshot_write(pixels, row_bytes)) {
                s_screenshot_ok = false;
                return;
            }
        }
    }

    s_screenshot_next_y = area->y2 + 1;
    s_screenshot_bytes += row_bytes * row_count;
}

static bool screenshot_refresh(screenshot_mode_t mode) {
    s_screenshot_mode = mode;
    s_screenshot_next_y = 0;
    s_screenshot_bytes = 0;
    s_screenshot_ok = true;

    lv_obj_invalidate(lv_display_get_screen_active(s_display));
    lv_refr_now(s_display);

    bool complete = s_screenshot_ok &&
                    s_screenshot_next_y == BSP_LCD_H &&
                    s_screenshot_bytes == (size_t)BSP_LCD_W * BSP_LCD_H * sizeof(uint16_t);
    s_screenshot_mode = SCREENSHOT_IDLE;
    return complete;
}

static void screenshot_send(void) {
    if (!bsp_lvgl_lock(1000)) {
        static const char error[] = "FAP_SCREENSHOT_ERROR lock\n";
        screenshot_write(error, sizeof(error) - 1);
        return;
    }

    // 先不输出地走一遍完整刷新，确认分块顺序和字节数正确后，
    // 才发送协议头，避免把已知不完整的帧伪装成成功响应。
    bool ready = lv_display_get_horizontal_resolution(s_display) == BSP_LCD_W &&
                 lv_display_get_vertical_resolution(s_display) == BSP_LCD_H &&
                 lv_display_get_color_format(s_display) == LV_COLOR_FORMAT_RGB565 &&
                 screenshot_refresh(SCREENSHOT_CHECK);
    if (!ready) {
        bsp_lvgl_unlock();
        static const char error[] = "FAP_SCREENSHOT_ERROR refresh\n";
        screenshot_write(error, sizeof(error) - 1);
        return;
    }

    esp_log_level_t previous_log_level = esp_log_get_level_master();
    esp_log_set_level_master(ESP_LOG_NONE);
    bool complete = screenshot_write(SCREENSHOT_HEADER, sizeof(SCREENSHOT_HEADER) - 1) &&
                    screenshot_refresh(SCREENSHOT_WRITE);
    usb_serial_jtag_wait_tx_done(pdMS_TO_TICKS(5000));
    esp_log_set_level_master(previous_log_level);
    bsp_lvgl_unlock();

    if (!complete) ESP_LOGE(TAG, "抓屏帧输出不完整");
}

static void screenshot_command_task(void *arg) {
    (void)arg;
    char line[32];
    size_t length = 0;

    while (true) {
        char ch;
        int count = usb_serial_jtag_read_bytes(&ch, 1, portMAX_DELAY);
        if (count != 1) {
            continue;
        }
        if (ch == '\r') continue;
        if (ch == '\n') {
            line[length] = '\0';
            if (strcmp(line, SCREENSHOT_COMMAND) == 0) screenshot_send();
            length = 0;
            continue;
        }
        if (length < sizeof(line) - 1) line[length++] = ch;
        else length = 0;
    }
}

static void menu_refresh(void) {
    for (size_t i = 0; i < DEMO_COUNT; i++) {
        lv_label_set_text_fmt(s_rows[i], "%s%s",
                              DEMOS[i].name,
                              s_ok[i] ? "" : "  [FAIL]");
        ui_pixel_set_selected(s_cards[i], (int)i == s_sel, s_ok[i]);
        lv_obj_set_style_text_color(s_rows[i],
            s_ok[i] ? lv_color_hex(UI_INK) : lv_color_hex(0x7A2020), 0);
    }
}

static void menu_build(void) {
    s_menu_scr = ui_pixel_screen_create("FoloToy");

    for (size_t i = 0; i < DEMO_COUNT; i++) {
        int x = 11 + (int)(i % 2) * 112;
        int y = 52 + (int)(i / 2) * 47;
        s_cards[i] = ui_pixel_panel_create(s_menu_scr, x, y, 102, 40, UI_PAPER);
        s_rows[i] = lv_label_create(s_cards[i]);
        lv_obj_set_style_text_font(s_rows[i], &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_align(s_rows[i], LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_center(s_rows[i]);
    }

    s_mascot = ui_pixel_mascot_create(s_menu_scr, 101, 242);

    menu_refresh();
    lv_screen_load(s_menu_scr);
}

static void enter_menu(void) {
    s_active = -1;
    menu_build();
}

// 按键回调运行在 button 组件的任务里,操作 LVGL 必须加锁。
static void on_key(bsp_btn_t btn, bsp_btn_ev_t ev, void *user) {
    (void)user;
    if (!bsp_lvgl_lock(500)) return;

    if (s_active >= 0) {
        if (btn == BSP_BTN_OK && ev == BSP_BTN_LONG) {     // 统一返回
            DEMOS[s_active].exit();
            enter_menu();
        } else {
            DEMOS[s_active].key(btn, ev);
        }
    } else if (ev == BSP_BTN_CLICK) {
        if (btn == BSP_BTN_UP)   { s_sel = (s_sel + DEMO_COUNT - 1) % DEMO_COUNT; menu_refresh(); }
        if (btn == BSP_BTN_DOWN) { s_sel = (s_sel + 1) % DEMO_COUNT;              menu_refresh(); }
        if (btn == BSP_BTN_OK && s_ok[s_sel]) {
            s_active = s_sel;
            ui_pixel_mascot_jump(s_mascot);
            lv_obj_delete(s_menu_scr);
            s_menu_scr = NULL;
            s_mascot = NULL;
            DEMOS[s_active].enter();
        } else if (btn == BSP_BTN_UP || btn == BSP_BTN_DOWN) {
            ui_pixel_mascot_jump(s_mascot);
        }
    }
    bsp_lvgl_unlock();
}

void app_main(void) {
    // 启动早期就接管 USB 串口，让主机打开端口导致的重启期间也能尽早
    // 缓存抓屏命令。VFS 同步切换到该驱动，普通启动日志仍沿用同一端口。
    usb_serial_jtag_driver_config_t serial_config = {
        .tx_buffer_size = 2048,
        .rx_buffer_size = 256,
    };
    s_screenshot_serial_ready =
        usb_serial_jtag_driver_install(&serial_config) == ESP_OK;
    if (s_screenshot_serial_ready) usb_serial_jtag_vfs_use_driver();

    ESP_LOGI(TAG, "FoloToy AI Passport BSP demo 启动");
    esp_sleep_wakeup_cause_t wakeup = esp_sleep_get_wakeup_cause();
    if (wakeup != ESP_SLEEP_WAKEUP_UNDEFINED) {
        ESP_LOGI(TAG, "休眠唤醒原因: %d", wakeup);
    }

    bsp_i2c_init();
    bsp_i2c_scan();

    // 屏幕是本 demo 的 UI 载体,失败就没有菜单可言 —— 打清楚日志后退出,
    // 不做"串口菜单"降级(那会让本文件复杂一倍,违背参考示例的初衷)。
    if (bsp_display_init() != ESP_OK || !(s_display = bsp_lvgl_init())) {
        ESP_LOGE(TAG, "显示/LVGL 初始化失败,demo 无法继续。"
                      "检查 SPI 接线(MOSI=%d SCLK=%d CS=%d DC=%d BL=%d)",
                 BSP_LCD_MOSI, BSP_LCD_SCLK, BSP_LCD_CS, BSP_LCD_DC, BSP_LCD_BL);
        return;
    }
    bsp_display_backlight(100);
    lv_display_add_event_cb(s_display, screenshot_flush_event, LV_EVENT_FLUSH_START, NULL);

    // 其余外设单项失败不阻塞:菜单里标 [FAIL],其他项照常可测。
    s_ok[0] = true;                                   // Display 已确认可用
    s_ok[1] = (bsp_button_init(on_key, NULL) == ESP_OK);
    s_ok[2] = (bsp_audio_init() == ESP_OK);
    s_ok[3] = (bsp_battery_init() == ESP_OK);
    s_ok[4] = true;                                    // 页面内按需初始化并显示错误
    s_ok[5] = true;
    s_ok[6] = true;

    // The shipping experience is the TheGreatMe terminal, so start its BLE
    // companion immediately after boot. The shared long-OK handler still exits
    // to the hardware demo menu when board diagnostics are needed.
    if (bsp_lvgl_lock(1000)) {
        s_sel = 5;
        s_active = 5;
        DEMOS[s_active].enter();
        bsp_lvgl_unlock();
    }

    // 完整重绘会比单纯命令读取多占用约 3 KB 栈，6 KB 给 LVGL 调用链留余量。
    if (s_screenshot_serial_ready &&
        xTaskCreate(screenshot_command_task, "screen_capture", 6144, NULL, 3, NULL) != pdPASS) {
        ESP_LOGE(TAG, "抓屏命令任务创建失败");
    } else if (!s_screenshot_serial_ready) {
        ESP_LOGE(TAG, "抓屏串口驱动初始化失败");
    }

    ESP_LOGI(TAG, "就绪:Display=%d Button=%d Audio=%d Battery=%d",
             s_ok[0], s_ok[1], s_ok[2], s_ok[3]);
}
