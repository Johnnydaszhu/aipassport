<p align="right">
  <strong>简体中文</strong> · <a href="README.md">English</a>
</p>

# 图片资源（Images）

本目录存放项目可复用的图片资源，如 UI 图标、背景、RGB565 资源等。

## 如何使用

- 图片文件复制到本目录，并在本项目 `README.md` 记录分辨率、格式、用途与来源。
- 与固件集成时，参考 [`components/bsp/include/bsp_display.h`](../../components/bsp/include/bsp_display.h) 与相关示例分支的图片资源管线，转换为固件所需格式（如 RGB565 数组）。
- 图片资源占用 Flash 与内存，集成前请评估 ESP32-C3 无 PSRAM 的限制。

## 目录说明

## 产品操作图资源

| 文件 | 格式与尺寸 | 用途与来源 |
| --- | --- | --- |
| [`thegreatme-passport-terminal-callouts-16x9.png`](thegreatme-passport-terminal-callouts-16x9.png) | RGB PNG，1920 × 1080 | 用于公开 README 的操作说明图，基于项目自有的透明 AI Passport 终端 mockup。纯黑 16:9 画面使用白色引导线，分别说明屏幕、左侧电源键，以及右侧的上键、确定键和下键。该图片只用于文档，不会写入固件。 |
