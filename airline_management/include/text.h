#ifndef _TEXT_H_
#define _TEXT_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <math.h>
#include <linux/input.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <linux/input.h>

#include <dirent.h>
 

#define PEOPLE 0
#define SOLDIER 1
#define HANDICAPPED 2
#define DOCTOR 3
#define NURSE 4




struct usr_list{
	char name[20];
	char passwd[10];
	char tel[12];
	char question[50];
	char answer[50];
	char id_card_number[20];
	char leave_say[50];
	
	int age;
	int vip_grade;       //vip等级
	int identity;       //身份
	int usr_integral;   //用户积分
	int usr_money;
	int grand_total_money;
	int verified;       //实名认证
	
	struct usr_list *next;
	struct usr_list *prev;
	
};

struct airplane_list{
	char number[10];
	char departure[10];
	char destination[10];
	char day[20];
	char time[10];
	
	char airplane_type[3];
	int how_much;
	int leave_seat_window;
	int leave_seat_unwindow;
	
	
	struct airplane_list *next;
	struct airplane_list *prev;
};

struct spusr_list{
	char spname[20];
	char sppasswd[10];
	
	struct spusr_list *next;
	struct spusr_list *prev;
};

struct info_list{
	char name[20];
	char id_card_number[20];
	
	int identity; 
	
	struct info_list* next;
	struct info_list* prev;
	
};

struct landscape_list{
	char picname[50];
	struct landscape_list *next;
	struct landscape_list *prev;
};





typedef unsigned char uint_8;
typedef unsigned short uint_16;
typedef unsigned int  uint_32;

#pragma pack(1) //设置1字节对齐
typedef struct BitFileHeader{
	uint_16 bfType;//位图类别，根据不同的操作系统而不同，在Windows中，此字段的值总为‘BM’BMP
	uint_32 bfSize;//BMP图像文件的大小
	uint_16 bfReserved1; 
	uint_16 bfReserved2;
	uint_32 bfOffBits;//BMP图像数据的地址
	uint_32 biSize; //表示本结构的大小
	uint_32 biWidth; //位图的宽度
	uint_32 biHeight; //位图的高度
	uint_16 biPlanes; 
	uint_16 biBitCount;//BMP图像的色深，即一个像素用多少位表示，常见有1、4、8、16、24和32，分别对应单色、16色、256色、16位高彩色、24位真彩色和32位增强型真彩色
	uint_32 biCompression; //压缩方式，0表示不压缩，1表示RLE8压缩，2表示RLE4压缩，3表示每个像素值由指定的掩码决定
	uint_32 biSizeImage; //BMP图像数据大小，必须是4的倍数，图像数据大小不是4的倍数时用0填充补足
	uint_32 biXPelsPerMeter; //水平分辨率，单位像素/m
	uint_32 biYPelsPerMeter; //垂直分辨率，单位像素/m
	uint_32 biClrUsed; //BMP图像使用的颜色，0表示使用全部颜色，对于256色位图来说，此值为100h=256
	uint_32 biClrImportant;//重要的颜色数，此值为0时所有颜色都重要，对于使用调色板的BMP图像来说，当显卡不能够显示所有颜色时，此值将辅助驱动程序显示颜色
} bmph_tp;
#pragma pack(0)//设置为默认字节对齐

typedef struct Bmp{
			int w;//
			int h;//
			int pix;//
			uint_8 *data;//
}bmp_tp;

typedef struct coordinates{
	int x;
	int y;
}coor_tp;

typedef struct Lcd{
	int fd;//文件描述符
	int width;
	int height;
	int pix;
	uint_32 *mptr;
}lcd_tp;



lcd_tp *create_lcd();//映射屏幕
bool destroy_lcd(lcd_tp *lcd);//删除LCD
void show_bmp(int x,int y,bmp_tp *bmp,lcd_tp *lcd,bool flag);//展示bmp
coor_tp get_xy(int fd);//获取坐标
bool destroy_bmp(bmp_tp *bmp);//删除bmp
bool Rectangle_button(int x,int y,int w,int h,coor_tp coor);//按键选择
bmp_tp *create_bmp(const uint_8 *path);//打开bmp
struct usr_list* make_head();//制作普通用户登陆的头和注册的头
struct spusr_list* make_sphead();//制作超级用户登陆的头和注册的头
struct airplane_list* make_air_head();//制作航班的头
struct info_list* make_info_head();//实名认证的头
struct landscape_list* make_landscape_head();//制作风景头
int show_color_mmap(int color);//刷颜色
void show_all_airplane(struct airplane_list* air_head);//展示所有航班信息
void usr_register(struct usr_list *r_head,lcd_tp *ld,int fd);//普通用户注册
void spusr_register(struct spusr_list *sr_head,lcd_tp *ld,int fd);//超级用户注册
void usr_register_meun(struct usr_list *r_head,struct spusr_list *sr_head,lcd_tp *ld,int fd,coor_tp coor);//用户注册菜单
void show_chioce_airplane_next(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//普通用户查询航班,飞机类型、金钱、有无靠窗
void show_chioce_airplane(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//普通用户筛选查询航班,出发地、目的地、日期
void usr_show_airplane_meun(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//普通用户查询航班主页面
void info_name_my(struct usr_list *r_head,struct info_list* info_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//实名认证本账号
void info_name_other(struct usr_list *r_head,struct info_list* info_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//实名认证其他账号
void change_age(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//修改年龄
void change_idcard(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//修改身份证
void change_usr_data_next(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//修改个人信息，年龄、身份证
void change_passwd(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//用户更改密码
void change_tel(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//用户更改电话
void show_usr_data(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//展示用户信息
void change_usr_data(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//修改个人信息，密码、电话
int buy_discount(struct airplane_list* q,struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//选座位后打折处理
int pay_money(int money,struct airplane_list* q,struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//用户账号扣钱
void buy_airplane_my(struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//本账号买票
void buy_airplane_other(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* r_head);//其他账号买票
void buy_airplane_chioce(struct usr_list* r_head,struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//买票功能,自己买、帮他人买
void info_ture_name(struct usr_list *r_head,struct info_list* info_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//用户实名认证，本用户、其他用户
void usr_recharge(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//充值功能
void show_usr_money(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//查看余额
void usr_integral_shop1(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//商品1
void usr_integral_shop2(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//商品2
void usr_integral_shop3(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//商品3
void save_usr(struct usr_list* p);//保存用户信息
void usr_integral_shop(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//积分商城
void usr_Balance_meun(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//余额菜单，充值、积分商城
void buy_airplane_meun(struct usr_list* r_head,struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//购票菜单，购票、余额
void insert_node(struct landscape_list* landscape_head,char *picname);//为风景图申请空间附上bmp名
void buy_air_landscape(struct usr_list* p,struct landscape_list* q,struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//看风景点击进入买票
void show_landscape(struct usr_list* p,struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//展示风景
void strtok_data_usr_air(char *buf);//查票切割信息
void read_file_usr_air(char *filename);//查票打开文件
void cheak_airplane(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//查票打开目录
void refund_airplane(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//退票
void change_airplane(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p,struct airplane_list* air_head);//改签
void cheak_airplane_meun(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p,struct airplane_list* air_head);//查看已买的票，查看、改签、退票
void logining_sc_next(struct usr_list *r_head,struct usr_list *l_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p,struct airplane_list* air_head);//普通用户进入后，看已经购票的信息、修改个人信息、风景
void logining_sc(struct usr_list *r_head,struct usr_list *l_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p,struct airplane_list* air_head,struct info_list* info_head);//普通用户进入后，查询、购票、实名
void usr_login(struct usr_list *r_head,struct usr_list *l_head,lcd_tp *ld,int fd,coor_tp coor,struct airplane_list* air_head,struct info_list* info_head);//普通用户登陆中
void airplane_input(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//sp录入航班信息
void strtok_data_deleta_air(struct airplane_list* air_head,char *buf);//切割展示已删除
void read_file_delete_air(struct airplane_list* air_head,char *filename);//读取删除的信息
void show_deleta(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//删除信息读取
void deleta_air(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list *r_head);//删除航班信息
void strtok_data_recover_air(struct airplane_list* air_head,char *buf);//切割恢复信息
void read_file_recover_air(struct airplane_list* air_head,char *filename,lcd_tp *ld,int fd,coor_tp coor);//读取恢复的信息
void recover_deleta(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//恢复信息读取
void delete_airplane_meun(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list *r_head);//删除航班信息，删除、查看已删除、恢复
void effective_insurance(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list *r_head);//生效保险
void logining_spsc_next(struct spusr_list* sl_head,lcd_tp *ld,int fd,coor_tp coor,struct spusr_list* p,struct airplane_list* air_head,struct usr_list *r_head);//超级用户进入后，生效保险，删除数据
void logining_spsc(struct spusr_list* sl_head,lcd_tp *ld,int fd,coor_tp coor,struct spusr_list* p,struct airplane_list* air_head,struct usr_list *r_head);//超级用户进入后，查看航班、航班录入
void spusr_login(struct spusr_list *sr_head,struct spusr_list* sl_head,lcd_tp *ld,int fd,coor_tp coor,struct airplane_list* air_head,struct usr_list *r_head);//超级用户登陆中
void find_passwd(struct usr_list* r_head,lcd_tp *ld,int fd);//找回密码
void theck_in(struct usr_list* r_head,lcd_tp *ld,int fd);//取票
void usr_login_menu(struct usr_list *r_head,struct spusr_list *sr_head,struct usr_list *l_head,struct spusr_list *sl_head,lcd_tp *ld,int fd,coor_tp coor,struct airplane_list* air_head,struct info_list* info_head);//用户登陆菜单
void strtok_data(struct usr_list *r_head,char *buf);//切割数据写入注册链
void read_file(struct usr_list *r_head,char *filename);//读取老用户文件内容
void make_old_usr(struct usr_list* r_head);//老用户信息读取
void strtok_data_spusr(struct spusr_list* sr_head,char *buf);//切割超级用户数据写入注册链
void read_file_sp(struct spusr_list* sr_head,char *filename);//读取超级老用户文件内容
void make_old_spusr(struct spusr_list* sr_head);//老超级用户信息读取
void strtok_data_airplane(struct airplane_list* air_head,char *buf);//切割航班信息
void read_file_airplane(struct airplane_list* air_head,char *filename);//读取航班信息文件内容
void make_old_airplane(struct airplane_list* air_head);//航班信息的读取
void strtok_data_info(struct info_list* info_head,char *buf);//切割实名信息
void read_file_info(struct info_list* info_head,char *filename);//打开文件读取实名信息
void make_info(struct info_list* info_head);//实名认证的信息读取







#endif