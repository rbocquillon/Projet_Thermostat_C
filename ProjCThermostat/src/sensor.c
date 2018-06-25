/**
 * @file sensor.c
 * Sonde de température.
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */

#include "appalloc.h"
#include "sensor.h"
#include "port.h"

// structure d'un sensor contenant un port pour récupérer la température
struct Sensor {
	IPort port;
};

/**
 * Créer un Sensor (de température).
 *
 * @return L'adresse du Sensor créé.
 */
ISensor createSensor(IPort port) {
	ISensor sensor = malloc(sizeof(struct Sensor));
	sensor->port = port;
	return sensor;
}

/**
 * Fournir la température courante en dixièmes de degrés Celsius.
 *
 * @return La température courante [0.255].
 */
int sensorGetTemp(ISensor sensor) {
	// on retourne la valeur contenu dans le port d'entrée.
	return portRead(sensor->port);
}

