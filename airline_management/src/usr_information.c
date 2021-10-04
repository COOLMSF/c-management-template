#include "text.h"


void change_age(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p)//修改年龄
{
	int age;
	bmp_tp *bpws = create_bmp("./changing.bmp");//打开bmp
	show_bmp(0,0,bpws,ld,0);//展示bmp
	destroy_bmp(bpws);//删除bmp
	sleep(2);
	printf("pls iuput new age:");
	scanf("%d",&age);
	p->age = age;
	if(p->age == age)
	{
		bmp_tp *bp1 = create_bmp("./change_sc.bmp");//打开bmp
		show_bmp(0,0,bp1,ld,0);//展示bmp
		destroy_bmp(bp1);//删除bmp
		sleep(2);
	}
	else{
		printf("new passwd not same!!!\n");
		bmp_tp *bp2 = create_bmp("./change_error.bmp");//打开bmp
		show_bmp(0,0,bp2,ld,0);//展示bmp
		destroy_bmp(bp2);//删除bmp
		sleep(2);
	}
}

void change_idcard(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p)//修改身份证
{
	char new_idcard[20];
	bmp_tp *bpws = create_bmp("./changing.bmp");//打开bmp
	show_bmp(0,0,bpws,ld,0);//展示bmp
	destroy_bmp(bpws);//删除bmp
	sleep(2);
	printf("pls iuput new idcard:");
	scanf("%s",new_idcard);
	bzero(p->id_card_number,strlen(p->id_card_number));
	strcpy(p->id_card_number,new_idcard);
	if(strcmp(p->id_card_number,new_idcard)==0)
	{
		bmp_tp *bp1 = create_bmp("./change_sc.bmp");//打开bmp
		show_bmp(0,0,bp1,ld,0);//展示bmp
		destroy_bmp(bp1);//删除bmp
		sleep(2);
	}
	else{
		printf("change idcard error!!!\n");
		bmp_tp *bp2 = create_bmp("./change_error.bmp");//打开bmp
		show_bmp(0,0,bp2,ld,0);//展示bmp
		destroy_bmp(bp2);//删除bmp
		sleep(2);
	}
}

void change_usr_data_next(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p)//修改个人信息，年龄、身份证
{
	while(1)
	{
		
		bmp_tp *bpws = create_bmp("./change_usr2.bmp");//打开bmp
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
			change_age(r_head,ld,fd,coor,p);
		}
		
		if(Rectangle_button(600,200,150,75,coor))
		{
			change_idcard(r_head,ld,fd,coor,p);
		}
		
		if(Rectangle_button(600,360,150,75,coor))
		{
			
			break;
		}
		
		
	}
}

void change_passwd(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p)//用户更改密码
{
	
	char as[20];
	char new_passwd[10];
	char new_passwd1[10];
	bmp_tp *bpws = create_bmp("./changing.bmp");//打开bmp
	show_bmp(0,0,bpws,ld,0);//展示bmp
	destroy_bmp(bpws);//删除bmp
	sleep(2);
	printf("pls ask this question:");
	printf("%s\n",p->question);
	scanf("%s",as);
	
	if(strcmp(as,p->answer)==0);
	{
		printf("pls input new passwd:");
		scanf("%s",new_passwd);
		printf("pls input new passwd again:");
		scanf("%s",new_passwd1);
		
		if(strcmp(new_passwd,new_passwd1)==0)
		{
			bmp_tp *bp1 = create_bmp("./change_sc.bmp");//打开bmp
			show_bmp(0,0,bp1,ld,0);//展示bmp
			destroy_bmp(bp1);//删除bmp
			sleep(2);
			
			strcpy(p->passwd,new_passwd);
		}
		else{
			printf("new passwd not same!!!\n");
			bmp_tp *bp2 = create_bmp("./change_error.bmp");//打开bmp
			show_bmp(0,0,bp2,ld,0);//展示bmp
			destroy_bmp(bp2);//删除bmp
			sleep(2);
		}
	}
}

void change_tel(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p)//用户更改电话
{
	char new_tel[12];
	bmp_tp *bpws = create_bmp("./changing.bmp");//打开bmp
	show_bmp(0,0,bpws,ld,0);//展示bmp
	destroy_bmp(bpws);//删除bmp
	sleep(2);
	printf("pls iuput new tel:");
	scanf("%s",new_tel);
	bzero(p->tel,strlen(p->tel));
	strcpy(p->tel,new_tel);
	if(strcmp(p->tel,new_tel)==0)
	{
		bmp_tp *bp1 = create_bmp("./change_sc.bmp");//打开bmp
		show_bmp(0,0,bp1,ld,0);//展示bmp
		destroy_bmp(bp1);//删除bmp
		sleep(2);
	}
	else{
		printf("new passwd not same!!!\n");
		bmp_tp *bp2 = create_bmp("./change_error.bmp");//打开bmp
		show_bmp(0,0,bp2,ld,0);//展示bmp
		destroy_bmp(bp2);//删除bmp
		sleep(2);
	}
}

void show_usr_data(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p)//展示用户信息
{
	
	bmp_tp *bpws = create_bmp("./usr_showing.bmp");//打开bmp
	show_bmp(0,0,bpws,ld,0);//展示bmp
	destroy_bmp(bpws);//删除bmp
	sleep(2);
	printf("plane name:%s\n",p->name);
	printf("tel:%s\n",p->tel);
	printf("id_card_number:%s\n",p->id_card_number);
	printf("age:%d\n",p->age);
	printf("vip_grade:%d\n",p->vip_grade);
	printf("usr_money:%d\n",p->usr_money);
	printf("identity:%d\n",p->identity);
	printf("usr_integral:%d\n",p->usr_integral);
	printf("verified:%d\n",p->verified);
	printf("grand_total_money:%d\n",p->grand_total_money);
	printf("================================\n");
	

}

void change_usr_data(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p)//修改个人信息，密码、电话
{
	while(1)
	{
		
		bmp_tp *bpws = create_bmp("./change_usr1.bmp");//打开bmp
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
		
		bmp_tp *bp5 = create_bmp("./show.bmp");//打开bmp
		show_bmp(10,360,bp5,ld,0);
		destroy_bmp(bp5);
		
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		
		if(Rectangle_button(600,40,150,75,coor))
		{
			change_passwd(r_head,ld,fd,coor,p);
		}
		
		if(Rectangle_button(600,200,150,75,coor))
		{
			change_tel(r_head,ld,fd,coor,p);
		}
		
		if(Rectangle_button(600,360,150,75,coor))
		{
			
			break;
		}
		
		if(Rectangle_button(440,360,150,75,coor))
		{
			change_usr_data_next(r_head,ld,fd,coor,p);
			
		}
		if(Rectangle_button(10,360,150,75,coor))
		{
			show_usr_data(r_head,ld,fd,coor,p);
			
		}
		
	}
	char filename[50] = {0};
	sprintf(filename,"./usr_data/%s.txt",p->id_card_number);
	
	char filedata[400] = {0};
	sprintf(filedata,"%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d",p->name
												 ,p->passwd
												 ,p->tel
												 ,p->id_card_number
												 ,p->question
												 ,p->answer
												 ,p->leave_say
												 ,p->usr_money
												 ,p->age
												 ,p->vip_grade
												 ,p->identity
												 ,p->usr_integral
												 ,p->verified
												 ,p->grand_total_money);
												 
	FILE *fp = fopen(filename,"w");
	if(fp == NULL)
		printf("fopen error!\n");
	
	fwrite(filedata,strlen(filedata),1,fp);
	
	fclose(fp);
}
