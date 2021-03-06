#ifndef __AUDIO_PCM_H
#define __AUDIO_PCM_H

#define LINUX_AUDIO_PLAYBACK	0
#define LINUX_AUDIO_CAPTURE	1

//linux open audio device for capture or playback
//play_mode : SND_PCM_STREAM_PLAYBACK or SND_PCM_STREAM_CAPTURE
snd_pcm_t* l_audio_open(snd_pcm_stream_t play_mode,char* device_name,unsigned int sample_rate=44100,int channels=2,snd_pcm_format_t cap_bit=SND_PCM_FORMAT_S16_LE);

//linux capture audio
int l_audio_capture(snd_pcm_t* capture_handle,char* context,const int frame_num);

//linux playback audio
int l_audio_playback(snd_pcm_t* playback_handle,char* context,const int frame_num);

#endif