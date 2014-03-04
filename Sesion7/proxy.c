/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2014 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

//Constantes
#define tamano 1024
#define longnombre 50

void bloqueodesbloqueo(int dbloqueo, int orden) {
	
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

//Punto de entrada al programa
int main(int argc, char **argv){


	//Con esto redirigimos STDERR_FILENO --> STDOUT_FILENO 
	dup2(STDOUT_FILENO, STDERR_FILENO);

	int buf[tamano];
	int numbytes;

	//Creamos un fichero temporal
	FILE *tmpFile = tmpfile();
	int orden=0;

	//Leo los datos de la entrada estándar
	numbytes=read(STDIN_FILENO,buf,sizeof(buf));
	

	//Comprobamos si hay algo en el FIFO
	if(numbytes==-1){
		perror("Error en la lectura en del proxy.\n");
		exit(EXIT_FAILURE);
	}
	
	//Si no da error, empezamos a leer
	while(numbytes>0){

		if(fwrite(buf,sizeof(char),numbytes,tmpFile)==-1){

			perror("Error en fwrite la escritura del proxy1\n");
			exit(EXIT_FAILURE);

		}

		numbytes=read(STDIN_FILENO,buf,sizeof(buf));
		if(numbytes==-1){

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
			
			//comento el desbloqueo un momento
			//bloqueodesbloqueo(STDOUT_FILENO, F_WRLCK);
			if(write(STDOUT_FILENO,buf,numbytes)==-1){
				perror("Error en la escritura del proxy1\n");
				exit(EXIT_FAILURE);
			}

			numbytes=fread(buf,sizeof(char),sizeof(buf),tmpFile);
			if(numbytes ==- 1) {
				perror("Error en la fread lectura en del proxy2\n");
				exit(EXIT_FAILURE);
			}
			orden++;
			
		}

		if(numbytes==0){
			//comento el desbloqueo un momento
			//bloqueodesbloqueo(STDOUT_FILENO, F_UNLCK);
			fclose(tmpFile);
			exit(EXIT_FAILURE);
		}
				
	}//if(numbytes==0)

}

