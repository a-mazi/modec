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
#include <MotionDetector.h>
#include <opencv2/imgproc.hpp>
#include <Log.h>


MotionDetector::MotionDetector() :
  watcherLink{},
  displayLink{},
  alpha{0}
{
}

void MotionDetector::attachWatcher(const WatcherPtr& watcher)
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  if (!mainLoopThread.joinable())
  {
    watcherLink = watcher;
  }
}

void MotionDetector::attachDisplay(const DisplayPtr& display)
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  if (!mainLoopThread.joinable())
  {
    displayLink = display;
  }
}

bool MotionDetector::openVideo(const char* videoDevice)
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  if (!video.isOpened())
  {
    if (video.open(videoDevice))
    {
      if (video.read(referenceFrame))
      {
        cv::cvtColor(referenceFrame, referenceFrame, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(referenceFrame, referenceFrame, cv::Size(21, 21), 0);
      }
      return true;
    }
  }
  return false;
}

void MotionDetector::closeVideo()
{
  if (video.isOpened())
  {
    std::lock_guard<std::mutex> processingControlLock{processingControl};
    video.release();
  }
}

void MotionDetector::start()
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  if (!mainLoopThread.joinable())
  {
    doProcessing.store(true);
    mainLoopThread = std::thread{&MotionDetector::mainLoop, this};
  }
}

void MotionDetector::stop()
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  if (mainLoopThread.joinable())
  {
    doProcessing.store(false);
    mainLoopThread.join();
  }
}

void MotionDetector::mainLoop()
{

  cv::Mat frame, gray, blur, frameDelta, thresh, toDisplay;
  std::vector<std::vector<cv::Point>> countours;

  while (doProcessing.load())
  {
    std::unique_lock<std::mutex> processingControlLock{processingControl};
    if (video.read(frame))
    {
      Watcher::FrameId frameId = video.get(cv::CAP_PROP_POS_MSEC) - 1;

      cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
      cv::GaussianBlur(gray, blur, cv::Size(21, 21), 0);

      cv::absdiff(referenceFrame, blur, frameDelta);
      cv::addWeighted(referenceFrame, 0.90, blur, 0.10, 0.0, referenceFrame);
      cv::threshold(frameDelta, thresh, 25, 255, cv::THRESH_BINARY);

      cv::dilate(thresh, thresh, cv::Mat(), cv::Point(-1,-1), 2);
      cv::findContours(thresh, countours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
      auto detectedCountours = countours.size();

      cv::addWeighted(gray, alpha, thresh, (1 - alpha), 0.0, toDisplay);

      processingControlLock.unlock();

      auto watcher = watcherLink.lock();
      if (watcher)
      {
        watcher->notifyMarkers(frameId, detectedCountours);
        for(unsigned int countourNo = 0; countourNo < detectedCountours; countourNo++)
        {
          int contourArea = cv::contourArea(countours[countourNo]);
          if(contourArea > 100)
          {
            watcher->notifyMotion(frameId, contourArea);
            break;
          }
        }
      }

      auto display = displayLink.lock();
      if (display)
      {
        display->showFrame(frameId, toDisplay);
      }
    }
    else
    {
      doProcessing.store(false);
    }
  }
}

void MotionDetector::setAlpha(double value)
{
  alpha = value;
}
