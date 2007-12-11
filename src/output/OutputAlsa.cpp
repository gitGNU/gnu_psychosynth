/***************************************************************************
 *                                                                         *
 *   PSYCHOSYNTH                                                           *
 *   ===========                                                           *
 *                                                                         *
 *   Copyright (C) 2007 by Juan Pedro Bolivar Puente                       *
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

#include <iostream>
#include <pthread.h>

#include "output/OutputAlsa.h"

using namespace std;

OutputAlsa::OutputAlsa(const AudioInfo& info, const std::string& device) :
    Output(info),
    alsa_device(device),
    alsa_thread(this)
{
}

OutputAlsa::~OutputAlsa()
{
    if (getState() != NOTINIT)
	close();
}

void OutputAlsa::run()
{
    snd_pcm_sframes_t nframes;
	
    while (getState() == RUNNING) {
	//ut << "out_state " << out_state << '\n';
	snd_pcm_wait (alsa_pcm, 1000);  

	if ((nframes = snd_pcm_avail_update (alsa_pcm)) < 0) {
	    if (nframes == -EPIPE)
		cout << _("ERROR: Alsa buffer underrun ocurred.") << endl;
	    else
		cout << _("ERROR: Unknown Alsa avail update return value.") << endl;
	}

	process(getInfo().block_size);
    }
}

void OutputAlsa::start()
{
    if (getState() == IDLE) {
	setState(RUNNING);
	alsa_thread.start();
    } else {
	cout << _("ERROR: Alsa output thread already started or Alsa subsystem not initialized.") << endl;
    }
}

void OutputAlsa::stop()
{
    if (getState() == RUNNING) {
	setState(IDLE);
	alsa_thread.join();
    } else {
	cout << _("ERROR: Alsa output thread not running.") << endl;
    }
}

bool OutputAlsa::open()
{
    int err;
    int dir = 0;
    unsigned int uirate = getInfo().sample_rate;
	
    if (getState() == NOTINIT) {
	if ((err = snd_pcm_open (&alsa_pcm, alsa_device.c_str(), SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
	    cerr << _("ERROR: Cannot open ALSA device: ") << alsa_device 
		 << " (" << snd_strerror(err) << ")." << endl;
	    return false;
	}
		
	snd_pcm_hw_params_malloc (&alsa_hwparams);
	snd_pcm_hw_params_any (alsa_pcm, alsa_hwparams);
	snd_pcm_hw_params_set_access (alsa_pcm, alsa_hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
		
	alsa_format = SND_PCM_FORMAT_S16;
		
	snd_pcm_hw_params_set_format (alsa_pcm, alsa_hwparams, alsa_format);
	snd_pcm_hw_params_set_rate_near (alsa_pcm, alsa_hwparams, &uirate, &dir);
	snd_pcm_hw_params_set_channels (alsa_pcm, alsa_hwparams, getInfo().num_channels);
	snd_pcm_hw_params (alsa_pcm, alsa_hwparams);
	snd_pcm_prepare (alsa_pcm);
		
	snd_pcm_sw_params_malloc (&alsa_swparams);
	snd_pcm_sw_params_set_avail_min(alsa_pcm, alsa_swparams, getInfo().block_size);
	snd_pcm_sw_params_set_start_threshold (alsa_pcm, alsa_swparams, 0U);
	snd_pcm_sw_params (alsa_pcm, alsa_swparams);
		
	m_buf = new short int[getInfo().block_size * getInfo().num_channels * sizeof(short int)];
		
	setState(IDLE);
		
	return true;
    } else {
	cerr << _("WARNING: ALSA output object already initialized.") << endl;
	return false;
    }
}

bool OutputAlsa::put(const AudioBuffer& in_buf, size_t nframes)
{
    int err, j;
    size_t i;
    short int* bufp;
    bool ret = true;
    Real r;
	
    if (in_buf.getInfo().num_channels != getInfo().num_channels 
	|| in_buf.getInfo().sample_rate != getInfo().sample_rate) {
	/* TODO: Adapt the audio signal to fit our requeriments. */
	WARNING("Cant send data to the device: data and output system properties missmatch.");
	return false;
    }

    if (getState() != NOTINIT) {
	size_t copyframes = getInfo().block_size;
	size_t index = 0;
	
	while (nframes > 0) {
	    if (nframes < copyframes)
		copyframes = nframes;
			
	    bufp = m_buf;
	    for (i = 0; i < copyframes; i++, index++) {
		for (j = 0; j < getInfo().num_channels; j++) {
		    r = in_buf[j][index]; /* TODO: Optimize interleaving */
		    if (r < -1) r = -1; 
		    else if (r > 1) r = 1;	
		    *(bufp++) = (short int)(r*32766.0);
		}
	    }

	    if ((err = snd_pcm_writei (alsa_pcm, m_buf, copyframes)) != (int)copyframes) {
		WARNING( _("Write to ALSA audio interface failed.")
			 << " (" << snd_strerror (err) << ").");
		ret = false;
	    }
	    
	    nframes -= copyframes;
	}
		
    } else {
	cerr << _("ERROR: ALSA output device not initialized. Cannot write.") << endl;
	ret = false;
    }
	
    return ret;
}

bool OutputAlsa::close()
{
    if (getState() != NOTINIT) {
	if (getState() == RUNNING)
	    stop();
	
	snd_pcm_close(alsa_pcm);
	snd_pcm_hw_params_free(alsa_hwparams);
	snd_pcm_sw_params_free(alsa_swparams);
	setState(NOTINIT);
	return true;
		
    } else {
	cerr << _("ERROR: ALSA output device not initialized. Cannot end");
	return false;
    }
}
