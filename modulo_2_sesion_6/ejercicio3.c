/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2013 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 *
 *	Construir un programa que verifique que, efectivamente, el kernel comprueba
 *	que puede darse una situacion de interbloqueo en el bloqueo de archivos.
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[]) {

	//Declaracion de variables
	struct flock cerrojo;
	int fd, i;

	if(argc != 2) {

		//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
		printf("Modo de uso: %s <programa> <archivo>\n\n", argv[0]);
		exit(1);

	} else {

		//Extraemos el nombre del archivo a usar (por comodidad)
		char *str_file = argv[1];

		//Abrimos el archivo
		if ((fd=open(str_file, O_RDWR)) == -1 ){
			perror("Fallo al abrir el archivo");
			return 0;
		}

		cerrojo.l_type=F_WRLCK;
		cerrojo.l_whence=SEEK_SET;
		cerrojo.l_start=0;
		cerrojo.l_len=0; //Bloquearemos el archivo entero
		
		//Intentamos un bloqueo de escritura del archivo
		printf ("Intentando bloquear %s\n", str_file);
		if (fcntl (fd, F_SETLKW, &cerrojo) == EDEADLK) {
			
			//Si el cerrojo falla, pintamos un mensaje
			printf ("%s ha dado un EDEADLK\n", str_file);

		} //Mientras el bloqueo no tenga exito 

		//Ahora el bloqueo tiene exito y podemos procesar el archivo
		printf ("Procesando el archivo %s\n", str_file);

		//Hacemos un bucle con sleep para que de tiempo a lanzar otra vez el programa
		for (i = 0; i < 10; i++) {
			sleep(1);
		}
		//Una vez finalizado el trabajo, desbloqueamos el archivo
		cerrojo.l_type=F_UNLCK;
		cerrojo.l_whence=SEEK_SET;
		cerrojo.l_start=0;
		cerrojo.l_len=0;
		if (fcntl (fd, F_SETLKW, &cerrojo) == -1) {
			perror ("Error al desbloquear el archivo");
		}
		
		return 0;
	}
}

