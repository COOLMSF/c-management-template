#include "head.h"


char chars[10];
char sendbuf[1024];

struct list_node{
    char name[10];//用户名
    char passwd[10];//密码
    char number[10];//航班号
    char staddress[10];//起点站
    char arraddress[10];//终点站
    char date[10];//班期
    char type[10];//机型
    char stime[10];//起飞时间
    char price[10];//票价
    char aggregate[10];//总金额
    struct list_head list;//内核链表指针域
};

struct list_node_ip{
    int connfd;
    char ip[50];
    pthread_t tid;
    struct list_head list;
};

struct list_node_ip *head = NULL;

//初始化内核链表头
struct list_node_ip *init_list_head_ip(struct list_node_ip *head)
{
    head = (struct list_node_ip *)malloc(sizeof(struct list_node_ip));
    if(head ==NULL)
        printf("malloc head error!\n");

    INIT_LIST_HEAD(&(head->list));

    return head;
}


//尾插数据
int add_list_node_ip(struct list_node_ip *head,int sockfd,char ip[50],pthread_t tid)
{
    struct list_node_ip *Node = NULL;

    Node = (struct list_node_ip *)malloc(sizeof(struct list_node_ip));
    if(Node == NULL)
        printf("malloc Node error\n");

    Node->connfd = sockfd;
    strcpy(Node->ip,ip);
    Node->tid = tid;

    list_add_tail(&(Node->list),&(head->list));

    return 0;
}


//线程退出删除对应链表
int delete_list_node_num_ip(struct list_node_ip *head,pthread_t tid)
{
    
    struct list_node_ip *p = NULL;
    struct list_node_ip *q = NULL;
    
    list_for_each_entry_safe(p,q,&(head->list),list)
    {
        if(p->tid == tid)
        {
            list_del(&(p->list));
            free(p);
        }
    }
    return 0;
}
//根据提供的connfd查找线程是否存在,存在返回1，不存在返回0
int find_list_node_num(struct list_node_ip *head,int connfd)
{
    
    struct list_node_ip *p = NULL;
    struct list_node_ip *q = NULL;
    
    list_for_each_entry_safe(p,q,&(head->list),list)
    {
        if(p->connfd == connfd)
        {
            return p->connfd ;
        }
    }
    return 0;
}
//删除整条链表
int delete_list_ip(struct list_node_ip *head)
{
    struct list_node_ip *p = NULL;
    struct list_node_ip *q = NULL;
    
    list_for_each_entry_safe(p,q,&(head->list),list)
    {
        list_del(&(p->list));
        free(p);
    }
    
    list_del(&(head->list));
    free(head);
    
    return 0;
}

/*获取数字字符*/
int char_input(int connfd)
{
    bzero(chars,sizeof(chars));
    fd_set rset;
    char buf[10];
    char *str;
    int get;
    int maxfd = connfd > STDIN_FILENO ? connfd : STDIN_FILENO;
    while(1)
    {
        FD_ZERO(&rset);
        FD_SET(connfd,&rset);
        FD_SET(STDIN_FILENO,&rset);
        select(maxfd+1,&rset,NULL,NULL,NULL);

        if(FD_ISSET(STDIN_FILENO,&rset))
        {
            bzero(chars,sizeof(chars));
            fgets(chars,sizeof(chars),stdin);
            return 1;
        }
        if(connfd != 0)
        {
            bzero(buf,sizeof(buf));
            if(recv(connfd,buf,sizeof(buf),0)>0)
                str = strtok(buf, "\n");
            strcpy(chars,str);
            printf("%s\n",chars);
                return 1;    
        }

    }
    return ;
}

/*申请链表头,返回链表头*/
struct list_node *init_list_head(struct list_node *_head,int connfd)
{
    _head = (struct list_node *)malloc(sizeof(struct list_node));
    if(_head == NULL)
    {
        printf("malloc head error!\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"malloc head error!\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
    }
    
    INIT_LIST_HEAD(&(_head->list));
    
    return _head;
}


/*管理员用户新增航班*/
int tail_add_list_plant(struct list_node *p_head,int connfd)
{    
    char buff[50] = {0};
    char name[10],passwd[10];
    struct list_node *Node = NULL;
    Node = (struct list_node *)malloc(sizeof(struct list_node));
    if(Node == NULL)
    {
        printf("malloc Node error!\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"malloc Node error!\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
    }

    printf("请输入你的名字:\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"请输入你的名字:\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(name,chars);
            break;
        }
    printf("密码:");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"密码:\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(passwd,chars);
            break;
        }
    if(strcmp(name,"admin") == 0 && strcmp(passwd,"admin123") == 0)//管理员用户才可以修改航班信息，账户为admin，密码为admin123
    {    
        strcpy(Node->name,name);
        strcpy(Node->passwd,passwd);
        printf("序号:");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"序号:\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(Node->number,chars);
            break;
        }
        //1. 打开目录
        DIR *dp = opendir("./date");
        if(dp == NULL)
        {
            printf("opendir error!\n");
            bzero(sendbuf,sizeof(sendbuf));
            sprintf(sendbuf,"opendir error!\n");
            send(connfd,sendbuf,strlen(sendbuf),0);
        }
        
        //2. 切换到目录下
        chdir("./date");

        sprintf(buff,"%s.txt",Node->number);

        FILE *fp = fopen(buff,"a");
        bzero(buff,sizeof(buff));    

        printf("出发地址:");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"出发地址:\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(Node->staddress,chars);
            break;
        }
        
        printf("目的地址:");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"目的地址:\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(Node->arraddress,chars);
            break;
        }
        
        printf("日期:");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"日期:\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(Node->date,chars);
            break;
        }

        printf("类型:");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"类型:\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(Node->type,chars);
            break;
        }

        printf("时间:");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"时间:\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(Node->stime,chars);
            break;
        }

        printf("价格:");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"价格:\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(Node->price,chars);
            break;
        }

        sprintf(buff,"%s,%s,%s,%s,%s,%s,%s",Node->number,Node->staddress,Node->arraddress,Node->date,Node->type,Node->stime,Node->price);

        fwrite(buff,50,1,fp);

        list_add_tail(&(Node->list),&(p_head->list));
        fclose(fp);
        chdir("../");
        closedir(dp);
        return 0;
    }
    else 
        printf("你不是管理员\n");
        return 1;
}
/*管理员删除航班*/
int delete_list_node_number(struct list_node *p_head,int connfd)
{
    char buff[50] = {0};
    char buf[50] = {0};
    char name[10],passwd[10],number[10];
    struct list_node *p = NULL;
    struct list_node *q = NULL;
    
    printf("please input your name:\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"请输入你的名字:\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(name,chars);
            break;
        }

    printf("密码:");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"密码:\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(passwd,chars);
            break;
        }
    if(strcmp(name,"admin") == 0 && strcmp(passwd,"admin123") == 0)//管理员用户才可以修改航班信息，账户为admin，密码为admin123
    {
        printf("number:");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"number:\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(number,chars);
            break;
        }
        list_for_each_entry_safe(p,q,&(p_head->list),list)
        {
            if(strcmp(p->number,number) == 0)
            {    
                DIR *dp = opendir("./date");
                if(dp == NULL)
                {
                    bzero(sendbuf,sizeof(sendbuf));
                    sprintf(sendbuf,"opendir error!\n");
                    send(connfd,sendbuf,strlen(sendbuf),0);
                    printf("opendir error!\n");
                }
        
                //2. 切换到目录下
                chdir("./date");
                sprintf(buff,"rm %s.txt",number);
                system(buff); 

                list_del(&(p->list));
                free(p);
                printf("%s remove\n",number);
                bzero(sendbuf,sizeof(sendbuf));
                sprintf(sendbuf,"%s remove!\n",number);
                send(connfd,sendbuf,strlen(sendbuf),0);

                chdir("../");
                closedir(dp);
                return 0;
            }
        }
    }
    else
    printf("你没有访问权限\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"你没有访问权限\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    return -1;
}

/*普通用户注册*/
int tail_add_list_user(struct list_node *u_head,int connfd)
{
    struct list_node *Node = NULL;
    char buff[100];
    Node = (struct list_node *)malloc(sizeof(struct list_node));
    if(Node == NULL)
    {
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"malloc Node error!\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        printf("malloc Node error!\n");
    }


    printf("请输入你的名字:\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"请输入你的名字:\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(Node->name,chars);
            break;
        }

    printf("密码:");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"密码:\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(Node->passwd,chars);
            break;
        }
    DIR *dp = opendir("./register");
    if(dp == NULL)
    {
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"opendir error!\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        printf("opendir error!\n");
    }
        
    chdir("./register");

    sprintf(buff,"%s.txt",Node->name);

    FILE *fp = fopen(buff,"a");
    bzero(buff,sizeof(buff));    

    sprintf(buff,"%s,%s",Node->name,Node->passwd);

    fwrite(buff,50,1,fp);

    fclose(fp);

    chdir("../");
    closedir(dp);

    list_add_tail(&(Node->list),&(u_head->list));
        
    printf("注册成功\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"注册成功\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    return 0;
}

/*普通用户登入购买票系统,返回注册后用户的地址*/
struct list_node *user_name_show_list(struct list_node *u_head,int connfd)
{
    struct list_head *p = NULL;
    struct list_node *tmp = NULL;
    char name[10],passwd[10];

    printf("请输入你的名字:\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"请输入你的名字:\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(name,chars);
            break;
        }
    printf("密码:");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"密码:\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(passwd,chars);
            break;
        }

    list_for_each_prev(p,&(u_head->list))
    {
        tmp = list_entry(p,struct list_node,list);
        if(strcmp(name,tmp->name) == 0 && strcmp(passwd,tmp->passwd) == 0)
            return tmp;
    }

    printf("你还没有注册\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"你还没有注册\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    return NULL;
}

/*普通用户购票,输入登陆用户的地址和航班的头*/
int user_buy_fun(struct list_node *u_head,struct list_node *p_head,int connfd)
{
    int a,b,c;
    char number[10],buf[10];
    struct list_head *p = NULL;
    struct list_node *tmp = NULL;
    
    printf("请输入序号:\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"请输入序号:\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(number,chars);
            break;
        }
    list_for_each_prev(p,&(p_head->list))//查看用户是否购买过此航班
    {
        tmp = list_entry(p,struct list_node,list);
        if(strcmp(tmp->number,u_head->number) == 0)
        {
            printf("你有%s个航班 \n",tmp->number);
            bzero(sendbuf,sizeof(sendbuf));
            sprintf(sendbuf,"你有%s个航班 \n",tmp->number);
            send(connfd,sendbuf,strlen(sendbuf),0);
        }
    }

    list_for_each_prev(p,&(p_head->list))//未购买则执行买票程序
    {
        tmp = list_entry(p,struct list_node,list);
        if(strcmp(tmp->number,number) == 0)
        {
            if(u_head->price == 0)//判断是否是第一张机票
            {
                printf("%s\n",u_head->name);
                bzero(sendbuf,sizeof(sendbuf));
                sprintf(sendbuf,"%s\n",u_head->name);

                send(connfd,sendbuf,strlen(sendbuf),0);
                strcpy(u_head->number,tmp->number);
                strcpy(u_head->staddress,tmp->staddress);
                strcpy(u_head->arraddress,tmp->arraddress);
                strcpy(u_head->date,tmp->date);    
                strcpy(u_head->type,tmp->type);
                strcpy(u_head->stime,tmp->stime);
                strcpy(u_head->price,tmp->price);
                strcpy(u_head->price,tmp->price);
                strcpy(u_head->aggregate,u_head->price);
                printf("你需要支付 %s rmb",u_head->aggregate);
                bzero(sendbuf,sizeof(sendbuf));
                sprintf(sendbuf,"你需要支付 %s rmb",u_head->aggregate);
                send(connfd,sendbuf,strlen(sendbuf),0);
            }
            else//如果不是第一张机票则在链表后加一节点
            {
                struct list_node *Node = NULL;
                Node = (struct list_node *)malloc(sizeof(struct list_node));
                if(Node == NULL)
                {
                    bzero(sendbuf,sizeof(sendbuf));
                    sprintf(sendbuf,"malloc Node error!\n");
                    send(connfd,sendbuf,strlen(sendbuf),0);
                    printf("malloc Node error!\n");
                }

                printf("%s\n",u_head->name);
                bzero(sendbuf,sizeof(sendbuf));
                sprintf(sendbuf,"%s\n",u_head->name);
                send(connfd,sendbuf,strlen(sendbuf),0);
                strcpy(Node->name,u_head->name);
                strcpy(Node->passwd,u_head->passwd);
                strcpy(Node->number,tmp->number);
                strcpy(Node->staddress,tmp->staddress);
                strcpy(Node->arraddress,tmp->arraddress);
                strcpy(Node->date,tmp->date);    
                strcpy(Node->type,tmp->type);
                strcpy(Node->stime,tmp->stime);
                strcpy(Node->price,tmp->price);
                strcpy(Node->price,tmp->price);
                a = atoi(u_head->price);
                b = atoi(tmp->price);
                c = a+b;
                sprintf(buf,"%d",c);
                strcpy(Node->aggregate,buf);
                printf("你需要支付 %d rmb\n",b);
                bzero(sendbuf,sizeof(sendbuf));
                sprintf(sendbuf,"你需要支付 %d rmb\n",b);
                send(connfd,sendbuf,strlen(sendbuf),0);

                list_add_tail(&(Node->list),&(u_head->list));

            }
            printf("购买成功\n");
            bzero(sendbuf,sizeof(sendbuf));
            sprintf(sendbuf,"购买成功\n");
            send(connfd,sendbuf,strlen(sendbuf),0);
        }
    }
    return 0;
}

/*普通用户退票,输入登陆用户的地址和航班的头*/
int user_ret_fun(struct list_node *u_head,struct list_node *p_head,int connfd)
{
    char number[10];
    struct list_head *p = NULL;
    struct list_node *tmp = NULL;
    struct list_head *q = NULL;
    struct list_node *obj = NULL;
    
    printf("===========================================================================\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"===========================================================================\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("  number    staddress    arraddress    date    type    stime    price    \n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"  number    staddress    arraddress    date    type    stime    price    \n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    
    list_for_each_prev(p,&(p_head->list))
        {
            tmp = list_entry(p,struct list_node,list);
            if(strcmp(tmp->number,u_head->number) == 0)
            {
                tmp = list_entry(p,struct list_node,list);
                printf("  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
                bzero(sendbuf,sizeof(sendbuf));
                sprintf(sendbuf,"  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
                send(connfd,sendbuf,strlen(sendbuf),0);
            }
            else 
                list_for_each_prev(q,&(u_head->list))
                {
                    obj = list_entry(q,struct list_node,list);
                    if(strcmp(obj->number,tmp->number) == 0 && strcmp(obj->name,u_head->name) ==0)
                    {
                        printf("  %s        %s            %s      %s   %s     %s   %s    \n",obj->number,obj->staddress,obj->arraddress,obj->date,obj->type,obj->stime,obj->price);
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"  %s        %s            %s      %s   %s     %s   %s    \n",obj->number,obj->staddress,obj->arraddress,obj->date,obj->type,obj->stime,obj->price);
                        send(connfd,sendbuf,strlen(sendbuf),0);            
                    }
                    
                }
        }
    printf("===========================================================================\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"===========================================================================\n");
    send(connfd,sendbuf,strlen(sendbuf),0);

    printf("请输入你想要返回的航班");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"请输入你想要返回的航班:\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(number,chars);
            break;
        }
    list_for_each_safe(p,q,&(u_head->list))
        {
            tmp = list_entry(p,struct list_node,list);
            if(strcmp(tmp->number,number) == 0 && strcmp(tmp->name,u_head->name) == 0)
            {    
                    printf("return you %s rmb\n",tmp->price);
                    bzero(sendbuf,sizeof(sendbuf));
                    sprintf(sendbuf,"return you %s rmb\n",tmp->price);
                    send(connfd,sendbuf,strlen(sendbuf),0);
                    list_del(p); 
                    free(tmp); 
                    return 0;
            }        
        }

    printf("你没有航班\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"你没有航班\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    return -1;
}
/*普通用户改签,输入登陆用户的地址和航班的头*/
int user_tic_fun(struct list_node *u_head,struct list_node *p_head,int connfd)
{
    int a,b,c,d;
    char number[10],p_number[10];
    struct list_head *p = NULL;
    struct list_head *q = NULL;
    struct list_node *tmp = NULL;
    struct list_node *obj = NULL;
    char buff[50],buf[10];
    bzero(buff,sizeof(buff));
    printf("===========================================================================\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"===========================================================================\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("  number    staddress    arraddress    date    type    stime    price    \n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"  number    staddress    arraddress    date    type    stime    price    \n");
    send(connfd,sendbuf,strlen(sendbuf),0);

    list_for_each_prev(p,&(p_head->list))
        {
            tmp = list_entry(p,struct list_node,list);
            if(strcmp(tmp->number,u_head->number) == 0)
            {
                printf("%s    %s    %s  %s      %s   %s     %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
                bzero(sendbuf,sizeof(sendbuf));
                sprintf(sendbuf,"  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
                send(connfd,sendbuf,strlen(sendbuf),0);
            }
            else 
                list_for_each_prev(q,&(u_head->list))
                {
                    obj = list_entry(q,struct list_node,list);
                    if(strcmp(obj->number,tmp->number) == 0 && strcmp(obj->name,u_head->name) == 0)
                    {
                        printf("%s    %s    %s  %s      %s   %s     %s    \n",obj->number,obj->staddress,obj->arraddress,obj->date,obj->type,obj->stime,obj->price);    
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"  %s        %s            %s      %s   %s     %s   %s    \n",obj->number,obj->staddress,obj->arraddress,obj->date,obj->type,obj->stime,obj->price);
                        send(connfd,sendbuf,strlen(sendbuf),0);    
                    }
                    
                }
        }
    printf("===========================================================================\n");    
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"===========================================================================\n");
    send(connfd,sendbuf,strlen(sendbuf),0);

    printf("请输入你要更改的航班:\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"请输入你要更改的航班:\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(number,chars);
            break;
        }
    printf("Flights to be rescheduled:\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"Flights to be rescheduled:\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(p_number,chars);
            break;
        }
    list_for_each_prev(p,&(u_head->list))
    {
        tmp = list_entry(p,struct list_node,list);
        if(strcmp(tmp->number,number) == 0 && strcmp(tmp->name,u_head->name) == 0)
        {
            list_for_each_prev(q,&(p_head->list))
            {
            obj = list_entry(q,struct list_node,list);
            if(strcmp(obj->number,p_number) == 0)
            {
                strcpy(tmp->number,obj->number);
                strcpy(tmp->staddress,obj->staddress);
                strcpy(tmp->arraddress,obj->arraddress);
                strcpy(tmp->date,obj->date);    
                strcpy(tmp->type,obj->type);
                strcpy(tmp->stime,obj->stime);
                a = atoi(tmp->price);
                strcpy(tmp->price,obj->price);
                b = atoi(obj->price);
                c = a - b;
                d = atoi(tmp->aggregate);
                d = d + c;
                sprintf(buf,"%d",d);
                strcpy(tmp->aggregate,buf);
                printf("你需要支付 %d rmb\n",c);
                bzero(sendbuf,sizeof(sendbuf));
                sprintf(sendbuf,"你需要支付 %d rmb\n",c);
                send(connfd,sendbuf,strlen(sendbuf),0);
                printf("Endorsed to success\n");
                bzero(sendbuf,sizeof(sendbuf));
                sprintf(sendbuf,"Endorsed to success\n");
                send(connfd,sendbuf,strlen(sendbuf),0);
                return 0;
            }
            }
            
        }
    }    
    

    printf("你没有航班\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"你没有航班\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    return -1;
}    

/*普通用户查看所有航班信息*/
int user_show_all_list(struct list_node *p_head,int connfd)
{
    struct list_head *p = NULL;
    struct list_node *tmp = NULL;
    
    printf("===========================================================================\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"===========================================================================\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("  number    staddress    arraddress    date    type    stime    price    \n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"  number    staddress    arraddress    date    type    stime    price    \n");
    send(connfd,sendbuf,strlen(sendbuf),0);

    list_for_each_prev(p,&(p_head->list))
    {
        tmp = list_entry(p,struct list_node,list);
        printf("  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);    
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
        send(connfd,sendbuf,strlen(sendbuf),0);
    }
    printf("===========================================================================\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"===========================================================================\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    return 0;
}

/*普通用户通过航班号查看航班==========快速查询*/
int user_number_show_list(struct list_node *p_head,char number[10],int connfd)
{
    struct list_head *p = NULL;
    struct list_node *tmp = NULL;
    
    printf("===========================================================================\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"===========================================================================\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("  number    staddress    arraddress    date    type    stime    price    \n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"  number    staddress    arraddress    date    type    stime    price    \n");
    send(connfd,sendbuf,strlen(sendbuf),0);

    list_for_each(p,&(p_head->list))
    {
        tmp = list_entry(p,struct list_node,list);
        if(strcmp(tmp->number,number) == 0)
        {
            printf("  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
            bzero(sendbuf,sizeof(sendbuf));
            sprintf(sendbuf,"  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
            send(connfd,sendbuf,strlen(sendbuf),0);
        }
    }

    printf("===========================================================================\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"===========================================================================\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    
    return 0;
}

/*普通用户时间/日期/机型查询航班查看航班==========条件查询*/
int user_condition_show_list(struct list_node *p_head,int connfd)
{
    char num;
    char stime[10],date[10],type[10];
    struct list_head *p = NULL;
    struct list_node *tmp = NULL;char buff[50];
    bzero(buff,sizeof(buff));

    
    while(1)
    {    
        printf("你想要查询什么:\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"你想要查询什么:\n");
        send(connfd,sendbuf,strlen(sendbuf),0);

        printf("1.出发时间\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"1.出发时间\n");
        send(connfd,sendbuf,strlen(sendbuf),0);

        printf("2.时间\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"2.时间\n");
        send(connfd,sendbuf,strlen(sendbuf),0);

        printf("3.类型\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"3.类型\n");
        send(connfd,sendbuf,strlen(sendbuf),0);

        printf("4.退出\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"4.退出\n");
        send(connfd,sendbuf,strlen(sendbuf),0);

        while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(&num,chars);
            break;
        }
        switch(num)
        {
            case 1:
                printf("请输入时间:");
                bzero(sendbuf,sizeof(sendbuf));
                sprintf(sendbuf,"请输入时间:\n");
                send(connfd,sendbuf,strlen(sendbuf),0);

                while(1)
                if(char_input(connfd) == 1)
                {
                    strcpy(stime,chars);
                    break;
                }
                list_for_each(p,&(p_head->list))
                {
                    tmp = list_entry(p,struct list_node,list);
                    if(strcmp(tmp->stime,stime) == 0)
                    {
                        printf("===========================================================================\n");
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"===========================================================================\n");
                        send(connfd,sendbuf,strlen(sendbuf),0);

                        printf("  number    staddress    arraddress    date    type    stime    price    \n");
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"  number    staddress    arraddress    date    type    stime    price    \n");
                        send(connfd,sendbuf,strlen(sendbuf),0);

                        printf("  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
                        send(connfd,sendbuf,strlen(sendbuf),0);

                        printf("============================================================================\n");
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"===========================================================================\n");
                        send(connfd,sendbuf,strlen(sendbuf),0);
                    }
                }
                break;
            case 2:
                printf("请输入日期:");
                bzero(sendbuf,sizeof(sendbuf));
                sprintf(sendbuf,"请输入日期:\n");
                send(connfd,sendbuf,strlen(sendbuf),0);

                while(1)
                if(char_input(connfd) == 1)
                {
                    strcpy(date,chars);
                    break;
                }
                list_for_each(p,&(p_head->list))
                {
                    tmp = list_entry(p,struct list_node,list);
                    if(strcmp(tmp->date,date) == 0)
                    {
                        printf("===========================================================================\n");
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"===========================================================================\n");
                        send(connfd,sendbuf,strlen(sendbuf),0);

                        printf("  number    staddress    arraddress    date    type    stime    price    \n");
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"  number    staddress    arraddress    date    type    stime    price    \n");
                        send(connfd,sendbuf,strlen(sendbuf),0);

                        printf("  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
                        send(connfd,sendbuf,strlen(sendbuf),0);

                        printf("============================================================================\n");
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"===========================================================================\n");
                        send(connfd,sendbuf,strlen(sendbuf),0);
                    }
                }
                break;
            case 3:
                printf("输入类型:");
                bzero(sendbuf,sizeof(sendbuf));
                sprintf(sendbuf,"输入类型:\n");
                send(connfd,sendbuf,strlen(sendbuf),0);

                while(1)
                if(char_input(connfd) == 1)
                {
                    strcpy(type,chars);
                    break;
                }

                list_for_each(p,&(p_head->list))
                {
                    tmp = list_entry(p,struct list_node,list);
                    if(strcmp(tmp->type,type) == 0)
                    {
                        printf("===========================================================================\n");
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"===========================================================================\n");
                        send(connfd,sendbuf,strlen(sendbuf),0);

                        printf("  number    staddress    arraddress    date    type    stime    price    \n");
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"  number    staddress    arraddress    date    type    stime    price    \n");
                        send(connfd,sendbuf,strlen(sendbuf),0);

                        printf("  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"  %s        %s            %s      %s   %s     %s   %s    \n",tmp->number,tmp->staddress,tmp->arraddress,tmp->date,tmp->type,tmp->stime,tmp->price);
                        send(connfd,sendbuf,strlen(sendbuf),0);

                        printf("============================================================================\n");
                        bzero(sendbuf,sizeof(sendbuf));
                        sprintf(sendbuf,"===========================================================================\n");
                        send(connfd,sendbuf,strlen(sendbuf),0);
                    }
                }
                break;
            case 4:
                return 0;
                break;
            default:
                break;
    
        }
        
        }
    return -1;
}

/*释放整条链表*/
int delete_list(struct list_node *_head)
{
    struct list_node *p = NULL;
    struct list_node *q = NULL;
    
    list_for_each_entry_safe(p,q,&(_head->list),list)
    {
        list_del(&(p->list));
        free(p);
    }
    
    list_del(&(_head->list));
    free(_head);
    
    return 0;
}

/*航班管理触摸处理函数*/
int flight_track_fun(int connfd)
{

    char buff[6];
    int get;
    fd_set rset;
    int maxfd = connfd > STDIN_FILENO ? connfd : STDIN_FILENO;
    while(1)
    {
        FD_ZERO(&rset);
        FD_SET(connfd,&rset);
        FD_SET(STDIN_FILENO,&rset);
        select(maxfd+1,&rset,NULL,NULL,NULL);

            
        if(FD_ISSET(STDIN_FILENO,&rset))
        {
            bzero(buff,sizeof(buff));
            fgets(buff,sizeof(buff),stdin);
            get = atoi(buff);
            return get;
        }
        if(connfd != 0)
        {
            bzero(buff,sizeof(buff));
            if(recv(connfd,buff,sizeof(buff),0)>0)
            {
                get = atoi(buff);
                return get;    
            }    
        }
    }

    return 0;
}

/*子主界面触摸处理函数*/
int sun_main_interface_fun(int connfd)
{

    char buff[6];
    int get;
    fd_set rset;    
    int maxfd = connfd > STDIN_FILENO ? connfd: STDIN_FILENO;

    while(1)
    {
        FD_ZERO(&rset);
        FD_SET(connfd,&rset);
        FD_SET(STDIN_FILENO,&rset);
        select(maxfd+1,&rset,NULL,NULL,NULL);

            
        if(FD_ISSET(STDIN_FILENO,&rset))
        {
            bzero(buff,sizeof(buff));
            fgets(buff,sizeof(buff),stdin);
            get = atoi(buff);
            return get;
        }
        if(connfd != 0)
        {
            bzero(buff,sizeof(buff));
            if(recv(connfd,buff,sizeof(buff),0)>0)
            {
                get = atoi(buff);
                return get;    
            }    
        }
    }

    return 0;
}

/*主界面触摸处理函数*/
int main_interface_fun(int connfd)
{
    
    char buff[6];
    int get;
    fd_set rset;
    int maxfd =connfd > STDIN_FILENO ? connfd : STDIN_FILENO;
    while(1)
    {
        FD_ZERO(&rset);
        FD_SET(connfd,&rset);
        FD_SET(STDIN_FILENO,&rset);
        select(maxfd+1,&rset,NULL,NULL,NULL);

            
        if(FD_ISSET(STDIN_FILENO,&rset))
        {
            bzero(buff,sizeof(buff));
            fgets(buff,sizeof(buff),stdin);
            get = atoi(buff);
            return get;
        }
        if(connfd != 0)
        {
            bzero(buff,sizeof(buff));
            if(recv(connfd,buff,sizeof(buff),0)>0)
            {
                get = atoi(buff);
                return get;    
            }    
        }

    }

    return 0;
}

/*管理航班信息，输入航班头*/
int manage_fun(struct list_node *p_head,int connfd)
{
    char num;
    while(1)
    {
        printf("========manage menu=======\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"========manage menu=======\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        printf("1.添加航班\n");//添加航班信息
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"1.添加航班\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        printf("2.修改航班\n");//添加航班信息
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"2.修改航班\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        printf("3.退出\n");//添加航班信息
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"3.退出\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        printf("===========================\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"===========================\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        printf("输入你的选择:");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"输入你的选择:\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
        while(1)
        if(char_input(connfd) == 1)
        {
            strcpy(&num,chars);
            break;
        }
        switch(num)
        {
            case 1:
                tail_add_list_plant(p_head,connfd);
                break;
            case 2:
                delete_list_node_number(p_head,connfd);
                break;
            case 3:
                return 0;
            default:
                break;
        }
    }
    return -1;
}

/*主界面---显示登陆界面：*/
int home_screen_fun(int connfd)
{
    printf("========Home Screen=========\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"========Home Screen=========\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("1.注册\n");//用户注册，需调用tail_add_list_user(struct list_node *u_head)
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"1.注册\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("2.login\n");//用户登陆，需调用menu_fun()
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"2.登陆\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("3.管理\n");//管理航班，添加航班或删除航班,需调用manage_fun(struct list_node *p_head)
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"3.管理\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("4.退出\n");//退出程序
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"4.退出\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("============================\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"============================\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
}

/*主菜单---显示登陆成功：*/
int menu_fun(int connfd)
{
    printf("========MENU=========\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"========MENU=========\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("1.查询所有航\n");//查询所有航班
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"1.查询所有航\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("2.条件查询\n");//条件查询
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"2.条件查询\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("3.快速查询\n");//快速查询
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"3.快速查询\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("4.购票\n");//购票
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"4.购票\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("5.改签\n");//改签
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"5.改签\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("6.退票\n");//退票
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"6.退票\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("7.退出\n");//退出登陆
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"7.退出\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
    printf("=====================\n");
    bzero(sendbuf,sizeof(sendbuf));
    sprintf(sendbuf,"============================\n");
    send(connfd,sendbuf,strlen(sendbuf),0);
}


/*管理航班子菜单*/
int manage_fun_su(struct list_node *p_head,int connfd)
{
    int ret;
    while(1)
    {
        ret = flight_track_fun(connfd);
        char buf[100] = "1:) add plant\n2:) del plant\n";
        // fwrite(buf, sizeof(buf), 1, connfd);
        switch(ret)
        {
            case 1:
                tail_add_list_plant(p_head,connfd);
                break;
            case 2:
                delete_list_node_number(p_head,connfd);
                break;
            case 3:
                return 1;
                break;
            default:
                break;
        }

    }
    return 0;
}

/*用户登陆子函数*/
int usre_landing_fun(struct list_node *u_head,struct list_node *p_head,int connfd)
{
    int touch;
    char num[10];

    while(1)
    {    
        menu_fun(connfd);//crt显示菜单
        touch = sun_main_interface_fun(connfd);
        switch(touch)
        {
                case 1://1、查询所有航班
                    user_show_all_list(p_head,connfd);
                    break;
                case 2://2、条件查询
                    user_condition_show_list(p_head,connfd);
                    break;
                case 3://3、快速查询
                    printf("请输入你要查询的序号:\n");
                    bzero(sendbuf,sizeof(sendbuf));
                    sprintf(sendbuf,"请输入你要查询的序号:\n");
                    send(connfd,sendbuf,strlen(sendbuf),0);
                    while(1)
                    if(char_input(connfd) == 1)
                    {
                        strcpy(num,chars);
                        break;
                    }
                    user_number_show_list(p_head,num,connfd);
                    break;
                case 4://4、购票
                    user_buy_fun(u_head,p_head,connfd);
                    break;
                case 5://5、改签
                    user_tic_fun(u_head,p_head,connfd);
                    break;
                case 6://6、退票
                    user_ret_fun(u_head,p_head,connfd);
                    break;
                case 7://7、退出登陆
                    return 1;
                    break;
                default:
                    break;
        }

    }
    return 0;
}

/*从文件中读取航班信息*/
int date_reading_fun(struct list_node *p_head,int connfd)
{
    char name[10],passwd[10];

    //1. 打开目录
    DIR *dp = opendir("./date");
    if(dp == NULL)
    {
        printf("opendir error!\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"opendir error!\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
    }
    
    //2. 切换到目录下
    chdir("./date");
    
    //3. 读取目录中每一项
    struct dirent *ep = NULL;
    while(1)
    {
        ep = readdir(dp);
        if(ep == NULL)  //读取完毕
            break;
            
        if(ep->d_name[0] == '.')
            continue;
    
        char str[50] = {0};
        FILE *fp = fopen(ep->d_name,"r");
        
        fread(str,10,5,fp);
        //printf("%s\n",str);
        char seps[] = ","; 
        struct list_node *Node = NULL;
        Node = (struct list_node *)malloc(sizeof(struct list_node));
        if(Node == NULL)
        {
            printf("malloc Node error!\n");
            bzero(sendbuf,sizeof(sendbuf));
            sprintf(sendbuf,"malloc Node error!\n");
            send(connfd,sendbuf,strlen(sendbuf),0);
        }

        strcpy(Node->name,"admin");
        strcpy(Node->passwd,"admin123");

        char *tmp;
        tmp = strtok(str,seps);
        strcpy(Node->number,tmp);
        
        tmp = strtok(NULL,seps); 
        strcpy(Node->staddress,tmp);
        
        tmp = strtok(NULL,seps); 
        strcpy(Node->arraddress,tmp);

        tmp = strtok(NULL,seps); 
        strcpy(Node->date,tmp);

        tmp = strtok(NULL,seps); 
        strcpy(Node->type,tmp);

        tmp = strtok(NULL,seps); 
        strcpy(Node->stime,tmp);

        tmp = strtok(NULL,seps);
        strcpy(Node->price,tmp);

        fclose(fp);

        list_add_tail(&(Node->list),&(p_head->list));
    }

    chdir("../");//切换回上一级目录
    //4. 关闭目录
    closedir(dp);
    
    return 0;
}

/*从文件中读取用户注册信息*/
int register_reading_fun(struct list_node *u_head,int connfd)
{
    char name[10],passwd[10];

    //1. 打开目录
    DIR *dp = opendir("./register");
    if(dp == NULL)
    {
        printf("opendir error!\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"opendir error!\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
    }
    
    //2. 切换到目录下
    chdir("./register");
    
    //3. 读取目录中每一项
    struct dirent *ep = NULL;
    while(1)
    {
        ep = readdir(dp);
        if(ep == NULL)  //读取完毕
            break;
            
        if(ep->d_name[0] == '.')
            continue;
    
        char str[50] = {0};
        FILE *fp = fopen(ep->d_name,"r");
        
        fread(str,10,5,fp);
        //printf("%s\n",str);// name,passwd
        
        char seps[] = ",";  //分隔的字符是什么
        struct list_node *Node = NULL;
        Node = (struct list_node *)malloc(sizeof(struct list_node));
        if(Node == NULL)
        {
            printf("malloc Node error!\n");
            bzero(sendbuf,sizeof(sendbuf));
            sprintf(sendbuf,"malloc Node error!\n");
            send(connfd,sendbuf,strlen(sendbuf),0);
        }

        char *tmp;
        tmp = strtok(str,seps);
        strcpy(Node->name,tmp);
        
        tmp = strtok(NULL,seps); 
        strcpy(Node->passwd,tmp);

        fclose(fp);

        list_add_tail(&(Node->list),&(u_head->list));
    }

    chdir("../");//切换回上一级目录
    //4. 关闭目录
    closedir(dp);
    
    return 0;
}

/*从文件中读取用户购票信息*/
int date_login_fun(struct list_node *u_head,int connfd)
{
    char name[10],passwd[10];

    //1. 打开目录
    DIR *dp = opendir("./user");
    if(dp == NULL)
    {
        printf("opendir error!\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"opendir error!\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
    }
    
    //2. 切换到目录下
    chdir("./user");
    
    //3. 读取目录中每一项
    struct dirent *ep = NULL;
    while(1)
    {
        ep = readdir(dp);
        if(ep == NULL)  //读取完毕
            break;
            
        if(ep->d_name[0] == '.')
            continue;

        char str[500] = {0};
        FILE *fp = fopen(ep->d_name,"r");
        
        fread(str,100,5,fp);
        //printf("%s\n",str);
        char seps[] = ","; 
        struct list_node *Node = NULL;
        Node = (struct list_node *)malloc(sizeof(struct list_node));
        if(Node == NULL)
        {
            printf("malloc Node error!\n");
            bzero(sendbuf,sizeof(sendbuf));
            sprintf(sendbuf,"malloc Node error!\n");
            send(connfd,sendbuf,strlen(sendbuf),0);
        }

        char *tmp;

        tmp = strtok(str,seps);
        strcpy(Node->name,tmp);

        tmp = strtok(NULL,seps);
        strcpy(Node->passwd,tmp);
        
        tmp = strtok(NULL,seps);
        strcpy(Node->number,tmp);

        tmp = strtok(NULL,seps); 
        strcpy(Node->staddress,tmp);
        
        tmp = strtok(NULL,seps); 
        strcpy(Node->arraddress,tmp);

        tmp = strtok(NULL,seps); 
        strcpy(Node->date,tmp);

        tmp = strtok(NULL,seps); 
        strcpy(Node->type,tmp);

        tmp = strtok(NULL,seps); 
        strcpy(Node->stime,tmp);

        tmp = strtok(NULL,seps);
        strcpy(Node->price,tmp);

        tmp = strtok(NULL,seps);
        strcpy(Node->aggregate,tmp);

        fclose(fp);

        list_add_tail(&(Node->list),&(u_head->list));
    }

    chdir("../");//切换回上一级目录
    //4. 关闭目录
    closedir(dp);
    
    return 0;
}
/*存储所有购票信息*/
int store_all_ticket(struct list_node *u_head,int connfd)
{    
    char buff[500],nm[10];
    char name[10],passwd[10],number[10],staddress[10],arraddress[10],date[10],type[10],stime[10],price[10],aggregate[10];
    struct list_head *p = NULL;
    struct list_node *tmp = NULL;
    //1. 打开目录
    DIR *dp = opendir("./user");
    if(dp == NULL)
    {
        printf("opendir error!\n");
        bzero(sendbuf,sizeof(sendbuf));
        sprintf(sendbuf,"opendir error!\n");
        send(connfd,sendbuf,strlen(sendbuf),0);
    }

    //2. 切换到目录下
    chdir("./user");
    
    bzero(nm,sizeof(nm));
    // sprintf(nm,"rm *.txt");
    // system(nm);

    bzero(buff,sizeof(buff));
    bzero(nm,sizeof(nm));

    list_for_each(p,&(u_head->list))
    {
        
        tmp = list_entry(p,struct list_node,list);
        if (atoi(tmp->price) != 0)
        {
        sprintf(nm,"%s %s.txt",tmp->name,tmp->number);
        FILE *fp = fopen(nm,"w+");

        strcpy(name,tmp->name);
        strcpy(passwd,tmp->passwd);    
        strcpy(number,tmp->number);
        strcpy(staddress,tmp->staddress);
        strcpy(arraddress,tmp->arraddress);
        strcpy(date,tmp->date);
        strcpy(type,tmp->type);
        strcpy(stime,tmp->stime);
        strcpy(price,tmp->price);
        strcpy(aggregate,tmp->aggregate);
        sprintf(buff,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",tmp->name,passwd,number,staddress,arraddress,date,type,stime,price,aggregate);
        fwrite(buff,50,1,fp);

        fclose(fp);
        }
    }
    chdir("../");
    closedir(dp);
    return 0;
}

/*退出程序*/
int exit_fun(struct list_node *p_head,struct list_node *u_head,int connfd)
{

    /*保存用户的购票信息*/
    store_all_ticket(u_head,connfd);
    /*释放链表*/
    delete_list(u_head);

    delete_list(p_head);
    return 0;
}


//子线程处理函数
void * fun(void *arg)
{
    int connfd;
    pthread_t tid;
    
    //获得连接的客户端已连接套接字、本线程的tid
    connfd = *(int *)arg;
    tid = pthread_self();

    /*初始化链表*/
    struct list_node *p_head = NULL;
    struct list_node *u_head = NULL;
    u_head = init_list_head(u_head,connfd);
    p_head = init_list_head(p_head,connfd);
    
    date_reading_fun(p_head,connfd);
    date_login_fun(u_head,connfd);
    register_reading_fun(u_head,connfd);

    
    /*定义变量*/
    int ret;//用于判定用户选择
    struct list_node *u_ret;//普通用户登陆，接收user_name_show_list返回值
    sleep(3);//等待3秒
    while(1)
        {    
            home_screen_fun(connfd);//crt显示主界面
            ret = main_interface_fun(connfd);
            switch(ret)
            {
                case 1 ://用户注册
                    tail_add_list_user(u_head,connfd);
                    break;
                case 2 ://用户登陆
                    u_ret = user_name_show_list(u_head,connfd);
                    if(u_ret == NULL)
                        break;
                    usre_landing_fun(u_ret,p_head,connfd);//用户登陆子菜单
                    break;
                case 3 ://管理航班
                    manage_fun_su(p_head,connfd);
                    break;
                case 4 ://退出程序
                    exit_fun(p_head,u_head,connfd);//退出程序
                    sleep(1);    
                    break;
                default:
                    break;


            }
        }
        

    //删除该线程对应的链表节点
    delete_list_node_num_ip(head,tid);
    //关闭连接套接字
    close(connfd);
    //线程退出
    pthread_exit(NULL);

}

int main(int argc, char const *argv[])
{
    pthread_t tid;

    //初始化链表
    head = init_list_head_ip(head);

    //创建未连接套接字
    int sockfd;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    printf("socket:%d\n",sockfd);

    //绑定IP地址，端口号等到未连接的套接字中
    struct sockaddr_in srvaddr;
    socklen_t len = sizeof(srvaddr);
    bzero(&srvaddr,len);

    srvaddr.sin_family = AF_INET;            //协议
    srvaddr.sin_port = htons(atoi(argv[1]));//端口号
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);//服务器ip
    bind(sockfd,(struct sockaddr *)&srvaddr,len);

    //设置监听套接字
    listen(sockfd,5);

    //等待连接
    struct sockaddr_in cliaddr;
    int connfd;
    while(1)
    {
        //等待连接，连接上就创建线程并将connfd传递给子线程，信息记入链表
        connfd = accept(sockfd,(struct sockaddr *)&cliaddr,&len);
        pthread_create(&tid,NULL,fun,(void *)&connfd);
        add_list_node_ip(head,connfd,(char *)inet_ntoa(cliaddr.sin_addr),tid);
        printf("============================\n");
        printf("connfd:%d\n",connfd);
        printf("new connection:%s\n",(char *)inet_ntoa(cliaddr.sin_addr));
        printf("tid:%d\n",(int)tid);
        printf("============================\n");
        continue;
    }

    //接合最后一个线程
    pthread_join(tid,NULL);

    //删除整条链表
    delete_list_ip(head);

    //关闭未来连接套接字
    close(sockfd);

    return 0;
}
