#include "text.h"




int main(int argc,char *argv[])
{
	coor_tp coor;
	coor.x = 0;coor.y = 0;
	
	lcd_tp *ld = create_lcd();//映射屏幕
	if(ld == NULL)
		printf("open ld error!\n");	
	int fd = open("/dev/input/event0",O_RDWR);	//打开触摸屏设备
	if(fd < 0)
		printf("open fd error!\n");
	
	struct usr_list *r_head = NULL;//普通用户注册头
	r_head = make_head();
	
	struct usr_list *l_head = NULL;//普通用户登陆头
	l_head = make_head();
	
	struct spusr_list *sr_head = NULL;//超级用户注册头
	sr_head = make_sphead();
	
	struct spusr_list *sl_head = NULL;//超级用户登陆头
	sl_head = make_sphead();
	
	struct airplane_list *air_head = NULL;//航班的头
	air_head = make_air_head();
	
	struct info_list *info_head = NULL;//实名认证头
	info_head = make_info_head();
	
	make_old_usr(r_head);
	
	make_old_spusr(sr_head);
	make_old_airplane(air_head);
	make_info(info_head);
	
	bmp_tp *bp = create_bmp("./welcome.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
	//show_bmp_mmap_c("welcome.bmp");
	sleep(3);
	
	
	
	
	
	while(1)
	{
		
		bmp_tp *bpws = create_bmp("./welcome_s.bmp");//打开bmp
		show_bmp(0,0,bpws,ld,0);//展示bmp
		destroy_bmp(bpws);//删除bmp
		//show_bmp_mmap_c("welcome_s.bmp");
		
		bmp_tp *bp1 = create_bmp("./7.bmp");//打开bmp
		show_bmp(600,40,bp1,ld,0);
		destroy_bmp(bp1);
		
		bmp_tp *bp2 = create_bmp("./7.bmp");//打开bmp
		show_bmp(600,200,bp2,ld,0);
		destroy_bmp(bp2);
		
		bmp_tp *bp3 = create_bmp("./1.bmp");//打开bmp
		show_bmp(600,360,bp3,ld,0);
		destroy_bmp(bp3);
		
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		
		if(Rectangle_button(600,40,150,75,coor))
		{
			usr_register_meun(r_head,sr_head,ld,fd,coor);
		}
		
		if(Rectangle_button(600,200,150,75,coor))
		{
			usr_login_menu(r_head,sr_head,l_head,sl_head,ld,fd,coor,air_head,info_head);
		}
		
		if(Rectangle_button(600,360,150,75,coor))
		{
			printf("bye!bye!\n");
			break;
		}
	}
	
	bmp_tp *bp4 = create_bmp("./end.bmp");//打开bmp
	show_bmp(0,0,bp4,ld,0);//展示bmp
	destroy_bmp(bp4);//删除bmp
	//show_bmp_mmap_c("end.bmp");
	sleep(3);
	bmp_tp *bp5 = create_bmp("./b.bmp");//打开bmp
	show_bmp(0,0,bp5,ld,0);//展示bmp
	destroy_bmp(bp5);//删除bmp
	//show_color_mmap(0x00000000);  //黑色
	close(fd);
	
	return 0;
}