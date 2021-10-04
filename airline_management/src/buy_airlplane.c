#include "text.h"


int buy_discount(struct airplane_list* q,struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//选座位后打折处理
{
	int usr_give_money;
	usr_give_money = q->how_much;
	if(p->identity == SOLDIER || p->identity == HANDICAPPED)
	{
		usr_give_money = usr_give_money/2;
	}
	if(p->vip_grade == 1)
	{
		usr_give_money = usr_give_money*0.98;
	}
	if(p->vip_grade == 2)
	{
		usr_give_money = usr_give_money*0.95;
	}
	if(p->vip_grade == 3)
	{
		usr_give_money = usr_give_money*0.9;
	}
	if(p->vip_grade == 4)
	{
		usr_give_money = usr_give_money*0.85;
	}
	if(p->vip_grade == 5)
	{
		usr_give_money = usr_give_money*0.75;
	}
	printf("this page:%d\n",usr_give_money);
	return usr_give_money;
}

int pay_money(int money,struct airplane_list* q,struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//用户账号扣钱
{
	char usr_chioce[5];
	int insurance;
	insurance = 0;
	p->usr_money -= money;
	if(p->usr_money < 0)
	{
		bmp_tp *bp5 = create_bmp("/project1/insufficient_balance.bmp");//打开bmp
		show_bmp(0,0,bp5,ld,0);//展示bmp
		destroy_bmp(bp5);//删除bmp
		sleep(2);
		return insurance;
	}
	bmp_tp *bp = create_bmp("/project1/insurance.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
	printf("did you buy delay insurance?(Y/N):");
	scanf("%s",usr_chioce);
	if(strcmp(usr_chioce,"Y")==0)
	{
		insurance = 1;
		p->usr_money -= 50;
		bmp_tp *bp1 = create_bmp("./buy_sc.bmp");//打开bmp
		show_bmp(0,0,bp1,ld,0);//展示bmp
		destroy_bmp(bp1);//删除bmp
	}
	return insurance;
}

void buy_airplane_my(struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//本账号买票
{
	char airplane_number[10];
	struct airplane_list* q = NULL;
	int seat_chioce;
	int ret_money;
	int ret_insurance;
	char filename[50] = {0};
	char filedata[400] = {0};
	char filename1[50] = {0};
	char filedata1[400] = {0};
	char filename2[50] = {0};
	char filedata2[400] = {0};
	char filename3[50] = {0};
	char filedata3[400] = {0};
	char cmd[50] = {0};
	char cmd1[50] = {0};
	char filename4[50] = {0};
	char filedata4[400] = {0};
	char filename5[50] = {0};
	char filedata5[400] = {0};
	
	struct dirent *ep = NULL;
	if(p->verified == 1)
	{
		bmp_tp *bp = create_bmp("/project1/buying.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		sleep(2);
		
		printf("pls input airplane number:");
		scanf("%s",airplane_number);
		
		sprintf(cmd1,"%s.txt",airplane_number);
		sprintf(cmd,"/project1/usr_airplane_data/%s",p->name);
		DIR* dp = opendir(cmd);
		if(dp == NULL)
			printf("opendir error!!!\n");
		chdir(cmd);
		
		
		while(1)
		{
			ep = readdir(dp);
			if(ep == NULL)
			{
				break;
			}
			if(ep->d_name[0] == '.')
			{
				continue;
			}
			
			if(strcmp(ep->d_name,cmd1)==0)
			{
				bmp_tp *bp = create_bmp("/project1/buy_same.bmp");//打开bmp
				show_bmp(0,0,bp,ld,0);//展示bmp
				destroy_bmp(bp);//删除bmp
				sleep(2);
				chdir("../..");
				closedir(dp);
				return;
				
			}
		}
		
		chdir("../..");
		closedir(dp);
		
		for(q=air_head->next;q != air_head;q=q->next)
		{
			if(strcmp(q->number,airplane_number)==0)
			{
				printf("pls chioce seat(1.leave_seat_window:%d 2.leave_seat_unwindow:%d):",q->leave_seat_window,q->leave_seat_unwindow);
				scanf("%d",&seat_chioce);
				if(seat_chioce == 1)
				{
					if(q->leave_seat_window>0)
					{
						q->leave_seat_window = q->leave_seat_window-1;
						ret_money = buy_discount(q,air_head,p,ld,fd,coor);
						ret_insurance = pay_money(ret_money,q,air_head,p,ld,fd,coor);
						if(p->usr_money<0)
						{
							p->usr_money += ret_money;
							bmp_tp *bp5 = create_bmp("/project1/insufficient_balance.bmp");//打开bmp
							show_bmp(0,0,bp5,ld,0);//展示bmp
							destroy_bmp(bp5);//删除bmp
							sleep(2);
							return;
						}
						printf("Congratulations!! you get 100 usr_integral!!!\n");
						p->usr_integral += 100;
						printf("you have %d usr_integral\n",p->usr_integral);
						
						//保存机票
						sprintf(filename2,"./usr_airplane_data/%s/%s.txt",p->name,airplane_number);
						sprintf(filedata2,"%s,%s,%s,%s,%s,%s,%d,%d",p->name
																	 ,q->departure
																	 ,q->destination
																	 ,q->number
																	 ,q->day
																	 ,q->time
																	 ,ret_insurance
																	 ,ret_money);
																	 
						FILE *fp2 = fopen(filename2,"w");
						if(fp2 == NULL)
							printf("fopen error!\n");
						
						fwrite(filedata2,strlen(filedata2),1,fp2);
						
						fclose(fp2);
						//保存用户信息
						sprintf(filename3,"./usr_data/%s.txt",p->id_card_number);
						sprintf(filedata3,"%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d",p->name
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
																	 
						FILE *fp3 = fopen(filename3,"w");
						if(fp3 == NULL)
							printf("fopen error!\n");
						
						fwrite(filedata3,strlen(filedata3),1,fp3);
						
						fclose(fp3);
						//保存航班信息
						
						sprintf(filename4,"./airplane_data/%s.txt",q->number);
						
						
						sprintf(filedata4,"%s,%s,%s,%s,%s,%s,%d,%d,%d",q->number
																	 ,q->departure
																	 ,q->destination
																	 ,q->day
																	 ,q->airplane_type
																	 ,q->time
																	 ,q->how_much
																	 ,q->leave_seat_window
																	 ,q->leave_seat_unwindow);
																	 
						FILE *fp4 = fopen(filename4,"w");
						if(fp4 == NULL)
							printf("fopen error!\n");
						
						fwrite(filedata4,strlen(filedata4),1,fp4);
						
						fclose(fp4);
						bmp_tp *bp8 = create_bmp("/project1/buy_sc.bmp");//打开bmp
						show_bmp(0,0,bp8,ld,0);//展示bmp
						destroy_bmp(bp8);//删除bmp
						sleep(2);
					}
					else if(q->leave_seat_window<0)
					{
						bmp_tp *bp1 = create_bmp("/project1/not_seat.bmp");//打开bmp
						show_bmp(0,0,bp1,ld,0);//展示bmp
						destroy_bmp(bp1);//删除bmp
						sleep(2);
					}
					
				}
				if(seat_chioce == 2)
				{
					if(q->leave_seat_unwindow>0)
					{
						q->leave_seat_unwindow = q->leave_seat_unwindow-1;
						ret_money = buy_discount(q,air_head,p,ld,fd,coor);
						ret_insurance = pay_money(ret_money,q,air_head,p,ld,fd,coor);
						if(p->usr_money<0)
						{
							p->usr_money += ret_money;
							bmp_tp *bp6 = create_bmp("/project1/insufficient_balance.bmp");//打开bmp
							show_bmp(0,0,bp6,ld,0);//展示bmp
							destroy_bmp(bp6);//删除bmp
							sleep(2);
							return;
						}
						printf("Congratulations!! you get 100 usr_integral!!!\n");
						p->usr_integral += 100;
						printf("you have %d usr_integral\n",p->usr_integral);
						//保存机票
						sprintf(filename,"./usr_airplane_data/%s/%s.txt",p->name,airplane_number);
						sprintf(filedata,"%s,%s,%s,%s,%s,%s,%d,%d",p->name
																	 ,q->departure
																	 ,q->destination
																	 ,q->number
																	 ,q->day
																	 ,q->time
																	 ,ret_insurance
																	 ,ret_money);
																	 
						FILE *fp = fopen(filename,"w");
						if(fp == NULL)
							printf("fopen error!\n");
						
						fwrite(filedata,strlen(filedata),1,fp);
						
						fclose(fp);
						//保存用户信息
						sprintf(filename1,"./usr_data/%s.txt",p->id_card_number);
						sprintf(filedata1,"%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d",p->name
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
																	 
						FILE *fp1 = fopen(filename1,"w");
						if(fp1 == NULL)
							printf("fopen error!\n");
						
						fwrite(filedata1,strlen(filedata1),1,fp1);
						
						fclose(fp1);
						//保存航班信息
						
						sprintf(filename5,"./airplane_data/%s.txt",q->number);
						
						
						sprintf(filedata5,"%s,%s,%s,%s,%s,%s,%d,%d,%d",q->number
																	 ,q->departure
																	 ,q->destination
																	 ,q->day
																	 ,q->airplane_type
																	 ,q->time
																	 ,q->how_much
																	 ,q->leave_seat_window
																	 ,q->leave_seat_unwindow);
																	 
						FILE *fp5 = fopen(filename5,"w");
						if(fp5 == NULL)
							printf("fopen error!\n");
						
						fwrite(filedata5,strlen(filedata5),1,fp5);
						
						fclose(fp5);
						bmp_tp *bp7 = create_bmp("/project1/buy_sc.bmp");//打开bmp
						show_bmp(0,0,bp7,ld,0);//展示bmp
						destroy_bmp(bp7);//删除bmp
						sleep(2);
					}
					else if(q->leave_seat_window<0)
					{
						bmp_tp *bp2 = create_bmp("/project1/not_seat.bmp");//打开bmp
						show_bmp(0,0,bp2,ld,0);//展示bmp
						destroy_bmp(bp2);//删除bmp
						sleep(2);
					}
				}
				else if(q->leave_seat_window<0)
				{
					bmp_tp *bp3 = create_bmp("/project1/not_seat.bmp");//打开bmp
					show_bmp(0,0,bp3,ld,0);//展示bmp
					destroy_bmp(bp3);//删除bmp
					sleep(2);
				}
			}
		}
		
		
	}
	if(p->verified == 0)
	{
		bmp_tp *bp4 = create_bmp("/project1/uninfo.bmp");//打开bmp
		show_bmp(0,0,bp4,ld,0);//展示bmp
		destroy_bmp(bp4);//删除bmp
		sleep(2);
	}
}

void buy_airplane_other(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* r_head)//其他账号买票
{
	char chioce_usr[10];
	char airplane_number[10];
	struct airplane_list* q = NULL;
	int seat_chioce;
	int ret_money;
	int ret_insurance;
	char filename[50] = {0};
	char filedata[400] = {0};
	char filename1[50] = {0};
	char filedata1[400] = {0};
	char filename2[50] = {0};
	char filedata2[400] = {0};
	char filename3[50] = {0};
	char filedata3[400] = {0};
	char cmd[50] = {0};
	char cmd1[50] = {0};
	char filename4[50] = {0};
	char filedata4[400] = {0};
	char filename5[50] = {0};
	char filedata5[400] = {0};
	struct dirent *ep = NULL;
	
	struct usr_list* p = NULL;
	printf("pls input usr_name:");
	scanf("%s",chioce_usr);
	for(p=r_head->next;p != r_head;p=p->next)
	{
		if(strcmp(chioce_usr,p->name)==0)
		{
			
			if(p->verified == 1)
			{
				bmp_tp *bp = create_bmp("./buying.bmp");//打开bmp
				show_bmp(0,0,bp,ld,0);//展示bmp
				destroy_bmp(bp);//删除bmp
				sleep(2);
				
				printf("pls input airplane number:");
				scanf("%s",airplane_number);
				
				sprintf(cmd1,"%s.txt",airplane_number);
				sprintf(cmd,"/project1/usr_airplane_data/%s",p->name);
				DIR* dp = opendir(cmd);
				if(dp == NULL)
					printf("opendir error!!!\n");
				chdir(cmd);
				
				
				while(1)
				{
					ep = readdir(dp);
					if(ep == NULL)
					{
						break;
					}
					if(ep->d_name[0] == '.')
					{
						continue;
					}
					
					if(strcmp(ep->d_name,cmd1)==0)
					{
						bmp_tp *bp = create_bmp("/project1/buy_same.bmp");//打开bmp
						show_bmp(0,0,bp,ld,0);//展示bmp
						destroy_bmp(bp);//删除bmp
						sleep(2);
						chdir("../..");
						closedir(dp);
						return;
						
					}
				}
				
				chdir("../..");
				closedir(dp);
				
				for(q=air_head->next;q != air_head;q=q->next)
				{
					if(strcmp(q->number,airplane_number)==0)
					{
						printf("pls chioce seat(1.leave_seat_window:%d 2.leave_seat_unwindow:%d):",q->leave_seat_window,q->leave_seat_unwindow);
						scanf("%d",&seat_chioce);
						if(seat_chioce == 1)
						{
							if(q->leave_seat_window>0)
							{
								q->leave_seat_window = q->leave_seat_window-1;
								ret_money = buy_discount(q,air_head,p,ld,fd,coor);
								ret_insurance = pay_money(ret_money,q,air_head,p,ld,fd,coor);
								if(p->usr_money<0)
								{
									bmp_tp *bp5 = create_bmp("./insufficient_balance.bmp");//打开bmp
									show_bmp(0,0,bp5,ld,0);//展示bmp
									destroy_bmp(bp5);//删除bmp
									sleep(2);
									return;
								}
								printf("Congratulations!! you get 100 usr_integral!!!\n");
								p->usr_integral += 100;
								printf("you have %d usr_integral\n",p->usr_integral);
								
								//保存机票
								sprintf(filename2,"./usr_airplane_data/%s/%s.txt",p->name,airplane_number);
								sprintf(filedata2,"%s,%s,%s,%s,%s,%s,%d,%d",p->name
																			 ,q->departure
																			 ,q->destination
																			 ,q->number
																			 ,q->day
																			 ,q->time
																			 ,ret_insurance
																			 ,ret_money);
																			 
								FILE *fp2 = fopen(filename2,"w");
								if(fp2 == NULL)
									printf("fopen error!\n");
								
								fwrite(filedata2,strlen(filedata2),1,fp2);
								
								fclose(fp2);
								//保存用户信息
								sprintf(filename3,"./usr_data/%s.txt",p->id_card_number);
								sprintf(filedata3,"%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d",p->name
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
																			 
								FILE *fp3 = fopen(filename3,"w");
								if(fp3 == NULL)
									printf("fopen error!\n");
								
								fwrite(filedata3,strlen(filedata3),1,fp3);
								
								fclose(fp3);
								//保存航班信息
								
								sprintf(filename4,"./airplane_data/%s.txt",q->number);
								
								
								sprintf(filedata4,"%s,%s,%s,%s,%s,%s,%d,%d,%d",q->number
																			 ,q->departure
																			 ,q->destination
																			 ,q->day
																			 ,q->airplane_type
																			 ,q->time
																			 ,q->how_much
																			 ,q->leave_seat_window
																			 ,q->leave_seat_unwindow);
																			 
								FILE *fp4 = fopen(filename4,"w");
								if(fp4 == NULL)
									printf("fopen error!\n");
								
								fwrite(filedata4,strlen(filedata4),1,fp4);
								
								fclose(fp4);
								bmp_tp *bp8 = create_bmp("./buy_sc.bmp");//打开bmp
								show_bmp(0,0,bp8,ld,0);//展示bmp
								destroy_bmp(bp8);//删除bmp
								sleep(2);
							}
							else if(q->leave_seat_window<0)
							{
								bmp_tp *bp1 = create_bmp("./not_seat.bmp");//打开bmp
								show_bmp(0,0,bp1,ld,0);//展示bmp
								destroy_bmp(bp1);//删除bmp
								sleep(2);
							}
							
						}
						if(seat_chioce == 2)
						{
							if(q->leave_seat_unwindow>0)
							{
								q->leave_seat_unwindow = q->leave_seat_unwindow-1;
								ret_money = buy_discount(q,air_head,p,ld,fd,coor);
								ret_insurance = pay_money(ret_money,q,air_head,p,ld,fd,coor);
								if(p->usr_money<0)
								{
									bmp_tp *bp6 = create_bmp("./insufficient_balance.bmp");//打开bmp
									show_bmp(0,0,bp6,ld,0);//展示bmp
									destroy_bmp(bp6);//删除bmp
									sleep(2);
									return;
								}
								printf("Congratulations!! you get 100 usr_integral!!!\n");
								p->usr_integral += 100;
								printf("you have %d usr_integral\n",p->usr_integral);
								//保存机票
								sprintf(filename,"./usr_airplane_data/%s/%s.txt",p->name,airplane_number);
								sprintf(filedata,"%s,%s,%s,%s,%s,%s,%d,%d",p->name
																			 ,q->departure
																			 ,q->destination
																			 ,q->number
																			 ,q->day
																			 ,q->time
																			 ,ret_insurance
																			 ,ret_money);
																			 
								FILE *fp = fopen(filename,"w");
								if(fp == NULL)
									printf("fopen error!\n");
								
								fwrite(filedata,strlen(filedata),1,fp);
								
								fclose(fp);
								//保存用户信息
								sprintf(filename1,"./usr_data/%s.txt",p->id_card_number);
								sprintf(filedata1,"%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d",p->name
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
																			 
								FILE *fp1 = fopen(filename1,"w");
								if(fp1 == NULL)
									printf("fopen error!\n");
								
								fwrite(filedata1,strlen(filedata1),1,fp1);
								
								fclose(fp1);
								//保存航班信息
								
								sprintf(filename5,"./airplane_data/%s.txt",q->number);
								
								
								sprintf(filedata5,"%s,%s,%s,%s,%s,%s,%d,%d,%d",q->number
																			 ,q->departure
																			 ,q->destination
																			 ,q->day
																			 ,q->airplane_type
																			 ,q->time
																			 ,q->how_much
																			 ,q->leave_seat_window
																			 ,q->leave_seat_unwindow);
																			 
								FILE *fp5 = fopen(filename5,"w");
								if(fp5 == NULL)
									printf("fopen error!\n");
								
								fwrite(filedata5,strlen(filedata5),1,fp5);
								
								fclose(fp5);
								bmp_tp *bp7 = create_bmp("./buy_sc.bmp");//打开bmp
								show_bmp(0,0,bp7,ld,0);//展示bmp
								destroy_bmp(bp7);//删除bmp
								sleep(2);
							}
							else if(q->leave_seat_window<0)
							{
								bmp_tp *bp2 = create_bmp("./not_seat.bmp");//打开bmp
								show_bmp(0,0,bp2,ld,0);//展示bmp
								destroy_bmp(bp2);//删除bmp
								sleep(2);
							}
						}
						else if(q->leave_seat_window<0)
						{
							bmp_tp *bp3 = create_bmp("./not_seat.bmp");//打开bmp
							show_bmp(0,0,bp3,ld,0);//展示bmp
							destroy_bmp(bp3);//删除bmp
							sleep(2);
						}
					}
				}
				
				
			}
			if(p->verified == 0)
			{
				bmp_tp *bp4 = create_bmp("./uninfo.bmp");//打开bmp
				show_bmp(0,0,bp4,ld,0);//展示bmp
				destroy_bmp(bp4);//删除bmp
				sleep(2);
			}
			
			
		}
	}
}

void buy_airplane_chioce(struct usr_list* r_head,struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//买票功能,自己买、帮他人买
{
	while(1)
	{
		
		bmp_tp *bp = create_bmp("./buy_airplane_chioce.bmp");//打开bmp
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
			buy_airplane_my(air_head,p,ld,fd,coor);
		}
		
		if(Rectangle_button(600,200,150,75,coor))
		{
			buy_airplane_other(air_head,ld,fd,coor,r_head);
		}
		
		if(Rectangle_button(600,360,150,75,coor))
		{
			
			return;
		}
	}
}



void usr_recharge(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//充值功能
{
	int recharge;
	printf("pls input recharge:");
	scanf("%d",&recharge);
	bmp_tp *bp = create_bmp("./recharging.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
	sleep(2);
	
	p->usr_money += recharge;
	p->grand_total_money += recharge;
	if(p->grand_total_money>5000 && p->grand_total_money<=10000)
	{
		p->vip_grade = 1;
		p->usr_money += 300;
		p->grand_total_money += 300;
	}
	else if(p->grand_total_money>10000 && p->grand_total_money<=20000)
	{
		p->vip_grade = 2;
		p->usr_money += 800;
		p->grand_total_money += 800;
	}
	else if(p->grand_total_money>20000 && p->grand_total_money<=50000)
	{
		p->vip_grade = 3;
		p->usr_money += 2000;
		p->grand_total_money += 2000;
	}
	else if(p->grand_total_money>50000 && p->grand_total_money<=100000)
	{
		p->vip_grade = 4;
		p->usr_money += 6000;
		p->grand_total_money += 6000;
	}
	else if(p->grand_total_money>100000)
	{
		p->vip_grade = 5;
		p->usr_money += 15000;
		p->grand_total_money += 15000;
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

void show_usr_money(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p)//查看余额
{
	bmp_tp *bp = create_bmp("./usr_showing.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
	sleep(2);
	printf("usr_money:%d\n",p->usr_money);
}

void usr_integral_shop1(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//商品1
{
	if(p->usr_integral >= 200)
	{
		p->usr_integral -= 200;
		bmp_tp *bp5 = create_bmp("./buy_sc.bmp");//打开bmp
		show_bmp(0,0,bp5,ld,0);//展示bmp
		destroy_bmp(bp5);//删除bmp
		sleep(2);
	}
	if(p->usr_integral < 200)
	{
		bmp_tp *bp = create_bmp("./insufficient_balance.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		sleep(2);
	}
}

void usr_integral_shop2(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//商品2
{
	if(p->usr_integral >= 300)
	{
		p->usr_integral -= 300;
		bmp_tp *bp5 = create_bmp("./buy_sc.bmp");//打开bmp
		show_bmp(0,0,bp5,ld,0);//展示bmp
		destroy_bmp(bp5);//删除bmp
		sleep(2);
	}
	if(p->usr_integral < 300)
	{
		bmp_tp *bp = create_bmp("./insufficient_balance.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		sleep(2);
	}
}

void usr_integral_shop3(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//商品3
{
	if(p->usr_integral >= 1000)
	{
		p->usr_integral -= 1000;
		bmp_tp *bp5 = create_bmp("./buy_sc.bmp");//打开bmp
		show_bmp(0,0,bp5,ld,0);//展示bmp
		destroy_bmp(bp5);//删除bmp
		sleep(2);
	}
	if(p->usr_integral < 1000)
	{
		bmp_tp *bp = create_bmp("./insufficient_balance.bmp");//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		sleep(2);
	}
}



void usr_integral_shop(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//积分商城
{
	bmp_tp *bp = create_bmp("./shop_meun.bmp");//打开bmp
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
		usr_integral_shop1(p,ld,fd,coor);
		
	}
	
	if(Rectangle_button(610,142,150,75,coor))
	{
		usr_integral_shop2(p,ld,fd,coor);
	}
	
	if(Rectangle_button(610,262,150,75,coor))
	{
		usr_integral_shop3(p,ld,fd,coor);
	}
	
	if(Rectangle_button(610,382,150,75,coor))
	{
		save_usr(p);
		return;
	}
	
	
}

void refund_airplane(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p)//退票
{
	bmp_tp *bp = create_bmp("/project1/refunding.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
	char plane_number[10] = {0};
	printf("pls input airplane number:");
	scanf("%s",plane_number);
	char seqs[] = ",";
	char* tmp = NULL;
	char cmd[50] = {0};
	char cmd1[50] = {0};
	char cmd2[50] = {0};
	char cmd3[50] = {0};
	char buf[300] = {0};
	int money;
	sprintf(cmd1,"%s.txt",plane_number);
	sprintf(cmd,"/project1/usr_airplane_data/%s",p->name);
	DIR* dp = opendir(cmd);
	if(dp == NULL)
		printf("opendir error!!!\n");
	chdir(cmd);
	
	struct dirent *ep = NULL;
	while(1)
	{
		ep = readdir(dp);
		if(ep == NULL)
		{
			break;
		}
		if(ep->d_name[0] == '.')
		{
			continue;
		}
		if(strcmp(ep->d_name,cmd1)==0)
		{
			sprintf(cmd3,"/project1/usr_airplane_data/%s/%s",p->name,ep->d_name);
			
			FILE *fp = fopen(cmd3,"r");
			if(fp == NULL)
				printf("fopen error!!!\n");
			
			
			fread(buf,30,10,fp);
			fclose(fp);
			tmp = strtok(buf,seqs);
			tmp = strtok(NULL,seqs);
			tmp = strtok(NULL,seqs);
			tmp = strtok(NULL,seqs);
			tmp = strtok(NULL,seqs);
			tmp = strtok(NULL,seqs);
			tmp = strtok(NULL,seqs);
			tmp = strtok(NULL,seqs);
			money = atoi(tmp);
			if(p->verified == DOCTOR || p->verified == NURSE)
			{
				p->usr_money += money;
			}
			else if(p->verified != DOCTOR && p->verified != NURSE)
			{
				p->usr_money += money*0.95;
			}
			save_usr(p);
			sprintf(cmd2,"rm /project1/usr_airplane_data/%s/%s",p->name,ep->d_name);
			system(cmd2);
			
		}
		
	}
	
	chdir("../..");
	closedir(dp);
}

void change_airplane(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p,struct airplane_list* air_head)//改签
{
	refund_airplane(ld,fd,coor,p);
	show_all_airplane(air_head);
	buy_airplane_my(air_head,p,ld,fd,coor);
	
}
