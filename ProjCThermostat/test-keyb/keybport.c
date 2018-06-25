/**
 * @file keypport.c
 * $Id$
 *
 * Created on: 28 sept. 2012
 * @author Christophe TOMBELLE
 */
#include <stdio.h>

#include "appalloc.h"
#include "keybport.h"

/*
 * ***************************************
 * NE PAS MODIFIER LES PROGRAMMES DE TEST
 * ***************************************
 */

typedef struct Port_ {
	/** Valeur écrite du port de sortie. */
	int _iOut;
	/** Valeur simulée du port d'entrée. */
	int _iIn;
	/** port ligne */
	IPort _portLig;
	/** masque colonne */
	int _iCol;
	/** masque ligne */
	int _iLig;
} *Port;

void kbpSetLigPort(IPort prtLeds, IPort portLig) {
	Port portLeds = (Port) prtLeds;
	portLeds->_portLig = portLig;
}

void kbpSetCode(IPort prtLeds, int iCol, int iLig) {
	Port portLeds = (Port) prtLeds;
	portLeds->_iCol = iCol;
	portLeds->_iLig = iLig;
}

/**
 * Simuler la valeur du port d'entrée qui sera lue au prochain read.
 * @param iIn Valeur à simuler.
 * @see #read()
 */
void kbpSimInput(IPort prt, int iIn) {
	Port port = (Port) prt;
	port->_iIn = iIn;
}

/**
 * Lire le port d'entrée simulé.
 * @return Octet lu.
 */
static int _portRead(IPort prt) {
	Port port = (Port) prt;
//	printf("kbp_read\n");
	return port->_iIn;
}

static void _portWrite(IPort prtLeds, int i) {
	Port portLeds = (Port) prtLeds;
//	printf("kbp_write\n");
	portLeds->_iOut = i;
	if (portLeds->_portLig == NULL)
		return;
	if ((i & portLeds->_iCol) != 0) {
		kbpSimInput(portLeds->_portLig, portLeds->_iLig);
	} else
		kbpSimInput(portLeds->_portLig, 0);
}

static int _portReadLatch(IPort prt) {
	Port port = (Port) prt;
//	printf("simport_readLatch\n");
	return port->_iOut;
}

IPort createKeybPort() {
	IPort prt = (IPort) appalloc(sizeof(struct Port_), "_keybp_alloc");
	Port port = (Port) prt;
	port->_iIn = -1; // never simulated
	port->_iOut = -1; // never written
	port->_portLig = NULL;
	port->_iCol = -1; // never simulated
	port->_iLig = 0; // never simulated
	portRead = _portRead;
	portReadLatch = _portReadLatch;
	portWrite = _portWrite;
	return prt;
}

