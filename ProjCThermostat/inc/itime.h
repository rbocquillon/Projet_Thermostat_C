/**
 * @file itime.h
 * $Id$
 *
 * Created on: 26 sept. 2012
 * @author Christophe TOMBELLE
 */

#ifndef ITIME_H_
#define ITIME_H_

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

/**
 * Return the number of milliseconds since the program started.
 */
extern long long (* now)(void);

#endif /* ITIME_H_ */
