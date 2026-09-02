<p align="right">
  <a href="README.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# AI Passport × TheGreatMe Agent Terminal

Firmware that turns a FoloToy AI Passport into a pocket task and voice terminal
for TheGreatMe. The ESP32-C3 device displays the current goal and today's tasks,
records two push-to-talk routes, and plays text and speech replies received from
an iPhone over encrypted Bluetooth LE.

This repository contains the **device firmware**. The TheGreatMe iPhone bridge
is maintained separately and is required for task synchronization, speech
recognition, AI replies, and reply audio. Without that companion, the firmware
still boots and advertises over BLE, but the connected workflow cannot complete.

## What is implemented

- A 240 × 320 single-color terminal dashboard with four selectable themes.
- UI copy for Simplified and Traditional Chinese, English, Spanish, French,
  German, Korean, and Japanese, selected by the companion payload.
- Weekly, monthly, and yearly goal payloads plus up to five daily tasks.
- Task states for pending, blocked, and completed items.
- Long-press UP for Headquarters conversation and long-press DOWN for task
  capture; both routes stream 16 kHz IMA ADPCM microphone audio.
- Reply text and 8 kHz IMA ADPCM speaker playback, with compatibility for legacy
  16 kHz reply blocks.
- Encrypted GATT characteristics with persistent iPhone bonding.
- Direct terminal startup, while long-pressing OK still returns to the hardware
  demo menu for diagnostics.
- Host tests for the board protocol, ADPCM codec, and UI pixel calculations.

## Controls

| Control | Dashboard | Task detail | Conversation | Settings |
| --- | --- | --- | --- | --- |
| UP click | Previous task | Previous task | Return to dashboard | Return |
| DOWN click | Next task | Next task | Return to dashboard | — |
| UP hold | Record a Headquarters message | Record a Headquarters message | Record another message | — |
| DOWN hold | Record a task-organizing message | Record a task-organizing message | Record a task-organizing message | — |
| OK click | Open Settings | Open Settings | Open Settings | Change theme |
| OK double-click | Open selected task full screen | Return to dashboard | — | — |
| OK hold | Return to the hardware demo menu | Return to the hardware demo menu | Return to the hardware demo menu | Return to the hardware demo menu |

## BLE contract

The companion service uses 16-bit UUID `0xA2B0`:

| UUID | Direction | Purpose |
| --- | --- | --- |
| `0xA2B1` | iPhone → Passport | Board and dialogue snapshot |
| `0xA2B2` | Passport → iPhone | Control and voice-session events |
| `0xA2B3` | Passport → iPhone | Microphone audio |
| `0xA2B4` | iPhone → Passport | Reply audio |

The compact board payload is documented in
[`main/passport_protocol.h`](main/passport_protocol.h). Keep real names,
transcripts, goals, and tasks out of fixtures and logs before publishing them.

## Build and validate

Requirements: FoloToy AI Passport, ESP-IDF 5.5.3, and the toolchain installed
for `esp32c3`.

```bash
source <path-to-esp-idf-v5.5.3>/export.sh
idf.py --version
./tools/validate.sh
```

The complete gate runs repository checks, host tests, an isolated firmware
build, and merged-image verification. It produces:

- `build/FoloToy-AI-Passport-full.bin`: verified merged image for distribution.
- `build/FoloToy-AI-Passport.bin`: application payload for the mini-program.

Read [`docs/development/build-and-test.md`](docs/development/build-and-test.md)
before flashing. Do not erase a provisioned device: its identity data at
`0x356000` and permanent Recovery at `0x700000` must remain intact. A successful
build is not evidence of on-device behavior.

## Repository map

```text
main/                    Application UI, BLE, audio, and protocol code
components/bsp/          Board support package and hardware interfaces
tests/                   Hardware-independent host tests
tools/                   Local and CI validation
docs/                    Hardware, build, contribution, and safety documentation
assets/                  Reusable asset provenance and licensing notes
```

Start with [`AGENTS.md`](AGENTS.md) for AI-assisted changes and
[`CONTRIBUTING.md`](.github/CONTRIBUTING.md) for ordinary contributions. Hardware
facts and constraints remain in the upstream-derived
[`AI Hardware Development Guide`](docs/hardware-design/AI_HARDWARE_DEVELOPMENT_GUIDE.md).

## Upstream and license

This is a downstream product implementation based on
[`FoloToy/ai-passport`](https://github.com/FoloToy/ai-passport). Upstream updates
are reviewed and merged explicitly; they are not merged into this product branch
by an unattended workflow.

Source code is available under the [MIT License](LICENSE). The embedded Source
Han Sans bitmap remains under the SIL Open Font License 1.1; see
[`assets/fonts/SourceHanSans-LICENSE.txt`](assets/fonts/SourceHanSans-LICENSE.txt)
and the [font notes](assets/fonts/README.md).
