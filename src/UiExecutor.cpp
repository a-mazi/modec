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
#include <cassert>
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
  assert(uiMainWindow);
  assert(uiSettingsWindow);
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

void UiExecutor::slotMainWindowOpened()
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  if ((!isMotionDetectorRunning) && (videoDevice != nullptr))
  {
    auto motionDetector = motionDetectorLink.lock();
    if (motionDetector)
    {
      motionDetector->openVideo(videoDevice);
      motionDetector->start();
      isMotionDetectorRunning = true;
    }
  }
}

void UiExecutor::slotMainWindowClosed()
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  if (isMotionDetectorRunning)
  {
    auto motionDetector = motionDetectorLink.lock();
    if (motionDetector)
    {
      motionDetector->stop();
      motionDetector->closeVideo();
      isMotionDetectorRunning = false;
    }
  }
  uiSettingsWindow->close();
}

void UiExecutor::slotShowSettingsWindow(bool isTriggered)
{
  std::lock_guard<std::mutex> processingControlLock{processingControl};
  uiSettingsWindow->show();
}

void UiExecutor::slotAlphaChange(double value)
{
std::lock_guard<std::mutex> processingControlLock{processingControl};
  auto motionDetector = motionDetectorLink.lock();
  if (motionDetector)
  {
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
  QObject::connect(uiMainWindow.get(), &UiMainWindow::signalOpened, this, &UiExecutor::slotMainWindowOpened);
  QObject::connect(uiMainWindow.get(), &UiMainWindow::signalClosed, this, &UiExecutor::slotMainWindowClosed);
  QObject::connect(&uiMainWindow->actionShowSettingsWindow, &QAction::triggered, this, &UiExecutor::slotShowSettingsWindow);
  QObject::connect(uiSettingsWindow.get(), &UiSettingsWindow::signalAlphaChanged, this, &UiExecutor::slotAlphaChange);
}
