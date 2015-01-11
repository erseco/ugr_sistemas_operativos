![](Pictures/100002010000041A00000042C7CB0DFF.png)

Sistemas Operativos

Formulario de auto-evaluación

Modulo 2. Sesión 3. Llamadas al sistema para el Control de Procesos

\

Nombre y apellidos:

Ernesto Serrano Collado

\

​a) Cuestionario de actitud frente al trabajo.

El tiempo que he dedicado a la preparación de la sesión antes de asistir
al laboratorio ha sido de 30 minutos.

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

Mi solución al ejercicio 1 ha sido:

/\*

 \* Sistemas Operativos

 \* Grado en Ingeniería Informática

 \*

 \* 2013 © Copyleft - All Wrongs Reserved

 \*

 \* Ernesto Serrano \<erseco@correo.ugr.es\>

 \*

 \* Implementa un programa en C que tenga como argumento un numero
entero. Este

 \* programa debe crear un proceso hijo que se encargara de comprobar si
dicho

 \* numero es un numero par o impar e informara al usuario con un
mensaje que

 \* se enviara por la salida estandar. A su vez, el proceso padre
comprobara

 \* si dicho numero es divisible por 4, e informara si lo es o no usando

 \* igualmente la salida estandar.

 \*

 \*/

\

\#include\<sys/types.h\> //Primitive system data types for abstraction
of implementation-dependent data types.

//POSIX Standard: 2.6 Primitive System Data Types \<sys/types.h\>

\#include \<unistd.h\> //POSIX Standard: 2.10 Symbolic Constants
\<unistd.h\>

\#include \<stdio.h\>

\#include \<errno.h\>

\#include \<stdlib.h\>

\#include \<stdbool.h\> //Cabeceras para poder usar el tipo booleano

\

bool es\_par(int numero) {

return numero % 2 == 0;

}

\

bool es\_divisible\_entre\_cuatro(int numero) {

return numero % 4 == 0;

}

\

\

int main(int argc, char \*argv[]) {

int numero;

pid\_t pid;

\

//Comprobamos si se le ha pasado un pathname y unos permisos como
parámetros

if(argc!=2) {

\

//Si no se le han pasado los parámetros correctos muestra un mensaje de
ayuda

printf("Modo de uso: %s \<entero\>\\n\\n", argv[0]);

exit(1);

\

}

\

//Convertimos el argumento a int

numero=atoi(argv[1]);

\

if( (pid=fork())\<0) {

perror("\\nError en el fork");

exit(-1);

} else if(pid==0) { //proceso hijo ejecutando el programa

\

if (es\_par(numero)) {

printf("El numero %d es par\\n",numero);

\

}else

printf("El numero %d no es par\\n",numero);

\

} else { //proceso padre ejecutando el programa

\

if (es\_divisible\_entre\_cuatro(numero)) {

printf("El numero %d es divisible por 4\\n",numero);

}else

printf("El numero %d no es divisible por 4\\n",numero);

\

}

exit(0);

\

}

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

 \* Implementa un programa que lance cinco procesos hijo. Cada uno de
ellos se

 \* identificará en la salida estándar, mostrando un mensaje del tipo
Soy el

 \* hijo PID. El proceso padre simplemente tendrá que esperar la
finalización

 \* de todos sus hijos y cada vez que detecte la finalización de uno de
sus

 \* hijos escribirá en la salida estándar un mensaje del tipo:

 \* Acaba de finalizar mi hijo con \<PID\>

 \* Sólo me quedan \<NUM\_HIJOS\> hijos vivos

 \*

 \*/

\#include\<sys/types.h\>

\#include \<stdio.h\>

\#include \<errno.h\>

\#include \<stdlib.h\>

\#include \<sys/wait.h\>

\

int main(int argc, char \*argv[]) {

const int NUM\_HIJOS = 5;

pid\_t pid[NUM\_HIJOS];

\

switch (pid[0] =fork()){

\

case 0: //Hijo

printf("Soy el hijo %d\\n", getpid());

break;

default: //Padre

switch (pid[1] =fork()){

\

case 0: //Hijo

printf("Soy el hijo %d\\n", getpid());

break;

default: //Padre

switch (pid[2] =fork()){

\

case 0: //Hijo

printf("Soy el hijo %d\\n", getpid());

break;

default: //Padre

switch (pid[3] =fork()){

\

case 0: //Hijo

printf("Soy el hijo %d\\n", getpid());

break;

default: //Padre

switch (pid[4] =fork()){

\

case 0: //Hijo

printf("Soy el hijo %d\\n", getpid());

break;

}

break;

}

break;

\

}

break;

}

break;

\

}

\

//Esperamos a que se cierren

for (int i = 0;i\<NUM\_HIJOS;i++) {

\

if (waitpid(pid[i],0,0) \> 0) {

printf("Acaba de finalizar mi hijo con PID:%d\\n",pid[i]);

printf("Solo me quedan %d hijos vivos\\n",NUM\_HIJOS - i);

}

}

exit(0);

}

\

Mi solución a la ejercicio 4 ha sido:

/\*

 \* Sistemas Operativos

 \* Grado en Ingeniería Informática

 \*

 \* 2013 © Copyleft - All Wrongs Reserved

 \*

 \* Ernesto Serrano \<erseco@correo.ugr.es\>

 \*

 \* Implementa un programa que lance cinco procesos hijo. Cada uno de
ellos se

 \* identificará en la salida estándar, mostrando un mensaje del tipo
Soy el

 \* hijo PID. El proceso padre simplemente tendrá que esperar la
finalización

 \* de todos sus hijos y cada vez que detecte la finalización de uno de
sus

 \* hijos escribirá en la salida estándar un mensaje del tipo:

 \* Acaba de finalizar mi hijo con \<PID\>

 \* Sólo me quedan \<NUM\_HIJOS\> hijos vivos

 \*

 \* Implementa una modificacion sobre el anterior programa en la que el
proceso

 \* padre espera primero a los hijos creados en orden impar (1o,3o,5o) y
despues

 \* a los hijos pares (2o y 4o).

 \*/

\#include \<sys/types.h\>

\#include \<unistd.h\>

\#include \<stdio.h\>

\#include \<errno.h\>

\#include \<stdlib.h\>

\#include \<sys/wait.h\>

\

int main(int argc, char \*argv[]) {

const int NUM\_HIJOS = 5;

pid\_t pid[NUM\_HIJOS];

\

switch (pid[0] =fork()){

\

case 0: //Hijo

printf("Soy el hijo %d\\n", getpid());

break;

default: //Padre

switch (pid[1] =fork()){

\

case 0: //Hijo

printf("Soy el hijo %d\\n", getpid());

break;

default: //Padre

switch (pid[2] =fork()){

\

case 0: //Hijo

printf("Soy el hijo %d\\n", getpid());

break;

default: //Padre

switch (pid[3] =fork()){

\

case 0: //Hijo

printf("Soy el hijo %d\\n", getpid());

break;

default: //Padre

switch (pid[4] =fork()){

\

case 0: //Hijo

printf("Soy el hijo %d\\n", getpid());

break;

}

break;

}

break;

\

}

break;

}

break;

\

}

\

//Esperamos a que se cierren primero los impares

for (int i = 0;i\<NUM\_HIJOS;i = i + 2) {

if (waitpid(pid[i],0,0) \> 0) {

printf("Acaba de finalizar mi hijo con PID:%d\\n",pid[i]);

printf("Solo me quedan %d hijos vivos\\n",NUM\_HIJOS - i);

}

}

//Esperamos a que se cierren despues los pares

for (int i = 1;i\<NUM\_HIJOS;i = i + 2) {

if (waitpid(pid[i],0,0) \> 0) {

printf("Acaba de finalizar mi hijo con PID:%d\\n",pid[i]);

printf("Solo me quedan %d hijos vivos\\n",NUM\_HIJOS - i);

}

}

exit(0);

\

}

\

Mi solución a la ejercicio 6 ha sido:

/\*

 \* Sistemas Operativos

 \* Grado en Ingeniería Informática

 \*

 \* 2013 © Copyleft - All Wrongs Reserved

 \*

 \* Ernesto Serrano \<erseco@correo.ugr.es\>

 \*

 \* Escribe un programa que acepte como argumentos el nombre de un
programa,

 \* sus argumentos si los tiene, y opcionalmente la cadena “bg”. Nuesto

 \* programa deberá ejecutar el programa pasado como primer argumento en

 \* foreground si no se especifica la cadena “bg” y en background en
caso

 \* contrario. Si el programa tiene argumentos hay que ejecutarlo con
estos.

 \*

 \*/

\#include \<sys/types.h\>

\#include \<unistd.h\>

\#include \<stdio.h\>

\#include \<errno.h\>

\#include \<stdlib.h\>

\#include \<sys/wait.h\>

\#include \<stdbool.h\> //Cabeceras para poder usar el tipo booleano

\#include \<string.h\> //Cabeceras para poder usar cadenas

\

void execute\_command(char \*str\_command, char \*str\_params) {

\

\

if( (execl(str\_command, str\_params, str\_params,NULL)\<0)) {

\

//if( (execlp(str\_command,str\_params)\<0)) {

perror("Error en el execl\\n");

exit(-1);

}

\

}

\

int main(int argc, char \*argv[]) {

pid\_t pid;

bool en\_background = false;

int n\_params = argc;

const char \*str\_bg = "bg";

char \*str\_command;

char str\_params[200];

\

\

//Comprobamos si se le ha pasado un pathname y unos permisos como
parámetros

if(argc\<2) {

//Si no se le han pasado los parámetros correctos muestra un mensaje de
ayuda

printf("Modo de uso: %s \<programa\> [opciones] [bg]\\n\\n", argv[0]);

exit(1);

} else {

//Comprobamos si el ultimo parametro pasado es bg

if (strcmp(argv[argc-1], str\_bg) == 0) {

en\_background = true;

//Habrá un parametro menos

n\_params--;

}

\

//Extraemos el comando

str\_command = argv[1];

\

//Recorremos los parametros pasados a partir del 2

for (int i = 2; i \< n\_params; i++) {

strcat (str\_params, argv[i]);

strcat (str\_params, " ");

}

\

if (en\_background) {

printf("%s se ejecutara en background\\n", str\_command);

if ((pid =fork()) \< 0)

return (-1) ;

else if (pid != 0)

exit(0) ; //El proceso padre sale

\

execute\_command(str\_command,str\_params);

\

} else {

printf("%s se ejecutara en foreground\\n", str\_command);;

execute\_command(str\_command,str\_params);

}

\

}

\

exit(0);

\

}

\
