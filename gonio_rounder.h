/*
 * File:   gonio_rounder.h
 * Author: joris
 *
 * Created on March 16, 2012, 2:35 AM
 */

#ifndef GONIO_ROUNDER_H
#define	GONIO_ROUNDER_H

#include "atom.h"

/**
 * GONIO ROUNDER
 * used to provide a way of turning double/float values into common return-values for sin/cos/tan etc
 * eg: rather than providing the user with "0.258819045" we return "(sqrt(6)-sqrt(2))/4"
 **/
class gonio_rounder
{
public:
    static atom* round(double d);
    static atom* round(float f);
};

#endif	/* GONIO_ROUNDER_H */

