#include "head.h"

//任务： 负责接收消息。
void *routine(void *arg) //arg = &connfd
{
    int sockfd = *(int *)arg;
    
    char buf[1024];
    while(1)
    {
        bzero(buf,sizeof(buf));
        recv(sockfd,buf,sizeof(buf),0);
        printf("%s",buf);
    }
}

int main(int argc,char *argv[]) //./client 192.168.90.2 50001
{
    //1. 创建一个未连接套接字
    int sockfd;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    //2. 直接发起连接
    struct sockaddr_in srvaddr;
    socklen_t len = sizeof(srvaddr);
    bzero(&srvaddr,len);
    
    srvaddr.sin_family = AF_INET;//协议
    srvaddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET,argv[1],&srvaddr.sin_addr);
    
    int ret = connect(sockfd,(struct sockaddr *)&srvaddr,len);
    if(ret == -1)
        printf("connect error!\n");
    
    //2.5 创建线程
    pthread_t tid;
    pthread_create(&tid,NULL,routine,(void *)&sockfd);
    
    //3. 不断发送数据给服务器
    char buf[1024];
    while(1)
    {
        bzero(buf,sizeof(buf));
        fgets(buf,sizeof(buf),stdin);
        send(sockfd,buf,strlen(buf),0);

        if(strncmp(buf,"quit",4) == 0)
        {
            break;
        }
    }
    
    //4. 挂断电话
    close(sockfd);
    
    return 0;
}
