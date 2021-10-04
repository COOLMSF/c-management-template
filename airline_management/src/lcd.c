#include "text.h"


lcd_tp *create_lcd()//映射屏幕
{
	lcd_tp *lcds = calloc(1,sizeof(lcd_tp));
	lcds->fd = open("/dev/fb0",O_RDWR);
	struct fb_var_screeninfo info;
	int re = ioctl(lcds->fd,FBIOGET_VSCREENINFO,&info);
	lcds->width = info.xres;
	lcds->height = info.yres;
	lcds->pix = info.bits_per_pixel/8;
	lcds->mptr = (uint_32 *)mmap(NULL,lcds->width*lcds->height*lcds->pix,PROT_READ|PROT_WRITE,MAP_SHARED,lcds->fd, 0);
	return lcds;
}

bool destroy_lcd(lcd_tp *lcd)//删除LCD
{
	if (lcd == NULL)
	{
		printf("传入为空：未操作！\n");
		return false;
	}
	if (lcd->mptr != NULL)
	{
		munmap(lcd->mptr,lcd->width*lcd->height*lcd->pix);
		lcd->mptr == NULL;
	}
	free(lcd);
	return true;
}


void show_bmp(int x,int y,bmp_tp *bmp,lcd_tp *lcd,bool flag)//展示bmp
{
	int i = 0,j = 0;
	int bh = 0,bw = 0;
	uint_8 *p = bmp->data;
	uint_32 *ptr = NULL;
	((x+bmp->w)<lcd->width)?(bw = bmp->w):(bw = lcd->width-x);
	((y+bmp->h)<lcd->height)?(bh = bmp->h):(bh = lcd->height-y);
	if(flag){
		ptr = lcd->mptr+x+y*lcd->width;
	}else{
		ptr = lcd->mptr+x+y*lcd->width+lcd->width*(bh-1);
	}
	//---------------------------------------------------------
	for (i = 0; i < bh; ++i)
	{
		for (j = 0; j < bw; ++j)
		{
			memcpy(ptr+j,p+i*bmp->w*bmp->pix+j*bmp->pix,bmp->pix);			
		}
		flag?(ptr+=lcd->width):(ptr-=lcd->width);
	}
}


coor_tp get_xy(int fd)//获取坐标
{
	//设置触摸屏设备为不阻塞
	/*long flag = fcntl(fd,F_GETFL);
	flag |= O_NONBLOCK;
	int ret = fcntl(fd,F_SETFL,flag);*/

	struct input_event evt;
	coor_tp coor;
	coor.x = -1;coor.y =-1;
	//int i = 0;
	while(1)
	{
		int ret = read(fd, &evt, sizeof(evt));
		//i++;
		if(evt.type == EV_ABS)
		{
			//printf("触摸屏:");
			if(evt.code  == ABS_X)
			{
				coor.x = evt.value;
			}
			if(evt.code  == ABS_Y)
			{
				coor.y = evt.value;
			}
		}
		if((coor.x>=0)&&(coor.y>=0))return coor;//ret小于零是为了确保读取数据为最新
		//if ((coor.x==coor.y)||(i>100)){coor.x=0;coor.y=0;return coor;}//处理循环堵塞
	}
}

bool destroy_bmp(bmp_tp *bmp)//删除bmp
{
	if (bmp == NULL)
	{
	printf("传入为空：未操作！\n");
	return false;
	}
	if (bmp->data != NULL)
	{
	free(bmp->data);
	bmp->data = NULL;
	}
	free(bmp);
	return true;
}


bool Rectangle_button(int x,int y,int w,int h,coor_tp coor)//按键选择
{
	int xr = x*1024/800.0+0.5,yr = y*1024/800.0+0.5;
	int wr = w*1024/800.0+0.5,hr = h*1024/800.0+0.5;
	if ((coor.x>xr)+(coor.x<xr+wr)+(coor.y>yr)+(coor.y<yr+hr)==4)return true;
	return false;
}


bmp_tp *create_bmp(const uint_8 *path)//打开bmp
{
	bmp_tp *bmps = (bmp_tp *)malloc(sizeof(bmp_tp));
	bmph_tp head;
	FILE *file = fopen(path,"r");
	size_t re;
	re = fread(&head,1,sizeof(head),file);
	bmps->w =  head.biWidth;
	bmps->h =  head.biHeight;
	bmps->pix =  head.biBitCount/8;
	uint_32 rgbLen = head.bfSize-54;
	bmps->data = calloc(1,rgbLen);
	uint_32 rowsize = head.biSizeImage/bmps->h;
	uint_32 wide = rowsize-(bmps->w%4);
	int i = 0;
	for (int i = 0; i < bmps->h; ++i)
	{
		re = fread(bmps->data+i*wide,1,rowsize,file);
	}
	fclose(file);
	return bmps;
}

int show_color_mmap(int color)//刷颜色
{
	//1. 打开lcd液晶设备。
	int lcd = open("/dev/fb0",O_RDWR);
	if(lcd < 0)
		printf("open lcd error!\n");
	
	//2. 内存映射。
	int *p = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd,0);
	if(p == NULL)
		printf("mmap error!\n");
	
	//3. 刷颜色
	int i;
	for(i=0;i<800*480;i++)
	{
		memcpy(p+i,&color,4);
	}
	
	//4. 撤销映射。
	munmap(p,800*480*4);
	
	//5. 关闭文件。
	close(lcd);
	
	return 0;
}



void insert_node(struct landscape_list* landscape_head,char *picname)//为风景图申请空间附上bmp名
{
	struct landscape_list* new = malloc(sizeof(struct landscape_list));
	if(new == NULL)
		printf("malloc new error!\n");
	
	strcpy(new->picname,picname);
	new->next = landscape_head;
	
	struct landscape_list *p = landscape_head->prev;
	
	p->next = new;
	new->prev = p;
	landscape_head->prev = new;
	
	return;
}

void buy_air_landscape(struct usr_list* p,struct landscape_list* q,struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor)//看风景点击进入买票
{
	char seqs[] = ".";
	char* tmp = strtok(q->picname,seqs);
	struct airplane_list* k = NULL;
	for(k=air_head->next;k != air_head;k=k->next)
	{
		if(strcmp(k->destination,tmp)==0)
		{
			printf("plane number:%s\n",k->number);
			printf("departure:%s\n",k->departure);
			printf("destination:%s\n",k->destination);
			printf("day:%s\n",k->day);
			printf("time:%s\n",k->time);
			printf("airplane_type:%s\n",k->airplane_type);
			printf("how_much:%d\n",k->how_much);
			printf("window seat:%d\n",k->leave_seat_window);
			printf("unwindow seat:%d\n",k->leave_seat_unwindow);
			printf("================================\n");
		}
	}
	
	buy_airplane_my(air_head,p,ld,fd,coor);
}

void show_landscape(struct usr_list* p,struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor)//展示风景
{
	struct landscape_list* landscape_head = NULL;
	landscape_head = make_landscape_head();
	
	DIR *dp = opendir("/project1/bmp_data");
	if(dp == NULL)
		printf("opendir error!\n");
	
	chdir("/project1/bmp_data");
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
		
		insert_node(landscape_head,ep->d_name);
	}
	struct landscape_list* q = landscape_head->next;
	while(1)
	{
		bmp_tp *bp = create_bmp(q->picname);//打开bmp
		show_bmp(0,0,bp,ld,0);//展示bmp
		destroy_bmp(bp);//删除bmp
		coor.x = 0;coor.y = 0;
		coor = get_xy(fd);
		if(Rectangle_button(0,0,266,480,coor))
		{
			if(q->prev == landscape_head)
			{
				q = landscape_head->prev;
			}
			else{
				q = q->prev;
			}
			bmp_tp *bp1 = create_bmp(q->picname);//打开bmp
			show_bmp(0,0,bp1,ld,0);//展示bmp
			destroy_bmp(bp1);//删除bmp
		}
		
		if(Rectangle_button(266,0,266,480,coor))
		{
			buy_air_landscape(p,q,air_head,ld,fd,coor);
			return;
		}
		
		if(Rectangle_button(533,0,267,480,coor))
		{
			if(q->next == landscape_head)
			{
				q = landscape_head->next;
			}
			else{
				q = q->next;
			}
			bmp_tp *bp2 = create_bmp(q->picname);//打开bmp
			show_bmp(0,0,bp2,ld,0);//展示bmp
			destroy_bmp(bp2);//删除bmp
		}
	}
	
	chdir("..");
	
	
}

