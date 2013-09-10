// Name:  power.h
//
// Copyright(c) 2013 Loctronix Corporation
// http://www.loctronix.com
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

#ifndef _POWER_H_
#define _POWER_H_

#include "dciproc.h"

/**
 * Initialization
 */
void Power_Init( );

/**
 * Handles received DCI messages 
 */
void Power_OnDciMessageReceived(Dci_Context * pctxt);

/**
 * Processes on going updates
 */
void Power_OnUpdate( );

#endif