![](Pictures/100002010000041A00000042C7CB0DFF.png)

Sistemas Operativos

Formulario de auto-evaluación

Modulo 2. Sesión 7. Construcción de un spool de impresión

\

Nombre y apellidos:

Ernesto Serrano Collado

\

​a) Cuestionario de actitud frente al trabajo.

El tiempo que he dedicado a la preparación de la sesión antes de asistir
al laboratorio ha sido de 20 minutos.

​1. He resuelto todas las dudas que tenía antes de iniciar la sesión de
prácticas: si (si/no). En caso de haber contestado “no”, indica los
motivos por los que no las has resuelto:

\

\

\

\

\

​2. Tengo que trabajar algo más los conceptos sobre:

\

\

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

El código de mi programa servidor.c ha sido:

/\*

 \* Sistemas Operativos

 \* Grado en Ingeniería Informática

 \*

 \* 2014 © Copyleft - All Wrongs Reserved

 \*

 \* Ernesto Serrano \<erseco@correo.ugr.es\>

 \*

 \* servidor.c

 \*

 \*/

\

\#include \<stdio.h\>

\#include \<stdlib.h\>

\#include \<string.h\>

\#include \<sys/types.h\>

\#include \<sys/stat.h\>

\#include \<fcntl.h\>

\#include \<unistd.h\>

\#include \<signal.h\>

\

//Constantes

\#define longnombre 50

\

//Manejador de senal

void mimanejador(int senial){

int estado;

wait(&estado);

}

\

//Punto de entrada al programa

int main(int argc, char\* argv[]){

\

//Declaracion de variables

int fdfifo\_e, fdfifo\_s, fdfifo\_proxy;

char peticion[10];

char nombrefifo\_proxy[longnombre];

char nombrefifo\_e[longnombre], nombrefifo\_s[longnombre];

size\_t bytesleidos;

pid\_t pid;

\

if(argc!=2){

printf("Modo de uso: %s \<nombrefifo\>\\n", argv[0]);

exit(EXIT\_FAILURE);

}

\

//Pone el manejador para la senal SIGHLD.

signal(SIGCHLD, mimanejador);

\

umask(0);

\

//Creamos el archivo de bloqueo

if( creat("bloqueo", S\_IRWXU) \< 0){

perror("Error al crear el archivo de bloqueo\\n");

exit(EXIT\_FAILURE);

}

//Generar los nombres de los archivos fifo de entrada y de salida

sprintf(nombrefifo\_e, "%se", argv[1]);

sprintf(nombrefifo\_s, "%ss", argv[1]);

\

//Borramos los archivos de los cauces (si existieran previamente)

unlink(nombrefifo\_e);

unlink(nombrefifo\_s);

\

//Crear los cauces de entrada y de salida

if (mkfifo(nombrefifo\_e, S\_IRWXU) \< 0){

perror("Error al crear el cauce de entrada\\n");

exit(EXIT\_FAILURE);

}

if (mkfifo(nombrefifo\_s, S\_IRWXU) \< 0){

perror("Error al crear el cauce de salida\\n");

exit(EXIT\_FAILURE);

}

// abrir los cauces de entrada y de salida

if ((fdfifo\_e = open(nombrefifo\_e,O\_RDWR)) \< 0){

perror("Error al abrir el fifo de entrada\\n");

exit(EXIT\_FAILURE);

}

\

if ((fdfifo\_s = open(nombrefifo\_s,O\_RDWR)) \< 0){

perror("Error al abrir el fifo de salida\\n");

exit(EXIT\_FAILURE);

}

while(1) {

\

bytesleidos = read(fdfifo\_e, &peticion, 10);

printf("Peticion recibida\\n");

\

if((pid=fork()) \< 0 ) {

perror("Error en el fork\\n");

exit(EXIT\_FAILURE);

} else if(pid == 0) {

\

//El hijo que sera el proxy

pid = getpid();

sprintf (nombrefifo\_proxy, "fifo.%d", pid);

//El hijo crea el fifo para comunicarse con el cliente

if(mkfifo(nombrefifo\_proxy, S\_IRWXU) \< 0){

perror("Error en mkfifo al crear el cauce de entrada\\n");

exit(EXIT\_FAILURE);

}

\

//Se envia el pid del proxy al cliente

printf("Enviando el pid del proxy %d al cliene\\n", pid);

if(write(fdfifo\_s, &pid, sizeof(pid)) != sizeof(pid)) {

perror("Error al escribir el pid del proxy en el fifo de salida\\n");

exit(EXIT\_FAILURE);

}

\

//Abre el fifo en modo solo lectura para recibir datos del cliente

if ((fdfifo\_proxy = open(nombrefifo\_proxy, O\_RDONLY)) \< 0) {

perror("Error al abrir el archivo fifo del proxy\\n");

exit(EXIT\_FAILURE);

}

//Redireccionamos la entrada estándar al fifo del proxy

dup2(fdfifo\_proxy, STDIN\_FILENO);

//Ejecutamos el proxy que lee de la entrada estandar y almacena los
datos en los archivo temporal,

//lee del archivo temporal e imprime en pantalla, al final libera el
cerrojo de pantalla

execlp("./proxy", "proxy", NULL);

\

//Si no pasa al codigo del proxy es que ha dado error, asi que lo
pintamos

perror("Error al ejecutar el proxy");

exit(EXIT\_FAILURE);

\

}

}

\

exit(EXIT\_SUCCESS);

}

\

\

El código de mi programa proxy.c ha sido:

/\*

 \* Sistemas Operativos

 \* Grado en Ingeniería Informática

 \*

 \* 2014 © Copyleft - All Wrongs Reserved

 \*

 \* Ernesto Serrano \<erseco@correo.ugr.es\>

 \*

 \* proxy.c

 \*

 \*/

\

\#include \<stdio.h\>

\#include \<stdlib.h\>

\#include \<unistd.h\>

\#include \<fcntl.h\>

\

//Constantes

\#define tamano 1024

\

void bloqueodesbloqueo(int dbloqueo, int orden) {

\

struct flock cerrojo;

//Inicializamos el cerrojo para bloquear todo el archivo

cerrojo.l\_type= orden;

cerrojo.l\_whence= SEEK\_SET;

cerrojo.l\_start= 0;

cerrojo.l\_len = 0;

\

//Si vamos a bloquearlo y ya lo esta, entonces el proceso duerme

if (fcntl(dbloqueo, F\_SETLKW, &cerrojo) == -1) {

perror ("Proxy: problemas al bloquear para impresion");

exit(EXIT\_FAILURE);

}

}

\

//Punto de entrada al programa

int main(int argc, char\* argv[]){

\

//Declaracion de variables

char bufer[tamano];

FILE\* fdtmp;

size\_t bytesleidos;

char nombrefifo[50];

int fdbloqueo;

if ((fdbloqueo = open("bloqueo", O\_RDWR)) \< 0){

perror("Error al abrir el archivo de bloqueo\\n");

exit(EXIT\_FAILURE);

}

//Creacion del archivo temporal

if ((fdtmp = tmpfile()) == NULL){

perror("Error en tmpfile: ");

exit(EXIT\_FAILURE);

}

//Leer desde la entrada estandar y almacenar los datos leidos en el
archivo temporal antes creado

while ((bytesleidos=read(STDIN\_FILENO, &bufer, tamano)) \> 0){

if(fwrite(&bufer, 1, bytesleidos, fdtmp) != bytesleidos){

perror("Error en fwrite\\n");

exit(EXIT\_FAILURE);

}

}

\

//Volvemos al inicio del descriptor del archivo

rewind(fdtmp);

//Obtener cerrojo para imprimir en pantalla

bloqueodesbloqueo(fdbloqueo, F\_WRLCK);

while((bytesleidos = fread(&bufer, 1, tamano,fdtmp)) \> 0){

printf("%s", bufer);

}

\

//Desbloqueamos

bloqueodesbloqueo(fdbloqueo, F\_UNLCK);

fclose(fdtmp);

\

//La eliminacion del archivo temporal es automatica

//Eliminamos el archivo fifo antes creado

sprintf(nombrefifo, "fifo.%d", getpid());

unlink(nombrefifo);

\

exit(EXIT\_SUCCESS);

\

}

\

\
