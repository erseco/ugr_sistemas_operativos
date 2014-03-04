/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2013 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 *
 */

#include <sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include <unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>	//Cabeceras para poder usar el tipo booleano

#define ARCHIVO_FIFO "comunicacion"


int main(int argc, char *argv[]) {
	int inicio;
	int fin;
	int mitad;

	int numBytes;
	char buffer[80];

	char params[2];

	//Comprobamos si se le ha pasado dos enteros como parámetro
	if(argc!=3) {

		//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
		printf("Modo de uso: %s <inicio> <fin>\n\n", argv[0]);
		exit(1);

	}

	//Convertimos el argumento a int
	inicio=atoi(argv[1]);
	fin=atoi(argv[2]);


	mitad = inicio + ((fin - inicio) / 2);


	int fd1[2];
	int fd2[2];
	pid_t PID;

	pipe(fd1); // Llamada al sistema para crear un pipe
	pipe(fd2); // Llamada al sistema para crear un pipe

	if ( (PID= fork())<0) {
		perror("Error al hacer fork");
		exit(1);
	}
	if (PID == 0) {
		//Cierre del descriptor de lectura en el proceso hijo
		close(fd1[0]);
		
		params[0] = inicio;
		params[1] = mitad;

		// Enviar el mensaje a través del cauce usando el descriptor de escritura

		printf("paso 1\n");
		dup2(fd1[1],STDOUT_FILENO);
		execlp("esclavo","esclavo", params, NULL);


		
	} else { // Estoy en el proceso padre porque PID != 0

		if ( (PID= fork())<0) {
			perror("Error al hacer fork");
			exit(1);
		}

		if (PID == 0) {

			close(fd2[0]);

			params[0] = mitad + 1;
			params[1] = fin;
 
			printf("paso 2\n");
			dup2(fd2[1],STDOUT_FILENO);
			execlp("esclavo","esclavo", params,NULL);

			exit(0);

		} else { // Estoy en el proceso padre porque PID != 0

		
			//Cerrar el descriptor de escritura en el proceso padre
			close(fd1[1]);
		
			//Leer datos desde el cauce.
			numBytes= read(fd1[0],buffer,sizeof(buffer));
			

			dup2(fd1[0],STDIN_FILENO);

			printf("paso 3\n");
			//Recorremos el archivo de lectura leyendo en tramos de 80 bytes
			while ((numBytes = read(fd1[0], &buffer, 4)) > 0) {
				
				printf("Valor: %d",1);

		 	}

		 }

	}
	
	exit(0);

}
