#include "audio_pcm.h"
#include "../../m_stdio.h/m_stdio.h"
#include <alsa/asoundlib.h>


int l_audio_open(snd_pcm_stream_t play_mode,char* device_name,int sample_rate,int channels,snd_pcm_format_t cap_bit)
{
	snd_pcm_t *handle=NULL;
	snd_pcm_hw_params_t *hw_params=NULL;

	if ((err = snd_pcm_open (&handle, device_name, play_mode, 0)) < 0)
	{
		log_to_file(FLOG_ERR,"cannot open audio device %s (%s)/n", argv[1],snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot allocate hardware parameter structure (%s)/n",snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_any (handle, hw_params)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot initialize hardware parameter structure (%s)/n",snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_access (handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot set access type (%s)/n",snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_format (handle, hw_params, cap_bit)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot set sample format (%s)/n",snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_rate_near (handle, hw_params, &sample_rate, 0)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot set sample rate (%s)/n",snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_channels (handle, hw_params, channels)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot set channel count (%s)/n",snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params (handle, hw_params)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot set parameters (%s)/n",snd_strerror (err));
		return -1;
	}

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare (handle)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot prepare audio interface for use (%s)/n",snd_strerror (err));
		return -1;
	}
}