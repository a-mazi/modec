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
#include <opencv2/imgproc.hpp>
#include <Log.h>
#include <UiExecutor.h>

UiExecutor::UiExecutor(const UiMainWindowPtr& uiMainWindow_, const UiSettingsWindowPtr& uiSettingsWindow_) :
  uiMainWindow{uiMainWindow_},
  uiSettingsWindow{uiSettingsWindow_},
  motionDetectorLink{},
  isMotionDetectorRunning{false},
  videoDevice{nullptr},
  markersFrameId{0},
  markers{0},
  motionFrameId{0},
  decisionWeight{0}
{
  setupUiConnections();
}

void UiExecutor::attachMotionDetector(const MotionDetectorPtr& motionDetector)
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  motionDetectorLink = motionDetector;

  double alphaValue = alphaInitialValue;
  alphaValue = uiSettingsWindow->alphaSpinBox->value();
  motionDetector->setAlpha(alphaValue);
}

void UiExecutor::attachVideoDevice(const char* videoDevice_)
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  videoDevice = videoDevice_;
}

void UiExecutor::mainWindowOpened()
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  if ((!isMotionDetectorRunning) && (!motionDetectorLink.expired()) && (videoDevice != nullptr))
  {
    auto motionDetector = motionDetectorLink.lock();
    motionDetector->openVideo(videoDevice);
    motionDetector->start();
    isMotionDetectorRunning = true;
  }
}

void UiExecutor::mainWindowClosed()
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  if ((isMotionDetectorRunning) && (!motionDetectorLink.expired()))
  {
    auto motionDetector = motionDetectorLink.lock();
    motionDetector->stop();
    motionDetector->closeVideo();
    isMotionDetectorRunning = false;
  }
  uiSettingsWindow->close();
}

void UiExecutor::showSettingsWindow(bool isTriggered)
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  uiSettingsWindow->show();
}

void UiExecutor::alphaChange(double value)
{
std::lock_guard<std::mutex> processingControlLock{processingControl};
  if (!motionDetectorLink.expired())
  {
    auto motionDetector = motionDetectorLink.lock();
    motionDetector->setAlpha(value);
  }
}

void UiExecutor::notifyMarkers(FrameId frameId, Markers markers_)
{
  markersFrameId = frameId;
  markers = markers_;
}

void UiExecutor::notifyMotion(FrameId frameId, DecisionWeight decisionWeight_)
{
  motionFrameId = frameId;
  decisionWeight = decisionWeight_;
}

void UiExecutor::showFrame(Watcher::FrameId frameId, cv::Mat& frame)
{
  cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
  QImage image((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
  QPixmap pixmap = QPixmap::fromImage(image);
//  mainWindow.video->setPixmap(pixmap);
  QMetaObject::invokeMethod(uiMainWindow->videoDisplay, "setPixmap", Qt::QueuedConnection, Q_ARG(QPixmap, pixmap));

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
//  QMetaObject::invokeMethod(mainWindow.statusbar, "showMessage", Qt::QueuedConnection,
//                            Q_ARG(QString, infoText.str().c_str()), Q_ARG(int, 0));
}

void UiExecutor::setupUiConnections()
{
  uiMainWindow->videoDisplay->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  QObject::connect(uiMainWindow.get(), &UiMainWindow::opened, this, &UiExecutor::mainWindowOpened);
  QObject::connect(uiMainWindow.get(), &UiMainWindow::closed, this, &UiExecutor::mainWindowClosed);
  QObject::connect(&uiMainWindow->actionShowSettingsWindow, &QAction::triggered, this, &UiExecutor::showSettingsWindow);
  QObject::connect(uiSettingsWindow.get(), &UiSettingsWindow::alphaChanged, this, &UiExecutor::alphaChange);
}
