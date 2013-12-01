#ifndef __YUV_PLAYER_H
#define __YUV_PLAYER_H
#include <SDL/SDL.h>

class Render_Yuv
{
public:
	Render_Yuv();
	~Render_Yuv();
	int	init(const char* title,const char* icon=NULL);
	void	set_screenwidth(int width);
	void	set_screenheight(int height);
	void 	set_bitsperpixel(int bitsperpixel);
	void	set_videowidth(int videowidth);
	void	set_videoheight(int videoheight);
	void	display_yuv(unsigned char* p_src,const char*	 fmt);
	//void	event_loop(void* para);
	
public:
	SDL_Rect	m_rect;
	SDL_Surface*	m_screen;
	SDL_Overlay*	m_overlay;
	SDL_Thread*	m_eventthread;	
	int 		m_swidth;
	int		m_sheight;
	int		m_lwidth;
	int		m_lheight;
	int 		m_bitsperpixel;
	int 		m_videowidth;
	int		m_videoheight;
	bool		m_eventflag;
};	


#endif
