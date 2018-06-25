/**
 * @file port.c
 * A simplistic Hardware Abstraction Layer.
 *
 * Created on: 11 juil. 2012
 * @author Christophe TOMBELLE
 */
#include <stdio.h>

#include "appalloc.h"
#include "simport.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

struct Port {
	/** Valeur écrite du port de sortie. */
	int _iOut;
	/** Valeur simulée du port d'entrée. */
	int _iIn;
};

/**
 * Lire le port d'entrée simulé.
 * @return Octet lu.
 */
static int _portRead(IPort port) {
//	printf("simport_read\n");
	return port->_iIn;
}

/**
 * Simuler la valeur du port d'entrée qui sera lue au prochain read.
 * @param iIn Valeur à simuler.
 * @see #read()
 */
void portSimInput(IPort port, int iIn) {
	port->_iIn = iIn;
}

/**
 * Ecrire un octet sur le port de sortie.
 * @param iOut octet à écrire.
 */
static void _portWrite(IPort port, int iOut) {
//	printf("simport_write\n");
	port->_iOut = iOut;
}

static int _portReadLatch(IPort port) {
//	printf("simport_readLatch\n");
	return port->_iOut;
}

IPort createSimPort() {
	IPort port = (IPort) appalloc(sizeof(struct Port), "_simp_alloc");
	port->_iIn = -1; // never simulated
	port->_iOut = -1; // never written
	return port;
}

int (* portRead)(IPort this) = _portRead;
/**
 * Get the value previously written to this output port.
 *
 * @param this This IO port.
 * @return The read value.
 */
int (* portReadLatch)(IPort this) = _portReadLatch;
/**
 * Write a value to this output port.
 *
 * @param this This IO port.
 * @param iValue The value to write to this output port [00-FF].
 */
void (* portWrite)(IPort this, int iValue) = _portWrite;

