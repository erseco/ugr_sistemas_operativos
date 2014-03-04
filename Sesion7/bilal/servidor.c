#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>


void proxy(int pid){
	char *nombre;
	int fd;
	sprintf(nombre, "/tmp/fifo.%d",pid);
	mkfifo(nombre,0777);
	if(fd=open(nombre,O_RDWR)<0){
	perror("Error en write\n");
	exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	execlp("./proxy", "./proxy", NULL);
	perror("fallo en execlp");
	exit(EXIT_FAILURE);

}

int main(){

int buf[1024];
int fd1, fd2, numbytes;
char * fifoe = "/tmp/fifoe";
char * fifos = "/tmp/fifos";
int pid;

umask(0);
if((mkfifo(fifoe, 0777)) == -1){
perror("\nError en mkfifo\n");
   exit(EXIT_FAILURE);
}
if((mkfifo(fifos, 0777)) == -1){
perror("\nError en mkfifo\n");
   exit(EXIT_FAILURE);
}

fd1=open(fifos,O_RDWR);
fd2=open(fifoe,O_RDWR);
numbytes=read(fd1,buf,sizeof(buf));
while(numbytes>0){
	pid=fork();
	if(pid==0){
		proxy(getpid());
	} else if (pid > 0) {
		printf("padre pid es %d\n",pid);		
		if(write(fd2,&pid,sizeof(int))<0){
			perror("Error en write\n");
			exit(EXIT_FAILURE);
			}
		} else if (pid == -1) {
			perror("fallo en fork\n");
			exit(EXIT_FAILURE);
		}

numbytes=read(fd1,buf,sizeof(buf));
}
if(numbytes==-1){
perror("\nError en read\n");
   exit(EXIT_FAILURE);
}

pid = wait(NULL);
while (pid > 0) {
  pid = wait(NULL);
	}
	/* si hay error, ignoramos si no hay más hijos a esperar. */
if (pid == -1 && errno != ECHILD) {
   perror("fallo en wait");
   exit(EXIT_FAILURE);
	}
}
