/*************************************************************************

  Copyright 2011-2013 Ibrahim Sha'ath

  This file is part of LibKeyFinder.

  LibKeyFinder is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LibKeyFinder is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LibKeyFinder.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************************/

#include "temporalwindowfactory.h"

namespace KeyFinder {

  TemporalWindowFactory::TemporalWindowWrapper::TemporalWindowWrapper(
    unsigned int frameSize, temporal_window_t temporalWindowType
  ) : function(temporalWindowType) {
    WindowFunction win;
    temporalWindow.resize(frameSize);
    std::vector<float>::iterator twIt = temporalWindow.begin();
    for (unsigned int i = 0; i < frameSize; i++) {
      *twIt = win.window(function, i, frameSize);
      std::advance(twIt, 1);
    }
  }

  unsigned int TemporalWindowFactory::TemporalWindowWrapper::getFrameSize() const {
    return temporalWindow.size();
  }

  temporal_window_t TemporalWindowFactory::TemporalWindowWrapper::getFunction() const {
    return function;
  }

  const std::vector<float>* TemporalWindowFactory::TemporalWindowWrapper::getTemporalWindow() const {
    return &temporalWindow;
  }

  TemporalWindowFactory::TemporalWindowFactory() : temporalWindows(0) { }

  TemporalWindowFactory::~TemporalWindowFactory() {
    for (unsigned int i = 0; i < temporalWindows.size(); i++)
      delete temporalWindows[i];
  }

  const std::vector<float>* TemporalWindowFactory::getTemporalWindow(
    unsigned int frameSize, temporal_window_t function
  ) {
    boost::mutex::scoped_lock lock(temporalWindowFactoryMutex);
    for (unsigned int i = 0; i < temporalWindows.size(); i++) {
      TemporalWindowWrapper* wrapper = temporalWindows[i];
      if (
        wrapper->getFrameSize() == frameSize &&
        wrapper->getFunction() == function
      ) {
        return wrapper->getTemporalWindow();
      }
    }
    temporalWindows.push_back(new TemporalWindowWrapper(frameSize, function));
    return temporalWindows[temporalWindows.size()-1]->getTemporalWindow();
  }

}
