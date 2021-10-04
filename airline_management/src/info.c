#include "text.h"




void info_name_my(struct usr_list *r_head,struct info_list* info_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//实名认证本账号
{
	char info_name[10]={0};
	char filename[50] = {0};
	char filedata[400] = {0};
	int ret;
	
	if(p->verified == 1)
	{
		bmp_tp *bp10 = create_bmp("./usr_infosu.bmp");//打开bmp
		show_bmp(0,0,bp10,ld,0);//展示bmp
		destroy_bmp(bp10);//删除bmp
		sleep(2);
	}
	if(p->verified == 0)
	{
		bmp_tp *bp4 = create_bmp("./infoing.bmp");//打开bmp
		show_bmp(0,0,bp4,ld,0);//展示bmp
		destroy_bmp(bp4);//删除bmp
		sleep(2);
		struct info_list *q = NULL;
		for(q=info_head->next;q != info_head;q=q->next)
		{
			if(strcmp(p->name,q->name)==0)
			{
				if(strcmp(p->id_card_number,q->id_card_number)==0)
				{
					bmp_tp *bp6 = create_bmp("./infosu.bmp");//打开bmp
					show_bmp(0,0,bp6,ld,0);//展示bmp
					destroy_bmp(bp6);//删除bmp
					sleep(2);
					p->verified = 1;
					p->identity = q->identity;
					sprintf(filename,"./usr_data/%s.txt",p->id_card_number);
					
					
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
					//printf("ret = %d\n",ret);
					bzero(filename,strlen(filename));
					bzero(filedata,strlen(filedata));
					break;
				}
			}
		}
		if(p->verified == 0)
		{
			bmp_tp *bp7 = create_bmp("./info_error.bmp");//打开bmp
			show_bmp(0,0,bp7,ld,0);//展示bmp
			destroy_bmp(bp7);//删除bmp
			sleep(2);
		}
		
	}
}


void info_name_other(struct usr_list *r_head,struct info_list* info_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor)//实名认证其他账号
{
	char info_name[10]={0};
	char filename[50] = {0};
	char filedata[400] = {0};
	int ret;
	
	printf("pls input name:");
	scanf("%s",info_name);



	bmp_tp *bp5 = create_bmp("./infoing.bmp");//打开bmp
	show_bmp(0,0,bp5,ld,0);//展示bmp
	destroy_bmp(bp5);//删除bmp
	sleep(2);
	struct info_list *k = NULL;
	struct usr_list *a = NULL;
	for(a=r_head->next;a != r_head; a = a->next)
	{
		if(strcmp(a->name,info_name)==0)
		{
			if(a->verified == 1)
			{
				bmp_tp *bp10 = create_bmp("./usr_infosu.bmp");//打开bmp
				show_bmp(0,0,bp10,ld,0);//展示bmp
				destroy_bmp(bp10);//删除bmp
				sleep(2);
			}
			if(a->verified == 0)
			{
				for(k=info_head->next;k != info_head;k=k->next)
				{
					if(strcmp(k->name,a->name)==0)
					{
						if(strcmp(k->id_card_number,a->id_card_number)==0)
						{
							bmp_tp *bp8 = create_bmp("./infosu.bmp");//打开bmp
							show_bmp(0,0,bp8,ld,0);//展示bmp
							destroy_bmp(bp8);//删除bmp
							sleep(2);
							a->verified = 1;
							a->identity = k->identity;
							sprintf(filename,"./usr_data/%s.txt",a->id_card_number);
							
							
							sprintf(filedata,"%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d",a->name
																		 ,a->passwd
																		 ,a->tel
																		 ,a->id_card_number
																		 ,a->question
																		 ,a->answer
																		 ,a->leave_say
																		 ,a->usr_money
																		 ,a->age
																		 ,a->vip_grade
																		 ,a->identity
																		 ,a->usr_integral
																		 ,a->verified
																		 ,a->grand_total_money);
																		 
							FILE *fp = fopen(filename,"w");
							if(fp == NULL)
								printf("fopen error!\n");
							
							ret = fwrite(filedata,strlen(filedata),1,fp);
							//printf("ret = %d\n",ret);
							bzero(filename,strlen(filename));
							bzero(filedata,strlen(filedata));
							fclose(fp);
						}
					}
				}
			}
		}
	}	
		/*if(a->verified == 0)
		{
			bmp_tp *bp9 = create_bmp("./info_error.bmp");//打开bmp
			show_bmp(0,0,bp9,ld,0);//展示bmp
			destroy_bmp(bp9);//删除bmp
			sleep(2);
		
		}	*/	
}
