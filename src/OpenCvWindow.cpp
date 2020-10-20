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
#include <OpenCvWindow.h>
#include <string>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

OpenCvWindow::OpenCvWindow() :
  markersFrameId{0},
  markers{0},
  motionFrameId{0},
  decisionWeight{0}
{
  cv::namedWindow("video", cv::WINDOW_AUTOSIZE);
}

void OpenCvWindow::notifyMarkers(FrameId frameId, Markers markers_)
{
  markersFrameId = frameId;
  markers = markers_;
}

void OpenCvWindow::notifyMotion(FrameId frameId, DecisionWeight decisionWeight_)
{
  motionFrameId = frameId;
  decisionWeight = decisionWeight_;
}

void OpenCvWindow::showFrame(Watcher::FrameId frameId, cv::Mat& frame)
{
  std::stringstream infoText;
  infoText << "Frame: " << frameId << " ";
  if (frameId == markersFrameId)
  {
    infoText << "Markers: " << markers << " ";
  }
  if (frameId == motionFrameId)
  {
    infoText << "Motion: " << decisionWeight;
  }
  cv::putText(frame, infoText.str(), cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255,255,255),2);
  cv::imshow("video", frame);
  cv::waitKey(1);
}
