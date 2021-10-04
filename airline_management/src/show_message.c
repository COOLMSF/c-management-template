#include "text.h"



void show_all_airplane(struct airplane_list* air_head)//展示所有航班信息
{
	struct airplane_list* p = NULL;
	for(p = air_head->next;p != air_head;p = p->next)
	{
		printf("plane number:%s\n",p->number);
		printf("departure:%s\n",p->departure);
		printf("destination:%s\n",p->destination);
		printf("day:%s\n",p->day);
		printf("time:%s\n",p->time);
		printf("airplane_type:%s\n",p->airplane_type);
		printf("how_much:%d\n",p->how_much);
		printf("window seat:%d\n",p->leave_seat_window);
		printf("unwindow seat:%d\n",p->leave_seat_unwindow);
		printf("================================\n");
	}
}
