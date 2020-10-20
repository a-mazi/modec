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
#include <ui/UiSettingsWindowBuilder.h>
#include <QtCore/qobject.h>
#include <QtWidgets/QAction>

class UiSettingsWindow: public QWidget, public Ui_SettingsWindow
{
  Q_OBJECT
public:
  UiSettingsWindow();
  ~UiSettingsWindow() = default;

  void setupUi(QWidget *settingsWindow);

  // slots
  void alphaSpinBoxChange(double alphaValue);
  void alphaSliderChange();

  signals:
  void alphaChanged(double alphaValue);

private:
  QAction actionCloseWindow;

  double alphaSpinBoxMin;
  double alphaSpinBoxMax;
  int alphaSliderMin;
  int alphaSliderMax;

  void detectAlphaSpinBoxLimits();
  void detectAlphaSliderLimits();

  void setSliderFromSpinBox(double value, QDoubleSpinBox* spinBox, double spinBoxMin, double spinBoxMax,
                                          QSlider*        slider,  int    sliederMin, int    sliderMax);
  double setSpinBoxFromSlider(            QSlider*        slider,  int    sliederMin, int    sliderMax,
                                          QDoubleSpinBox* spinBox, double spinBoxMin, double spinBoxMax);
};

using UiSettingsWindowPtr = std::shared_ptr<UiSettingsWindow>;
