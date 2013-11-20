#include <SDL/SDL.h>
#include "yuv_player.h"
#include "../../m_stdio.h"

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
	if (SDL_Init(SDL_INIT_EVERYTHING)<0)
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
		log_to_file(FLOG_INFO,"memory in hardware");
	}
	else
	{	
		log_to_file(FLOG_INFO,"memory in memory");	
	}
	
	if(m_screen->flags&SDL_DOUBLEBUF == SDL_DOUBLEBUF)
	{
		log_to_file(FLOG_INFO,"double buffer in hardware");
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

	return 0;	
		
}

void	Render_Yuv::display_yuv(const unsigned char* p_src,const unsigned char*	 fmt)
{
	//todo convert fmt to YV12
	if(strcmp(fmt,"YV12")!=0)
	{
		log_to_file(FLOG_ERR,"can not support format ,just yv12");
		return ;
	}

	//SDL_LockSurface(m_screen);
	SDL_LockYUVOverlay(m_overlay);

	
	unsigned char* py = p_src;
	unsigned char* pu = p_src+m_videowidth*m_videoheight;
	unsigned char* pv = p_u+(m_videowidth*m_videoheight)/4;

		
	memcpy(m_overlay->pixels[0],py,m_videowidth*m_videoheight);
	memcpy(m_overlay->pixels[1],pu,(m_videowidth*m_videoheight)/4);	
	memcpy(m_overlay->pixels[2],pv,(m_videowidth*m_videoheight)/4);
	
	SDL_UnlockYUVOverlay(m_overlay);
	//SDL_UnlockSurface(m_screen);
	
	rect.w = m_videowidth;
	rect.h = m_videoheight;
	rect.x = rect.y =0;	

	SDL_DisplayYUVOverlay(m_overlay, &m_rect);
		
}
	
