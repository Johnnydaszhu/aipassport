<p align="right">
  <a href="README.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Fonts

Store reusable font files and generated font sources here.

## Embedded terminal font

`main/lv_font_terminal_zh_16.c` is a 1-bit, 16 px LVGL bitmap generated
from `SourceHanSansSC-Normal.otf`, as distributed with LVGL. It covers ASCII
plus the GB2312 character set used by the Simplified Chinese terminal UI.
`main/lv_font_terminal_extra_16.c` is the small primary font used by the
terminal; it contains only the additional Traditional Chinese, Latin,
Japanese, and Korean glyphs required by localized terminal labels, then falls
back to `lv_font_terminal_zh_16` for ASCII and Simplified Chinese.

- Source family: [Adobe Source Han Sans](https://github.com/adobe-fonts/source-han-sans).
- Font license: SIL Open Font License 1.1; the required copyright and complete
  license text are in [`SourceHanSans-LICENSE.txt`](SourceHanSans-LICENSE.txt).
- Converter: `lv_font_conv`; the complete generation options are retained at
  the top of the generated C source.
- Firmware destinations: `main/lv_font_terminal_zh_16.c` and
  `main/lv_font_terminal_extra_16.c`.

- Use descriptive names that include the family, weight, size, and format when relevant.
- Document the source, license, character range, conversion command, and expected destination.
- Check Flash and internal-RAM impact before adding a font; the ESP32-C3 has no PSRAM.
- Do not commit fonts whose license does not permit redistribution.
