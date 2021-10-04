#include "text.h"


void save_usr(struct usr_list* p)//保存用户信息
{
	char filename[50] = {0};
	sprintf(filename,"/project1/usr_data/%s.txt",p->id_card_number);
	
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
