/*
 *  avr-rtos-lib.c
 *  Assorted functions for AVR RTOS that are written in C
 *
 *  Copyright (C) 2012 Nathaniel Lewis
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

#include "avr-rtos.h"

// Initalize all of the data
void avr_rtos_init()
{
    _system_counter = 0;
    _current_task = 0;
    _rtos_table[0].priority = 0;
    _rtos_table[0].valid = 1;        // Task 0 is main, it must be valid
    _rtos_table[0].nextrun = 0;
    _rtos_table[0].updateinterval = 0;
    uint8_t c = 1;
    for(c = 1; c < MAX_TASKS; c++)
    {
        _rtos_table[c].priority = 0;
        _rtos_table[c].valid = 0;
        _rtos_table[c].nextrun = 0;
        _rtos_table[c].updateinterval = 0;
    }
}

// System timing
/*uint32_t avr_rtos_tick()
{
    return ++_system_counter;
}

uint32_t get_system_time()
{
    return _system_counter;
}*/

// Gets the next empty task slot (0 for none left)
uint8_t avr_rtos_next_task_slot()
{
    uint8_t nId = 1;
    for(nId = 1; nId < MAX_TASKS; nId++)
    {
        if(_rtos_table[nId].valid == 0)  // If we find an invalidated slot, its valid
            return nId;
    }
    
    // We didn't find one, so return 0 for failure
    return 0;
}