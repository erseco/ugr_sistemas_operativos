/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2013 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 *
 *	Escribe un programa que suspenda la ejecución del proceso actual hasta que
 *	se reciba la señal SIGUSR1.
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	//definimos una nueva mascara
	sigset_t new_mask;

	//inicializar un conjunto con todas las señales
	sigfillset(&new_mask);

	//eliminamos la señal SIGUSR1
	sigdelset(&new_mask, SIGUSR1);

	//esperar solamente a la señal SIGUSR1
	sigsuspend(&new_mask);

	return 0;

	
}

