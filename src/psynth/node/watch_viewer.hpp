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

#ifndef PSYNTH_WATCH_VIEWER_H
#define PSYNTH_WATCH_VIEWER_H

#include <mutex>

#include <psynth/synth/ring_audio_buffer.hpp>
#include <psynth/synth/ring_sample_buffer.hpp>
#include <psynth/node/watch.hpp>

namespace psynth
{

template <class BufferType, class RingBufferType>
class watch_viewer : public watch
{
protected:
    RingBufferType m_ring;
    BufferType m_buffer;
    int m_points;
    float m_factor;
    float m_secs;
    bool m_updated;
    std::mutex m_mutex;
    
public:
    watch_viewer (int points, float secs) :
	m_ring(points),
	m_buffer(points),
	m_points(points),
	m_factor(1.0f),
	m_secs(secs),
	m_updated(false)
	{
	    m_ring.zero();
	    m_buffer.zero();
	}
    
    virtual void set_info (const audio_info& info) {
	m_factor = (float) info.sample_rate / ((float) m_points / m_secs);
	m_ring.zero();
    }

    virtual void update (const BufferType& buf) {
        std::unique_lock<std::mutex> lock (m_mutex);
	m_ring.write_fast_resample(buf, m_factor);
	//m_ring.write(buf);
	m_updated = false;
    }

    const BufferType& get_buffer () {
	if (!m_updated) {
	    std::unique_lock<std::mutex> lock (m_mutex);
	    typename RingBufferType::read_ptr ptr = m_ring.begin();
	    m_ring.read(ptr, m_buffer);
	    m_updated = true;
	}
	return m_buffer;
    }
};

typedef watch_viewer<sample_buffer, ring_sample_buffer> watch_view_control;

class watch_view_audio : public watch_viewer<audio_buffer, ring_audio_buffer>
{
public:
    watch_view_audio (int points, float secs) :
	watch_viewer<audio_buffer, ring_audio_buffer> (points, secs) {}
    
    virtual void set_info (const audio_info& newinfo) {
	audio_info info = newinfo;
	info.block_size = m_points;
	m_ring.set_info   (info);
	m_buffer.set_info (info);
	m_ring.zero();
	m_factor = (float) info.sample_rate / ((float) m_points / m_secs);
    }
};

} /* namespace psynth */

#endif /* PSYNTH_WATCHVIEWER */
