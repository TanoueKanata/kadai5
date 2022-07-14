#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
    struct sockaddr_in serv_addr;
    // ソケットを作る
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // サーバのアドレスを作る
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(50000);
    // コネクションを張るための要求をサーバに送る
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));
    // 128 バイトのデータをソケットで送る
    int buff[128];
    buff[0]='a';
    write(sockfd, buff, 128);
    // ソケットを終了する
    close(sockfd);
}