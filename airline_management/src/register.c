#include "text.h"




void usr_register(struct usr_list *r_head,lcd_tp *ld,int fd)//普通用户注册
{
	bmp_tp *bp = create_bmp("./register.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
	//show_bmp_mmap_c("register.bmp");
	struct usr_list *new = malloc(sizeof(struct usr_list));
	if(new == NULL)
		printf("malloc new error!\n");
	
	printf("pls input your name:");
	scanf("%s",new->name);
	
	printf("pls input your passwd:");
	scanf("%s",new->passwd);
	
	printf("pls input your tel:");
	scanf("%s",new->tel);
	
	printf("pls input your id_card_number:");
	scanf("%s",new->id_card_number);
	
	printf("pls input your age:");
	scanf("%d",&new->age);
	
	printf("pls input your question:");
	scanf("%s",new->question);
	
	printf("pls input your answer:");
	scanf("%s",new->answer);
	
	new->usr_money = 0;
	strcpy(new->leave_say,"not_leave_say");
	new->vip_grade = 0;       //vip等级
	new->identity = 0;       //身份
	new->usr_integral = 0;
	new->verified = 0;
	new->grand_total_money = 0;
	
	
	bmp_tp *bp1 = create_bmp("./register_su.bmp");//打开bmp
	show_bmp(0,0,bp1,ld,0);//展示bmp
	destroy_bmp(bp1);//删除bmp
	sleep(2);
	
	struct usr_list *p = NULL;
	p = r_head->prev;
	
	new->next = r_head;
	r_head->prev = new;
	p->next = new;
	new->prev = p;
	
	char filename[50] = {0};
	char filename1[50] = {0};
	sprintf(filename1,"mkdir /project1/usr_airplane_data/%s",new->name);
	system(filename1);
	sprintf(filename,"./usr_data/%s.txt",new->id_card_number);
	
	char filedata[400] = {0};
	sprintf(filedata,"%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d",new->name
												 ,new->passwd
												 ,new->tel
												 ,new->id_card_number
												 ,new->question
												 ,new->answer
												 ,new->leave_say
												 ,new->usr_money
												 ,new->age
												 ,new->vip_grade
												 ,new->identity
												 ,new->usr_integral
												 ,new->verified
												 ,new->grand_total_money);
												 
	FILE *fp = fopen(filename,"w");
	if(fp == NULL)
		printf("fopen error!\n");
	
	fwrite(filedata,strlen(filedata),1,fp);
	
	fclose(fp);
	
	
}

void spusr_register(struct spusr_list *sr_head,lcd_tp *ld,int fd)//超级用户注册
{
	bmp_tp *bp = create_bmp("./register.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
	//show_bmp_mmap_c("register.bmp");
	
	char spur_passwd[10] = "123456";
	char usr_sppasswd[10];
	printf("pls input register superusr passwd:");
	scanf("%s",usr_sppasswd);
	if(strcmp(spur_passwd,usr_sppasswd) == 0)
	{
		struct spusr_list *new = malloc(sizeof(struct spusr_list));
		if(new == NULL)
			printf("make new error!!!\n");
		printf("passwd correct!\n");
		printf("pls input your name:");
		scanf("%s",new->spname);
		printf("pls input your passwd:");
		scanf("%s",new->sppasswd);
		
		bmp_tp *bp1 = create_bmp("./register_su.bmp");//打开bmp
		show_bmp(0,0,bp1,ld,0);//展示bmp
		destroy_bmp(bp1);//删除bmp
		sleep(2);
		
		struct spusr_list *p = NULL;
		p = sr_head->prev;
		sr_head->prev = new;
		new->next = sr_head;
		p->next = new;
		new->prev = p;
		
		char filename[100] = {0};
		sprintf(filename,"./spusr_data/%s.txt",new->spname);
		
		char filedata[100] = {0};
		sprintf(filedata,"%s,%s",new->spname,new->sppasswd);
		
		FILE *fp = fopen(filename,"w");
		if(fp == NULL)
			printf("fopen error!!\n");
		
		fwrite(filedata,strlen(filedata),1,fp);
		
		fclose(fp);
	}
}
