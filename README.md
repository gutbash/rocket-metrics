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

## Notes
This is intentionally a lightweight first iteration focused on proving the overlay and MVP detection loop. We can iterate next with richer styling, per-match history, and more robust event-driven updates.
