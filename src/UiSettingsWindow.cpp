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
#include <UiToolBox.h>

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

  QObject::connect(alphaSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                   this,         &UiSettingsWindow::slotAlphaSpinBoxChange);
  QObject::connect(alphaSlider, &QSlider::valueChanged, this, &UiSettingsWindow::slotAlphaSliderChange);
}

void UiSettingsWindow::slotAlphaSpinBoxChange(double alphaSpinBoxValue)
{
  UiToolBox::updateAFromB(alphaSlider, alphaSpinBox);
  emit signalAlphaChanged(alphaSpinBoxValue);
}

void UiSettingsWindow::slotAlphaSliderChange()
{
  UiToolBox::updateAFromB(alphaSpinBox, alphaSlider);
  emit signalAlphaChanged(alphaSpinBox->value());
}
