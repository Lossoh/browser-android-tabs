// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/screen_ozone.h"

#include "ui/aura/window.h"
#include "ui/aura/window_tree_host.h"
#include "ui/display/display.h"
#include "ui/display/screen.h"

namespace aura {

ScreenOzone::ScreenOzone(std::unique_ptr<ui::PlatformScreen> platform_screen)
    : platform_screen_(std::move(platform_screen)) {}
ScreenOzone::~ScreenOzone() = default;

gfx::Point ScreenOzone::GetCursorScreenPoint() {
  return platform_screen_->GetCursorScreenPoint();
}

bool ScreenOzone::IsWindowUnderCursor(gfx::NativeWindow window) {
  return GetWindowAtScreenPoint(GetCursorScreenPoint()) == window;
}

gfx::NativeWindow ScreenOzone::GetWindowAtScreenPoint(const gfx::Point& point) {
  auto widget = platform_screen_->GetAcceleratedWidgetAtScreenPoint(point);
  if (!widget)
    return nullptr;

  aura::WindowTreeHost* host =
      aura::WindowTreeHost::GetForAcceleratedWidget(widget);
  return host ? host->window() : nullptr;
}

int ScreenOzone::GetNumDisplays() const {
  return GetAllDisplays().size();
}

const std::vector<display::Display>& ScreenOzone::GetAllDisplays() const {
  return platform_screen_->GetAllDisplays();
}

display::Display ScreenOzone::GetDisplayNearestWindow(
    gfx::NativeWindow window) const {
  gfx::AcceleratedWidget widget = GetAcceleratedWidgetForWindow(window);
  if (!widget)
    return GetPrimaryDisplay();

  return platform_screen_->GetDisplayForAcceleratedWidget(widget);
}

display::Display ScreenOzone::GetDisplayNearestView(
    gfx::NativeView view) const {
  return GetDisplayNearestWindow(view);
}

display::Display ScreenOzone::GetDisplayNearestPoint(
    const gfx::Point& point) const {
  return platform_screen_->GetDisplayNearestPoint(point);
}

display::Display ScreenOzone::GetDisplayMatching(
    const gfx::Rect& match_rect) const {
  return platform_screen_->GetDisplayMatching(match_rect);
}

display::Display ScreenOzone::GetPrimaryDisplay() const {
  return platform_screen_->GetPrimaryDisplay();
}

void ScreenOzone::AddObserver(display::DisplayObserver* observer) {
  platform_screen_->AddObserver(observer);
}

void ScreenOzone::RemoveObserver(display::DisplayObserver* observer) {
  platform_screen_->RemoveObserver(observer);
}

gfx::AcceleratedWidget ScreenOzone::GetAcceleratedWidgetForWindow(
    aura::Window* window) const {
  if (!window)
    return gfx::kNullAcceleratedWidget;

  aura::WindowTreeHost* host = window->GetHost();
  if (!host)
    return gfx::kNullAcceleratedWidget;

  return host->GetAcceleratedWidget();
}

}  // namespace aura