/***************************************************************************
 *                                                                         *
 *   PSYCHOSYNTH                                                           *
 *   ===========                                                           *
 *                                                                         *
 *   Copyright (C) Juan Pedro Bolivar Puente 2008                          *
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

#ifndef PSYNTH_OBJECTECHO_H
#define PSYNTH_OBJECTECHO_H

#include <vector>

#include <libpsynth/object/node.h>
#include <libpsynth/object/node_factory.h>

namespace psynth
{

class ObjectEcho : public node
{
public:	
    enum InAudioSocketID {
	IN_A_INPUT,
	N_IN_A_SOCKETS
    };
	
    enum InControlSocketID {
	IN_C_DELAY,
	IN_C_FEEDBACK,
	N_IN_C_SOCKETS
    };
	
    enum OutAudioSocketID {
	OUT_A_OUTPUT,
	N_OUT_A_SOCKETS
    };
	
    enum OutControlSocketID {
	N_OUT_C_SOCKETS
    };

    enum ParamID {
	PARAM_DELAY = node::N_COMMON_PARAMS,
	PARAM_FEEDBACK,
	PARAM_HIDAMP,
	N_PARAM
    };

    static const float MAX_DELAY = 1.0f;
    static const float DEFAULT_DELAY = 0.5f;
    static const float DEFAULT_FEEDBACK = 0.5f;
    static const float DEFAULT_HIDAMP = 0.5f;
    
private:
    float m_param_delay;
    float m_old_param_delay;
    float m_param_feedback;
    float m_param_hidamp;
    int m_pos;

    std::vector<sample> m_old_val;
    
    audio_buffer m_buffer;

    int doUpdateChannel(int chan);
    void do_update (const node* caller, int caller_port_type, int caller_port);
    void on_info_change ();
    void do_advance () {}
    
public:
    ObjectEcho(const audio_info& prop);
    ~ObjectEcho();
};

PSYNTH_DECLARE_NODE_FACTORY(ObjectEcho, "echo");

} /* namespace psynth */

#endif /* PSYNTH_OBJECTECHO_H */
