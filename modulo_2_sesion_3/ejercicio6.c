/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2013 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 *
 *	Escribe un programa que acepte como argumentos el nombre de un programa, 
 *	sus argumentos si los tiene, y opcionalmente la cadena “bg”. Nuesto 
 *	programa deberá ejecutar el programa pasado como primer argumento en 
 *	foreground si no se especifica la cadena “bg” y en background en caso 
 *	contrario. Si el programa tiene argumentos hay que ejecutarlo con estos.
 *	
 */
#include <sys/types.h>
#include <unistd.h>		
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>	//Cabeceras para poder usar el tipo booleano
#include <string.h>	//Cabeceras para poder usar cadenas 

void execute_command(char *str_command, char *str_params) {


    if( (execl(str_command, str_params, str_params,NULL)<0)) {

	//if( (execlp(str_command,str_params)<0)) {
		perror("Error en el execl\n");
		exit(-1);
	}

}

int main(int argc, char *argv[]) {

	pid_t pid;
	bool en_background = false;
	int n_params = argc;
	const char *str_bg = "bg";
	char *str_command;
	char str_params[200];


	//Comprobamos si se le ha pasado un pathname y unos permisos como parámetros
	if(argc<2) {
		//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
		printf("Modo de uso: %s <programa> [opciones] [bg]\n\n", argv[0]);
		exit(1);
	} else {
		//Comprobamos si el ultimo parametro pasado es bg
		if (strcmp(argv[argc-1], str_bg) == 0) {
			en_background = true;
			//Habrá un parametro menos
			n_params--;
		}

		//Extraemos el comando
		str_command = argv[1];

		//Recorremos los parametros pasados a partir del 2
		for (int i = 2; i < n_params; i++) { 
			strcat (str_params, argv[i]);
			strcat (str_params, " ");
		}


		if (en_background) {
			printf("%s se ejecutara en background\n", str_command);
		    if  ((pid =fork()) < 0)
		        return (-1) ;
		    else if (pid != 0)
		       exit(0) ; //El proceso padre sale

			execute_command(str_command,str_params);

		} else {
			printf("%s se ejecutara en foreground\n", str_command);;
			execute_command(str_command,str_params);
		}





	}


	exit(0);

}