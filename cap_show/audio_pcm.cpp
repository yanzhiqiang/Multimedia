#include "audio_pcm.h"
#include "../../m_stdio.h/m_stdio.h"
#include <alsa/asoundlib.h>


snd_pcm_t* l_audio_open(snd_pcm_stream_t play_mode,char* device_name,int sample_rate,int channels,snd_pcm_format_t cap_bit)
{
	snd_pcm_t *handle=NULL;
	snd_pcm_hw_params_t *hw_params=NULL;

	if ((err = snd_pcm_open (&handle, device_name, play_mode, 0)) < 0)
	{
		log_to_file(FLOG_ERR,"cannot open audio device %s (%s)/n", argv[1],snd_strerror (err));
		return NULL;
	}

	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot allocate hardware parameter structure (%s)/n",snd_strerror (err));
		return NULL;
	}

	if ((err = snd_pcm_hw_params_any (handle, hw_params)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot initialize hardware parameter structure (%s)/n",snd_strerror (err));
		return NULL;
	}

	if ((err = snd_pcm_hw_params_set_access (handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot set access type (%s)/n",snd_strerror (err));
		return NULL;
	}

	if ((err = snd_pcm_hw_params_set_format (handle, hw_params, cap_bit)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot set sample format (%s)/n",snd_strerror (err));
		return NULL;
	}

	if ((err = snd_pcm_hw_params_set_rate_near (handle, hw_params, &sample_rate, 0)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot set sample rate (%s)/n",snd_strerror (err));
		return NULL;
	}

	if ((err = snd_pcm_hw_params_set_channels (handle, hw_params, channels)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot set channel count (%s)/n",snd_strerror (err));
		return NULL;
	}

	if ((err = snd_pcm_hw_params (handle, hw_params)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot set parameters (%s)/n",snd_strerror (err));
		return NULL;
	}

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare (handle)) < 0) 
	{
		log_to_file(FLOG_ERR,"cannot prepare audio interface for use (%s)/n",snd_strerror (err));
		return NULL;
	}
}

int l_audio_capture(snd_pcm_t* capture_handle,char* context,const int frame_num)
{
	int err=0;
	if ((err = snd_pcm_readi (capture_handle, context, frame_num)) != frame_num) 
	{
		log_to_file(FLOG_ERR,"read audio interface failed (%s)/n",snd_strerror (err));
		return -1;
	}
}

int l_audio_playback(snd_pcm_t* playback_handle,char* context,const int frame_num)
{
	int err=0;
	if ((err = snd_pcm_writei (playback_handle, context, frame_num)) != frame_num) 
	{
		log_to_file(FLOG_ERR,"write to audio interface failed (%s)/n",snd_strerror (err));
		return -1;
	}
}