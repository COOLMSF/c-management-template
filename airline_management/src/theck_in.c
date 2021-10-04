#include "text.h"



void theck_in(struct usr_list* r_head,lcd_tp *ld,int fd)//取票
{
	struct usr_list* p =NULL;
	char cmd[50] = {0};
	char cmd1[50] = {0};
	char cmd2[50] = {0};
	char cmd3[50] = {0};
	char usr_chioce[5] = {0};
	char usr_name[10] = {0};
	char seqs[] = ",";
	char* tmp = NULL;
	char buf[300] = {0};
	char departure[20] = {0};
	char destination[20] = {0};
	char number[20] = {0};
	char day[20] = {0};
	char time[20] = {0};
	
	int ret_insurance;
	int ret_money;
	printf("pls input ticket need usr:");
	scanf("%s",usr_name);
	bmp_tp *bp = create_bmp("/project1/theck_in.bmp");//打开bmp
	show_bmp(0,0,bp,ld,0);//展示bmp
	destroy_bmp(bp);//删除bmp
		
	for(p=r_head->next;p!=r_head;p=p->next)
	{
		if(strcmp(usr_name,p->name)==0)
		{
			break;
		}
	}
	if(p == r_head)
	{
		printf("this usr does not exist\n");
		return;
	}
	sprintf(cmd1,"%s.txt",p->name);
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
		
		sprintf(cmd3,"/project1/usr_airplane_data/%s/%s",p->name,ep->d_name);
		FILE *fp = fopen(cmd3,"r");
		if(fp == NULL)
			printf("fopen error!!!\n");
		
		fread(buf,30,10,fp);
		fclose(fp);
		tmp = strtok(buf,seqs);
		tmp = strtok(NULL,seqs);
		strcpy(departure,tmp);
		tmp = strtok(NULL,seqs);
		strcpy(destination,tmp);
		tmp = strtok(NULL,seqs);
		strcpy(number,tmp);
		tmp = strtok(NULL,seqs);
		strcpy(day,tmp);
		tmp = strtok(NULL,seqs);
		strcpy(time,tmp);
		tmp = strtok(NULL,seqs);
		ret_insurance = atoi(tmp);
		tmp = strtok(NULL,seqs);
		ret_money = atoi(tmp);
		
		printf("did you check in %s?(Y/N):",number);
		scanf("%s",usr_chioce);
		if(strcmp(usr_chioce,"Y")==0)
		{
			printf("=====================================\n");
			printf("   name: %s                          \n",p->name);
			printf("   departure: %s                     \n",departure);
			printf("   destination: %s                   \n",destination);
			printf("   number: %s                        \n",number);
			printf("   day: %s                           \n",day);
			printf("   time: %s                          \n",time);
			printf("   insurance: %d                     \n",ret_insurance);
			printf("   money: %d                         \n",ret_money);
			printf("=====================================\n");
			
			sprintf(cmd2,"rm /project1/usr_airplane_data/%s/%s",p->name,ep->d_name);
			system(cmd2);
			chdir("../..");
			closedir(dp);
			return;
		}
		
			
			
			
		
		
	}
	
	chdir("../..");
	closedir(dp);
	
	
	
	
	
}
