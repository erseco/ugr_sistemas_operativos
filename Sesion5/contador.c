/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2013 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 *
 *	Escribe un programa en C llamado contador, tal que cada vez que reciba una
 *	señal que se pueda manejar, muestre por pantalla la señal y el número de 
 *	veces que se ha recibido ese tipo de señal, y un mensaje inicial indicando
 *	las señales que no puede manejar. 
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>	//Cabeceras para poder usar el tipo booleano

//Vector donde almacenar el contador de señales
const static int SIGNAL_SIZE = 10;
static int received_signals[SIGNAL_SIZE];


static void sig_handler (int signum) {

 	if (signum >= 1 && signum <= SIGNAL_SIZE) {

 		//Incrementamos el elemento del vector de señales recibidas
 		received_signals[signum]++;

 		printf("La señal %d se ha recibido %d veces\n", signum, received_signals[signum]);

 	} else {

 		printf("No puedo manejar la señal %d\n", signum);

	}
}

int main(int argc, char *argv[]) {
	
	//Declaración de variables
	struct sigaction sig_action;

	//Asociamos la funcion definida como manejador
	sig_action.sa_handler = sig_handler; 

	//inicializar un conjunto con todas las señales
	sigfillset(&sig_action.sa_mask); 
	

	sig_action.sa_flags = 0;
	
	for (int i = 1; i < SIGNAL_SIZE - 1; i++) {

		if (sigaction(i, &sig_action, NULL) < 0) {
			perror("Error al intentar establecer el manejador de señal\n");
			exit(-1);
		}
	}

	//Dejamos en ejecución un bucle infinito, asi el programa no termina
	while(true);

}
