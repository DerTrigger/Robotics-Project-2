# Robotics-Project-2
Instrucciones para correr el programa 

1. Crear un workspace
	mkdir proyecto2
2. Crear un paquete
	catkin_create_pkg car_trayectory
3. Dentro del paquete se tiene que crear el folder "src", dicho folder ya se encuentra en el GitHub, una vez descargado, se tiene que copiar 

4. Dentro del directorio del paquete ya con la carpeta "src" correr: 
	catkin_make
5. Ejecutar el setup.bash
	source devel/setup.bash
6. Ya con el modelo corriendo de gazebo5 ... es hora de correr el programa del control del carro para esto se correrán los siguientes comandos: 
	rosrun car_trayectory controlCar

7. A continuación se le pide al usuario ingresar la pose deseada, se ingresan la posición en x, la posición en y, y el ángulo final theta. 

8. La impresión que genera el código es la pose actual del carro y la pose final, a parte de que se incluye como un ROS_TOPIC_INFO la velocidad que lleva el carro para comprobar que una vez que está cerca de la pose final, dicha velocidad disminuye. 
