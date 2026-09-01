<p align="right">
  <a href="CHANGELOG.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Changelog

## Unreleased

- Reduced negotiated Passport reply audio to 8 kHz IMA ADPCM while preserving
  16 kHz microphone capture and legacy 16 kHz speaker blocks. Reply playback now
  prebuffers three 100 ms blocks, and the iPhone sends against a stable 100 ms
  cadence to prevent BLE queue underruns and overruns during longer replies.
- Set Passport reply playback to full volume by default. The conversation page
  now places the user's named transcript on the right and Headquarters on the
  left, keeps its recording waveform in the user's lane, and lets a short UP
  press return to the task dashboard while a long UP press continues speaking.
  Both UP and DOWN push-to-talk routes now play the same short rising cue before
  microphone capture starts, keeping the cue out of the recording sent to iPhone.
- Persist the iPhone BLE bond used by the encrypted companion characteristics,
  so ordinary App reinstalls and reconnects no longer leave CoreBluetooth and
  the Passport with mismatched pairing keys.
- Start directly in the TheGreatMe terminal after boot so BLE discovery and
  reconnection do not depend on manually reopening a demo page. Holding OK still
  exits to the hardware demo menu for diagnostics.
- Aligned connectable advertising with the standard NimBLE per-procedure GAP
  callback so connection and subscription events have one explicit owner.
- Added a four-bar link indicator to the terminal header: it stays dim while
  the iPhone is disconnected and lights to full signal once both BLE data
  channels are ready.
- Reworked the Passport conversation page into a frameless terminal exchange:
  the user's line and the AI response now read like adjacent command prompts,
  while recognition, typing, and speech states appear inline. Removed visible
  implementation labels such as fast mode and multi-turn context, and made the
  push-to-talk waveform part of the terminal flow instead of a second card.
  Empty-state guidance is now a single short prompt instead of repeated labels,
  placeholders, and footer instructions.
- Simplified the companion header to the two-line product identity "The Great
  Me" and "Agent Terminal". Removed the firmware version and active-theme label
  from the dashboard header; version and theme details remain in Settings.
- Advertise the companion service as its native 16-bit `0xA2B0` UUID in the
  primary BLE packet and move the device name to scan response, allowing
  CoreBluetooth service-filtered scans to discover AI Passport instead of
  remaining in "Looking" indefinitely.
- Rebuilt the TheGreatMe companion page as a single-color CRT terminal with
  dense status panes, square one-pixel dividers, and amber-phosphor glow. Short
  UP/DOWN presses move the highlighted selection through today's task list below
  the same goal card used by the iPhone. Holding UP opens the conversation detail
  page and starts push-to-talk; releasing it sends audio to the iPhone for ASR and a
  fast, multi-turn AI reply. The reply returns as both text and locally
  synthesized speech for the Passport speaker. Holding DOWN instead routes the
  recording into the iPhone home voice flow for automatic minimum-win, obstacle,
  and task analysis. Pressing OK opens a true full-screen Settings page; OK
  cycles amber, black, warm-white, and digital-green terminal skins there, and
  UP returns. Settings now keeps only connection status, firmware version, and
  theme selection instead of repeating operational and AI-mode details. All
  visible system copy is consistently Simplified Chinese instead of mixing Chinese data with English
  controls and status text. A bundled 1-bit, 16 px
  Simplified Chinese font covers all 7,445 decoded GB2312 characters without
  depending on LVGL's small CJK subset.
- Turned the BLE demo into a TheGreatMe companion page: connect securely to
  iPhone, synchronize its goal/task board, stream route-tagged conversation or
  task audio as framed IMA ADPCM, and receive AI reply audio over a dedicated
  speaker characteristic. Audio frame buffers live outside the worker stack so
  entering the companion page remains stable on-device.
- Made mini-program BLE install compatibility a template-level invariant: fixed
  protected `cardid`/Recovery partitions, retained the five-second UP-key
  Recovery boot hook, and added CI validation for merged-image structure,
  partition MD5/ranges, the 3 MB app limit, and protected payload exclusion.
- Documented a release-title convention for multi-app releases: name tags as `v<version>-<app-name>` (e.g. `v0.1.0-voice-keychain`) so the release title carries the version and the app, and confirm the title after the release is published so a release list is scannable by app.
- Added a post-release follow-up workflow: an `issue-suggestions` skill for filing user feedback as issues against the upstream project, an `experience-pr` skill for submitting reusable development experience as a documentation PR, a `docs/experiences/` directory for per-entry experience files, and supporting `project-completion`, `file-issues`, and experience-index documents.
- Simplified the tracked repository root: moved GitHub-recognized community documents into `.github/`, moved the changelog into `docs/`, updated every reference, and added a root-document allowlist to repository checks.
- Repository-wide language policy: every maintained Markdown default `.md` file is English, Simplified Chinese uses a paired `.zh_CN.md`, and both provide language switches. Static checks reject missing peers, missing switches, and Chinese prose in English defaults.
- Phase one of the AI development workflow: streamlined task-based context routing, unified local/CI validation, added PR checks and a template, and committed the dependency lock for reproducible builds.
- PR review fixes: pinned GitHub Actions to full commit SHAs, split build/release jobs by least privilege, disabled persisted sync checkout credentials, added Feature Request and Usage Question forms, clarified private security-report fallback, and corrected stale README, CI-trigger, and branch descriptions.
- Changed commit titles, PR titles, and PR bodies from Chinese-default to English; updated the Chinese punctuation rule so it no longer applies to PR descriptions.
- Reworked `build-firmware.yml` to pass `SDKCONFIG_DEFAULTS=sdkconfig.defaults`, enable `partitions.csv`, preserve the 8 MB image header, merge a flashable `FoloToy-AI-Passport-full.bin`, publish only that artifact, and use Actions cache v5.
- Integrated upstream PR #6 to resolve PR #4 conflicts: Wi-Fi, Bluetooth LE, radio lifecycle, and low-power demos; a 3 MB factory partition; build/menu/configuration updates; hardware-guide coverage; and bilingual capability tables.
- Defined English imperative Conventional Commit formatting for both commits and PR titles.
- Removed stale sync-workflow template comments and generalized an irrelevant Redis TTL rule to cache components.
- Added Chinese punctuation, credential safety, and recoverable file-deletion conventions.
- Expanded source-comment requirements for functions, state, ownership, concurrency, timing, registers, and magic values.
- Removed AI execution instructions from product READMEs so they remain human-facing product and repository overviews.
- Added `docs/development/agent-guide.md` as the focused AI workflow guide.
- Updated `AGENTS.md`, `docs/INDEX.md`, and the development index for the agent guide.
- Documented why the root README path is reserved for fork owners and how GitHub README precedence supports it.
- Created `main-update` from the upstream-aligned baseline and combined the repository-structure, firmware-CI, and upstream-sync work.
- Corrected the merged documentation index, workflow path, project tree, and CI references.
- Moved CI documentation from software design to `docs/development/`.
- Moved fork-only documentation assets from `assets/docs/` to `docs/assets/`.
- Moved the upstream English/Chinese project READMEs under `docs/` and renamed the documentation catalog to `docs/INDEX.md`.
- Initialized `AGENTS.md`, `CLAUDE.md`, and `CHANGELOG.md`.
- Standardized the initial project README language filenames.
- Added the `docs/`, `assets/`, and `skills/` directory structure.
- Moved the upstream hardware guide into `docs/hardware-design/`.
- Standardized subdirectory README capitalization and introduced fork conventions.
- Allowed fork-owned root README and supplemental documentation content on fork `main`.
- Added and documented the fork-only supplemental-document directory.
- Moved the build CI document to its dedicated CI branch before consolidation.
- Documented clean-`main` reasons, the direct-development exception, and Actions enablement for forks.
- Split the original agent rules into contribution, development, and fork documents with a compact root index.
- Updated software-design and project README references for the new documentation structure.
- Added the documentation catalog and task-triggered routing based on the earlier repository model.
- Added bilingual contribution, code-of-conduct, security, and support documents tailored to this ESP-IDF and fork workflow.
