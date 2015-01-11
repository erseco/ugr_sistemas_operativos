![](Pictures/100002010000041A00000042C7CB0DFF.png)

Sistemas Operativos

Formulario de auto-evaluación

Modulo 2. Sesión 4. Comunicación entre procesos utilizando cauces

\

Nombre y apellidos:

Ernesto Serrano Collado

\

​a) Cuestionario de actitud frente al trabajo.

El tiempo que he dedicado a la preparación de la sesión antes de asistir
al laboratorio ha sido de 60 minutos.

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

Se llama de la siguiente manera ./productorFIFO mensaje |
./consumidorFIFO

El programa productorFIFO almacena la cadena mensaje en el archivo
ComunicacionFIFO y consumidorFIFO lo lee, pero eso si, es el programa
consumidorFIFO el que crea el archivo ComunicacionFIFO, con lo que
productorFIFO espera a poder acceder al archivo, escribe en el, y
consumidor lee de el.

\

Mi solución a la ejercicio 2 ha sido:

El programa crea un pipe, luego hace un fork y hace que el proceso hijo
escriba en el proceso padre el mensaje

\

\

\

Mi solución a la ejercicio 4 ha sido:

Es parecido al anterior pero usando dup2 que nos permite indicar
explicitamente el descriptor que vamos a usar en lugar de coger el
primer descriptor que estuviera libre

El funcionamiento es simple:\
Despues del fork redirige la salida estandar al file descriptor que
queremos\
Ejecuta ls en el hijo \
El padre lo recibe desde la entrada estandar y se lo pasa a sort

\

Mi solución a la ejercicio 5 ha sido:

/\*

 \* Sistemas Operativos

 \* Grado en Ingeniería Informática

 \*

 \* 2013 © Copyleft - All Wrongs Reserved

 \*

 \* Ernesto Serrano \<erseco@correo.ugr.es\>

 \*

 \*/

\

\#include \<sys/types.h\> //Primitive system data types for abstraction
of implementation-dependent data types.

//POSIX Standard: 2.6 Primitive System Data Types \<sys/types.h\>

\#include \<unistd.h\> //POSIX Standard: 2.10 Symbolic Constants
\<unistd.h\>

\#include \<stdio.h\>

\#include \<errno.h\>

\#include \<stdlib.h\>

\#include \<stdbool.h\> //Cabeceras para poder usar el tipo booleano

\

\#define ARCHIVO\_FIFO "comunicacion"

\

\

int main(int argc, char \*argv[]) {

int inicio;

int fin;

int mitad;

\

int numBytes;

char buffer[80];

\

char params[2];

\

//Comprobamos si se le ha pasado dos enteros como parámetro

if(argc!=3) {

\

//Si no se le han pasado los parámetros correctos muestra un mensaje de
ayuda

printf("Modo de uso: %s \<inicio\> \<fin\>\\n\\n", argv[0]);

exit(1);

\

}

\

//Convertimos el argumento a int

inicio=atoi(argv[1]);

fin=atoi(argv[2]);

\

\

mitad = inicio + ((fin - inicio) / 2);

\

\

int fd1[2];

int fd2[2];

pid\_t PID;

\

pipe(fd1); // Llamada al sistema para crear un pipe

pipe(fd2); // Llamada al sistema para crear un pipe

\

if ( (PID= fork())\<0) {

perror("Error al hacer fork");

exit(1);

}

if (PID == 0) {

//Cierre del descriptor de lectura en el proceso hijo

close(fd1[0]);

params[0] = inicio;

params[1] = mitad;

\

// Enviar el mensaje a través del cauce usando el descriptor de
escritura

\

printf("paso 1\\n");

dup2(fd1[1],STDOUT\_FILENO);

execlp("esclavo","esclavo", params, NULL);

\

\

} else { // Estoy en el proceso padre porque PID != 0

\

if ( (PID= fork())\<0) {

perror("Error al hacer fork");

exit(1);

}

\

if (PID == 0) {

\

close(fd2[0]);

\

params[0] = mitad + 1;

params[1] = fin;

printf("paso 2\\n");

dup2(fd2[1],STDOUT\_FILENO);

execlp("esclavo","esclavo", params,NULL);

\

exit(0);

\

} else { // Estoy en el proceso padre porque PID != 0

//Cerrar el descriptor de escritura en el proceso padre

close(fd1[1]);

//Leer datos desde el cauce.

numBytes= read(fd1[0],buffer,sizeof(buffer));

\

dup2(fd1[0],STDIN\_FILENO);

\

printf("paso 3\\n");

//Recorremos el archivo de lectura leyendo en tramos de 80 bytes

while ((numBytes = read(fd1[0], &buffer, 4)) \> 0) {

printf("Valor: %d",1);

\

}

\

}

\

}

exit(0);

\

}

\

/\*

 \* Sistemas Operativos

 \* Grado en Ingeniería Informática

 \*

 \* 2013 © Copyleft - All Wrongs Reserved

 \*

 \* Ernesto Serrano \<erseco@correo.ugr.es\>

 \*

 \*/

\

\#include \<sys/types.h\>

\#include \<unistd.h\>

\#include \<stdio.h\>

\#include \<errno.h\>

\#include \<stdlib.h\>

\#include \<math.h\>

\

int main(int argc, char \*argv[]) {

\

int inicio;

int fin;

\

//Comprobamos si se le ha pasado dos enteros como parámetro

if(argc!=3) {

\

//Si no se le han pasado los parámetros correctos muestra un mensaje de
ayuda

printf("Modo de uso: %s \<inicio\> \<fin\>\\n\\n", argv[0]);

exit(1);

\

}

\

//Convertimos el argumento a int

inicio=atoi(argv[1]);

fin=atoi(argv[2]);

\

for (int n = inicio; n \< fin; n++) {

\

int c;

\

//NOTA: Calculo si es primo a la vieja usanza por que no acabo de hacer
funcionar

//el método rapido con la raiz cuadrada

for (c = 2;c \<= n - 1; c++) {

if ( n % c == 0 )

break; //No es primo, paramos la iteracion

}

\

if (c == n)

printf("El numero %d es primo.\\n", n);

\

}

\

exit(0);

\

}

\

\

\

\
