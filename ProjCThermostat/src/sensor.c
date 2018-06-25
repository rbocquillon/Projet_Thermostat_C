/**
 * @file sensor.c
 * Sonde de temp�rature.
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */

#include "appalloc.h"
#include "sensor.h"
#include "port.h"

// structure d'un sensor contenant un port pour r�cup�rer la temp�rature
struct Sensor {
	IPort port;
};

/**
 * Cr�er un Sensor (de temp�rature).
 *
 * @return L'adresse du Sensor cr��.
 */
ISensor createSensor(IPort port) {
	ISensor sensor = malloc(sizeof(struct Sensor));
	sensor->port = port;
	return sensor;
}

/**
 * Fournir la temp�rature courante en dixi�mes de degr�s Celsius.
 *
 * @return La temp�rature courante [0.255].
 */
int sensorGetTemp(ISensor sensor) {
	// on retourne la valeur contenu dans le port d'entr�e.
	return portRead(sensor->port);
}

