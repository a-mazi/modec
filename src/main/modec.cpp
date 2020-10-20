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
#include <Log.h>
#include <string>
#include <thread>
#include <chrono>
#include <memory>
#include <mo/UiMainWindow.h>
#include <mo/UiSettingsWindow.h>
#include <UiExecutor.h>
#include <MotionDetector.h>

int main(int argc, char * argv[]) {

#ifdef RELEASE
  LOGI("modec: Release %s\n", RELEASE);
#else
  LOGI("modec: Debug build\n");
#endif
  if (argc < 2)
  {
    return 1;
  }
  QApplication app(argc,argv);

  auto uiMainWindow = std::make_shared<UiMainWindow>();
  auto uiSettingsWindow = std::make_shared<UiSettingsWindow>();
  auto uiExecutor = std::make_shared<UiExecutor>(uiMainWindow, uiSettingsWindow);

  auto motionDetector = std::make_shared<MotionDetector>();
  uiExecutor->attachMotionDetector(motionDetector);
  motionDetector->attachDisplay(uiExecutor);
  motionDetector->attachWatcher(uiExecutor);

  uiExecutor->attachVideoDevice(argv[1]);

  uiMainWindow->show();
  return app.exec();
}
