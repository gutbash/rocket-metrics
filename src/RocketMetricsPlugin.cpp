#include "pch.h"
#include "RocketMetricsPlugin.h"

constexpr auto kPluginVersion = "0.1.0";

BAKKESMOD_PLUGIN(RocketMetricsPlugin, "Rocket Metrics", kPluginVersion, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RocketMetricsPlugin::onLoad() {
	_globalCvarManager = cvarManager;

	auto enabledCvar = cvarManager->registerCvar("rocketmetrics_enabled", "1", "Enable the Rocket Metrics overlay.",
	                                             true, true, 0, true, 1);
	enabled_ = enabledCvar.getBoolValue();
	enabledCvar.addOnValueChanged(
	    [this](std::string, CVarWrapper updatedCvar) { enabled_ = updatedCvar.getBoolValue(); });

	auto xCvar =
	    cvarManager->registerCvar("rocketmetrics_overlay_x", "48", "Overlay X position.", true, true, 0, false, 0);
	overlayX_ = xCvar.getFloatValue();
	xCvar.addOnValueChanged([this](std::string, CVarWrapper updatedCvar) { overlayX_ = updatedCvar.getFloatValue(); });

	auto yCvar =
	    cvarManager->registerCvar("rocketmetrics_overlay_y", "120", "Overlay Y position.", true, true, 0, false, 0);
	overlayY_ = yCvar.getFloatValue();
	yCvar.addOnValueChanged([this](std::string, CVarWrapper updatedCvar) { overlayY_ = updatedCvar.getFloatValue(); });

	auto scaleCvar = cvarManager->registerCvar("rocketmetrics_overlay_scale", "1.0", "Overlay text scale.", true, true,
	                                           0.25F, true, 4.0F);
	overlayScale_ = scaleCvar.getFloatValue();
	scaleCvar.addOnValueChanged(
	    [this](std::string, CVarWrapper updatedCvar) { overlayScale_ = updatedCvar.getFloatValue(); });

	gameWrapper->RegisterDrawable(std::bind(&RocketMetricsPlugin::RenderOverlay, this, std::placeholders::_1));
	cvarManager->log("[RocketMetrics] Loaded MVP overlay MVP.");
}

void RocketMetricsPlugin::onUnload() {
	gameWrapper->UnregisterDrawables();
}

PriWrapper RocketMetricsPlugin::GetLocalPRI() const {
	if (!gameWrapper->IsInOnlineGame() && !gameWrapper->IsInGame()) {
		return {};
	}

	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (server.IsNull()) {
		return {};
	}

	CarWrapper car = gameWrapper->GetLocalCar();
	if (car.IsNull()) {
		return {};
	}

	PriWrapper pri = car.GetPRI();
	if (pri.IsNull()) {
		return {};
	}

	return pri;
}

bool RocketMetricsPlugin::IsLocalPlayerCurrentMvp(const PriWrapper& localPri) const {
	if (localPri.IsNull()) {
		return false;
	}

	TeamWrapper team = localPri.GetTeam();
	if (team.IsNull()) {
		return false;
	}

	auto teamMembers = team.GetMembers();
	int localScore = localPri.GetMatchScore();
	for (auto& teammate : teamMembers) {
		if (teammate.IsNull()) {
			continue;
		}

		if (teammate.GetUniqueIdWrapper().GetUID() == localPri.GetUniqueIdWrapper().GetUID()) {
			continue;
		}

		if (teammate.GetMatchScore() > localScore) {
			return false;
		}
	}

	return true;
}

void RocketMetricsPlugin::RenderOverlay(CanvasWrapper canvas) {
	if (!enabled_) {
		return;
	}

	PriWrapper localPri = GetLocalPRI();
	if (localPri.IsNull()) {
		return;
	}

	const bool isMvp = IsLocalPlayerCurrentMvp(localPri);
	const std::string mvpStatus = isMvp ? "YES" : "NO";

	Vector2 position{overlayX_, overlayY_};
	Vector2 scale{overlayScale_, overlayScale_};

	canvas.SetColor(255, 255, 255, 220);
	canvas.SetPosition(position);
	canvas.DrawString("Rocket Metrics (MVP)", overlayScale_, overlayScale_);

	position.Y += 24.0F * overlayScale_;
	canvas.SetPosition(position);
	canvas.DrawString("Goals: " + std::to_string(localPri.GetMatchGoals()), scale.X, scale.Y);

	position.Y += 20.0F * overlayScale_;
	canvas.SetPosition(position);
	canvas.DrawString("Assists: " + std::to_string(localPri.GetMatchAssists()), scale.X, scale.Y);

	position.Y += 20.0F * overlayScale_;
	canvas.SetPosition(position);
	canvas.DrawString("Saves: " + std::to_string(localPri.GetMatchSaves()), scale.X, scale.Y);

	position.Y += 20.0F * overlayScale_;
	canvas.SetPosition(position);
	canvas.DrawString("Shots: " + std::to_string(localPri.GetMatchShots()), scale.X, scale.Y);

	position.Y += 20.0F * overlayScale_;
	canvas.SetPosition(position);
	canvas.DrawString("Score: " + std::to_string(localPri.GetMatchScore()), scale.X, scale.Y);

	position.Y += 20.0F * overlayScale_;
	canvas.SetPosition(position);
	canvas.SetColor(isMvp ? 255 : 180, isMvp ? 215 : 180, isMvp ? 0 : 180, 255);
	canvas.DrawString("MVP: " + mvpStatus, scale.X, scale.Y);
}
