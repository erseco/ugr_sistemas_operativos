![](Pictures/100002010000041A00000042C7CB0DFF.png)

Sistemas Operativos

Formulario de auto-evaluación

Sesión 3. Monitorización del sistema.

\

Nombre y apellidos:

Ernesto Serrano Collado

\

​a) Cuestionario de actitud frente al trabajo.

El tiempo que he dedicado a la preparación de la sesión antes de asistir
al laboratorio ha sido de 60 minutos.

​1. He resuelto todas las dudas que tenía antes de iniciar la sesión de
prácticas: SI (si/no). En caso de haber contestado “no”, indica los
motivos por los que no las has resuelto:

\

\

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

Mi solución a la actividad 3.1 ha sido:

Ejecutamos el comando uptime, que nos dará la información:\
El sistema lleva 37 minutos encendido\
Hay un usuario trabajando\
En los últimos 15 minutos ha tenido una carga de 0.02

\

\

Mi solución a la actividad 3.2 ha sido:

Vim prueba\_procesos

Introducimos lo siguiente:\
\#!/bin/bash\
i=0\
aux=0

while [ \$i – lt \$1 ];\
do\
 let aux=56\*46\
 let i=i+1\
done\
\
echo “El valor de la variable es \$i”\
\
una vez introducido le damos permisos de ejecución con chmod +x
prueba\_procesos\

lo lanzamos con ./prueba\_procesos 10000 & \

se inicia con prioridad 20\
\
lanzamos con time ./prueba:procesos 10000 & y le hacemos un renice -20
\<pid\> y la columna PR se pone a 0 y la NI a -20\
\
lanzando varios se observa que los que tienen valor negativo terminan
antes que los que tienen valor negativo

\

Mi solución a la actividad 3.4 ha sido:

He instalado el paquete systat(...).rpm desde /fenix, luego he ejecutado
lo siguiente:

ejecutamos mpstat y las columnas irq está a 0.00 y la soft también está
a 0.00\
ejecutamos top y leemos la linea swap aunque tambien podriamos verlo con
el comando free\

\

\

\

Mi solución a la actividad 4.2 ha sido:

Ejecutamos vmstat 2 20 \> fichero\_datos\
\
en este fichero tendremos la información que queremos

\

\

\

\

Mi solución a la actividad 5.2 ha sido:

ls -ltau para ultimo acceso\
ls -ltac para ultima modificación

hay que agregar el parámetro t para que ordene aparte de mostrar el
valor

\

\

\

\

Mi solución a la actividad 5.3 ha sido:

Ejecutamos el comando df y vemos la columna blocks

para verlo para “humanos” le agregamos el parametro -h

\

\

\

\

Mi solución a la actividad 5.6 ha sido:

\

Ejecutamos el comando du -h | grep 4.0K

el tamaño por omision podriamos obtenerlo con tune2fs -l \<nombre
dispositivo\> pero no se puede obtener en clase ya que UML monta un
sistema de ficheros virtual

\

\

\

\

Mi solución a la actividad 5.7 ha sido:

\

Ejecutamos: ls \> archivo.txt y ls -l \> target\_hardLink2.txt para
crear dos archivos de texto con contenido\
ln -s archivo.txt softLink\
ln archivo.txt hardLink\
ln target\_hardLink2.txt hardLink2\

Lo del contador es porque solo contabiliza los enlaces “duros” no los
simbólicos

\

\

\

Mi solución a la actividad 5.9 ha sido:

Ejecutamos:

mknod /dev/prueba c 4 64 \#creamos un archivo de caracteres

lo mostramos:

ls -l /dev/prueba

crw-r--r-- 1 root wheel 4, 64 14 oct 21:27 /dev/prueba

mknod /dev/prueba c 4 64 \#creamos un archivo de bloques

/dev/prueba2

brw-r--r-- 1 root wheel 4, 64 14 oct 21:29 /dev/prueba2

\

los podemos diferenciar por la primera letra del resultado del ls

\

\

\
