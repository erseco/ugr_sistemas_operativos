#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

void bloqueodesbloqueo (int dbloqueo, int orden) {
	struct flock cerrojo;
	// Inicializamos el cerrojo para bloquear todo el archivo
	cerrojo.l_type= orden;
	cerrojo.l_whence= SEEK_SET;
	cerrojo.l_start= 0;
	cerrojo.l_len = 0;
	//Si vamos a bloquearlo y ya lo esta, entonces el proceso duerme
	if (fcntl(dbloqueo, F_SETLKW, &cerrojo) == -1) {
	perror ("Proxy: problemas al bloquear para impresion");
	exit(1);
		}
}

int main(){
	int buf[1024];
	int numbytes;
	FILE *tmpFile = tmpfile();
	int orden=0;

	numbytes=read(STDIN_FILENO,buf,sizeof(buf));
	
	if(numbytes==-1){
			perror("Error en la lectura en del proxy.\n");
			exit(EXIT_FAILURE);
		}
	while(numbytes>0){
		if(fwrite(buf,sizeof(char),numbytes,tmpFile)==-1){
			perror("Error en fwrite la escritura del proxy1\n");
			exit(EXIT_FAILURE);
		}
	if(numbytes=read(STDIN_FILENO,buf,sizeof(buf))==-1){
			perror("Error en la lectura en del proxy2\n");
			exit(EXIT_FAILURE);
		}
	}
	
	if(numbytes==0){
			

			numbytes=fread(buf,sizeof(char),sizeof(buf),tmpFile);

	if(numbytes==-1){
			perror("Error en fread la lectura en del proxy.\n");
			exit(EXIT_FAILURE);
		}

	while(numbytes>0){
		bloqueodesbloqueo (STDOUT_FILENO,F_WRLCK);
		if(write(STDOUT_FILENO,buf,numbytes)==-1){
			perror("Error en la escritura del proxy1\n");
			exit(EXIT_FAILURE);
			}
	if(numbytes=fread(buf,sizeof(char),sizeof(buf),tmpFile)==-1){
			perror("Error en la fread lectura en del proxy2\n");
			exit(EXIT_FAILURE);
			}
		orden++;
		}
	if(numbytes==0){
			bloqueodesbloqueo (STDOUT_FILENO,F_UNLCK);
			fclose(tmpFile);
			exit(EXIT_FAILURE);
		}
				
	}//if(numbytes==0)

}
