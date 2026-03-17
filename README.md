# Rocket Metrics (MVP)

This repository contains an MVP BakkesMod plugin for Rocket League that renders a simple in-game stats overlay for the local player.

## Current MVP features
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

## Install and test in Rocket League (Windows)

### Prerequisites
1. **BakkesMod installed** and working with Rocket League.
2. **BakkesMod SDK** downloaded somewhere on disk.
3. **Visual Studio 2022** (Desktop development with C++ workload) and CMake installed.

### One-command build + install
From PowerShell in this repo:

```powershell
./scripts/windows/build_and_install.ps1 `
  -BakkesModSdkPath "C:\dev\bakkesmodsdk" `
  -BakkesModFolder "C:\Program Files\BakkesMod" `
  -Configuration Release
```

This script will:
- configure CMake with your SDK path,
- build `RocketMetrics.dll`,
- copy the DLL into `BakkesMod\bakkesmod\plugins\RocketMetrics.dll`.

### Manual build (if you prefer)
```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DBAKKESMOD_SDK_PATH="C:\dev\bakkesmodsdk" -DROCKETMETRICS_REQUIRE_SDK=ON
cmake --build build --config Release
copy .\build\Release\RocketMetrics.dll "C:\Program Files\BakkesMod\bakkesmod\plugins\RocketMetrics.dll"
```

### In-game test flow
1. Launch Rocket League with BakkesMod.
2. Open BakkesMod console (`F6`).
3. Run:
   - `plugin load RocketMetrics` (or restart game/BakkesMod if already auto-loaded)
   - `rocketmetrics_enabled 1`
4. Start Freeplay or join a match.
5. Confirm the overlay appears and updates live.
6. Try:
   - `rocketmetrics_overlay_x 200`
   - `rocketmetrics_overlay_y 200`
   - `rocketmetrics_overlay_scale 1.5`

## Notes
This is intentionally a lightweight first iteration focused on proving the overlay and MVP detection loop. We can iterate next with richer styling, per-match history, and more robust event-driven updates.


### Why CI configure works without the SDK
In GitHub Actions, the SDK is not available, so CMake runs in **configure-only mode** (no plugin target created).
Local Windows builds should pass `-DROCKETMETRICS_REQUIRE_SDK=ON` (or use the install script) to fail fast if SDK paths are wrong.
