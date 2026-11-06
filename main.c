#define _POSIX_C_SOURCE 200809L
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
static volatile sig_atomic_t running=1;
static void stop(int n){(void)n;running=0;}
int main(int argc,char**argv){if(argc!=2){fprintf(stderr,"Usage: app PORT (0 selects an available loopback port)\n");return 2;}char*end;long port=strtol(argv[1],&end,10);if(*end||port<0||port>65535)return 2;int server=socket(AF_INET,SOCK_STREAM,0);if(server<0)return 1;int yes=1;setsockopt(server,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes);struct sockaddr_in a;memset(&a,0,sizeof a);a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);a.sin_port=htons((unsigned short)port);if(bind(server,(struct sockaddr*)&a,sizeof a)||listen(server,32)){perror("listen");close(server);return 1;}socklen_t size=sizeof a;getsockname(server,(struct sockaddr*)&a,&size);printf("Listening 127.0.0.1:%u\n",ntohs(a.sin_port));fflush(stdout);int ep=epoll_create1(0);if(ep<0)return 1;struct epoll_event event;event.events=EPOLLIN;event.data.fd=server;epoll_ctl(ep,EPOLL_CTL_ADD,server,&event);int clients[64];for(int i=0;i<64;i++)clients[i]=-1;signal(SIGTERM,stop);signal(SIGINT,stop);signal(SIGPIPE,SIG_IGN);while(running){struct epoll_event events[64];int n=epoll_wait(ep,events,64,500);if(n<0){if(errno==EINTR)continue;break;}for(int i=0;i<n;i++){int fd=events[i].data.fd;if(fd==server){int c=accept(server,NULL,NULL);if(c<0)continue;int slot=0;while(slot<64&&clients[slot]>=0)slot++;if(slot==64){close(c);continue;}fcntl(c,F_SETFL,fcntl(c,F_GETFL,0)|O_NONBLOCK);clients[slot]=c;event.events=EPOLLIN|EPOLLRDHUP;event.data.fd=c;epoll_ctl(ep,EPOLL_CTL_ADD,c,&event);}else{char buffer[4096];ssize_t count=recv(fd,buffer,sizeof buffer,0);if(count>0){for(int j=0;j<64;j++)if(clients[j]>=0){ssize_t sent=send(clients[j],buffer,(size_t)count,MSG_NOSIGNAL);if(sent!=count){epoll_ctl(ep,EPOLL_CTL_DEL,clients[j],NULL);close(clients[j]);clients[j]=-1;}}}else if(count==0||(errno!=EAGAIN&&errno!=EINTR)){epoll_ctl(ep,EPOLL_CTL_DEL,fd,NULL);close(fd);for(int j=0;j<64;j++)if(clients[j]==fd)clients[j]=-1;}}}}for(int j=0;j<64;j++)if(clients[j]>=0)close(clients[j]);close(ep);close(server);return 0;}
