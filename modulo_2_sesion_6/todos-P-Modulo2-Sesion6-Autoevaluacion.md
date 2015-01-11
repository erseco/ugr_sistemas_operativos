![](Pictures/100002010000041A00000042C7CB0DFF.png)

Sistemas Operativos

Formulario de auto-evaluación

Modulo 2. Sesión 6. Control de archivos y archivos proyectados en
memoria

\

Nombre y apellidos:

Ernesto Serrano Collado

\

​a) Cuestionario de actitud frente al trabajo.

El tiempo que he dedicado a la preparación de la sesión antes de asistir
al laboratorio ha sido de 60 minutos.

​1. He resuelto todas las dudas que tenía antes de iniciar la sesión de
prácticas: Si (si/no). En caso de haber contestado “no”, indica los
motivos por los que no las has resuelto:

\

\

\

\

​2. Tengo que trabajar algo más los conceptos sobre:

\

\

\

​3. Comentarios y sugerencias:

\

\

\

\

\

​b) Cuestionario de conocimientos adquiridos.

Mi solución a la ejercicio 1 ha sido:

/\*

 \* Sistemas Operativos

 \* Grado en Ingeniería Informática

 \*

 \* 2013 © Copyleft - All Wrongs Reserved

 \*

 \* Ernesto Serrano \<erseco@correo.ugr.es\>

 \*

 \* Implementa un programa que admita tres argumentos. El primer
argumento sera

 \* una orden de Linux; el segundo, uno de los siguientes caracteres
“\<” o “\>”,

 \* y el tercero el nombre de un archivo (que puede existir o no). El
programa

 \* ejecutara la orden que se especifica como argumento primero e
implementara

 \* la redireccion especificada por el segundo argumento hacia el
archivo

 \* indicado en el tercer argumento. Por ejemplo, si deseamos
redireccionar la

 \* salida estandar de sort a un archivo temporal, ejecutariamos (el
caracter

 \* de redireccion \> lo ponemos entrecomillado para que no lo
interprete el

 \* shell y se coja como argumento del programa):

 \*

 \* \$ ./mi\_programa sort “\>” temporal

 \*/

\

\#include \<stdio.h\>

\#include \<signal.h\>

\#include \<unistd.h\>

\#include \<stdlib.h\>

\#include \<string.h\>

\#include \<fcntl.h\>

\

int main(int argc, char \*argv[]) {

\

//Comprobamos si se le ha pasado un pathname y unos permisos como
parámetros

if(argc\<3) {

//Si no se le han pasado los parámetros correctos muestra un mensaje de
ayuda

printf("Modo de uso: %s \<programa\> \<simbolo\> \<archivo\>\\n\\n",
argv[0]);

exit(1);

} else {

\

//Declaracion de variables

char \*str\_command;

char \*str\_file;

int fd;

\

//Extraemos el comando

str\_command = argv[1];

str\_file = argv[3];

\

//Comprobamos el segundo parametro, tiene que ser \< o \>

if (strcmp(argv[2], "\<") == 0) {

\

//Redireccion de entrada

fd = open (str\_file, O\_RDONLY);

close(STDIN\_FILENO);

if (fcntl(fd, F\_DUPFD, STDIN\_FILENO) == -1 ) perror ("fcntl falló");

\

} else if (strcmp(argv[2], "\>") == 0) {

\

//Redireccion de salida

fd = open (str\_file, O\_CREAT|O\_WRONLY);

close (STDOUT\_FILENO);

if (fcntl(fd, F\_DUPFD, STDOUT\_FILENO) == -1 ) perror ("fcntl falló");

\

\

} else {

\

printf("Debe pasarse \\"\<\\" o \\"\>\\" con las comillas %s\\n\\n",
argv[2]);

\

//Si no se le han pasado los parámetros correctos muestra un mensaje de
ayuda

printf("Modo de uso: %s \<programa\> [opciones] [bg]\\n\\n", argv[0]);

exit(1);

\

}

\

//Ejecutamos el comando

if( (execlp(str\_command, "", NULL) \< 0)) {

\

perror("Error en el execlp\\n");

exit(-1);

}

\

//Cerramos el fichero

close(fd);

\

}

\

}

\

\

Mi solución a la ejercicio 3 ha sido:

/\*

 \* Sistemas Operativos

 \* Grado en Ingeniería Informática

 \*

 \* 2013 © Copyleft - All Wrongs Reserved

 \*

 \* Ernesto Serrano \<erseco@correo.ugr.es\>

 \*

 \* Construir un programa que verifique que, efectivamente, el kernel
comprueba

 \* que puede darse una situacion de interbloqueo en el bloqueo de
archivos.

 \*/

\

\#include \<stdio.h\>

\#include \<signal.h\>

\#include \<unistd.h\>

\#include \<stdlib.h\>

\#include \<string.h\>

\#include \<fcntl.h\>

\#include \<errno.h\>

int main(int argc, char \*argv[]) {

\

//Declaracion de variables

struct flock cerrojo;

int fd, i;

\

if(argc != 2) {

\

//Si no se le han pasado los parámetros correctos muestra un mensaje de
ayuda

printf("Modo de uso: %s \<programa\> \<archivo\>\\n\\n", argv[0]);

exit(1);

\

} else {

\

//Extraemos el nombre del archivo a usar (por comodidad)

char \*str\_file = argv[1];

\

//Abrimos el archivo

if ((fd=open(str\_file, O\_RDWR)) == -1 ){

perror("Fallo al abrir el archivo");

return 0;

}

\

cerrojo.l\_type=F\_WRLCK;

cerrojo.l\_whence=SEEK\_SET;

cerrojo.l\_start=0;

cerrojo.l\_len=0; //Bloquearemos el archivo entero

//Intentamos un bloqueo de escritura del archivo

printf ("Intentando bloquear %s\\n", str\_file);

if (fcntl (fd, F\_SETLKW, &cerrojo) == EDEADLK) {

//Si el cerrojo falla, pintamos un mensaje

printf ("%s ha dado un EDEADLK\\n", str\_file);

\

} //Mientras el bloqueo no tenga exito

\

//Ahora el bloqueo tiene exito y podemos procesar el archivo

printf ("Procesando el archivo %s\\n", str\_file);

\

//Hacemos un bucle con sleep para que de tiempo a lanzar otra vez el
programa

for (i = 0; i \< 10; i++) {

sleep(1);

}

//Una vez finalizado el trabajo, desbloqueamos el archivo

cerrojo.l\_type=F\_UNLCK;

cerrojo.l\_whence=SEEK\_SET;

cerrojo.l\_start=0;

cerrojo.l\_len=0;

if (fcntl (fd, F\_SETLKW, &cerrojo) == -1) {

perror ("Error al desbloquear el archivo");

}

return 0;

}

}

\

Mi solución a la ejercicio 5 ha sido:

\

/\*

 \* Sistemas Operativos

 \* Grado en Ingeniería Informática

 \*

 \* 2013 © Copyleft - All Wrongs Reserved

 \*

 \* Ernesto Serrano \<erseco@correo.ugr.es\>

 \*

 \* Escribir un programa, similar a la orden cp, que utilice para su
implementacion

 \* la llamada al sistema mmap() y una funcion de C que nos permite
copiar memoria,

 \* como por ejemplo memcpy(). Para conocer el tamaño del archivo origen
podemos

 \* utilizar stat() y para establecer el tamaño del archivo destino se
puede usar

 \* ftruncate().

 \*/

\

\#include \<stdio.h\>

\#include \<signal.h\>

\#include \<unistd.h\>

\#include \<stdlib.h\>

\#include \<string.h\>

\#include \<fcntl.h\>

\#include \<sys/types.h\>

\#include \<sys/stat.h\>

\#include \<fcntl.h\>

\

int main(int argc, char \*argv[]) {

\

//Comprobamos si se le ha pasado un pathname y unos permisos como
parámetros

if(argc != 3) {

//Si no se le han pasado los parámetros correctos muestra un mensaje de
ayuda

printf("Modo de uso: %s \<programa\> \<origen\> \<destino\>\\n\\n",
argv[0]);

exit(1);

\

} else {

\

//Declaracion de variables

struct stat sb;

char \*str\_orig = argv[1];

char \*str\_dest = argv[2];

\

int fd\_orig, fd\_dest;

char \*mem\_orig, \*mem\_dest;

int filesize;

\

\

//Abrimos el fichero de origen

fd\_orig = open(str\_orig, O\_RDONLY);

if (fd\_orig == -1) {

perror("Fallo al abrir el archivo de origen\\n");

exit(2);

}

\

//Obtenemos su stat, para comprobar si es regular y obtener su tamaño

if (fstat (fd\_orig, &sb) == -1) {

printf("Error al hacer stat en el fichero de origen\\n");

return 1;

}

\

if (!S\_ISREG (sb.st\_mode)) {

printf ("El fichero de origen no es un archivo regular\\n");

return 1;

}

\

//Guardamos el tamaño en una variable (por comodidad)

filesize = sb.st\_size;

\

\

//Creamos el archivo de destino

umask(0);

fd\_dest = open(str\_dest, O\_RDWR|O\_CREAT|O\_EXCL, S\_IRWXU);

if (fd\_dest == -1) {

perror("Fallo al crear el archivo de salida");

exit(2);

}

\

//Asignamos el espacion en el fichero de destino

ftruncate(fd\_dest, filesize);

\

//Creamos el mapa de memoria del fichero de origen

mem\_orig = (char \*) mmap(0, filesize, PROT\_READ, MAP\_SHARED,
fd\_orig, 0);

if(mem\_orig == MAP\_FAILED) {

\

perror("Fallo mapeando el archivo de entrada");

exit(2);

\

}

\

//Creamos el mapa de memoria del fichero de destino

mem\_dest = (char \*) mmap(0, filesize, PROT\_WRITE, MAP\_SHARED,
fd\_dest, 0);

if(mem\_dest == MAP\_FAILED) {

perror("Fallo mapeando el archivo de salida");

exit(2);

\

}

//Copiamos un mapa de memoria en otro

memcpy(mem\_dest, mem\_orig, filesize);

\

\

//Liberamos los mapas de memoria

munmap(mem\_orig, filesize);

munmap(mem\_dest, filesize);

\

//Cerramos los descriptores de fichero

close(fd\_orig);

close(fd\_dest);

\

//Terminamos la ejecución del programa

return 0;

\

\

}

\

}

\

\
