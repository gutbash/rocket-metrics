#pragma once

#include "pch.h"

class RocketMetricsPlugin final : public BakkesMod::Plugin::BakkesModPlugin {
  public:
	void onLoad() override;
	void onUnload() override;

  private:
	void RenderOverlay(CanvasWrapper canvas);
	PriWrapper GetLocalPRI() const;
	bool IsLocalPlayerCurrentMvp(const PriWrapper& localPri) const;

	bool enabled_ = true;
	float overlayX_ = 48.0F;
	float overlayY_ = 120.0F;
	float overlayScale_ = 1.0F;
};
