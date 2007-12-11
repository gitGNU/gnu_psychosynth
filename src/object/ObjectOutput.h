/***************************************************************************
 *                                                                         *
 *   PSYCHOSYNTH                                                           *
 *   ===========                                                           *
 *                                                                         *
 *   Copyright (C) 2007 Juan Pedro Bolivar Puente                          *
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
 
#ifndef OBJECTOUTPUT_H
#define OBJECTOUTPUT_H

#include <list>

#include "common/RingAudioBuffer.h"
#include "common/Mutex.h"
#include "object/Object.h"
#include "output/Output.h"

class ObjectManager;

class ObjectOutput : public Object
{
    struct Slot {
	RingAudioBuffer::ReadPtr m_ptr;
	Output* m_out;
	ObjectOutput* m_parent;
	AudioBuffer m_buf;
	
	Slot(Output* out, ObjectOutput* parent, RingAudioBuffer::ReadPtr ptr, const AudioInfo& info) :
	    m_ptr(ptr),
	    m_out(out),
	    m_parent(parent),
	    m_buf(info) {
	    out->setCallback(&ObjectOutput::outputCallback, this);
	}
		
	~Slot() {
	    delete m_out;
	}
    };
	
    RingAudioBuffer m_buffer;
    
    ObjectManager* m_manager;
    std::list<Slot*> m_slots;

    RWLock m_buflock;
        
    static void outputCallback(int nframes, void* arg);
    
    void output(Slot& slot, size_t nframes);
	
    void doUpdate();

public:
    enum InAudioSocketID {
	IN_A_INPUT,
	N_IN_A_SOCKETS
    } ;
	
    enum InControlSocketID{
	N_IN_C_SOCKETS
    };
	
    enum OutAudioSocketID {
	N_OUT_A_SOCKETS
    };
	
    enum  OutControlSocketID {
	N_OUT_C_SOCKETS
    };

    enum ParamID {
	N_PARAM
    };
	
    ObjectOutput(AudioInfo& info);

    ~ObjectOutput() {
	for (std::list<Slot*>::iterator i = m_slots.begin(); i != m_slots.end(); ++i)
	    delete *i;
    }
	
    bool setManager(ObjectManager* mgr) {
	if (m_manager != NULL && mgr != NULL) 
	    return false; /* Already attached */
		
	m_manager = mgr;
	return true;
    }
	
    void attachOutput(Output* out) {
	m_buflock.readLock();
	m_slots.push_back(new Slot(out, this, m_buffer.end(), getAudioInfo()));
	m_buflock.unlock();
    };
	
    void detachOutput(Output* out) {
	for (std::list<Slot*>::iterator i = m_slots.begin(); i != m_slots.end();) {
	    if ((*i)->m_out == out) {
		(*i)->m_out->setCallback(NULL, NULL); /* This should take threading into account. */
		delete *i;
		m_slots.erase(i++);
	    } else
		++i;
	}
    };
};

#endif /* OUBJECTOUTPUT_H */
