#include "text.h"


void usr_login(struct usr_list *r_head,struct usr_list *l_head,lcd_tp *ld,int fd,coor_tp coor,struct airplane_list* air_head,struct info_list* info_head)//��ͨ�û���½��
{
	bmp_tp *bp = create_bmp("./logining.bmp");//��bmp
	show_bmp(0,0,bp,ld,0);//չʾbmp
	destroy_bmp(bp);//ɾ��bmp
	char name[10] = {0};
	char passwd[10] = {0};
	printf("pls input your name:");
	scanf("%s",name);
	
	struct usr_list* p = NULL;
	for(p=r_head->next;p!=r_head;p=p->next)
	{
		if(strcmp(p->name,name) == 0)
		{
			printf("%s exist pls input passwd:",name);
			scanf("%s",passwd);
			if(strcmp(p->passwd,passwd) == 0)
			{
				bmp_tp *bp1 = create_bmp("./login_su.bmp");//��bmp
				show_bmp(0,0,bp1,ld,0);//չʾbmp
				destroy_bmp(bp1);//ɾ��bmp
				sleep(2);
				logining_sc(r_head,l_head,ld,fd,coor,p,air_head,info_head);
				return;
			}
			else
			{
				printf("passwd error!!!\n");
			}
		}
	}
	bmp_tp *bp2 = create_bmp("./login_er.bmp");//��bmp
	show_bmp(0,0,bp2,ld,0);//չʾbmp
	destroy_bmp(bp2);//ɾ��bmp
	sleep(2);
	
	
}

void spusr_login(struct spusr_list *sr_head,struct spusr_list* sl_head,lcd_tp *ld,int fd,coor_tp coor,struct airplane_list* air_head,struct usr_list *r_head)//�����û���½��
{
	bmp_tp *bp = create_bmp("./logining.bmp");//��bmp
	show_bmp(0,0,bp,ld,0);//չʾbmp
	destroy_bmp(bp);//ɾ��bmp
	char name[10] = {0};
	char passwd[10] = {0};
	printf("pls input your name:");
	scanf("%s",name);
	
	struct spusr_list* p = NULL;
	for(p = sr_head->next;p != sr_head;p = p->next)
	{
		if(strcmp(p->spname,name) == 0)
		{
			printf("%s exist pls input passwd:",name);
			scanf("%s",passwd);
			if(strcmp(p->sppasswd,passwd) == 0)
			{
				bmp_tp *bp1 = create_bmp("./login_su.bmp");//��bmp
				show_bmp(0,0,bp1,ld,0);//չʾbmp
				destroy_bmp(bp1);//ɾ��bmp
				sleep(2);
				logining_spsc(sl_head,ld,fd,coor,p,air_head,r_head);
				return;
			}
			else
			{
				printf("passwd error!!!\n");
			}
		}
	}
	bmp_tp *bp2 = create_bmp("./login_er.bmp");//��bmp
	show_bmp(0,0,bp2,ld,0);//չʾbmp
	destroy_bmp(bp2);//ɾ��bmp
	sleep(2);
	
}

void find_passwd(struct usr_list* r_head,lcd_tp *ld,int fd)//�һ�����
{
	char usr[20];
	char as[20];
	char new_passwd[10];
	char new_passwd1[10];
	bmp_tp *bpws = create_bmp("/project1/changing.bmp");//��bmp
	show_bmp(0,0,bpws,ld,0);//չʾbmp
	destroy_bmp(bpws);//ɾ��bmp
	sleep(2);
	printf("pls inout usr name :");
	scanf("%s",usr);
	struct usr_list *p = NULL;
	for(p = r_head->next;p != r_head;p = p->next)
	{
		if(strcmp(p->name,usr)==0)
		{
			break;
		}
	}
	if(p == r_head)
	{
		
		return;
	}
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
			bmp_tp *bp1 = create_bmp("./change_sc.bmp");//��bmp
			show_bmp(0,0,bp1,ld,0);//չʾbmp
			destroy_bmp(bp1);//ɾ��bmp
			sleep(2);
			
			strcpy(p->passwd,new_passwd);
		}
		else{
			printf("new passwd not same!!!\n");
			bmp_tp *bp2 = create_bmp("./change_error.bmp");//��bmp
			show_bmp(0,0,bp2,ld,0);//չʾbmp
			destroy_bmp(bp2);//ɾ��bmp
			sleep(2);
		}
	}
}
