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

class Watcher
{
public:
  using FrameId = double;
  using Markers = unsigned int;
  using DecisionWeight = int;

  virtual void notifyMarkers(FrameId frameId, Markers markers) = 0;
  virtual void notifyMotion(FrameId frameId, DecisionWeight decisionWeight) = 0;
};

using WatcherPtr = std::shared_ptr<Watcher>;
