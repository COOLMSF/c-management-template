#include "text.h"

struct usr_list* make_head()//������ͨ�û���½��ͷ��ע���ͷ
{
	struct usr_list *head = malloc(sizeof(struct usr_list));
	if(head == NULL)
		printf("malloc head error!!!!!\n");
	
	
	head->next = head;
	head->prev = head;
	
	return head;
}

struct spusr_list* make_sphead()//���������û���½��ͷ��ע���ͷ
{
	struct spusr_list *head = malloc(sizeof(struct spusr_list));
	if(head == NULL)
		printf("malloc head error!!!!!\n");
	
	
	head->next = head;
	head->prev = head;
	
	return head;
}

struct airplane_list* make_air_head()//���������ͷ
{
	struct airplane_list* air_head = malloc(sizeof(struct airplane_list));
	if(air_head == NULL)
		printf("make air_head error!!!\n");
	
	air_head->next = air_head;
	air_head->prev = air_head;
	
	return air_head;
}

struct info_list* make_info_head()//ʵ����֤��ͷ
{
	struct info_list* info_head = malloc(sizeof(struct info_list));
	if(info_head == NULL)
		printf("make info head error!!!!!\n");
	
	info_head->next = info_head;
	info_head->prev = info_head;
	
	return info_head;
	
}

struct landscape_list* make_landscape_head()//�����羰ͷ
{
	struct landscape_list* landscape_head = malloc(sizeof(struct landscape_list));
	if(landscape_head == NULL)
		printf("make landscape head error!!!\n");
	
	landscape_head->next = landscape_head;
	landscape_head->prev = landscape_head;
	
	return landscape_head;
}
