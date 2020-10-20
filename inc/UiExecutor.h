/* Copyright © Artur Maziarek MMXX
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#pragma once

#include <memory>
#include <mutex>
#include <QtCore/qobject.h>
#include <Watcher.h>
#include <Display.h>
#include <mo/UiMainWindow.h>
#include <mo/UiSettingsWindow.h>
#include <MotionDetector.h>

class UiExecutor : public QObject, public Watcher, public Display  {
public:
  UiExecutor() = delete;
  UiExecutor(const UiMainWindowPtr& uiMainWindow, const UiSettingsWindowPtr& uiSettingsWindow);
  ~UiExecutor() = default;

  void attachMotionDetector(const MotionDetectorPtr& motionDetector);
  void attachVideoDevice(const char* videoDevice);

  // slots
  void mainWindowOpened();
  void mainWindowClosed();
  void showSettingsWindow(bool isTriggered);
  void alphaChange(double value);

  void notifyMarkers(FrameId frameId, Markers markers) final;
  void notifyMotion(FrameId frameId, DecisionWeight decisionWeight) final;
  void showFrame(Watcher::FrameId frameId, cv::Mat& frame) final;

private:
  static constexpr double alphaInitialValue = 0.8;

  UiMainWindowPtr uiMainWindow;
  UiSettingsWindowPtr uiSettingsWindow;
  std::weak_ptr<MotionDetector> motionDetectorLink;

  std::mutex processingControl;
  bool isMotionDetectorRunning;

  const char* videoDevice;

  FrameId markersFrameId;
  Markers markers;
  FrameId motionFrameId;
  DecisionWeight decisionWeight;

  void setupUiConnections();
};
