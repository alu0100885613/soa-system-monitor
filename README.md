# System Monitor

Con la ayuda de Qt se ha creado un monitor del sistema para Linux, cabe destacar que se ha realizado el desarrollo en una máquina virtual ([Virtual Box](https://www.virtualbox.org/)), por lo que no se ha podido implementar el apartado de **sensores** (ya que no tiene), en su lugar se ha implementado con su método de concurrencia correspondiente un apartado de red.

## Cómo compilar

1. Abrir una terminal
2. Situarse en el directorio del proyecto
3. Ejecutar `qmake`. Esto generará en la carpeta un [Makefile](http://www.cprogramming.com/tutorial/makefiles.html).
4. Ejecutar `make`. Esto genererá archivos .o, el ejecutable y los archivos moc y qrc.
5. Listo, ejecutar el systemonitor.app (doble click en carpeta) para abrir la aplicación.
6. Al terminar la ejecución y uso, usar `make clean` y `make distclean`. Las cosas bien recogidas.


## Requisitos para compilar y ejecutar

1. Que la bash conozca el comando `qmake`.
    
    1.1. Para esto, hará falta editar el fichero **~/profile** en Linux :penguin: y **~/.bash_profile** en MAC.
Podemos usar `vi ~/.bash_profile`,para añadir la ruta en la que se encuentra `qmake` al PATH, la cuál será de esta forma: 
`PATH=$PATH:/Users/<tu usuario>/Qt/<versión (5.8 recomendada) >/clang_64/bin`.

2. Tener el compilador g++.

## Ventana principal

La ventana principal consta de tres pestañas:

1.**Procesos**: se listan en un [QTableWidget](http://doc.qt.io/qt-4.8/qtablewidget.html) los siguientes 5 campos de cada uno de los procesos del sistema sacados del directorio /proc.

	- *PID*
	- *Owner*
	- *Cmdline*
	- *Threads*
	- *State*

El owner es mostrado por el alias, ya que se ha buscado su uid en /etc/passwd.
Tiene un timer que cada 5 segundos actualiza la información, para ver en cada momento que estado tiene cada proceso.
El método de concurrencia que se ha usado para esta pestaña es el de [QtConcurrent](http://doc.qt.io/qt-5/qtconcurrent.html#run).

2. **Net**: se listan las interfaces de red extraídas del comando `ifconfig`.
Se utiliza un buffer compartido circular, el cual en cada posición guarda 1 línea de información sobre las interfaces (como si se hubiese hecho un split por el \n).
El método de concurrencia que se ha usado es el de heredar de [QThread](http://doc.qt.io/qt-4.8/qthread.html) y sobreescribir su método *run()*.

3.**Hardware**: se lista el hardware de la máquina, usando un [QTreeView](http://doc.qt.io/qt-4.8/qtreeview.html) y asignandole un modelo `QJsonmodel` y cargando la salida del [QProcess](http://doc.qt.io/qt-5/qprocess.html) del comando `lshw -json`.
El método de concurrencia utilizado es por medio de señales y slots, este intercambio lo hará la clase **windowWorker** con la **Mainwindow**. Uno procesará el comando y cuando tenga el resultado avisará al otro para que actualize la UI.

## Métodos de concurrencia

**QtConcurrent**: simple de implementar aunque difícil de captar la idea inicial en cuanto a su uso. Existe en ThreadPool que recibe una lista de tareas (1 tarea cada vez que se lama al run) y cada hilo cogera una tarea para procesarla, por lo que también es muy escalable.

**Señales y slots**: simple de implementar y de entender, probablemente la mejor opción para newbies. Crear un una clase que haga el trabajo costoso y moverlo a un hilo creado y que cuando las cosas esten listas avise con una simple señal. Probablemente sea poco escalable ya que tendrás que crear muchas instancias de la clase que hace el trabajo con muchos hilos a los que moverlos y entonces se complique más que la opcion del QtConcurrent.

**QThread sobreescribiendo el método run()**: simple de entender la idea pero muy costosa de implementar ya que hay que estar manejando variables de condición, mutex... Complica una buena depuración y tampoco parece muy robusta ya que el hilo hay que avisarle de que termine y esperar por él. Tampoco parece muy escalable ya que hay que crear buffers compartidos que ocupan y pueden ser objeto de errores de acceso a memoria, además de la creación de bastantes hilos.


Eduardo Borges Fernández
ULL


