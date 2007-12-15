/***************************************************************************
 *                                                                         *
 *   PSYCHOSYNTH                                                           *
 *   ===========                                                           *
 *                                                                         *
 *   Copyright (C) Juan Pedro Bolivar Puente 2007                          *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#ifndef GUILOGSINK_H
#define GUILOGSINK_H

#include "common/Logger.h"
#include "CEGUI/CEGUI.h"

class GuiLogSink : public LogSink
{
    CEGUI::Window* m_window;
    std::string m_buffer;

    void dump(Log& log, int level, const std::string& msg);
    
public:
    GuiLogSink(CEGUI::Window* win = NULL) :
	m_window(win) {}

    void setWindow(CEGUI::Window* win) {
	m_window = win;
    };
};

#endif /* GUILOGSINK_H */