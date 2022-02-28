//programa en C para consultar los datos de la base de datos
//Incluir esta libreria para poder hacer las llamadas en shiva2.upc.es
//#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	int posicion;
	char test[20];
	char fecha[20];
	char consulta [80];
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexi??n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "Campeonato",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexi??n: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	printf("Escribe la fecha que quieres buscar (en formato yyyy-mm-dd): ");
	scanf("%s", test);
	// consulta SQL para obtener una tabla con todos datos
	// de los jugadores en una fecha determinada
	strcpy(consulta, "SELECT j.Nombre, r.Posicion, p.FechaInicio FROM Jugadores j INNER JOIN Relacion r ON r.IdJ = j.Id INNER JOIN Partidas p ON (p.Id = r.IdP AND p.FechaInicio LIKE '");
	strcat(consulta,test);
	strcat(consulta,"%')");
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	// En una fila hay tantas columnas como datos tiene una
	// persona. En nuestro caso hay tres columnas: nombre(row[0]),
	// Posicion(row[1]) y Fecha (row[2]).
	if (row == NULL)
		printf ("No se han obtenido datos en la consulta\n");
	else
		while (row !=NULL) {
			// la columna 1 contiene la posicion
			// la convertimos a entero 
			posicion = atoi (row[1]);
			// las columnas 0 y 2 contienen Nombre y Fecha 
			printf ("%s ha quedado en la posicion %d a fecha: %s\n", row[0], posicion, row[2]);
			// obtenemos la siguiente fila
			row = mysql_fetch_row (resultado);
	}
	
		mysql_close (conn);
		exit(0);
}
