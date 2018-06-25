/**
 * @file appalloc.c
 * $Id$
 *
 * Created on: 16 nov. 2012
 * @author Christophe TOMBELLE
 */
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "appalloc.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

/**
 * Allocate a block of memory of the specified and initialize it with 0s. If no
 * space left, display the message and exit with failure status.
 *
 * @param sz The size of the block
 * @param msg The message to display in case of insufficient memory space.
 * @return The address of the memory block.
 */
void *appalloc(size_t sz, char *msg) {
	void *mem = malloc(sz);
	if (mem == 0) {
		fprintf(stderr, "%s : \n", msg);
		fprintf(stderr, "out of dynamic memory\n"); fflush(stderr);
		exit(1);
	}
	memset(mem, 0, sz);
	return mem;
}
