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

#include <algorithm>
#include "object/ObjectEcho.h"
#include "object/KnownObjects.h"

using namespace std;

namespace psynth
{

PSYNTH_DEFINE_NODE_FACTORY(ObjectEcho);

ObjectEcho::ObjectEcho(const audio_info& prop) : 
    node (prop,
	  OBJ_ECHO,
	  "echo",
	  N_IN_A_SOCKETS,
	  N_IN_C_SOCKETS,
	  N_OUT_A_SOCKETS,
	  N_OUT_C_SOCKETS),
    m_param_delay(DEFAULT_DELAY),
    m_old_param_delay(DEFAULT_DELAY),
    m_param_feedback(DEFAULT_FEEDBACK),
    m_param_hidamp(DEFAULT_HIDAMP),
    m_pos(0),
    m_old_val(prop.num_channels, 0.0f),
    m_buffer(prop, prop.sample_rate * MAX_DELAY * 2.0f)
{
    add_param ("delay", node_param::FLOAT, &m_param_delay);
    add_param ("feedback", node_param::FLOAT, &m_param_feedback);

    m_buffer.zero();
}

void ObjectEcho::on_info_change ()
{
    m_old_val.resize(get_info().num_channels, 0.0f);
    m_buffer.resize(get_info().sample_rate * MAX_DELAY * 2.0f);
    m_buffer.zero();
}

ObjectEcho::~ObjectEcho()
{
}

int ObjectEcho::doUpdateChannel(int chan)
{
    const audio_buffer* _input     = get_input<audio_buffer>(LINK_AUDIO, IN_A_INPUT);
    const sample_buffer* _delay    = get_input<sample_buffer>(LINK_CONTROL, IN_C_DELAY);
    const sample_buffer* _feedback = get_input<sample_buffer>(LINK_CONTROL, IN_C_FEEDBACK);
    audio_buffer* _output          = get_output<audio_buffer>(LINK_AUDIO, IN_A_INPUT);    

    const sample* in_buf  = _input    ? _input->get_channel(chan) : 0;
    const sample* fb_buf  = _feedback ? _feedback->get_data()     : 0;
    const sample* del_buf = _delay    ? _delay->get_data()        : 0;

    sample* out_buf = _output->get_channel(chan);
    sample* tmp_buf = m_buffer.get_channel(chan);

    envelope_simple in_env = get_in_envelope (LINK_AUDIO, IN_A_INPUT);
    
    float delay;
    float in_val;
    int i;
    int pos = m_pos;
    float val;
  
    delay = m_param_delay * get_info ().sample_rate;
    for (i = 0; i < get_info ().block_size; ++i) {
	if (pos > delay)
	    pos = 0;
	
	in_val = in_buf ? *in_buf++ * in_env.update() : 0;
	val = tmp_buf[pos];

	//*out_buf++ = val;	
	if (fb_buf)
	    val = in_val - val * (m_param_feedback + m_param_feedback * *fb_buf++);
	else
	    val = in_val - val * m_param_feedback;
	*out_buf++ = val;

	/* Low pass filter. */
	val = val * m_param_hidamp + m_old_val[chan] * (1.0 - m_param_hidamp);
	tmp_buf[pos] = val;
	m_old_val[chan] = val;
	pos++;
    }

    return pos;
}

void ObjectEcho::do_update (const node* caller,
			    int caller_port_type, int caller_por)
{
    if (m_param_delay != m_old_param_delay)
	m_buffer.zero();
    m_old_param_delay = m_param_delay;

    int new_pos = m_pos;
 
    for (int i = 0; i < get_info ().num_channels; ++i)
	new_pos = doUpdateChannel(i);
    
    m_pos = new_pos;
}

}

