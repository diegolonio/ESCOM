# Alumno: Apolonio Villegas Diego Armando
# Boleta: 2020630050
# Grupo: 4CV11
# Fecha: 8 de mayo del 2024

export CATALINA_HOME="/home/apolonio/tomcat/"
javac -cp $CATALINA_HOME/lib/javax.ws.rs-api-2.0.1.jar:$CATALINA_HOME/lib/gson-2.3.1.jar:. servicio_json/Servicio.java
rm -rf WEB-INF/classes/servicio_json/*
cp servicio_json/*.class WEB-INF/classes/servicio_json/
jar cvf Servicio.war WEB-INF META-INF
rm -rf $CATALINA_HOME/webapps/Servicio.war $CATALINA_HOME/webapps/Servicio
cp Servicio.war $CATALINA_HOME/webapps/
