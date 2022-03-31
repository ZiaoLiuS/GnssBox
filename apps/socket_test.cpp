
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main(){
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    //服务器的ip为本地，端口号1234
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("47.114.134.129");
    serv_addr.sin_port = htons(7190);

    //向服务器发送连接请求
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    //发送并接收数据
    char buffer[40];
    read(sock, buffer, sizeof(buffer) - 1);
    printf("Serve send: %s\n", buffer);

    //断开连接
    close(sock);

    return 0;
}

