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
	int vip_grade;       //vip�ȼ�
	int identity;       //���
	int usr_integral;   //�û�����
	int usr_money;
	int grand_total_money;
	int verified;       //ʵ����֤
	
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

#pragma pack(1) //����1�ֽڶ���
typedef struct BitFileHeader{
	uint_16 bfType;//λͼ��𣬸��ݲ�ͬ�Ĳ���ϵͳ����ͬ����Windows�У����ֶε�ֵ��Ϊ��BM��BMP
	uint_32 bfSize;//BMPͼ���ļ��Ĵ�С
	uint_16 bfReserved1; 
	uint_16 bfReserved2;
	uint_32 bfOffBits;//BMPͼ�����ݵĵ�ַ
	uint_32 biSize; //��ʾ���ṹ�Ĵ�С
	uint_32 biWidth; //λͼ�Ŀ��
	uint_32 biHeight; //λͼ�ĸ߶�
	uint_16 biPlanes; 
	uint_16 biBitCount;//BMPͼ���ɫ���һ�������ö���λ��ʾ��������1��4��8��16��24��32���ֱ��Ӧ��ɫ��16ɫ��256ɫ��16λ�߲�ɫ��24λ���ɫ��32λ��ǿ�����ɫ
	uint_32 biCompression; //ѹ����ʽ��0��ʾ��ѹ����1��ʾRLE8ѹ����2��ʾRLE4ѹ����3��ʾÿ������ֵ��ָ�����������
	uint_32 biSizeImage; //BMPͼ�����ݴ�С��������4�ı�����ͼ�����ݴ�С����4�ı���ʱ��0��䲹��
	uint_32 biXPelsPerMeter; //ˮƽ�ֱ��ʣ���λ����/m
	uint_32 biYPelsPerMeter; //��ֱ�ֱ��ʣ���λ����/m
	uint_32 biClrUsed; //BMPͼ��ʹ�õ���ɫ��0��ʾʹ��ȫ����ɫ������256ɫλͼ��˵����ֵΪ100h=256
	uint_32 biClrImportant;//��Ҫ����ɫ������ֵΪ0ʱ������ɫ����Ҫ������ʹ�õ�ɫ���BMPͼ����˵�����Կ����ܹ���ʾ������ɫʱ����ֵ����������������ʾ��ɫ
} bmph_tp;
#pragma pack(0)//����ΪĬ���ֽڶ���

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
	int fd;//�ļ�������
	int width;
	int height;
	int pix;
	uint_32 *mptr;
}lcd_tp;



lcd_tp *create_lcd();//ӳ����Ļ
bool destroy_lcd(lcd_tp *lcd);//ɾ��LCD
void show_bmp(int x,int y,bmp_tp *bmp,lcd_tp *lcd,bool flag);//չʾbmp
coor_tp get_xy(int fd);//��ȡ����
bool destroy_bmp(bmp_tp *bmp);//ɾ��bmp
bool Rectangle_button(int x,int y,int w,int h,coor_tp coor);//����ѡ��
bmp_tp *create_bmp(const uint_8 *path);//��bmp
struct usr_list* make_head();//������ͨ�û���½��ͷ��ע���ͷ
struct spusr_list* make_sphead();//���������û���½��ͷ��ע���ͷ
struct airplane_list* make_air_head();//���������ͷ
struct info_list* make_info_head();//ʵ����֤��ͷ
struct landscape_list* make_landscape_head();//�����羰ͷ
int show_color_mmap(int color);//ˢ��ɫ
void show_all_airplane(struct airplane_list* air_head);//չʾ���к�����Ϣ
void usr_register(struct usr_list *r_head,lcd_tp *ld,int fd);//��ͨ�û�ע��
void spusr_register(struct spusr_list *sr_head,lcd_tp *ld,int fd);//�����û�ע��
void usr_register_meun(struct usr_list *r_head,struct spusr_list *sr_head,lcd_tp *ld,int fd,coor_tp coor);//�û�ע��˵�
void show_chioce_airplane_next(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//��ͨ�û���ѯ����,�ɻ����͡���Ǯ�����޿���
void show_chioce_airplane(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//��ͨ�û�ɸѡ��ѯ����,�����ء�Ŀ�ĵء�����
void usr_show_airplane_meun(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//��ͨ�û���ѯ������ҳ��
void info_name_my(struct usr_list *r_head,struct info_list* info_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//ʵ����֤���˺�
void info_name_other(struct usr_list *r_head,struct info_list* info_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//ʵ����֤�����˺�
void change_age(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//�޸�����
void change_idcard(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//�޸����֤
void change_usr_data_next(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//�޸ĸ�����Ϣ�����䡢���֤
void change_passwd(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//�û���������
void change_tel(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//�û����ĵ绰
void show_usr_data(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//չʾ�û���Ϣ
void change_usr_data(struct usr_list *r_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//�޸ĸ�����Ϣ�����롢�绰
int buy_discount(struct airplane_list* q,struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//ѡ��λ����۴���
int pay_money(int money,struct airplane_list* q,struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//�û��˺ſ�Ǯ
void buy_airplane_my(struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//���˺���Ʊ
void buy_airplane_other(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* r_head);//�����˺���Ʊ
void buy_airplane_chioce(struct usr_list* r_head,struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//��Ʊ����,�Լ��򡢰�������
void info_ture_name(struct usr_list *r_head,struct info_list* info_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//�û�ʵ����֤�����û��������û�
void usr_recharge(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//��ֵ����
void show_usr_money(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//�鿴���
void usr_integral_shop1(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//��Ʒ1
void usr_integral_shop2(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//��Ʒ2
void usr_integral_shop3(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//��Ʒ3
void save_usr(struct usr_list* p);//�����û���Ϣ
void usr_integral_shop(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//�����̳�
void usr_Balance_meun(struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//���˵�����ֵ�������̳�
void buy_airplane_meun(struct usr_list* r_head,struct airplane_list* air_head,struct usr_list* p,lcd_tp *ld,int fd,coor_tp coor);//��Ʊ�˵�����Ʊ�����
void insert_node(struct landscape_list* landscape_head,char *picname);//Ϊ�羰ͼ����ռ丽��bmp��
void buy_air_landscape(struct usr_list* p,struct landscape_list* q,struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//���羰���������Ʊ
void show_landscape(struct usr_list* p,struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//չʾ�羰
void strtok_data_usr_air(char *buf);//��Ʊ�и���Ϣ
void read_file_usr_air(char *filename);//��Ʊ���ļ�
void cheak_airplane(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//��Ʊ��Ŀ¼
void refund_airplane(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p);//��Ʊ
void change_airplane(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p,struct airplane_list* air_head);//��ǩ
void cheak_airplane_meun(lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p,struct airplane_list* air_head);//�鿴�����Ʊ���鿴����ǩ����Ʊ
void logining_sc_next(struct usr_list *r_head,struct usr_list *l_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p,struct airplane_list* air_head);//��ͨ�û�����󣬿��Ѿ���Ʊ����Ϣ���޸ĸ�����Ϣ���羰
void logining_sc(struct usr_list *r_head,struct usr_list *l_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list* p,struct airplane_list* air_head,struct info_list* info_head);//��ͨ�û�����󣬲�ѯ����Ʊ��ʵ��
void usr_login(struct usr_list *r_head,struct usr_list *l_head,lcd_tp *ld,int fd,coor_tp coor,struct airplane_list* air_head,struct info_list* info_head);//��ͨ�û���½��
void airplane_input(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//sp¼�뺽����Ϣ
void strtok_data_deleta_air(struct airplane_list* air_head,char *buf);//�и�չʾ��ɾ��
void read_file_delete_air(struct airplane_list* air_head,char *filename);//��ȡɾ������Ϣ
void show_deleta(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//ɾ����Ϣ��ȡ
void deleta_air(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list *r_head);//ɾ��������Ϣ
void strtok_data_recover_air(struct airplane_list* air_head,char *buf);//�и�ָ���Ϣ
void read_file_recover_air(struct airplane_list* air_head,char *filename,lcd_tp *ld,int fd,coor_tp coor);//��ȡ�ָ�����Ϣ
void recover_deleta(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor);//�ָ���Ϣ��ȡ
void delete_airplane_meun(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list *r_head);//ɾ��������Ϣ��ɾ�����鿴��ɾ�����ָ�
void effective_insurance(struct airplane_list* air_head,lcd_tp *ld,int fd,coor_tp coor,struct usr_list *r_head);//��Ч����
void logining_spsc_next(struct spusr_list* sl_head,lcd_tp *ld,int fd,coor_tp coor,struct spusr_list* p,struct airplane_list* air_head,struct usr_list *r_head);//�����û��������Ч���գ�ɾ������
void logining_spsc(struct spusr_list* sl_head,lcd_tp *ld,int fd,coor_tp coor,struct spusr_list* p,struct airplane_list* air_head,struct usr_list *r_head);//�����û�����󣬲鿴���ࡢ����¼��
void spusr_login(struct spusr_list *sr_head,struct spusr_list* sl_head,lcd_tp *ld,int fd,coor_tp coor,struct airplane_list* air_head,struct usr_list *r_head);//�����û���½��
void find_passwd(struct usr_list* r_head,lcd_tp *ld,int fd);//�һ�����
void theck_in(struct usr_list* r_head,lcd_tp *ld,int fd);//ȡƱ
void usr_login_menu(struct usr_list *r_head,struct spusr_list *sr_head,struct usr_list *l_head,struct spusr_list *sl_head,lcd_tp *ld,int fd,coor_tp coor,struct airplane_list* air_head,struct info_list* info_head);//�û���½�˵�
void strtok_data(struct usr_list *r_head,char *buf);//�и�����д��ע����
void read_file(struct usr_list *r_head,char *filename);//��ȡ���û��ļ�����
void make_old_usr(struct usr_list* r_head);//���û���Ϣ��ȡ
void strtok_data_spusr(struct spusr_list* sr_head,char *buf);//�и���û�����д��ע����
void read_file_sp(struct spusr_list* sr_head,char *filename);//��ȡ�������û��ļ�����
void make_old_spusr(struct spusr_list* sr_head);//�ϳ����û���Ϣ��ȡ
void strtok_data_airplane(struct airplane_list* air_head,char *buf);//�и����Ϣ
void read_file_airplane(struct airplane_list* air_head,char *filename);//��ȡ������Ϣ�ļ�����
void make_old_airplane(struct airplane_list* air_head);//������Ϣ�Ķ�ȡ
void strtok_data_info(struct info_list* info_head,char *buf);//�и�ʵ����Ϣ
void read_file_info(struct info_list* info_head,char *filename);//���ļ���ȡʵ����Ϣ
void make_info(struct info_list* info_head);//ʵ����֤����Ϣ��ȡ







#endif