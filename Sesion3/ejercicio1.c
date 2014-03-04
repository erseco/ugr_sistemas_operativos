/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2013 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 *
 *	Implementa un programa en C que tenga como argumento un numero entero. Este 
 *	programa debe crear un proceso hijo que se encargara de comprobar si dicho 
 *	numero es un numero par o impar e informara al usuario con un mensaje que 
 *	se enviara por la salida estandar. A su vez, el proceso padre comprobara 
 *	si dicho numero es divisible por 4, e informara si lo es o no usando 
 *	igualmente la salida estandar.
 *	
 */

#include<sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include <unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>	//Cabeceras para poder usar el tipo booleano

bool es_par(int numero) {

	return numero % 2 == 0;

}

bool es_divisible_entre_cuatro(int numero) {

	return numero % 4 == 0;

}


int main(int argc, char *argv[]) {
	int numero;
	pid_t pid;

	//Comprobamos si se le ha pasado un pathname y unos permisos como parámetros
	if(argc!=2) {

		//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
		printf("Modo de uso: %s <entero>\n\n", argv[0]);
		exit(1);

	}

	//Convertimos el argumento a int
	numero=atoi(argv[1]);


	if( (pid=fork())<0) {
		perror("\nError en el fork");
		exit(-1);
	} else if(pid==0) {  //proceso hijo ejecutando el programa

		if (es_par(numero)) {
			printf("El numero %d es par\n",numero);

		}else
			printf("El numero %d no es par\n",numero);

	} else { //proceso padre ejecutando el programa

		if (es_divisible_entre_cuatro(numero)) {
			printf("El numero %d es divisible por 4\n",numero);
		}else
			printf("El numero %d no es divisible por 4\n",numero);

	}
			
	exit(0);

}
