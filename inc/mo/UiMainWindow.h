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
#include <ui/UiMainWindowBuilder.h>
#include <QtCore/qobject.h>
#include <QtWidgets/QAction>

class UiMainWindow: public QMainWindow, public Ui_MainWindow
{
  Q_OBJECT
public:
  UiMainWindow();
  ~UiMainWindow() = default;

  QAction actionShowSettingsWindow;

  void setupUi(QMainWindow* mainWindow);

  void showEvent(QShowEvent* event) final;
  void closeEvent(QCloseEvent* event) final;

  signals:
  void opened();
  void closed();
};

using UiMainWindowPtr = std::shared_ptr<UiMainWindow>;