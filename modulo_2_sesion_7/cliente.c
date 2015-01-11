#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>


void peticion(const char * argv1, const char * argv2){

int fd,fd1,fd2, numbytes;
int pid;
char nombre[15];
char buf[]="peticion";
char chorada[]="Si tienes 1 manzana y yo tengo otra manzana...y las intercambiamos, ambos seguiremos teniendo 1 manzana.Pero...si tu tienes 1 idea y yo tengo otra idea...y las intercambiamos, ambos tendremos 2 ideas.\n";

if((fd1=open(argv1,O_RDONLY))<0){
	perror("Error en open_1\n");
	exit(EXIT_FAILURE);
	}
if((fd2=open(argv2,O_WRONLY))<0){
	perror("Error en open_2\n");
	exit(EXIT_FAILURE);
	}

if(write(fd2,buf,sizeof(buf))<0){
	perror("Error en write_1\n");
	exit(EXIT_FAILURE);
	}
printf("\nHemos abierto \n");


if(read(fd1,&pid,sizeof(pid))<0){
	perror("Error en read_1\n");
	exit(EXIT_FAILURE);
	}

printf("\nHemos leido \n");


printf("\n el numero de pid es %d\n",pid);
sprintf(nombre, "/tmp/fifo.%d", pid);

if(fd=open(nombre,O_RDWR)<0){
perror("Error en open_3\n");
exit(EXIT_FAILURE);
}

if(numbytes=write(fd,chorada,sizeof(chorada))<0){
	perror("Error en write_2\n");
	exit(EXIT_FAILURE);
	}
close(fd2);
close(fd1);
close(fd);

}

int main(int argc, char* argv[]){



if(argc!=3){
printf("\nError. Argumentos clientes <nombre_fifo_escritura> <número_clientes>\n");
exit(-1);
}
char fifoe[6];
char fifos[6];
sprintf(fifoe,"/tmp/fifo%s",argv[1]);
sprintf(fifos,"/tmp/fifo%s",argv[2]);
int NUM_HIJOS=atoi(argv[3]);

int pid, i;

	for (i=0; i<NUM_HIJOS; i++) {
		pid = fork();
		if (pid == 0) {
			printf("\nEstoy aqui1\n");
			peticion(fifoe,fifos);
			exit(EXIT_SUCCESS);
			}
		else if (pid == -1) {
			perror("fallo en fork");
			exit(EXIT_FAILURE);
		}
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
