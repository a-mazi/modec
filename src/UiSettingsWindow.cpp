/* Copyright © Artur Maziarek MMXX
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
#include <mo/UiSettingsWindow.h>

UiSettingsWindow::UiSettingsWindow() :
  actionCloseWindow{this}
{
  setupUi(this);
}

void UiSettingsWindow::setupUi(QWidget* settingsWindow)
{
  Ui_SettingsWindow::setupUi(settingsWindow);

  actionCloseWindow.setShortcut(Qt::Key_Escape);
  addAction(&actionCloseWindow);
  QObject::connect(&actionCloseWindow, &QAction::triggered, [&]{close();});

  detectAlphaSpinBoxLimits();
  detectAlphaSliderLimits();

  QObject::connect(alphaSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                   this,         &UiSettingsWindow::alphaSpinBoxChange);
  QObject::connect(alphaSlider, &QSlider::valueChanged, this, &UiSettingsWindow::alphaSliderChange);
}

void UiSettingsWindow::alphaSpinBoxChange(double alphaValue)
{
  setSliderFromSpinBox(alphaValue, alphaSpinBox, alphaSpinBoxMin, alphaSpinBoxMax,
                                   alphaSlider,  alphaSliderMin,  alphaSliderMax);
  emit alphaChanged(alphaValue);
}

void UiSettingsWindow::alphaSliderChange()
{
  double alphaValue = setSpinBoxFromSlider(alphaSlider,  alphaSliderMin,  alphaSliderMax,
                                           alphaSpinBox, alphaSpinBoxMin, alphaSpinBoxMax);
  emit alphaChanged(alphaValue);
}

void UiSettingsWindow::detectAlphaSpinBoxLimits()
{
  alphaSpinBoxMin = alphaSpinBox->minimum();
  alphaSpinBoxMax = alphaSpinBox->maximum();
}

void UiSettingsWindow::detectAlphaSliderLimits()
{
  alphaSliderMin = alphaSlider->minimum();
  alphaSliderMax = alphaSlider->maximum();
}

void UiSettingsWindow::setSliderFromSpinBox(double value, QDoubleSpinBox* spinBox, double spinBoxMin, double spinBoxMax,
                                            QSlider* slider, int sliderMin, int sliderMax)
{
  double valuePercent = (value - spinBoxMin) / (spinBoxMax - spinBoxMin);
  int sliderValue = sliderMin + valuePercent * (sliderMax - sliderMin);
  slider->blockSignals(true);
  slider->setValue(sliderValue);
  slider->blockSignals(false);
}

double UiSettingsWindow::setSpinBoxFromSlider(QSlider* slider, int sliderMin, int sliderMax, QDoubleSpinBox* spinBox,
                                              double spinBoxMin, double spinBoxMax)
{
  int sliderValue = slider->value();
  double valuePercent = static_cast<double>(sliderValue - sliderMin) / (sliderMax - sliderMin);
  double value = spinBoxMin + valuePercent * (spinBoxMax - spinBoxMin);
  spinBox->blockSignals(true);
  spinBox->setValue(value);
  spinBox->blockSignals(false);
  return value;
}
