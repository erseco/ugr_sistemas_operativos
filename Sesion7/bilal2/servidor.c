#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define size 1024
#define namelen 50
#define permissions 0777
#define bloquear F_WRLCK
#define desbloquear F_UNLCK

char errormsg[100];
int dbloqueo;


// Funciones para el Proxy
void cerrojo(int dbloqueo, int orden){
	struct flock cerrojo;

	// Inicializacion del cerrojo
	cerrojo.l_type = orden;
	cerrojo.l_whence = SEEK_SET;
	cerrojo.l_start = 0;
	cerrojo.l_len = 0;

	// Dormir el proceso en caso de estar bloqueado por otro
	if (fcntl(dbloqueo, F_SETLKW, &cerrojo) == -1){
		sprintf(errormsg, "Proxy %d: Problema al abrir el fifo del proxy", getpid());
		perror(errormsg);
		exit(-1);
	}
}

void proxy(){
	char buffer[size], proxyfifo[namelen];
	int dproxyfifo, dtmpfile, leido;

//	// Creacion del FIFO del proxy		// Se ha creado finalmente en el Servidor pues de ser al revés había veces en las que el cliente se adelantaba y daba un error al no existir el fichero
	sprintf(proxyfifo,"fifo.%d",getpid());
//	mkfifo(proxyfifo, permissions);

	// Apertura del FIFO creado
	if ((dproxyfifo = open(proxyfifo, O_RDWR)) == -1){
		sprintf(errormsg,"Proxy %d: Problema al abrir el fifo del proxy", getpid());
		perror(errormsg);
                exit(-1);
        }

	// Creacion del fichero temporal
	if ((dtmpfile = fileno(tmpfile())) == -1){
		sprintf(errormsg,"Proxy %d: Problema al crear/abrir fichero temporal", getpid());
		unlink(proxyfifo);
		perror(errormsg);
		exit(-1);
	}

	// Lectura de los datos del FIFO y escritura en fichero temporal
	do{
		// lectura del fifo
		if ((leido = read (dproxyfifo, &buffer, size)) == -1 ) {
			sprintf(errormsg,"Proxy %d: Problema al leer datos del fifo", getpid());
			unlink(proxyfifo);
			perror(errormsg);
			exit(-1);
		}

		// escritura en temporal
		if ((write(dtmpfile, &buffer, size)) == -1){
			sprintf(errormsg,"Proxy %d: Problema al escribir en fichero temporal", getpid());
			unlink(proxyfifo);
			perror(errormsg);
			exit(-1);
		}
	}while(leido == size);


	// Una vez terminada la transferencia se procede a eliminar el fifo y a mostrar por pantalla

	// Eliminacion del fifo del proxy
	unlink(proxyfifo);

	// Antes de leer/mostrar se debe reposicionar el descriptor

	if ((lseek(dtmpfile, 0, SEEK_SET)) == -1 ) {
		sprintf(errormsg,"Proxy %d: Problema al reubicar el descriptor", getpid());
		perror(errormsg);
		exit(-1);
	}

	// A continuacion se procede a leer/mostrar por pantalla
	leido = size;
	do{
		if ((leido = read(dtmpfile, &buffer, size)) == -1 ) {
			sprintf(errormsg,"Proxy %d: Problema al leer del fichero temporal", getpid());
			perror(errormsg);
			exit(-1);
		}

		cerrojo(dbloqueo, bloquear);
		printf("%s", buffer);
		cerrojo(dbloqueo, desbloquear);
		
	}while(leido == size);

}

// Funciones del Servidor
void crearComunicadores(char *come, char *coms){
	mkfifo(come, permissions);
	mkfifo(coms, permissions);
}

void eliminarComunicadores(char *come, char *coms){
	unlink(come);
	unlink(coms);
}

int crearBloqueo(){
	return fileno(tmpfile());
}

int main (int argc, char **argv) {
	
	char comunicadore[namelen], comunicadors[namelen];
	int pid, dcomunicadore, dcomunicadors, pidcliente;

	sprintf(comunicadore, "%se", argv[0]);
        sprintf(comunicadors, "%ss", argv[0]);

/*
	////////////////////////	Area anyadida para cerrar el servidor mediante <ctrl><c>
	void salir(){
		eliminarComunicadores(comunicadore, comunicadors);
		exit(0);
	}
	////////////////////////
	struct sigaction sa;
	sa.sa_handler = salir;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGINT, &sa, NULL) == -1)
		printf("error en el manejador");
	///////////////////////
*/


	// Creacion de comunicadores entre cliente-servidor y del fichero de bloqueo
	crearComunicadores(comunicadore, comunicadors);
	dbloqueo=crearBloqueo();

	if ((dcomunicadore = open(comunicadore, O_RDWR)) == -1){
		perror("Servidor: Problema al abrir el comunicador de entrada");
		eliminarComunicadores(comunicadore, comunicadors);
		exit(-1);
	}
	if ((dcomunicadors = open(comunicadors, O_WRONLY)) == -1){
		perror("Servidor: Problema al abrir el comunicador de salida");
		eliminarComunicadores(comunicadore, comunicadors);
		exit(-1);
	}
	
	do{
		if ((read(dcomunicadore, &pidcliente, sizeof(int))) == -1){
			perror("Servidor: Problema al leer pid de cliente");
			eliminarComunicadores(comunicadore, comunicadors);
			exit(-1);
		}

		if ((pid = fork()) == -1){
			perror("Servidor: Problema al crear proceso proxy");
			eliminarComunicadores(comunicadore, comunicadors);
			exit(-1);
		}

		if (pid == 0){	// En caso de ser el proxy
			proxy();
		}
		else{	// En caso de ser el Servidor
			char proxyfifo[namelen];

			sprintf(proxyfifo,"fifo.%d",pid);
			mkfifo(proxyfifo, permissions);
			if ((write(dcomunicadors, &pid, sizeof(int))) == -1){
				perror("Servidor: Problema al escribir en comunicador de salida");
				eliminarComunicadores(comunicadore, comunicadors);
				unlink(proxyfifo);
				exit(-1);
			}
		}
	}while(pid != 0);
}
