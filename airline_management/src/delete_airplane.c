#include "text.h"


void strtok_data_deleta_air(struct airplane_list* air_head,char *buf)//切割展示已删除
{
	
	char seqs[] = ",";
	
	char* tmp = strtok(buf,seqs);
	printf("number:%s\n",tmp);
	
	tmp = strtok(NULL,seqs);
	printf("departure:%s\n",tmp);
	
	tmp = strtok(NULL,seqs);
	printf("destination:%s\n",tmp);
	
	tmp = strtok(NULL,seqs);
	printf("day:%s\n",tmp);
	
	tmp = strtok(NULL,seqs);
	printf("airplane_type:%s\n",tmp);
	
	tmp = strtok(NULL,seqs);
	printf("time:%s\n",tmp);
	
	tmp = strtok(NULL,seqs);
	printf("how_much:%d\n",atoi(tmp));

	tmp = strtok(NULL,seqs);
	printf("leave_seat_window:%d\n",atoi(tmp));
	
	tmp = strtok(NULL,seqs);
	printf("leave_seat_unwindow:%d\n",atoi(tmp));
	printf("=============================\n");
}

void read_file_delete_air(struct airplane_list* air_head,char *filename)//读取删除的信息
{
	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
		printf("fopen error!!!\n");
	
	char buf[300] = {0};
	fread(buf,20,10,fp);
	
	fclose(fp);
	strtok_data_deleta_air(air_head,buf);
	
	return;
}

void show_deleta(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor)//删除信息读取
{
	bmp_tp *bp = create_bmp("./usr_showing.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
	sleep(2);
	
	DIR* dp = opendir("./delete_air_data");
	if(dp == NULL)
		printf("opendir error!!!\n");
	chdir("./delete_air_data");
	
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
		
		read_file_delete_air(air_head,ep->d_name);
	}
	
	chdir("..");
	closedir(dp);
	return;
}


void deleta_air(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list *r_head)//删除航班信息
{
	char filename[50] = {0};
	char filedata[400] = {0};
	char cmd[100] = {0};
	char cmd1[50] = {0};
	char cmd2[100] = {0};
	char cmd3[100] = {0};
	char cmd4[100] = {0};
	int money;
	char buf[300] = {0};
	char seqs[] = ",";
	char* tmp = NULL;
	bmp_tp *bp = create_bmp("/project1/delete_airing.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
	char number[10];
	scanf("%s",number);
	struct dirent *ep = NULL;
	struct usr_list* p = NULL;
	sprintf(cmd1,"%s.txt",number);
	for(p=r_head->next;p!=r_head;p=p->next)
	{
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
				sprintf(cmd3,"/project1/usr_airplane_data/%s/%s",p->name,ep->d_name);
				printf("%s\n",cmd3);
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
				p->usr_money += money;
				p->usr_integral += 1000;
				save_usr(p);
				sprintf(cmd2,"rm /project1/usr_airplane_data/%s/%s",p->name,ep->d_name);
				system(cmd2);
				
			}
			
		}
		chdir("../..");
		closedir(dp);
	}
	
	struct airplane_list* q = NULL;
	for(q=air_head->next;q != air_head;q=q->next)
	{
		if(strcmp(q->number,number)==0)
		{
			bmp_tp *bp1 = create_bmp("/project1/deleteing.bmp");//打开bmp
			show_bmp(0,0,bp1,ld,0);//展示bmp
			destroy_bmp(bp1);//删除bmp
			sleep(2);
			
			sprintf(filename,"./delete_air_data/%s.txt",q->number);
			sprintf(cmd4,"rm /project1/airplane_data/%s.txt",q->number);
			
			sprintf(filedata,"%s,%s,%s,%s,%s,%s,%d,%d,%d",q->number
														 ,q->departure
														 ,q->destination
														 ,q->day
														 ,q->airplane_type
														 ,q->time
														 ,q->how_much
														 ,q->leave_seat_window
														 ,q->leave_seat_unwindow);
														 
			FILE *fp = fopen(filename,"w");
			if(fp == NULL)
				printf("fopen error!\n");
			
			fwrite(filedata,strlen(filedata),1,fp);
			
			fclose(fp);
			
			q->prev->next = q->next;
			q->next->prev = q->prev;
			free(q);
			
			system(cmd4);
			return;
			
		}
	}
	
}

void strtok_data_recover_air(struct airplane_list* air_head,char *buf)//切割恢复信息
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
	
	char filename[50] = {0};
	sprintf(filename,"/project1/airplane_data/%s.txt",new->number);
	
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

void read_file_recover_air(struct airplane_list* air_head,char *filename,lcd_tp *ld,int fd,coor_tp coor)//读取恢复的信息
{
	

	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
		printf("fopen error!!!\n");
	
	char buf[300] = {0};
	char buf1[30] = {0};

	fread(buf,20,10,fp);
	fclose(fp);
	strtok_data_recover_air(air_head,buf);
	sprintf(buf1,"rm %s",filename);
	system(buf1);
	
	return;
}

void recover_deleta(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor)//恢复信息读取
{
	bmp_tp *bp = create_bmp("./usr_showing.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
	sleep(2);
	printf("pls input airplane number:");
	char number[10]={0};
	char buf[20]={0};
	scanf("%s",number);
	sprintf(buf,"%s.txt",number);
	DIR* dp = opendir("./delete_air_data");
	if(dp == NULL)
		printf("opendir error!!!\n");
	chdir("./delete_air_data");
	
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
		if(strcmp(buf,ep->d_name)==0)
		{
			bmp_tp *bp = create_bmp("../recovering.bmp");//打开bmp
			show_bmp(0,0,bp,ld,0);//展示bmp
			destroy_bmp(bp);//删除bmp
			sleep(2);
			read_file_recover_air(air_head,ep->d_name,ld,fd,coor);
		}
		
	}
	
	chdir("..");
	closedir(dp);
	return;
	
}

void delete_airplane_meun(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list *r_head)//删除航班信息，删除、查看已删除、恢复
{
	while(1)
	{
		
		bmp_tp *bp = create_bmp("./delete_airplane_meun.bmp");//打开bmp
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
			deleta_air(air_head,ld,fd,coor,r_head);
		}
		
		if(Rectangle_button(610,142,150,75,coor))
		{
			show_deleta(air_head,ld,fd,coor);
		}
		
		if(Rectangle_button(610,262,150,75,coor))
		{
			recover_deleta(air_head,ld,fd,coor);
		}
		
		if(Rectangle_button(610,382,150,75,coor))
		{
			
			return;
		}
	}
}
