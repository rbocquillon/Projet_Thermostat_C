/**
 * @file hal.h
 * $Id$
 *
 * Created on: 14 nov. 2012
 * @author Christophe TOMBELLE
 */

#ifndef HAL_H_
#define HAL_H_

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

/** The function pointer type of the Interrupt Service Routine. */
typedef void (* eic_isr)(void);

/**
 * Install an Interrupt Service Routine for the External Interrupt pin of the
 * processor.
 *
 * @param isr The address of the C function to install as an Interrupt Service
 * Routine.
 */
void installISR(eic_isr isr);

/**
 * Uninstall Interrupt Service Routine previously installed by installISR.
 *
 * @param isr The address of the ISR to uninstall.
 * @see #installISR
 */
void uninstallISR(eic_isr isr);

#include "port.h"

#endif /* HAL_H_ */
