/*
 * @file IProgram.h
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <stdbool.h>

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

#define HOURS_PER_DAY 24

typedef struct Program *IProgram;

IProgram createProgram(void);
void progSetFixed(IProgram prog);
void progSetChoice(IProgram prog, bool bDay, int iHour);
bool progIsDay(IProgram prog, int iHour);

#endif /* PROGRAM_H_ */
