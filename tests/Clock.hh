/* $Id: Clock.hh,v 1.1 2000-12-22 13:22:41 legoater Exp $
 * 
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef __CLOCK_H
#define __CLOCK_H

#include <iostream>

#ifdef __osf__
typedef long usec_t;    /* number of microseconds since 1970/01/01 */
#define INT64_CONSTANT(val)  (val##L)
#endif

#ifdef __linux__
typedef long long int usec_t;
#define INT64_CONSTANT(val)  (val##LL)
#endif

class Clock
{
 public:
    static usec_t 	time(void);

    explicit		Clock(void);
    virtual		~Clock(void);

    bool		active(void) const { return _active; }
    usec_t		start(void);
    usec_t		reset(void) { return start(); }
    usec_t		elapsed(void);
    usec_t		stop(void);

 private:
    usec_t		_start;    
    usec_t		_elapsed;
    bool		_active;
};

#endif
