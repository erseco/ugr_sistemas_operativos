/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2013 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 *
 *	Implementa un programa que admita tres argumentos. El primer argumento sera
 *	una orden de Linux; el segundo, uno de los siguientes caracteres “<” o “>”, 
 *	y el tercero el nombre de un archivo (que puede existir o no). El programa 
 *	ejecutara la orden que se especifica como argumento primero e implementara
 *	la redireccion especificada por el segundo argumento hacia el archivo 
 *	indicado en el tercer argumento. Por ejemplo, si deseamos redireccionar la 
 *	salida estandar de sort a un archivo temporal, ejecutariamos (el caracter 
 *	de redireccion > lo ponemos entrecomillado para que no lo interprete el 
 *	shell y se coja como argumento del programa):
 *	
 *      $ ./mi_programa sort “>” temporal
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>	//Cabeceras para poder usar el tipo booleano

int main(int argc, char *argv[]) {

	//Comprobamos si se le ha pasado un pathname y unos permisos como parámetros
	if(argc != 4) {
		//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
		printf("Modo de uso: %s <programa> <simbolo> <archivo>\n\n", argv[0]);
		exit(1);
	} else {

		//Declaracion de variables
		char *str_command;
		char *str_file;
		int fd;

		//Extraemos el comando
		str_command = argv[1];
		str_file = argv[3];

		//Comprobamos el segundo parametro, tiene que ser < o >
		if (strcmp(argv[2], "<") == 0) {

			//Redireccion de entrada
			fd = open (str_file, O_RDONLY);
			close(STDIN_FILENO);
			if (fcntl(fd, F_DUPFD, STDIN_FILENO) == -1 ) perror ("fcntl falló");

		} else if (strcmp(argv[2], ">") == 0) {

			//Redireccion de salida
			fd = open (str_file, O_CREAT|O_WRONLY);
			close (STDOUT_FILENO);
			if (fcntl(fd, F_DUPFD, STDOUT_FILENO) == -1 ) perror ("fcntl falló");


		} else {

			printf("Debe pasarse \"<\" o \">\" con las comillas %s\n\n", argv[2]);

			//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
			printf("Modo de uso: %s <programa> [opciones] [bg]\n\n", argv[0]);
			exit(1);

		}

		//Ejecutamos el comando
	    if( (execlp(str_command, "", NULL) < 0)) {

			perror("Error en el execlp\n");
			exit(-1);
		}

		//Cerramos el fichero
		close(fd);

	}

}

