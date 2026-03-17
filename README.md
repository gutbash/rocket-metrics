# Rocket Metrics (MVP)

This repository contains an MVP BakkesMod plugin for Rocket League that renders a simple in-game stats overlay for the local player.

## Current MVP Features
- Toggleable overlay (`rocketmetrics_enabled`)
- Displays local player's:
  - Goals
  - Assists
  - Saves
  - Shots
  - Score
  - **MVP status** (derived from current scoreboard lead)
- Simple positioning and scale CVars:
  - `rocketmetrics_overlay_x`
  - `rocketmetrics_overlay_y`
  - `rocketmetrics_overlay_scale`

## Commands / CVars
- `rocketmetrics_enabled 0|1`
- `rocketmetrics_overlay_x <float>`
- `rocketmetrics_overlay_y <float>`
- `rocketmetrics_overlay_scale <float>`

## CI/CD pipeline
This project now includes a starter GitHub Actions pipeline:

- **CI (`.github/workflows/ci.yml`)** on every PR/push:
  - `clang-format` check for consistent C++ style
  - `cppcheck` static analysis
  - CMake configure smoke test
- **Release (`.github/workflows/release.yml`)** on version tags (`v*`):
  - packages source as a zip artifact
  - publishes a GitHub Release with attached artifact

### Recommended next CI/CD iteration
For full plugin binary delivery, add a Windows build job that has access to the BakkesMod SDK and emits the `.dll` as a release artifact.

## Notes
This is intentionally a lightweight first iteration focused on proving the overlay and MVP detection loop. We can iterate next with richer styling, per-match history, and more robust event-driven updates.
