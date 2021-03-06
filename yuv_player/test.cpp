#include <stdio.h>
#include "yuv_player.h"
#include "../../m_stdio.h/m_stdio.h"
#include <SDL/SDL.h>

int main(int argc,char* argv[])
{	
	set_log_filename("yuv_player");
	set_log_level(FLOG_NORMAL);
	if(argc <2)
	{
		log_to_file(FLOG_ERR,"eg:./yuv_player filename");
		return -1;	
	}	
	Render_Yuv t_ryuv;
	unsigned char* src_content = NULL;
	FILE* fp=NULL;

	//t_ryuv.set_screenwidth(720);
	//t_ryuv.set_screenheight(576);	
	t_ryuv.set_videowidth(1920);
	t_ryuv.set_videoheight(1080);
	t_ryuv.init("video_show");
		
	fp=fopen64(argv[1],"rb");
	if(!fp)
	{
		log_to_file(FLOG_ERR,"open %s failed",argv[1]);
		return -1;	
	}
	
	//
	src_content = (unsigned char*)malloc(sizeof(unsigned char)*1920*1080*3/2);
	if(!src_content)
	{
		log_to_file(FLOG_ERR,"new memory failed");
		goto bail;		
	}	
	
	while(!feof(fp))
	{
		fread(src_content,1,1920*1080*3/2,fp);
		t_ryuv.display_yuv(src_content,NULL);			
		if(t_ryuv.m_eventflag==false)
			break;
		SDL_Delay(40);
	}
	
bail:
	if(src_content)
	{
		free(src_content);
		src_content=NULL;
	}
	if(fp)
	{
		fclose(fp);
		fp=NULL;
	}
	
	return 0;
}
