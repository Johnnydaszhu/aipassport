<p align="right">
  <strong>English</strong> · <a href="README.zh_CN.md">简体中文</a>
</p>

# TheGreatMe Passport

Open firmware that turns a FoloToy AI Passport into a pocket task and voice
terminal for TheGreatMe. It shows your goals and daily tasks, records messages
for Headquarters, and receives text and voice replies from the iPhone companion.

![AI Passport terminal controls](assets/images/thegreatme-passport-terminal-callouts-16x9.png)

## Build it with one prompt

Copy this entire prompt into Codex, Claude Code, Cursor, or another coding agent:

```text
Set up and build TheGreatMe Passport for me:
https://github.com/Johnnydaszhu/TheGreatMe_Passport

Work autonomously and request permission only when the environment requires it.

1. Clone the repository, then read AGENTS.md, docs/development/environment-setup.md,
   and docs/development/build-and-test.md before making changes.
2. Use exactly ESP-IDF v5.5.3 with the esp32c3 target. Reuse a matching local
   installation or install it side by side without replacing other versions.
3. Run ./tools/validate.sh and fix only setup or build problems required to pass it.
4. If a FoloToy AI Passport is connected, discover its actual serial port, flash
   only build/FoloToy-AI-Passport.bin at 0x10000, and verify the flashed digest.
   Never erase flash or overwrite cardid at 0x356000 or Recovery at 0x700000.
5. Report Build, Host tests, Device tests, and Unverified separately, and show me
   the paths to both generated firmware files.

Do not change product behavior, commit, or push unless I explicitly ask.
```

## Everyday controls

- Hold the left button to power on or off.
- Hold **UP** to talk to Headquarters; short-press it to stop a playing reply.
- Hold **DOWN** to capture and organize a task.
- Short-press **UP/DOWN** to move between tasks.
- Short-press **OK** to change theme; double-press it to open Settings.

## Output and safety

- `build/FoloToy-AI-Passport-full.bin` — verified merged image for distribution.
- `build/FoloToy-AI-Passport.bin` — application image for safe app-only flashing.
- Never erase a provisioned Passport; its identity and permanent Recovery must remain.

The TheGreatMe iPhone bridge is required for synchronization, recognition, AI
replies, and reply audio. Detailed setup, protocol, hardware, and contribution
documentation lives in [`docs/`](docs/). This downstream project is based on
[`FoloToy/ai-passport`](https://github.com/FoloToy/ai-passport) and uses the
[MIT License](LICENSE).
