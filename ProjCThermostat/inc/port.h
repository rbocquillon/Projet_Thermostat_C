/**
 * @file port.h
 * $Id$
 *
 * Created on: 11 oct. 2012
 * @author Christophe TOMBELLE
 */

#ifndef PORT_H_
#define PORT_H_

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

typedef struct Port *IPort;

/**
 * Read a value from this input port.
 *
 * @param this This IO port.
 * @return The value read from this input port.
 */
extern int (* portRead)(IPort this);
/**
 * Get the value previously written to this output port.
 *
 * @param this This IO port.
 * @return The read value.
 */
extern int (* portReadLatch)(IPort this);
/**
 * Write a value to this output port.
 *
 * @param this This IO port.
 * @param iValue The value to write to this output port [00-FF].
 */
extern void (* portWrite)(IPort this, int iValue);

/**
 * Construct an IO port to access the register at the specified address.
 *
 * @param iAddress The address to access the IO register [0-FFFFFF].
 */
IPort createPort(int iAddress);

#endif /* PORT_H_ */
