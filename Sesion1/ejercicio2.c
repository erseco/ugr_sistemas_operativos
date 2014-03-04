/*  
 *  Sistemas Operativos
 *  Grado en Ingeniería Informática
 * 
 *  2013 © Copyleft - All Wrongs Reserved
 *
 *  Ernesto Serrano <erseco@correo.ugr.es>
 */
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h> //Esto es para que no de warnings en Mac
#include<string.h>

int main(int argc, char *argv[])
{

	//Declaracion de variables    
    int fd1, fd2;
	char buffer[80];
	int numbytes;
	int i = 1;

	//Comprobamos si se le ha pasado un pathname como parámetro
	if(argc<2) {

		//como no se le ha pasado ningun fichero se lee la entrada estandar
		fd1 = STDIN_FILENO;

	}else {
	
		//abrimos el archivo de lectura
		if ((fd1 = open(argv[1], O_RDONLY))<0) {
			printf("\nError %d en open",errno); 
			perror("\nError en open"); 
			exit(-1);
		}
	
	}

	//Abrimos el archivo de escritura
	if( (fd2=open("salida.txt",O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR))<0) {
		printf("\nError %d en open",errno); 
		perror("\nError en open"); 
		exit(-1);
	}
	
	//Le metemos caracteres nulos al principio para meter luego una frase
	write(fd2, &buffer, 27);
	
	//Escribimos un salto de linea
	write(fd2, "\n", 1);
	
	//Recorremos el archivo de lectura leyendo en tramos de 80 bytes
	while ((numbytes = read(fd1, &buffer, 80)) > 0) {
		
		char numerobloque[8];
		sprintf(numerobloque, "Bloque %i", i);
	
		//Escribimos la cadena Bloque
		write(fd2, &numerobloque, sizeof(numerobloque));
		
		//Escribimos un salto de linea
		write(fd2, "\n", 1);
		
		//Escribimos los 80 bytes
		write(fd2, &buffer, numbytes);
		
		//Escribimos un salto de linea
		write(fd2, "\n", 1);
      	
      	//Incrementamos el iterador
      	i++;
 	}
	
	//Nos vamos a la primera linea para introducir ahi el texto
	if(lseek(fd2,0,SEEK_SET) < 0) {
		perror("\nError en lseek");
		exit(-1);
	}
	
	//Escribimos el total de bloques (es el iterador menos 1)
	char totalbloques[27];
	sprintf(totalbloques, "El número de bloques es %i", i-1);
	
	//Escribimos la cadena Bloque
	write(fd2, &totalbloques, sizeof(totalbloques));

	//Cerramos los archivos
	close(fd2);
  	close(fd1);
  
	return 0; 
}
