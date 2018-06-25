/**
 * @file appalloc.h
 * $Id$
 *
 * Created on: 16 nov. 2012
 * @author Christophe TOMBELLE
 */

#ifndef APPALLOC_H_
#define APPALLOC_H_

#include <stdlib.h>

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

/**
 * Allocate a block of memory of the specified and initialize it with 0s. If no
 * space left, display the message and exit with failure status.
 *
 * @return The address of the memory block.
 */
void *appalloc(size_t sz, char *msg);

#endif /* APPALLOC_H_ */
