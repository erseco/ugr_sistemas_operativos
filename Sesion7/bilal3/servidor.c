/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2014 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 *
 *	servidor.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

//Constantes
#define longnombre 50

//Manejador de senal
void mimanejador(int senial){
	int estado;
	wait(&estado);
}

//Punto de entrada al programa
int main(int argc, char* argv[]){

	//Declaracion de variables
	int fdfifo_e, fdfifo_s, fdfifo_proxy;
	char peticion[10];
	char nombrefifo_proxy[longnombre];
	char nombrefifo_e[longnombre], nombrefifo_s[longnombre];
	size_t bytesleidos;
	pid_t pid;

	if(argc!=2){
		printf("Modo de uso: %s <nombrefifo>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//Pone el manejador para la senal SIGHLD.
	signal(SIGCHLD, mimanejador);

	umask(0);

	//Creamos el archivo de bloqueo
	if( creat("bloqueo", S_IRWXU) < 0){
		perror("Error al crear el archivo de bloqueo\n");
		exit(EXIT_FAILURE);
	}	
	
	//Generar los nombres de los archivos fifo de entrada y de salida
	sprintf(nombrefifo_e, "%se", argv[1]);
	sprintf(nombrefifo_s, "%ss", argv[1]);

	//Borramos los archivos de los cauces (si existieran previamente)
	unlink(nombrefifo_e);
	unlink(nombrefifo_s);

	//Crear los cauces de entrada y de salida
	if (mkfifo(nombrefifo_e, S_IRWXU) < 0){
		perror("Error al crear el cauce de entrada\n");
		exit(EXIT_FAILURE);
	}
	if (mkfifo(nombrefifo_s, S_IRWXU) < 0){
		perror("Error al crear el cauce de salida\n");
		exit(EXIT_FAILURE);
	}
	
	// abrir los cauces de entrada y de salida
	if ((fdfifo_e = open(nombrefifo_e,O_RDWR)) < 0){
		perror("Error al abrir el fifo de entrada\n");
		exit(EXIT_FAILURE);
	}

	if ((fdfifo_s = open(nombrefifo_s,O_RDWR)) < 0){
		perror("Error al abrir el fifo de salida\n");
		exit(EXIT_FAILURE);
	}
	
	while(1) {

		bytesleidos = read(fdfifo_e, &peticion, 10);
		printf("Peticion recibida\n");

		if((pid=fork()) < 0 ) {
		
			perror("Error en el fork\n");
			exit(EXIT_FAILURE);
		
		} else if(pid == 0) {   

			//El hijo que sera el proxy 
			pid = getpid();
			sprintf (nombrefifo_proxy, "fifo.%d", pid);
			
			//El hijo crea el fifo para comunicarse con el cliente
			if(mkfifo(nombrefifo_proxy, S_IRWXU)  < 0){
				perror("Error en mkfifo al crear el cauce de entrada\n");
				exit(EXIT_FAILURE);
			}

			//Se envia el pid del proxy al cliente
			printf("Enviando el pid del proxy %d al cliene\n", pid);
			if(write(fdfifo_s, &pid, sizeof(pid)) != sizeof(pid)) {
				perror("Error al escribir el pid del proxy en el fifo de salida\n");
				exit(EXIT_FAILURE);
			}

			//Abre el fifo en modo solo lectura para recibir datos del cliente
			if ((fdfifo_proxy = open(nombrefifo_proxy, O_RDONLY)) < 0) {
				perror("Error al abrir el archivo fifo del proxy\n");
				exit(EXIT_FAILURE);
			}
			
			//Redireccionamos la entrada estándar al fifo del proxy
			dup2(fdfifo_proxy, STDIN_FILENO);
			
			//Ejecutamos el proxy que lee de la entrada estandar y almacena los datos en los archivo temporal, 
			//lee del archivo temporal e imprime en pantalla, al final libera el cerrojo de pantalla
			execlp("./proxy", "proxy", NULL);

			//Si no pasa al codigo del proxy es que ha dado error, asi que lo pintamos
			perror("Error al ejecutar el proxy");
			exit(EXIT_FAILURE);

		}
		
	}

	exit(EXIT_SUCCESS);
	
}
