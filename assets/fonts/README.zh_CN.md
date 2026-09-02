<p align="right">
  <strong>简体中文</strong> · <a href="README.md">English</a>
</p>

# 字库资源（Fonts）

本目录存放项目可复用的字库资源。每个字库子目录或单个字库文件，应附说明。

## 终端内嵌字库

`main/lv_font_terminal_zh_16.c` 是由 LVGL 随附的 `SourceHanSansSC-Normal.otf`
生成的 1-bit、16 px LVGL 位图字库，覆盖 ASCII 和简体中文终端使用的 GB2312 字符集。
`main/lv_font_terminal_extra_16.c` 是终端实际使用的小型主字库，只包含本地化终端标签
额外需要的繁体中文、拉丁文、日文与韩文字形，再回退到
`lv_font_terminal_zh_16` 显示 ASCII 与简体中文。

- 来源：[Adobe Source Han Sans](https://github.com/adobe-fonts/source-han-sans)。
- 字体许可证：SIL Open Font License 1.1；所需版权声明与完整许可证见
  [`SourceHanSans-LICENSE.txt`](SourceHanSans-LICENSE.txt)。
- 转换工具：`lv_font_conv`；完整生成参数保留在生成的 C 源码顶部。
- 固件目标：`main/lv_font_terminal_zh_16.c` 与
  `main/lv_font_terminal_extra_16.c`。

## 如何使用

- 字库文件（如 `.ttf`、`.otf`、LVGL 使用的 C 数组字库等）复制到本目录，并在本项目 `README.md` 记录字名、字号、支持字符集与版权信息。
- 若需集成到 ESP-IDF 固件，参考 [`components/bsp/include/bsp_display.h`](../../components/bsp/include/bsp_display.h) 与 LVGL 字体接口，将字库转换为对应格式并放入正确资源目录。
- 字库占用 Flash 与内存，需在集成前评估 ESP32-C3 无 PSRAM 的限制（详见 `docs/hardware-design/AI_HARDWARE_DEVELOPMENT_GUIDE.md`）。

## 目录说明

新增字库时请同步更新本索引，并保留来源、许可证与可复现的转换参数。
