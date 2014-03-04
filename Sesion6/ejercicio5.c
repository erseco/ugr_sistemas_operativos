/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2013 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 *
 *	Escribir un programa, similar a la orden cp, que utilice para su implementacion 
 *	la llamada al sistema mmap() y una funcion de C que nos permite copiar memoria, 
 *	como por ejemplo memcpy(). Para conocer el tamaño del archivo origen podemos 
 *	utilizar stat() y para establecer el tamaño del archivo destino se puede usar 
 *	ftruncate().
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>	//Cabeceras para poder usar el tipo booleano
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

	//Comprobamos si se le ha pasado un pathname y unos permisos como parámetros
	if(argc != 3) {
		//Si no se le han pasado los parámetros correctos muestra un mensaje de ayuda
		printf("Modo de uso: %s <programa> <origen> <destino>\n\n", argv[0]);
		exit(1);

	} else {

		//Declaracion de variables
        struct stat sb;
		char *str_orig = argv[1];
		char *str_dest = argv[2];

		
		int fd_orig, fd_dest;
		char *mem_orig, *mem_dest;
		int filesize;


		//Abrimos el fichero de origen
		fd_orig = open(str_orig, O_RDONLY);
		if (fd_orig == -1) {
			perror("Fallo al abrir el archivo de origen\n");
			exit(2);
		}

		//Obtenemos su stat, para comprobar si es regular y obtener su tamaño
        if (fstat (fd_orig, &sb) == -1) {
                printf("Error al hacer stat en el fichero de origen\n");
                return 1;
        }

        if (!S_ISREG (sb.st_mode)) {
                printf ("El fichero de origen no es un archivo regular\n");
                return 1;
        }

        //Guardamos el tamaño en una variable (por comodidad)
        filesize = sb.st_size;


        //Creamos el archivo de destino
        umask(0);
		fd_dest = open(str_dest, O_RDWR|O_CREAT|O_EXCL, S_IRWXU);
		if (fd_dest == -1) {
			perror("Fallo al crear el archivo de salida");
			exit(2);
		}  

		//Asignamos el espacion en el fichero de destino
		ftruncate(fd_dest, filesize);

		//Creamos el mapa de memoria del fichero de origen
		mem_orig = (char *) mmap(0, filesize, PROT_READ, MAP_SHARED, fd_orig, 0);
		if(mem_orig == MAP_FAILED) {

			perror("Fallo mapeando el archivo de entrada");
			exit(2);

		}

		//Creamos el mapa de memoria del fichero de destino
		mem_dest = (char *) mmap(0, filesize, PROT_WRITE, MAP_SHARED, fd_dest, 0);
		if(mem_dest == MAP_FAILED) {
			
			perror("Fallo mapeando el archivo de salida");
			exit(2);

		}
		
		//Copiamos un mapa de memoria en otro
		memcpy(mem_dest, mem_orig, filesize);


 		//Liberamos los mapas de memoria
		munmap(mem_orig, filesize);
		munmap(mem_dest, filesize);

 		//Cerramos los descriptores de fichero
		close(fd_orig);
		close(fd_dest);

		//Terminamos la ejecución del programa
		return 0;


	}

}

