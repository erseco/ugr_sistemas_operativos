/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2013 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 *
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {

	int inicio;
	int fin;

	//Comprobamos si se le ha pasado dos enteros como parámetro
	if(argc!=3) {

		//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
		printf("Modo de uso: %s <inicio> <fin>\n\n", argv[0]);
		exit(1);

	}

	//Convertimos el argumento a int
	inicio=atoi(argv[1]);
	fin=atoi(argv[2]);

	for (int n = inicio; n < fin; n++) {

	int c;

	//NOTA: Calculo si es primo a la vieja usanza por que no acabo de hacer funcionar
	//el método rápido con la raiz cuadrada
	for (c = 2;c <= n - 1; c++) {
		if ( n % c == 0 ) 
			break; //No es primo, paramos la iteracion
	}

	if (c == n)
		printf("El numero %d es primo.\n", n);

	}

	exit(0);

}
