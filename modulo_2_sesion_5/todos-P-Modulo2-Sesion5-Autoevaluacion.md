![](Pictures/100002010000041A000000423576346A.png)

Prácticas de Sistemas Operativos

Módulo II. Uso de los Servicios del SO Linux mediante la API

Sesión 5. Llamadas al sistema para gestión y control de señales.

1. Objetivos principales
========================

\

En esta sesión trabajaremos con las llamadas al sistema relacionadas con
la gestión y el control de señales. El control de señales en Linux
incluye las llamadas al sistema necesarias para cambiar el
comportamiento de un proceso cuando recibe una determinada señal,
examinar y cambiar la máscara de señales y el conjunto de señales
bloqueadas, y suspender un proceso, así como comunicar procesos.

-   Conocer las llamadas al sistema para el control de señales.

-   Conocer las funciones y las estructuras de datos que me permiten
    trabajar con señales.

-   Aprender a utilizar las señales como mecanismo de comunicación entre
    procesos.

\

2. Señales
==========

\

Las señales constituyen un mecanismo básico de sincronización que
utiliza el núcleo de Linux para indicar a los procesos la ocurrencia de
determinados eventos síncronos/asíncronos con su ejecución. Aparte del
uso de señales por parte del núcleo, los procesos pueden enviarse
señales para la notificación de cierto evento (la señal es generada
cuando ocurre este evento) y, lo que es más importante, pueden
determinar qué acción realizarán como respuesta a la recepción de una
señal determinada.

\

Se dice que una señal es depositada cuando el proceso inicia una acción
en base a ella, y se dice que una señal está pendiente si ha sido
generada pero todavía no ha sido depositada. Además un proceso puede
bloquear la recepción de una o varias señales a la vez.

\

Las señales bloqueadas de un proceso se almacenan en un conjunto de
señales llamado máscara de bloqueo de señales. No se debe confundir una
señal bloqueada con una señal ignorada, ya que una señal ignorada es
desechada por el proceso, mientras que una señal bloqueada permanece
pendiente y será depositada cuando el proceso la desenmascare (la
desbloquee). Si una señal es recibida varias veces mientras está
bloqueada, se maneja como si se hubiese recibido una sola vez.

\

La lista de señales y su tratamiento por defecto se puede consultar con
man 7 signal (o en signal.h). En la tabla 1 se muestran las señales
posibles en POSIX.1. Cada señal posee un nombre que comienza por SIG,
mientras que el resto de los caracteres se relacionan con el tipo de
evento que representa. Realmente, cada señal lleva asociado un número
entero positivo, que es el que intercambia con el proceso cuando éste
recibe la señal. Se puede usar indistintamente el número o la constante
que representa a la señal.

\

Símbolo

\

Acción

Significado

SIGHUP

Term

Cuelgue detectado en la terminal de control o muerte del proceso de
control

SIGINT

Term

Interrupción procedente del teclado (Ctrl + C)

SIGQUIT

Core

Terminación procedente del teclado

SIGILL

Core

Excepción producida por la ejecución de una instrucción ilegal

SIGABRT

Core

Señal de aborto procedente de la llamada al sistema abort(3)

SIGFPE

Core

Excepción de coma flotante

SIGKILL

Term

Señal de matar o eliminar a un proceso

SIGSEGV

Core

Referencia inválida a memoria

SIGPIPE

Term

Tubería rota: escritura sin lectores

SIGALRM

Term

Señal de alarma procedente de la llamada al sistema alarm(2)

SIGTERM

Term

Señal de terminación

SIGUSR1

Term

Señal definida por el usuario (1)

SIGUSR2

Term

Señal definida por el usuario (2)

SIGCHLD

Ign

Proceso hijo terminado o parado

SIGCONT

Cont

Reanudar el proceso si estaba parado

SIGSTOP

Stop

Parar proceso

SIGTSTP

Stop

Parar la escritura en la tty

SIGTTIN

Stop

Entrada de la tty para un proceso de fondo

SIGTTOUT

Stop

Salida a la tty para un proceso de fondo

Tabla1: Lista de señales en POSIX.1

\

Las entradas en la columna "Acción" de la tabla anterior especifican la
acción por defecto para la señal de la manera siguiente:

-   Term La acción por defecto es terminar el proceso.

-   Ign La acción por defecto es ignorar la señal.

-   Core La acción por defecto es terminar el proceso y realizar un
    volcado de memoria.

-   Stop La acción por defecto es detener el proceso.

-   Cont La acción por defecto es que el proceso continúe su ejecución
    si está parado.

    \

Las llamadas al sistema que podemos utilizar en Linux para trabajar con
señales son principalmente:

-   kill, se utiliza para enviar una señal a un proceso o conjunto de
    procesos.

-   sigaction, permite establecer la acción que realizará un proceso
    como respuesta a la recepción de una señal. Las únicas señales que
    no pueden cambiar su acción por defecto son: SIGKILL y SIGSTOP.

-   sigprocmask, se emplea para cambiar la lista de señales bloqueadas
    actualmente.

-   sigpending, permite el examen de señales pendientes (las que se han
    producido mientras estaban bloqueadas).

-   sigsuspend, reemplaza temporalmente la máscara de señal para el
    proceso con la dada por el argumento mask y luego suspende el
    proceso hasta que se recibe una señal.

    \

\

\

\

\

Sinopsis

\

\#include \<signal.h\>

int kill(pid\_t pid, int sig)

int sigaction(int signum, const struct sigaction \*act, struct sigaction
\*oldact);

int sigprocmask(int how, const sigset\_t \*set, sigset\_t \*oldset);

int sigpending(sigset\_t \*set);

int sigsuspend(const sigset\_t \*mask);

\

2.1 La llamada kill
-------------------

La llamada kill se puede utilizar para enviar cualquier señal a un
proceso o grupo de procesos.

Sinopsis

\

\#include \<sys/types.h\>

\#include \<signal.h\>

int kill(pid\_t pid, int sig);

\

Argumentos

\

-   Si pid es positivo, entonces se envía la señal sig al proceso con
    identificador de proceso igual a pid. En este caso, se devuelve 0 si
    hay éxito, o un valor negativo si se produce un error.

-   Si pid es 0, entonces sig se envía a cada proceso en el grupo de
    procesos del proceso actual.

-   Si pid es igual a -1, entonces se envía la señal sig a cada proceso,
    excepto al primero, desde los números más altos en la tabla de
    procesos hasta los más bajos.

-   Si pid es menor que -1, entonces se envía sig a cada proceso en el
    grupo de procesos -pid.

-   Si sig es 0, entonces no se envía ninguna señal, pero sí se realiza
    la comprobación de errores.

2.2 La llamada sigaction
------------------------

La llamada al sistema sigaction se emplea para cambiar la acción tomada
por un proceso cuando recibe una determinada señal.

\

Sinopsis

\

int sigaction(int signum, const struct sigaction \*act, struct sigaction
\*oldact);

\

Argumentos

\

El significado de los parámetros de la llamada es el siguiente:

-   signum especifica la señal y puede ser cualquier señal válida salvo
    SIGKILL o SIGSTOP.

-   Si act no es NULL, la nueva acción para la señal signum se instala
    como act.

-   Si oldact no es NULL, la acción anterior se guarda en oldact.

\

Valor de retorno

\

0 en caso de éxito y -1 en caso de error

\

Estructuras de datos

\

struct sigaction {

void (\*sa\_handler)(int);

void (\*sa\_sigaction)(int, siginfo\_t \*, void \*);

sigset\_t sa\_mask;

int sa\_flags;

void (\*sa\_restorer)(void);

}

\

-   sa\_handler especifica la acción que se va a asociar con la señal
    signum y pudiendo ser:

-   SIG\_DFL para la acción predeterminada,

-   SIG\_IGN para ignorar la señal

-   o un puntero a una función manejadora para la señal.

-   sa\_mask permite establecer una máscara de señales que deberían
    bloquearse durante la ejecución del manejador de la señal. Además,
    la señal que lance el manejador será bloqueada, a menos que se
    activen las opciones SA\_NODEFER o SA\_NOMASK.

Para asignar valores a sa\_mask, se usan las siguientes funciones:

-   int sigemptyset(sigset\_t \*set);

inicializa a vacío un conjunto de señales (devuelve 0 si tiene éxito y
–1 en caso

contrario).

-   int sigfillset(sigset\_t \*set);

inicializa un conjunto con todas las señales (devuelve 0 si tiene éxito
y –1 en caso

contrario).

-   int sigismember(const sigset\_t \*set, int senyal);

determina si una señal senyal pertenece a un conjunto de señales set

(devuelve 1 si la señal se encuentra dentro del conjunto, y 0 en caso
contrario).

-   int sigaddset(sigset\_t \*set, int signo);

añade una señal a un conjunto de señales set previamente inicializado

(devuelve 0 si tiene éxito y –1 en caso contrario).

-   int sigdelset(sigset\_t \*set, int signo);

elimina una señal signo de un conjunto de señales set

(devuelve 0 si tiene éxito y –1 en caso contrario).

-   sa\_flags especifica un conjunto de opciones que modifican el
    comportamiento del proceso de manejo de señales. Se forma por la
    aplicación del operador de bits OR a cero o más de las siguientes
    constantes:

-   SA\_NOCLDSTOP

-   Si signum es SIGCHLD, indica al núcleo que el proceso no desea
    recibir notificación cuando los procesos hijos se paren (esto es,
    cuando los procesos hijos reciban una de las señales: SIGTSTP,
    SIGTTIN o SIGTTOU).

-   SA\_ONESHOT o SA\_RESETHAND

-   Indica al núcleo que restaure la acción para la señal al estado
    predeterminado una vez que el manejador de señal haya sido llamado.

-   SA\_RESTART

-   Proporciona un comportamiento compatible con la semántica de señales
    de BSD haciendo reejecutables algunas llamadas al sistema entre
    señales.

-   SA\_NOMASK o SA\_NODEFER

-   Se pide al núcleo que no impida la recepción de la señal desde su
    propio manejador

-   SA\_SIGINFO

El manejador de señal toma 3 argumentos, no uno. En este caso, se debe
configurar sa\_sigaction en lugar de sa\_handler.

\

-   -   -   

int si\_signo; /\* Número de señal \*/

int si\_errno; /\* Un valor errno \*/

int si\_code; /\* Código de señal \*/

pid\_t si\_pid; /\* ID del proceso emisor \*/

uid\_t si\_uid; /\* ID del usuario real del proceso emisor \*/

int si\_status; /\* Valor de salida o señal \*/

clock\_t si\_utime; /\* Tiempo de usuario consumido \*/

clock\_t si\_stime; /\* Tiempo de sistema consumido \*/

sigval\_t si\_value; /\* Valor de señal \*/

int si\_int; /\* señal POSIX.1b \*/

void \* si\_ptr; /\* señal POSIX.1b \*/

void \* si\_addr; /\* Dirección de memoria que ha producido el fallo \*/

int si\_band; /\* Evento de conjunto \*/

int si\_fd; /\* Descriptor de fichero \*/

}

-   si\_signo, si\_errno y si\_code están definidos para todas las
    señales kill, las señales POSIX.1b y SIGCHLD actualizan los campos
    si\_pid y si\_uid. SIGCHLD también actualiza los campos si\_status,
    si\_utime y si\_stime. El emisor de la señal POSIX.1b especifica los
    campos si\_int y si\_ptr.

-   SIGILL, SIGFPE, SIGSEGV y SIGBUS provocan que el núcleo actualice el
    campo si\_addr con la dirección del fallo. SIGPOLL rellena si\_band
    y si\_fd.

-   si\_code indica la causa por la cuál se ha enviado la señal. Es un
    valor, no una máscara de bits.

    \

    Los posibles valores para cualquier señal se pueden consultar con
    man sigaction.

\

Nota: El elemento sa\_restorer está obsoleto y no debería utilizarse.
POSIX no especifica un elemento sa\_restorer.

\

Los siguientes ejemplos ilustran el uso de la llamada al sistema
sigaction para establecer un manejador para la señal SIGINT que se
genera cuando se pulsa CTRL-C.

// tarea9.c

\#include \<stdio.h\>

\#include \<signal.h\>

int main(){

struct sigaction sa;

sa.sa\_handler = SIG\_IGN; // ignora la señal

sigemptyset(&sa.sa\_mask);

//Reiniciar las funciones que hayan sido interrumpidas por un manejador

sa.sa\_flags = SA\_RESTART;

if (sigaction(SIGINT, &sa, NULL) == -1){

printf("error en el manejador");}

while(1);

}

\

\

\

\

\

// tarea10.c

\#include \<stdio.h\>

\#include \<signal.h\>

static int s\_recibida=0;

static void handler (int signum){

printf("\\n Nueva acción del manejador \\n");

s\_recibida++;}

int main()

{

struct sigaction sa;

sa.sa\_handler = handler; // ignora la señal

sigemptyset(&sa.sa\_mask);

//Reiniciar las funciones que hayan sido interrumpidas por un manejador

sa.sa\_flags = SA\_RESTART;

if (sigaction(SIGINT, &sa, NULL) == -1){

printf("error en el manejador");}

while(s\_recibida\<3);

}

### 

### Actividad 1. Trabajo con las llamadas al sistema sigaction y kill.

A continuación se muestra el código fuente de dos programas. El programa
envioSignal permite el envío de una señal a un proceso identificado por
medio de su PID. El programa reciboSignal se ejecuta en background y
permite la recepción de señales.

Ejercicio 1. Compila y ejecuta los siguientes programas y trata de
entender su funcionamiento.

\

\

/\*

envioSignal.c

Trabajo con llamadas al sistema del Subsistema de Procesos conforme a
POSIX 2.10

Utilización de la llamada kill para enviar una señal:

0: SIGTERM

1: SIGUSR1

2: SIGUSR2

a un proceso cuyo identificador de proceso es PID.

SINTAXIS: envioSignal [012] \<PID\>

\*/

\

\

\#include \<sys/types.h\> //POSIX Standard: 2.6 Primitive System Data
Types

// \<sys/types.h\>

\#include\<limits.h\> //Incluye \<bits/posix1\_lim.h\> POSIX Standard:
2.9.2 //Minimum //Values Added to \<limits.h\> y \<bits/posix2\_lim.h\>

\#include \<unistd.h\> //POSIX Standard: 2.10 Symbolic Constants
\<unistd.h\>

\#include \<sys/stat.h\>

\#include \<stdio.h\>

\#include \<stdlib.h\>

\#include \<signal.h\>

\#include \<errno.h\>

int main(int argc, char \*argv[])

{

long int pid;

int signal;

if(argc\<3) {

printf("\\nSintaxis de ejecución: envioSignal [012] \<PID\>\\n\\n");

exit(-1);

}

pid= strtol(argv[2],NULL,10);

if(pid == LONG\_MIN || pid == LONG\_MAX)

{

if(pid == LONG\_MIN)

printf("\\nError por desbordamiento inferior LONG\_MIN %d",pid);

else

printf("\\nError por desbordamiento superior LONG\_MAX %d",pid);

perror("\\nError en strtol");

exit(-1);

}

signal=atoi(argv[1]);

switch(signal) {

case 0: //SIGTERM

kill(pid,SIGTERM); break;

case 1: //SIGUSR1

kill(pid,SIGUSR1); break;

case 2: //SIGUSR2

kill(pid,SIGUSR2); break;

default : // not in [012]

printf("\\n No puedo enviar ese tipo de señal");

}

}

\

\

\

/\*

reciboSignal.c

Trabajo con llamadas al sistema del Subsistema de Procesos conforme a
POSIX 2.10

Utilización de la llamada sigaction para cambiar el comportamiento del
proceso

frente a la recepción de una señal.

\*/

\

\#include \<sys/types.h\>

\#include \<unistd.h\>

\#include \<stdio.h\>

\#include \<signal.h\>

\#include \<errno.h\>

static void sig\_USR\_hdlr(int sigNum)

{

\

if(sigNum == SIGUSR1)

printf("\\nRecibida la señal SIGUSR1\\n\\n");

else if(sigNum == SIGUSR2)

printf("\\nRecibida la señal SIGUSR2\\n\\n");

}

\

int main(int argc, char \*argv[])

{

struct sigaction sig\_USR\_nact;

if(setvbuf(stdout,NULL,\_IONBF,0))

{

perror("\\nError en setvbuf");

}

\

//Inicializar la estructura sig\_USR\_na para especificar la nueva
acción para la señal.

\

sig\_USR\_nact.sa\_handler= sig\_USR\_hdlr;

\

//'sigemptyset' inicia el conjunto de señales dado al conjunto vacío.

\

sigemptyset (&sig\_USR\_nact.sa\_mask);

sig\_USR\_nact.sa\_flags = 0;

\

//Establecer mi manejador particular de señal para SIGUSR1

if( sigaction(SIGUSR1,&sig\_USR\_nact,NULL) \<0)

{

perror("\\nError al intentar establecer el manejador de señal para
SIGUSR1");

exit(-1);

}

//Establecer mi manejador particular de señal para SIGUSR2

if( sigaction(SIGUSR2,&sig\_USR\_nact,NULL) \<0)

{

perror("\\nError al intentar establecer el manejador de señal para
SIGUSR2");

exit(-1);

}

for(;;)

{

}

}

\

Ejercicio 2. Escribe un programa en C llamado contador, tal que, cada
vez que reciba una señal que se puede manejar, muestre por pantalla la
señal y el número de veces que se ha recibido ese tipo de señal. En el
cuadro siguiente se muestra un ejemplo de ejecución del programa.

\

kawtar@kawtar-VirtualBox:\~\$ ./contador &

[2] 1899

kawtar@kawtar-VirtualBox:\~\$

No puedo capturar la señal 9

No puedo capturar la señal 19

No puedo capturar la señal 32

No puedo capturar la señal 33

kill -SIGINT 1899

kawtar@kawtar-VirtualBox:\~\$ La señal 2 se ha recibido 1 veces

kill -SIGINT 1899

La señal 2 se ha recibido 2 veces

kill -34 1899

kawtar@kawtar-VirtualBox:\~\$ La señal 34 se ha recibido 1 veces

kill -111 1899

bash: kill: 111: especificación de señal inválida

kawtar@kawtar-VirtualBox:\~\$ kill -19 1899 // el programa no puede
capturar la señal 19

[2]+ Detenido ./contador

kawtar@kawtar-VirtualBox:\~\$ kill -cont 1899

La señal 18 se ha recibido 1 veces

kawtar@kawtar-VirtualBox:\~\$ kill -KILL 1899

[2]+ Terminado (killed) ./contador

\

\

2.3 La llamada sigprocmask
--------------------------

La llamada sigprocmask se emplea para examinar y cambiar la máscara de
señales.

Sinopsis

\

int sigprocmask(int how, const sigset\_t \*set, sigset\_t \*oldset);

\

Argumentos

\

-   El argumento how indica el tipo de cambio. Los valores que puede
    tomar son los siguientes:

-   SIG\_BLOCK: El conjunto de señales bloqueadas es la unión del
    conjunto actual y el argumento set.

-   SIG\_UNBLOCK: Las señales que hay en set se eliminan del conjunto
    actual de señales bloqueadas. Es posible intentar el desbloqueo de
    una señal que no está bloqueada.

-   SIG\_SETMASK: El conjunto de señales bloqueadas se pone según el
    argumento set.

-   set representa el puntero al nuevo conjunto de señales enmascaradas.
    Si set es diferente de NULL, apunta a un conjunto de señales, en
    caso contrario sigprocmask se utiliza para consulta.

-   oldset representa el conjunto anterior de señales enmascaradas. Si
    oldset no es NULL, el valor anterior de la máscara de señal se
    guarda en oldset. En caso contrario no se retorna la máscara la
    anterior.

    \

Valor de retorno

\

0 en caso de éxito y -1 en caso de error

2.4 La llamada sigpending
-------------------------

La llamada sigpending permite examinar el conjunto de señales bloqueadas
y/o pendientes de entrega. La máscara de señal de las señales pendientes
se guarda en set.

\

Sinopsis

\

int sigpending(sigset\_t \*set);

\

Argumento

\

set representa un puntero al conjunto de señales pendientes

\

Valor de retorno

\

0 en caso de éxito y -1 en caso de error

\

2.5 La llamada sigsuspend
-------------------------

\

La llamada sigsuspend reemplaza temporalmente la máscara de señal para
el proceso con la dada por el argumento mask y luego suspende el proceso
hasta que se recibe una señal.

\

Sinopsis

\

int sigsuspend(const sigset\_t \*mask);

\

Argumenentos

\

mask representa el puntero al nuevo conjunto de señales enmascaradas

\

Valor de retorno

\

-1 si sigsuspend es interrumpida por una señal capturada (no está
definida la terminación

correcta)

\

\

\

\

\

\

\

\

Ejemplo de uso: En el siguiente ejemplo se suspende la ejecución del
proceso actual hasta que se reciba una determinada señal.

\

//tarea11.c

\

\#include \<stdio.h\>

\#include \<signal.h\>

\

int main(){

sigset\_t new\_mask;

\

/\* inicializar la nueva mascara de señales \*/

sigemptyset(&new\_mask);

\

sigaddset(&new\_mask, SIGUSR1);

\

/\*esperar a cualquier señal excepto SIGUSR1 \*/

sigsuspend(&new\_mask);

\

}

\

\

Notas finales:

-   No es posible bloquear SIGKILL, ni SIGSTOP, con una llamada a
    sigprocmask. Los intentos de hacerlo no serán tenidos en cuenta por
    el núcleo.

-   De acuerdo con POSIX, el comportamiento de un proceso está
    indefinido después de que no haga caso de una señal SIGFPE, SIGILL o
    SIGSEGV, que no haya sido generada por las llamadas kill o raise
    (llamada al sistema que permite a un proceso mandarse a sí mismo una
    señal). La división entera entre cero da un resultado indefinido. En
    algunas arquitecturas generará una señal SIGFPE. No hacer caso de
    esta señal puede llevar a un bucle infinito.

-   sigaction puede llamarse con un segundo argumento nulo para conocer
    el manejador de señal en curso. También puede emplearse para
    comprobar si una señal dada es válida para la máquina donde está,
    llamándola con el segundo y el tercer argumento nulos.

-   POSIX (B.3.3.1.3) anula el establecimiento de SIG\_IGN como acción
    para SIGCHLD. Los comportamientos de BSD y SYSV difieren, provocando
    el fallo en Linux de aquellos programas BSD que asignan SIG\_IGN
    como acción para SIGCHLD.

-   La especificación POSIX sólo define SA\_NOCLDSTOP. El empleo de
    otros valores en sa\_flags no es portable.

-   La opción SA\_RESETHAND es compatible con la de SVr4 del mismo
    nombre.

-   La opción SA\_NODEFER es compatible con la de SVr4 del mismo nombre
    bajo a partir del núcleo 1.3.9.

-   Los nombres SA\_RESETHAND y SA\_NODEFER para compatibilidad con SVr4
    están presentes solamente en la versión de la biblioteca 3.0.9 y
    superiores.

-   La opción SA\_SIGINFO viene especificada por POSIX.1b. El soporte
    para ella se añadió en la versión 2.2 de Linux.

### 

### Actividad 2 Trabajo con las llamadas al sistema sigsuspend y sigprocmask

\

Ejercicio 3. Escribe un programa que suspenda la ejecución del proceso
actual hasta que se reciba la señal SIGUSR1.

Ejercicio 4. Compila y ejecuta el siguiente programa y trata de entender
su funcionamiento.

\

//tarea12.c

\

\#include \<signal.h\>

\#include \<stdio.h\>

\#include \<string.h\>

\#include \<unistd.h\>

\

static int signal\_recibida = 0;

\

static void manejador (int sig)

{

signal\_recibida = 1;

}

\

int main (int argc, char \*argv[])

{

sigset\_t conjunto\_mascaras;

sigset\_t conj\_mascaras\_original;

struct sigaction act;

\

//Iniciamos a 0 todos los elementos de la estructura act

memset (&act, 0, sizeof(act));

\

act.sa\_handler = manejador;

\

if (sigaction(SIGTERM, &act, 0)) {

perror ("sigaction");

return 1;

}

//Iniciamos un nuevo conjunto de mascaras

sigemptyset (&conjunto\_mascaras);

//Añadimos SIGTERM al conjunto de mascaras

sigaddset (&conjunto\_mascaras, SIGTERM);

//Bloqueamos SIGTERM

if (sigprocmask(SIG\_BLOCK, &conjunto\_mascaras,
&conj\_mascaras\_original) \< 0) {

perror ("primer sigprocmask");

return 1;

}

\

sleep (10);

\

//Restauramos la señal – desbloqueamos SIGTERM

if (sigprocmask(SIG\_SETMASK, &conj\_mascaras\_original, NULL) \< 0) {

perror ("segundo sigprocmask");

return 1;

}

\

sleep (1);

\

if (signal\_recibida)

printf ("\\nSenal recibida\\n");

return 0;

}

\

\

![](Pictures/100002010000041A00000042C7CB0DFF.png)

Sistemas Operativos

Formulario de auto-evaluación

Modulo 2. Sesión 5. Llamadas al sistema para gestión y control de
señales.

\

Nombre y apellidos:

Ernesto Serrano Collado

\

​a) Cuestionario de actitud frente al trabajo.

El tiempo que he dedicado a la preparación de la sesión antes de asistir
al laboratorio ha sido de 60 minutos.

​1. He resuelto todas las dudas que tenía antes de iniciar la sesión de
prácticas:si (si/no). En caso de haber contestado “no”, indica los
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

​b) Cuestionario de conocimientos adquiridos.

Mi solución al ejercicio 2 ha sido:

/\*

\* Sistemas Operativos

\* Grado en Ingeniería Informática

\*

\* 2013 © Copyleft - All Wrongs Reserved

\*

\* Ernesto Serrano \<erseco@correo.ugr.es\>

\*

\* Escribe un programa en C llamado contador, tal que cada vez que
reciba una

\* señal que se pueda manejar, muestre por pantalla la señal y el número
de

\* veces que se ha recibido ese tipo de señal, y un mensaje inicial
indicando

\* las señales que no puede manejar.

\*/

\

\#include \<stdio.h\>

\#include \<signal.h\>

\#include \<unistd.h\>

\#include \<stdlib.h\>

\#include \<stdbool.h\> //Cabeceras para poder usar el tipo booleano

\

//Vector donde almacenar el contador de señales

const static int SIGNAL\_SIZE = 10;

static int received\_signals[SIGNAL\_SIZE];

\

static void sig\_handler (int signum) {

\

if (signum \>= 1 && signum \<= SIGNAL\_SIZE) {

\

//Incrementamos el elemento del vector de señales recibidas

received\_signals[signum]++;

\

printf("La señal %d se ha recibido %d veces\\n", signum,
received\_signals[signum]);

\

} else {

\

printf("No puedo manejar la señal %d\\n", signum);

}

}

\

int main(int argc, char \*argv[]) {

//Declaración de variables

struct sigaction sig\_action;

\

//Asociamos la funcion definida como manejador

sig\_action.sa\_handler = sig\_handler;

\

//inicializar un conjunto con todas las señales

sigfillset(&sig\_action.sa\_mask);

\

sig\_action.sa\_flags = 0;

for (int i = 1; i \< SIGNAL\_SIZE - 1; i++) {

\

if (sigaction(i, &sig\_action, NULL) \< 0) {

perror("Error al intentar establecer el manejador de señal\\n");

exit(-1);

}

}

\

//Dejamos en ejecución un bucle infinito, asi el programa no termina

while(true);

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

\* Escribe un programa que suspenda la ejecución del proceso actual
hasta que

\* se reciba la señal SIGUSR1.

\*/

\#include \<stdio.h\>

\#include \<signal.h\>

\#include \<unistd.h\>

\#include \<stdlib.h\>

int main(int argc, char \*argv[]) {

\

//definimos una nueva mascara

sigset\_t new\_mask;

\

//inicializar un conjunto con todas las señales

sigfillset(&new\_mask);

\

//eliminamos la señal SIGUSR1

sigdelset(&new\_mask, SIGUSR1);

\

//esperar solamente a la señal SIGUSR1

sigsuspend(&new\_mask);

\

//fin del programa

return 0;

}

\

\

Mi solución a la ejercicio 4 ha sido:

Crea un manejador de señales, inicializa todas las señales del manejador
a 0, luego crea un conjunto de mascaras vacia a la que agrega SIGTERM,
luego bloqueamos la señal, hacemos una pausa (sleep) de 10, restauramos
la señal y esperamos recibir las señales que estaban en espera en el
manejador cambiando el valor de signal\_recibida a 1

\

\
