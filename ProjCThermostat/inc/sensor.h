/*
 * @file sensor.h
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "port.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

typedef struct Sensor *ISensor;

ISensor createSensor(IPort port);
int sensorGetTemp(ISensor);

#endif /* SENSOR_H_ */
