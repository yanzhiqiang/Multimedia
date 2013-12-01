#include "yuv_player.h"
#include "../../m_stdio.h/m_stdio.h"
#include <SDL/SDL.h>
#include <unistd.h>

int	event_loop(void* para);

Render_Yuv::Render_Yuv()
{
	m_screen=NULL;
	m_overlay=NULL;
	m_swidth=0;
	m_sheight=0;
	m_lwidth=0;
	m_lheight=0;
	m_bitsperpixel=0;
	m_videowidth=1920;
	m_videoheight=1080;
	m_eventthread=NULL;
	m_eventflag=false;
}

Render_Yuv::~Render_Yuv()
{
	if(m_screen)
	{
		SDL_FreeSurface(m_screen);
		m_screen=NULL;	
	}
	if(m_overlay)
	{
		SDL_FreeYUVOverlay(m_overlay);
		m_overlay=NULL;
	}
	m_eventflag=false;
	sleep(1);
	if(m_eventflag)
	{
		SDL_KillThread(m_eventthread);
		m_eventthread=NULL;
	}	
}

void Render_Yuv::set_screenwidth(int width)
{
	m_swidth=width;
}

void Render_Yuv::set_screenheight(int height)
{
	m_sheight=height;
}

void Render_Yuv::set_bitsperpixel(int bitsperpixel)
{
	m_bitsperpixel=bitsperpixel;
}

void Render_Yuv::set_videowidth(int videowidth)
{
	m_videowidth=videowidth;
}

void Render_Yuv::set_videoheight(int videoheight)
{
	m_videoheight=videoheight;
}

int Render_Yuv::init(const char* title,const char* icon)
{
	if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_EVENTTHREAD)<0)
	{
		log_to_file(FLOG_ERR,"can not initialize SDL:%s",SDL_GetError());		
		return -1;
	}
	atexit(SDL_Quit);
	
	m_screen=SDL_SetVideoMode(m_swidth,m_sheight,m_bitsperpixel, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT | SDL_RESIZABLE);
	SDL_WM_SetCaption(title, icon);   
	if(!m_screen)
	{
		log_to_file(FLOG_ERR,"create screen surface failed");
		return -2;
	}
	
	//check hardware
	if(m_screen->flags&SDL_HWSURFACE == SDL_HWSURFACE)
	{
		log_to_file(FLOG_NORMAL,"memory in hardware");
	}
	else
	{	
		log_to_file(FLOG_NORMAL,"memory in memory");	
	}
	
	if(m_screen->flags&SDL_DOUBLEBUF == SDL_DOUBLEBUF)
	{
		log_to_file(FLOG_NORMAL,"double buffer in hardware");
	}
	
	//init overlay	 	
	if(m_videowidth==0||m_videoheight==0)
	{
		log_to_file(FLOG_ERR,"video width(%d)==0||height(%d) == 0",m_videowidth,m_videoheight);
		return -3;
	}
	m_overlay=SDL_CreateYUVOverlay(m_videowidth, m_videoheight, SDL_YV12_OVERLAY, m_screen);
	if(!m_overlay)
	{
		log_to_file(FLOG_ERR,"create overlay failed");
		return -4;
	}
	
	//create a thread
	m_eventthread = SDL_CreateThread(event_loop,(void*)this);
	usleep(10000);
	if(!m_eventthread || !m_eventflag)
	{
		log_to_file(FLOG_ERR,"create event thread failed");
		return -5;
	}	
	
	return 0;	
		
}

void	Render_Yuv::display_yuv(unsigned char* p_src,const char*	 fmt)
{
	//todo convert fmt to YV12

	if(fmt)
	{
		if(strcmp(fmt,"YV12")!=0)
		{
			log_to_file(FLOG_ERR,"can not support format ,just yv12");
			return ;
		}
	}

	//SDL_LockSurface(m_screen);
	SDL_LockYUVOverlay(m_overlay);

	
	unsigned char* py = p_src;
	unsigned char* pu = p_src+m_videowidth*m_videoheight;
	unsigned char* pv = pu+(m_videowidth*m_videoheight)/4;

		
	memcpy(m_overlay->pixels[0],py,m_videowidth*m_videoheight);
	memcpy(m_overlay->pixels[1],pu,(m_videowidth*m_videoheight)/4);	
	memcpy(m_overlay->pixels[2],pv,(m_videowidth*m_videoheight)/4);
	
	SDL_UnlockYUVOverlay(m_overlay);
	//SDL_UnlockSurface(m_screen);
	
	m_rect.w = m_videowidth;
	m_rect.h = m_videoheight;
	m_rect.x = m_rect.y =0;	

	SDL_DisplayYUVOverlay(m_overlay, &m_rect);
		
}
	
int	event_loop(void* para)
{
	Render_Yuv *t_pare=(Render_Yuv*) para;
	if(!para)
	{
		log_to_file(FLOG_ERR,"event_loop para is NULL");
		return -1;
	}
	t_pare->m_eventflag=true;
	SDL_Event t_event;
	while(t_pare->m_eventflag)
	{
		while(SDL_PollEvent(&t_event))
		{
			switch(t_event.type)
			{
				case SDL_QUIT:
					t_pare->m_eventflag=false;
					break;
				case SDL_KEYUP:
					switch(t_event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							log_to_file(FLOG_DEBUG,"press esc");
							t_pare->m_eventflag=false;
							break;
					}
				
			//resize the programe size but need to resize the video frame	
			case SDL_VIDEORESIZE:
               t_pare->m_screen = SDL_SetVideoMode(t_event.resize.w, t_event.resize.h, 0,
                                          SDL_HWSURFACE|SDL_RESIZABLE|SDL_ASYNCBLIT|SDL_HWACCEL);
                t_pare->m_swidth = t_event.resize.w;
                t_pare->m_sheight = t_event.resize.h;			
			}			
		}	
	}		
}
