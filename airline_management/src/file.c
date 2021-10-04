#include "text.h"


void strtok_data_usr_air(char *buf)//查票切割信息
{
	char seqs[] = ",";
	char* tmp = strtok(buf,seqs);
	printf("name:%s\n",tmp);
	
	tmp = strtok(NULL,seqs);
	printf("departure:%s\n",tmp);
	
	tmp = strtok(NULL,seqs);
	printf("destination:%s\n",tmp);
	
	tmp = strtok(NULL,seqs);
	printf("number:%s\n",tmp);
	
	tmp = strtok(NULL,seqs);
	printf("day:%s\n",tmp);
	
	tmp = strtok(NULL,seqs);
	printf("time:%s\n",tmp);
	
	tmp = strtok(NULL,seqs);
	printf("departure:%d\n",atoi(tmp));
	
	tmp = strtok(NULL,seqs);
	printf("departure:%d\n",atoi(tmp));
	printf("===============================\n");
	
	
	
	
}

void read_file_usr_air(char *filename)//查票打开文件
{
	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
		printf("fopen error!!!\n");
	
	char buf[300] = {0};
	int a=fread(buf,30,10,fp);
	
	fclose(fp);
	strtok_data_usr_air(buf);
	
	return;
}

void cheak_airplane(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p)//查票打开目录
{
	bmp_tp *bp = create_bmp("/project1/usr_showing.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
	sleep(2);
	char cmd[50] = {0};
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
		
		read_file_usr_air(ep->d_name);
	}
	
	chdir("../..");
	closedir(dp);
	return;
}

void airplane_input(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor)//sp录入航班信息
{
	bmp_tp *bp = create_bmp("./airplane_registering.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
	
	struct airplane_list* new = malloc(sizeof(struct airplane_list));
	if(new == NULL)
		printf("make new error!!!!\n");
	
	printf("pls input airplane number:");
	scanf("%s",new->number);
	
	printf("pls input airplane departure:");
	scanf("%s",new->departure);
	
	printf("pls input airplane destination:");
	scanf("%s",new->destination);
	
	printf("pls input airplane day:");
	scanf("%s",new->day);
	
	printf("pls input airplane airplane_type:");
	scanf("%s",new->airplane_type);
	
	printf("pls input airplane time:");
	scanf("%s",new->time);
	
	
	printf("pls input airplane how_much:");
	scanf("%d",&new->how_much);
	
	printf("pls input airplane leave_seat_window:");
	scanf("%d",&new->leave_seat_window);
	
	printf("pls input airplane leave_seat_unwindow:");
	scanf("%d",&new->leave_seat_unwindow);
	
	bmp_tp *bp1 = create_bmp("./airplane_registersu.bmp");//打开bmp
	show_bmp(0,0,bp1,ld,0);//展示bmp
	destroy_bmp(bp1);//删除bmp
	sleep(2);
	
	struct airplane_list *p = NULL;
	p = air_head->prev;
	
	new->next = air_head;
	air_head->prev = new;
	p->next = new;
	new->prev = p;
	
	char filename[50] = {0};
	sprintf(filename,"./airplane_data/%s.txt",new->number);
	
	char filedata[400] = {0};
	sprintf(filedata,"%s,%s,%s,%s,%s,%s,%d,%d,%d",new->number
												 ,new->departure
												 ,new->destination
												 ,new->day
												 ,new->airplane_type
												 ,new->time
												 ,new->how_much
												 ,new->leave_seat_window
												 ,new->leave_seat_unwindow);
												 
	FILE *fp = fopen(filename,"w");
	if(fp == NULL)
		printf("fopen error!\n");
	
	fwrite(filedata,strlen(filedata),1,fp);
	
	fclose(fp);
	
	
	
}

void strtok_data(struct usr_list *r_head,char *buf)//切割数据写入注册链
{
	struct usr_list *new = malloc(sizeof(struct usr_list));
	if(new == NULL)
		printf("malloc new error!\n");
	
	char seqs[] = ",";

	char* tmp = strtok(buf,seqs);
	strcpy(new->name,tmp);

	tmp = strtok(NULL,seqs);
	strcpy(new->passwd,tmp);
	
	tmp = strtok(NULL,seqs);
	strcpy(new->tel,tmp);
	
	tmp = strtok(NULL,seqs);
	strcpy(new->id_card_number,tmp);
	
	tmp = strtok(NULL,seqs);
	strcpy(new->question,tmp);
	
	tmp = strtok(NULL,seqs);
	strcpy(new->answer,tmp);
	
	tmp = strtok(NULL,seqs);
	strcpy(new->leave_say,tmp);
	
	tmp = strtok(NULL,seqs);
	new->usr_money = atoi(tmp);
	
	tmp = strtok(NULL,seqs);
	new->age = atoi(tmp);
	
	tmp = strtok(NULL,seqs);
	new->vip_grade = atoi(tmp);
	
	tmp = strtok(NULL,seqs);
	new->identity = atoi(tmp);
	
	tmp = strtok(NULL,seqs);
	new->usr_integral = atoi(tmp);
	
	tmp = strtok(NULL,seqs);
	new->verified = atoi(tmp);
										
	tmp = strtok(NULL,seqs);
	new->grand_total_money = atoi(tmp);
	
	struct usr_list *p = NULL;
	
	p = r_head->prev;
	
	new->next = r_head;
	r_head->prev = new;
	p->next = new;
	new->prev = p;
	
	return;
}

void read_file(struct usr_list *r_head,char *filename)//读取老用户文件内容
{
	
	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
		printf("fopen error!!!\n");
	
	char buf[300] = {0};
	int a=fread(buf,20,10,fp);
	
	fclose(fp);
	strtok_data(r_head,buf);
	
	return;
}

void make_old_usr(struct usr_list* r_head)//老用户信息读取
{
	DIR* dp = opendir("./usr_data");
	if(dp == NULL)
		printf("opendir error!!!\n");
	chdir("./usr_data");
	
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
		
		read_file(r_head,ep->d_name);
	}
	
	chdir("..");
	closedir(dp);
	return;
}

void strtok_data_spusr(struct spusr_list* sr_head,char *buf)//切割超级用户数据写入注册链
{
	struct spusr_list* new = malloc(sizeof(struct spusr_list));
	if(new == NULL)
		printf("malloc new error!!!\n");
	
	char seqs[] = ",";
	
	char* tmp = strtok(buf,seqs);
	strcpy(new->spname,tmp);
	
	tmp = strtok(NULL,seqs);
	strcpy(new->sppasswd,tmp);
	
	struct spusr_list* p = NULL;
	p = sr_head->prev;
	
	new->next = sr_head;
	sr_head->prev = new;
	new->prev = p;
	p->next = new;
	
	return;
	
}

void read_file_sp(struct spusr_list* sr_head,char *filename)//读取超级老用户文件内容
{
	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
		printf("fopen error!!!\n");
	
	char buf[200] = {0};
	fread(buf,20,10,fp);
	
	fclose(fp);
	strtok_data_spusr(sr_head,buf);
	
	return;
}

void make_old_spusr(struct spusr_list* sr_head)//老超级用户信息读取
{
	DIR* dp = opendir("./spusr_data");
	if(dp == NULL)
		printf("opendir error!!!\n");
	chdir("./spusr_data");
	
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
		
		read_file_sp(sr_head,ep->d_name);
	}
	
	chdir("..");
	closedir(dp);
	return;
}

void strtok_data_airplane(struct airplane_list* air_head,char *buf)//切割航班信息
{
	struct airplane_list* new = malloc(sizeof(struct airplane_list));
	if(new == NULL)
		printf("malloc new error!!!!\n");
	
	char seqs[] = ",";
	
	char* tmp = strtok(buf,seqs);
	strcpy(new->number,tmp);
	
	tmp = strtok(NULL,seqs);
	strcpy(new->departure,tmp);
	
	tmp = strtok(NULL,seqs);
	strcpy(new->destination,tmp);
	
	tmp = strtok(NULL,seqs);
	strcpy(new->day,tmp);
	
	tmp = strtok(NULL,seqs);
	strcpy(new->airplane_type,tmp);
	
	tmp = strtok(NULL,seqs);
	strcpy(new->time,tmp);
	
	tmp = strtok(NULL,seqs);
	new->how_much = atoi(tmp);

	tmp = strtok(NULL,seqs);
	new->leave_seat_window = atoi(tmp);
	
	tmp = strtok(NULL,seqs);
	new->leave_seat_unwindow = atoi(tmp);
	
	struct airplane_list *p =NULL;
	p = air_head->prev;
	
	p->next = new;
	new->prev = p;
	new->next = air_head;
	air_head->prev = new;
	
	
}

void read_file_airplane(struct airplane_list* air_head,char *filename)//读取航班信息文件内容
{
	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
		printf("fopen error!!!\n");
	
	char buf[300] = {0};
	fread(buf,20,10,fp);
	
	fclose(fp);
	strtok_data_airplane(air_head,buf);
	
	return;
}

void make_old_airplane(struct airplane_list* air_head)//航班信息的读取
{
	DIR* dp = opendir("./airplane_data");
	if(dp == NULL)
		printf("opendir error!!!\n");
	chdir("./airplane_data");
	
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
		
		read_file_airplane(air_head,ep->d_name);
	}
	
	chdir("..");
	closedir(dp);
	return;
}

void strtok_data_info(struct info_list* info_head,char *buf)//切割实名信息
{
	struct info_list *new = malloc(sizeof(struct info_list));
	if(new == NULL)
		printf("make new error!!!\n");
	
	char seqs[] = ",";
	
	char* tmp = strtok(buf,seqs);
	strcpy(new->name,tmp);
	
	tmp = strtok(NULL,seqs);
	strcpy(new->id_card_number,tmp);
	
	tmp = strtok(NULL,seqs);
	new->identity = atoi(tmp);
	
	struct info_list *p = NULL;
	p = info_head->prev;
	
	p->next = new;
	new->prev = p;
	new->next = info_head;
	info_head->prev = new;
	
}

void read_file_info(struct info_list* info_head,char *filename)//打开文件读取实名信息
{
	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
		printf("fopen error!!!\n");
	
	char buf[300] = {0};
	fread(buf,20,10,fp);
	
	fclose(fp);
	strtok_data_info(info_head,buf);
	
	return;
}

void make_info(struct info_list* info_head)//实名认证的信息读取
{
	DIR* dp = opendir("./info_data");
	if(dp == NULL)
		printf("opendir error!!!\n");
	chdir("./info_data");
	
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
		
		read_file_info(info_head,ep->d_name);
	}
	
	chdir("..");
	closedir(dp);
	return;
}