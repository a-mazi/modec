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

#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <Watcher.h>
#include <Display.h>

class MotionDetector
{
public:
  using FrameIndex = double;

  MotionDetector();

  void attachWatcher(const WatcherPtr& watcher);
  void attachDisplay(const DisplayPtr& display);
  bool openVideo(const char* videoDevice);
  void closeVideo();
  void start();
  void stop();

  void setAlpha(double value);

private:
  std::weak_ptr<Watcher> watcherLink;
  std::weak_ptr<Display> displayLink;
  cv::VideoCapture video;
  cv::Mat referenceFrame;

  std::mutex processingControl;
  std::atomic_bool doProcessing;
  std::thread mainLoopThread;

  double alpha;

  void mainLoop();
};

using MotionDetectorPtr = std::shared_ptr<MotionDetector>;
