#include "text.h"

void usr_register_meun(struct usr_list *r_head,struct spusr_list *sr_head,lcd_tp *ld,int fd,coor_tp coor)//用户注册菜单
{
	
	
	
	while(1)
	{
		
		bmp_tp *bp = create_bmp("./chioce_register.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		
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
			usr_register(r_head,ld,fd);
		}
		
		if(Rectangle_button(600,200,150,75,coor))
		{
			spusr_register(sr_head,ld,fd);
		}
		
		if(Rectangle_button(600,360,150,75,coor))
		{
			
			return;
		}
	}
	
	
	
	
}


void show_chioce_airplane_next(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor)//普通用户查询航班,飞机类型、金钱、有无靠窗
{
	char chioce[20];
	int chioce1;
	while(1)
	{
		bmp_tp *bp = create_bmp("./usr_show_airchioce2.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		bmp_tp *bp1 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,22,bp1,ld,0);
		destroy_bmp(bp1);
		
		bmp_tp *bp2 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,142,bp2,ld,0);
		destroy_bmp(bp2);
		
		bmp_tp *bp3 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,262,bp3,ld,0);
		destroy_bmp(bp3);
		
		bmp_tp *bp4 = create_bmp("./8.bmp");//打开bmp
		show_bmp(610,382,bp4,ld,0);
		destroy_bmp(bp4);
		
		
		
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		
		if(Rectangle_button(610,22,150,75,coor))
		{
			printf("pls input airplane_type:");
			scanf("%s",chioce);
			struct airplane_list* p;
			for(p=air_head->next;p != air_head;p=p->next)
			{
				if(strcmp(chioce,p->airplane_type) == 0)
				{
					printf("plane number:%s\n",p->number);
					printf("departure:%s\n",p->departure);
					printf("destination:%s\n",p->destination);
					printf("day:%s\n",p->day);
					printf("time:%s\n",p->time);
					printf("airplane_type:%s\n",p->airplane_type);
					printf("how_much:%d\n",p->how_much);
					printf("window seat:%d\n",p->leave_seat_window);
					printf("unwindow seat:%d\n",p->leave_seat_unwindow);
					printf("================================\n");
				}
			}
		}
		
		if(Rectangle_button(610,142,150,75,coor))
		{
			printf("pls input 0- ");
			scanf("%d",&chioce1);
			struct airplane_list* p;
			for(p=air_head->next;p != air_head;p=p->next)
			{
				if(chioce1 > p->how_much)
				{
					printf("plane number:%s\n",p->number);
					printf("departure:%s\n",p->departure);
					printf("destination:%s\n",p->destination);
					printf("day:%s\n",p->day);
					printf("time:%s\n",p->time);
					printf("airplane_type:%s\n",p->airplane_type);
					printf("how_much:%d\n",p->how_much);
					printf("window seat:%d\n",p->leave_seat_window);
					printf("unwindow seat:%d\n",p->leave_seat_unwindow);
					printf("================================\n");
				}
			}
		}
		
		if(Rectangle_button(610,262,150,75,coor))
		{
			printf("need larger window_set:");
			scanf("%d",&chioce1);
			struct airplane_list* p;
			for(p=air_head->next;p != air_head;p=p->next)
			{
				if(chioce1 < p->leave_seat_window)
				{
					printf("plane number:%s\n",p->number);
					printf("departure:%s\n",p->departure);
					printf("destination:%s\n",p->destination);
					printf("day:%s\n",p->day);
					printf("time:%s\n",p->time);
					printf("airplane_type:%s\n",p->airplane_type);
					printf("how_much:%d\n",p->how_much);
					printf("window seat:%d\n",p->leave_seat_window);
					printf("unwindow seat:%d\n",p->leave_seat_unwindow);
					printf("================================\n");
				}
			}
		}
		
		if(Rectangle_button(610,382,150,75,coor))
		{
			
			return;
		}
	}
}


void show_chioce_airplane(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor)//普通用户筛选查询航班,出发地、目的地、日期
{
	char chioce[20] = {0};
	while(1)
	{
		bmp_tp *bp = create_bmp("./usr_show_airchioce1.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		bmp_tp *bp1 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,22,bp1,ld,0);
		destroy_bmp(bp1);
		
		bmp_tp *bp2 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,142,bp2,ld,0);
		destroy_bmp(bp2);
		
		bmp_tp *bp3 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,262,bp3,ld,0);
		destroy_bmp(bp3);
		
		bmp_tp *bp4 = create_bmp("./1.bmp");//打开bmp
		show_bmp(610,382,bp4,ld,0);
		destroy_bmp(bp4);
		
		bmp_tp *bp5 = create_bmp("./9.bmp");//打开bmp
		show_bmp(450,382,bp5,ld,0);
		destroy_bmp(bp5);
		
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		
		if(Rectangle_button(610,22,150,75,coor))
		{
			printf("pls input departure:");
			scanf("%s",chioce);
			struct airplane_list* p;
			for(p=air_head->next;p != air_head;p=p->next)
			{
				if(strcmp(chioce,p->departure) == 0)
				{
					printf("plane number:%s\n",p->number);
					printf("departure:%s\n",p->departure);
					printf("destination:%s\n",p->destination);
					printf("day:%s\n",p->day);
					printf("time:%s\n",p->time);
					printf("airplane_type:%s\n",p->airplane_type);
					printf("how_much:%d\n",p->how_much);
					printf("window seat:%d\n",p->leave_seat_window);
					printf("unwindow seat:%d\n",p->leave_seat_unwindow);
					printf("================================\n");
				}
			}
		}
		
		if(Rectangle_button(610,142,150,75,coor))
		{
			printf("pls input destination:");
			scanf("%s",chioce);
			struct airplane_list* p;
			for(p=air_head->next;p != air_head;p=p->next)
			{
				if(strcmp(chioce,p->destination) == 0)
				{
					printf("plane number:%s\n",p->number);
					printf("departure:%s\n",p->departure);
					printf("destination:%s\n",p->destination);
					printf("day:%s\n",p->day);
					printf("time:%s\n",p->time);
					printf("airplane_type:%s\n",p->airplane_type);
					printf("how_much:%d\n",p->how_much);
					printf("window seat:%d\n",p->leave_seat_window);
					printf("unwindow seat:%d\n",p->leave_seat_unwindow);
					printf("================================\n");
				}
			}
		}
		
		if(Rectangle_button(610,262,150,75,coor))
		{
			printf("pls input day:");
			scanf("%s",chioce);
			struct airplane_list* p;
			for(p=air_head->next;p != air_head;p=p->next)
			{
				if(strcmp(chioce,p->day) == 0)
				{
					printf("plane number:%s\n",p->number);
					printf("departure:%s\n",p->departure);
					printf("destination:%s\n",p->destination);
					printf("day:%s\n",p->day);
					printf("time:%s\n",p->time);
					printf("airplane_type:%s\n",p->airplane_type);
					printf("how_much:%d\n",p->how_much);
					printf("window seat:%d\n",p->leave_seat_window);
					printf("unwindow seat:%d\n",p->leave_seat_unwindow);
					printf("================================\n");
				}
			}
		}
		
		if(Rectangle_button(610,382,150,75,coor))
		{
			
			return;
		}
		
		if(Rectangle_button(450,382,150,75,coor))
		{
			show_chioce_airplane_next(air_head,ld,fd,coor);
			
		}
		
	}
	
}

void usr_show_airplane_meun(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor)//普通用户查询航班主页面
{
	char chioce[20];
	while(1)
	{
		bmp_tp *bp = create_bmp("./usr_show_airplane.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		bmp_tp *bp1 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,22,bp1,ld,0);
		destroy_bmp(bp1);
		
		bmp_tp *bp2 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,142,bp2,ld,0);
		destroy_bmp(bp2);
		
		bmp_tp *bp3 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,262,bp3,ld,0);
		destroy_bmp(bp3);
		
		bmp_tp *bp4 = create_bmp("./1.bmp");//打开bmp
		show_bmp(610,382,bp4,ld,0);
		destroy_bmp(bp4);
		

		
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		
		if(Rectangle_button(610,22,150,75,coor))
		{
			bmp_tp *bp5 = create_bmp("./usr_showing.bmp");//打开bmp
			show_bmp(0,0,bp5,ld,0);//展示bmp
			destroy_bmp(bp5);//删除bmp
			sleep(2);
			show_all_airplane(air_head);
		}
		
		if(Rectangle_button(610,142,150,75,coor))
		{
			show_chioce_airplane(air_head,ld,fd,coor);
		}
		
		if(Rectangle_button(610,262,150,75,coor))
		{
			printf("pls input number:");
			scanf("%s",chioce);
			struct airplane_list* p;
			for(p=air_head->next;p != air_head;p=p->next)
			{
				if(strcmp(chioce,p->number) == 0)
				{
					printf("plane number:%s\n",p->number);
					printf("departure:%s\n",p->departure);
					printf("destination:%s\n",p->destination);
					printf("day:%s\n",p->day);
					printf("time:%s\n",p->time);
					printf("airplane_type:%s\n",p->airplane_type);
					printf("how_much:%d\n",p->how_much);
					printf("window seat:%d\n",p->leave_seat_window);
					printf("unwindow seat:%d\n",p->leave_seat_unwindow);
					printf("================================\n");
				}
			}
		}
		
		if(Rectangle_button(610,382,150,75,coor))
		{
			
			return;
		}
		
		
		
	}
}



void info_ture_name(struct usr_list *r_head,struct info_list* info_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//用户实名认证，本用户、其他用户
{
	char info_name[10]={0};
	char info_id[20]={0};
	char filename[50] = {0};
	char filedata[400] = {0};
	int ret;
	while(1)
	{
		
		bmp_tp *bpws = create_bmp("./info_chioce.bmp");//打开bmp
		show_bmp(0,0,bpws,ld,0);//展示bmp
		destroy_bmp(bpws);//删除bmp
		
		
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
			info_name_my(r_head,info_head,p,ld,fd,coor);
			
		}
		
		if(Rectangle_button(600,200,150,75,coor))
		{
			info_name_other(r_head,info_head,p,ld,fd,coor);

		}
		
		if(Rectangle_button(600,360,150,75,coor))
		{
			
			break;
		}
	}
}



void usr_Balance_meun(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//余额菜单，充值、积分商城
{
	while(1)
	{
		
		bmp_tp *bp = create_bmp("./balance_meun.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		
		bmp_tp *bp1 = create_bmp("./7.bmp");//打开bmp
		show_bmp(600,40,bp1,ld,0);
		destroy_bmp(bp1);
		
		bmp_tp *bp2 = create_bmp("./7.bmp");//打开bmp
		show_bmp(600,200,bp2,ld,0);
		destroy_bmp(bp2);
		
		bmp_tp *bp3 = create_bmp("./1.bmp");//打开bmp
		show_bmp(600,360,bp3,ld,0);
		destroy_bmp(bp3);
		
		bmp_tp *bp5 = create_bmp("./show.bmp");//打开bmp
		show_bmp(10,360,bp5,ld,0);
		destroy_bmp(bp5);
		
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		
		if(Rectangle_button(600,40,150,75,coor))
		{
			usr_recharge(p,ld,fd,coor);
		}
		
		if(Rectangle_button(600,200,150,75,coor))
		{
			usr_integral_shop(p,ld,fd,coor);
		}
		
		if(Rectangle_button(600,360,150,75,coor))
		{
			
			return;
		}
		if(Rectangle_button(10,360,150,75,coor))
		{
			show_usr_money(ld,fd,coor,p);
			
		}
	}
}


void buy_airplane_meun(struct usr_list* r_head,struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//购票菜单，购票、余额
{
	while(1)
	{
		
		bmp_tp *bp = create_bmp("./buy_airplane_meun.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		
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
			buy_airplane_chioce(r_head,air_head,p,ld,fd,coor);
		}
		
		if(Rectangle_button(600,200,150,75,coor))
		{
			usr_Balance_meun(p,ld,fd,coor);
		}
		
		if(Rectangle_button(600,360,150,75,coor))
		{
			
			return;
		}
	}
}


void cheak_airplane_meun(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p,struct airplane_list* air_head)//查看已买的票，查看、改签、退票
{
	while(1)
	{
		bmp_tp *bp = create_bmp("./check_meun.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		bmp_tp *bp1 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,22,bp1,ld,0);
		destroy_bmp(bp1);
		
		bmp_tp *bp2 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,142,bp2,ld,0);
		destroy_bmp(bp2);
		
		bmp_tp *bp3 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,262,bp3,ld,0);
		destroy_bmp(bp3);
		
		bmp_tp *bp4 = create_bmp("./1.bmp");//打开bmp
		show_bmp(610,382,bp4,ld,0);
		destroy_bmp(bp4);
		
		
		
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		
		if(Rectangle_button(610,22,150,75,coor))
		{
			cheak_airplane(ld,fd,coor,p);
		}
		
		if(Rectangle_button(610,142,150,75,coor))
		{
			change_airplane(ld,fd,coor,p,air_head);
		}
		
		if(Rectangle_button(610,262,150,75,coor))
		{
			refund_airplane(ld,fd,coor,p);
		}
		
		if(Rectangle_button(610,382,150,75,coor))
		{
			
			return;
		}
		
		
		
	}
}

void logining_sc_next(struct usr_list *r_head,struct usr_list *l_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p,struct airplane_list* air_head)//普通用户进入后，看已经购票的信息、修改个人信息、风景
{
	while(1)
	{
		bmp_tp *bp = create_bmp("./login_chioce2.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		bmp_tp *bp1 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,22,bp1,ld,0);
		destroy_bmp(bp1);
		
		bmp_tp *bp2 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,142,bp2,ld,0);
		destroy_bmp(bp2);
		
		bmp_tp *bp3 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,262,bp3,ld,0);
		destroy_bmp(bp3);
		
		bmp_tp *bp4 = create_bmp("./8.bmp");//打开bmp
		show_bmp(610,382,bp4,ld,0);
		destroy_bmp(bp4);
		
		
		
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		
		if(Rectangle_button(610,22,150,75,coor))
		{
			cheak_airplane_meun(ld,fd,coor,p,air_head);
		}
		
		if(Rectangle_button(610,142,150,75,coor))
		{
			change_usr_data(r_head,ld,fd,coor,p);
		}
		
		if(Rectangle_button(610,262,150,75,coor))
		{
			show_landscape(p,air_head,ld,fd,coor);
		}
		
		if(Rectangle_button(610,382,150,75,coor))
		{
			
			return;
		}
		
		
		
	}
}


void logining_sc(struct usr_list *r_head,struct usr_list *l_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p,struct airplane_list* air_head,struct info_list* info_head)//普通用户进入后，查询、购票、实名
{
	//struct usr_list *new = make_loginusr_new(l_head,ld,fd,coor,p);//登陆链表
	while(1)
	{
		bmp_tp *bp = create_bmp("./login_chioce1.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		bmp_tp *bp1 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,22,bp1,ld,0);
		destroy_bmp(bp1);
		
		bmp_tp *bp2 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,142,bp2,ld,0);
		destroy_bmp(bp2);
		
		bmp_tp *bp3 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,262,bp3,ld,0);
		destroy_bmp(bp3);
		
		bmp_tp *bp4 = create_bmp("./1.bmp");//打开bmp
		show_bmp(610,382,bp4,ld,0);
		destroy_bmp(bp4);
		
		bmp_tp *bp5 = create_bmp("./9.bmp");//打开bmp
		show_bmp(450,382,bp5,ld,0);
		destroy_bmp(bp5);
		
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		
		if(Rectangle_button(610,22,150,75,coor))
		{
			usr_show_airplane_meun(air_head,ld,fd,coor);
		}
		
		if(Rectangle_button(610,142,150,75,coor))
		{
			buy_airplane_meun(r_head,air_head,p,ld,fd,coor);
		}
		
		if(Rectangle_button(610,262,150,75,coor))
		{
			info_ture_name(r_head,info_head,p,ld,fd,coor);
		}
		
		if(Rectangle_button(610,382,150,75,coor))
		{
			return;
		}
		
		if(Rectangle_button(450,382,150,75,coor))
		{
			logining_sc_next(r_head,l_head,ld,fd,coor,p,air_head);
			
		}
		
	}
	
}

void logining_spsc_next(struct spusr_list* sl_head,lcd_tp *ld,int fd,coor_tp coor,struct spusr_list* p,struct airplane_list* air_head,struct usr_list *r_head)//超级用户进入后，生效保险，删除数据
{
	while(1)
	{
		
		bmp_tp *bpws = create_bmp("./splogin_chioce2.bmp");//打开bmp
		show_bmp(0,0,bpws,ld,0);//展示bmp
		destroy_bmp(bpws);//删除bmp
		//show_bmp_mmap_c("welcome_s.bmp");
		
		bmp_tp *bp1 = create_bmp("./7.bmp");//打开bmp
		show_bmp(600,40,bp1,ld,0);
		destroy_bmp(bp1);
		
		bmp_tp *bp2 = create_bmp("./7.bmp");//打开bmp
		show_bmp(600,200,bp2,ld,0);
		destroy_bmp(bp2);
		
		bmp_tp *bp3 = create_bmp("./8.bmp");//打开bmp
		show_bmp(600,360,bp3,ld,0);
		destroy_bmp(bp3);
		
		
		
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		
		if(Rectangle_button(600,40,150,75,coor))
		{
			effective_insurance(air_head,ld,fd,coor,r_head);
		}
		
		if(Rectangle_button(600,200,150,75,coor))
		{
			delete_airplane_meun(air_head,ld,fd,coor,r_head);
		}
		
		if(Rectangle_button(600,360,150,75,coor))
		{
			
			break;
		}
		
		
	}
}


void logining_spsc(struct spusr_list* sl_head,lcd_tp *ld,int fd,coor_tp coor,struct spusr_list* p,struct airplane_list* air_head,struct usr_list *r_head)//超级用户进入后，查看航班、航班录入
{
	while(1)
	{
		
		bmp_tp *bpws = create_bmp("./splogin_chioce1.bmp");//打开bmp
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
		
		bmp_tp *bp4 = create_bmp("./9.bmp");//打开bmp
		show_bmp(440,360,bp4,ld,0);
		destroy_bmp(bp4);
		
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		
		if(Rectangle_button(600,40,150,75,coor))
		{
			bmp_tp *bp5 = create_bmp("./usr_showing.bmp");//打开bmp
			show_bmp(0,0,bp5,ld,0);//展示bmp
			destroy_bmp(bp5);//删除bmp
			sleep(2);
			show_all_airplane(air_head);
		}
		
		if(Rectangle_button(600,200,150,75,coor))
		{
			airplane_input(air_head,ld,fd,coor);
		}
		
		if(Rectangle_button(600,360,150,75,coor))
		{
			
			break;
		}
		
		if(Rectangle_button(440,360,150,75,coor))
		{
			logining_spsc_next(sl_head,ld,fd,coor,p,air_head,r_head);
			
		}
	}
}
void usr_login_menu(struct usr_list *r_head,struct spusr_list *sr_head,struct usr_list *l_head,struct spusr_list *sl_head,lcd_tp *ld,int fd,coor_tp coor,struct airplane_list* air_head,struct info_list* info_head)//用户登陆菜单
{
	
	while(1)
	{
		
		bmp_tp *bp = create_bmp("./chioce_login.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		
		bmp_tp *bp1 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,22,bp1,ld,0);
		destroy_bmp(bp1);
		
		bmp_tp *bp2 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,142,bp2,ld,0);
		destroy_bmp(bp2);
		
		bmp_tp *bp3 = create_bmp("./7.bmp");//打开bmp
		show_bmp(610,262,bp3,ld,0);
		destroy_bmp(bp3);
		
		bmp_tp *bp4 = create_bmp("./1.bmp");//打开bmp
		show_bmp(610,382,bp4,ld,0);
		destroy_bmp(bp4);
		
		bmp_tp *bp5 = create_bmp("./2.bmp");//打开bmp
		show_bmp(10,382,bp5,ld,0);
		destroy_bmp(bp5);
		
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		
		if(Rectangle_button(610,22,150,75,coor))
		{
			usr_login(r_head,l_head,ld,fd,coor,air_head,info_head);
		}
		
		if(Rectangle_button(610,142,150,75,coor))
		{
			spusr_login(sr_head,sl_head,ld,fd,coor,air_head,r_head);
		}
		
		if(Rectangle_button(610,262,150,75,coor))
		{
			find_passwd(r_head,ld,fd);
		}
		
		if(Rectangle_button(610,382,150,75,coor))
		{
			
			return;
		}
		if(Rectangle_button(10,382,150,75,coor))
		{
			
			theck_in(r_head,ld,fd);
		}
	}
}
