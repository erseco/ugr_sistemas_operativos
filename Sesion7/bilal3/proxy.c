/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2014 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 *
 *	proxy.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <fcntl.h>

//Constantes
#define tamano 1024

void bloqueodesbloqueo(int dbloqueo, int orden) {

	struct flock cerrojo;
	
	//Inicializamos el cerrojo para bloquear todo el archivo
	cerrojo.l_type= orden;
	cerrojo.l_whence= SEEK_SET;
	cerrojo.l_start= 0;
	cerrojo.l_len = 0;

	//Si vamos a bloquearlo y ya lo esta, entonces el proceso duerme
	if (fcntl(dbloqueo, F_SETLKW, &cerrojo) == -1) {
		perror ("Proxy: problemas al bloquear para impresion");
		exit(EXIT_FAILURE);
	}
}

//Punto de entrada al programa
int main(int argc, char* argv[]){

	//Declaracion de variables
	char bufer[tamano];
	FILE* fdtmp;
	size_t bytesleidos;
	char nombrefifo[50];
	int fdbloqueo;
	
	if ((fdbloqueo = open("bloqueo", O_RDWR)) < 0){
		perror("Error al abrir el archivo de bloqueo\n");
		exit(EXIT_FAILURE);
	}
	
	//Creacion del archivo temporal
	if ((fdtmp = tmpfile()) == NULL){
		perror("Error en tmpfile: ");
		exit(EXIT_FAILURE);
	}
	
	//Leer desde la entrada estandar y almacenar los datos leidos en el archivo temporal antes creado
	while ((bytesleidos=read(STDIN_FILENO, &bufer, tamano)) > 0){
		if(fwrite(&bufer, 1, bytesleidos, fdtmp) != bytesleidos){
			perror("Error en fwrite\n");
			exit(EXIT_FAILURE);
		}
	}

	//Volvemos al inicio del descriptor del archivo
	rewind(fdtmp);	
	
	//Obtener cerrojo para imprimir en pantalla
	bloqueodesbloqueo(fdbloqueo, F_WRLCK);
	while((bytesleidos = fread(&bufer, 1, tamano,fdtmp)) > 0){
		printf("%s", bufer);
	}

	//Desbloqueamos
	bloqueodesbloqueo(fdbloqueo, F_UNLCK);
	fclose(fdtmp);

	//La eliminacion del archivo temporal es automatica 
	
	//Eliminamos el archivo fifo antes creado
	sprintf(nombrefifo, "fifo.%d", getpid());
	unlink(nombrefifo);

	exit(EXIT_SUCCESS);

}
