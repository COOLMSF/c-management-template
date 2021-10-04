#include "text.h"



void effective_insurance(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list *r_head)//ÉúÐ§±£ÏÕ
{
	char number[10];
	struct usr_list* p = NULL;
	char cmd[100] = {0};
	char cmd1[50] = {0};
	char cmd3[100] = {0};
	char seqs[] = ",";
	char* tmp = NULL;
	char buf[300] = {0};
	int ret_insurance;
	printf("pls input airplane number:");
	scanf("%s",number);
	
	sprintf(cmd1,"%s.txt",number);
	
	for(p=r_head->next;p!=r_head;p=p->next)
	{
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
				ret_insurance = atoi(tmp);
				tmp = strtok(NULL,seqs);
				if(ret_insurance==1)
				{
					p->usr_money += 500;
					save_usr(p);
				}
				
				/*
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
				*/
			}
			
		}
		chdir("../..");
		closedir(dp);
	}
	
	
	
	
	
}
