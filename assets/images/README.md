<p align="right">
  <a href="README.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Images

Store reusable source images and generated display assets here.

- Use descriptive names and document dimensions, pixel format, conversion steps, and destination.
- Prefer formats suitable for the 240 × 320 RGB565 display and account for Flash and internal RAM.
- Preserve editable sources where licensing permits, and record the source and license.
- Never commit device QR secrets, credentials, or personal data in images.

## Product walkthrough assets

| File | Format and dimensions | Purpose and source |
| --- | --- | --- |
| [`thegreatme-passport-terminal-callouts-16x9.png`](thegreatme-passport-terminal-callouts-16x9.png) | RGB PNG, 1920 × 1080 | Public README operation guide based on the project-owned transparent AI Passport terminal mockup. The pure-black 16:9 composition uses white leader lines for the display, left-side power control, and the three right-side UP, OK, and DOWN controls. This is documentation artwork and is not embedded in the firmware. |
| [`thegreatme-passport-terminal-callouts-zh-cn-16x9.png`](thegreatme-passport-terminal-callouts-zh-cn-16x9.png) | RGB PNG, 1920 × 1080 | Simplified Chinese localization of the same public README operation guide. It preserves the device, black backdrop, control mapping, and callout layout while localizing both the terminal screen and annotations. This is documentation artwork and is not embedded in the firmware. |
