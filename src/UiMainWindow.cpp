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
#include <mo/UiMainWindow.h>
#include <QtGui/QCloseEvent>

UiMainWindow::UiMainWindow() :
    QMainWindow(0, Qt::Window),
    actionShowSettingsWindow{this}
{
  setupUi(this);
}

void UiMainWindow::setupUi(QMainWindow* mainWindow)
{
  Ui_MainWindow::setupUi(mainWindow);

  actionShowSettingsWindow.setShortcut(Qt::Key_F2);
  addAction(&actionShowSettingsWindow);
}

void UiMainWindow::showEvent(QShowEvent* event)
{
  emit opened();
  event->accept();
}

void UiMainWindow::closeEvent(QCloseEvent* event)
{
  emit closed();
  event->accept();
}
