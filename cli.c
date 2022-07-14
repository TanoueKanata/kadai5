#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
    int val;
    struct sockaddr_in serv_addr;
    // ソケットを作る
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // アドレスを作る
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(50000);
    // ソケットにアドレスを割り当てる
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));
    // コネクション要求を待ち始めるよう指示
    listen(sockfd, 5);
    // 要求があったらそれを受け付ける(なければ待つ)
    int new_sockfd = accept(sockfd, NULL, NULL);
    // クライアントからデータを受け取る
    char buff[128];
    val=read(new_sockfd, buff, 128);
    // 1 秒待ってソケットを終了する
    sleep(1);
    close(new_sockfd);
    close(sockfd);
    printf("%s\n",buff);
    return 0;
}