<p align="right">
  <a href="fork-guide.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Downstream Workflow

This repository is a downstream product implementation based on
`FoloToy/ai-passport`. Its `main` branch contains the current TheGreatMe terminal
firmware and intentionally differs from the upstream hardware baseline.

## Repository roles

```text
docs/                  product, contribution, development, and design documents
components/bsp/        stable board APIs and hardware implementation
main/                  TheGreatMe terminal application and diagnostic demos
assets/                reusable fonts, images, music, and sound effects
skills/                reusable AI-agent skills
tests/                 host-runnable logic tests
sdkconfig.defaults     reproducible ESP32-C3 defaults
```

The root `README.md` pair describes this product. The upstream-derived
`docs/README.md` pair retains the detailed hardware capability contract.

## Branch and upstream rules

- Treat this repository's `main` as the product branch and start ordinary work
  from it on a short-lived `feature/*`, `fix/*`, or `docs/*` branch.
- Keep the official baseline configured as the read-only `upstream` remote:
  `https://github.com/FoloToy/ai-passport.git`.
- Review upstream changes on a dedicated branch before merging them into the
  product. Resolve application, partition, BLE, and documentation differences
  deliberately and run the complete validation gate.
- Do not use an unattended workflow to merge `upstream/main` into this product's
  `main` branch.
- Keep reusable board logic in `components/bsp`; keep TheGreatMe application
  behavior in `main`.

Use `docs/assets/` for product architecture notes, design material, and images
that supplement the root README. Reusable media belongs in the matching
subdirectory under `assets/`, with its source and license recorded.

Documentation and experience follow the same split. Product-specific material
stays in this repository. General hardware facts, reusable interfaces, build
improvements, and experience notes that benefit every AI Passport user may be
proposed upstream as a separate pull request. The `plays/` archive and
post-release experience workflow remain documented in
`docs/development/project-completion.md`.

All maintained documentation uses English at the default `.md` path and
Simplified Chinese at `.zh_CN.md`, with reciprocal language links.
