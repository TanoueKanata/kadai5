#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<errno.h>
#include<stdlib.h>

int main(){
    int mode=0; //1:post,0:get
    int val;
    struct sockaddr_in serv_addr;
    // ソケットを作る
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        perror("socket error\n");
        exit(1);
    }
    // アドレスを作る
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(80);

    int flag=1;
    val=setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    if(val<0){
        perror("setsocketopt error\n");
        exit(1);
    }
    // ソケットにアドレスを割り当てる
    val=bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));
    if(val<0){
        perror("bind error\n");
        exit(1);
    }
    // コネクション要求を待ち始めるよう指示
    val=listen(sockfd, 5);
    if(val<0)
    {
        perror("listen error\n");
        exit(1);
    }
    int new_sockfd;
    FILE *istream;
    char buff[1024];
    while(1){
        // 要求があったらそれを受け付ける(なければ待つ)
        new_sockfd = accept(sockfd, NULL, NULL);
        if(new_sockfd<0){
            perror("accept error\n");
            exit(1);
        }
        // クライアントからデータを受け取る
        istream=fdopen(new_sockfd,"r+");
        if(istream==NULL){
            perror("file open error\n");
            exit(1);
        }
        val=setvbuf(istream,NULL,_IONBF,0);
        if(val!=0){
            perror("setvbuf error\n");
            exit(1);
        }
        int len;
	char addr[100];
        while(1){
            if(fgets(buff,1024,istream)==0) break;
            if(strncmp(buff,"GET",3)==0){
                mode=0;
            }   //GETモード
            if(strncmp(buff,"POST",4)==0) mode=1;   //POSTモード
            if(strncmp(buff,"Content-Length:",15)==0){
			sscanf(buff,"Content-Length: %d",&len);
            }
            if(strcmp(buff,"\r\n")==0) break;
            printf("%s\n",buff);
        }
        if(mode==0) fprintf(istream,"HTTP/1.1 200 OK\r\nContet-Type:text/html\r\n\r\nHello\r\n");
        if(mode==1){
            fgets(buff,len+1,istream);
            sscanf(buff,"addr=%s",addr);
            fprintf(istream,"HTTP/1.1 200 OK\r\nContet-Type:text/html\r\n\r\nYour address is %s\r\n",addr);
        }
        sleep(1);
        val=fclose(istream);
        if(sockfd<0){
            perror("file close error\n");
            exit(1);
        }
    }
}
