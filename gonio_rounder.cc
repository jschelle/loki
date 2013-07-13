/*
 * File:   gonio_rounder.cpp
 * Author: joris
 *
 * Created on March 16, 2012, 2:35 AM
 */

#include "elemt_functors.h"
#include "gonio_rounder.h"
#include "number.h"
#include "power_functors.h"
#include <math.h>

/**
 * GONIO ROUNDER
 **/
atom* gonio_rounder::round(double d)
{

    double margin = 5 * pow(10,-16);

    double diff00 = d > ((sqrt(6.0)-sqrt(2.0))/4.0) ? d - ((sqrt(6.0)-sqrt(2.0))/4.0) : ((sqrt(6.0)-sqrt(2.0))/4.0) - d;
    double diff01 = d > 0.5 ? d - 0.5 : 0.5 - d;
    double diff02 = d > sqrt(2.0)/2.0 ? d - sqrt(2.0)/2.0 : sqrt(2.0)/2.0 - d;
    double diff03 = d > sqrt(3.0)/2.0 ? d - sqrt(3.0)/2.0 : sqrt(3.0)/2.0 - d;
    double diff04 = d > ((sqrt(6.0)+sqrt(2.0))/4.0) ? d - ((sqrt(6.0)+sqrt(2.0))/4.0) : ((sqrt(6.0)+sqrt(2.0))/4.0) - d;
    double diff05 = d > 2.0-sqrt(3) ? d - (2.0-sqrt(3)) : (2.0-sqrt(3)) - d;
    double diff06 = d > sqrt(3)/3.0 ? d - sqrt(3)/3.0 : sqrt(3)/3.0 - d;
    double diff07 = d > sqrt(3) ? d - sqrt(3) : sqrt(3) - d;
    double diff08 = d > 2.0+sqrt(3) ? d - (2.0+sqrt(3)) : (2.0+sqrt(3)) - d;
    double diff09 = d > sqrt(6)-sqrt(2) ? d- (sqrt(6)-sqrt(2)) : (sqrt(6)-sqrt(2)) - d;
    double diff10 = d > 2.0*sqrt(3)/3.0 ? d - 2.0*sqrt(3)/3.0 : 2.0*sqrt(3)/3.0 - d;
    double diff11 = d > sqrt(2) ? d - sqrt(2) : sqrt(2) - d;
    double diff12 = d > 2.0 ? d - 2.0 : 2.0 - d;
    double diff13 = d > sqrt(6)+sqrt(2) ? d- (sqrt(6)+sqrt(2)) : (sqrt(6)+sqrt(2)) - d;
    double diff14 = d > 0 ? d : -d;
    double diff15 = d > 1 ? d-1 : 1-d;
    double diff16 = d > -1 ? d + 1 : -1-d;

    if(diff00 < margin)
    {

        atom* p0 = new power();
        p0->set_arg(0,new fraction(1,2));
        p0->set_arg(1,new inumber(6));

        atom* p1 = new power();
        p1->set_arg(0,new fraction(1,2));
        p1->set_arg(1, new inumber(2));

        atom* m0 = new minus();
        m0->set_arg(0, p1);
        m0->set_arg(1, p0);

        atom* d0 = new divide();
        d0->set_arg(0, new inumber(4));
        d0->set_arg(1, m0);

        return d0;

    }
    else if(diff01 < margin)
    {

        return new fraction(1,2);

    }
    else if(diff02 < margin)
    {

        atom* p0 = new power();
        p0->set_arg(0, new fraction(1,2));
        p0->set_arg(1, new inumber(2));

        atom* d0 = new divide();
        d0->set_arg(0, new inumber(2));
        d0->set_arg(1, p0);

        return d0;

    }
    else if(diff03 < margin)
    {

        atom* p0 = new power();
        p0->set_arg(0, new fraction(1,2));
        p0->set_arg(1, new inumber(3));

        atom* d0 = new divide();
        d0->set_arg(0, new inumber(2));
        d0->set_arg(1, p0);

        return d0;

    }
    else if(diff04 < margin)
    {

        atom* p0 = new power();
        p0->set_arg(0,new fraction(1,2));
        p0->set_arg(1,new inumber(6));

        atom* p1 = new power();
        p1->set_arg(0,new fraction(1,2));
        p1->set_arg(1, new inumber(2));

        atom* p2 = new plus();
        p2->set_arg(0, p1);
        p2->set_arg(1, p0);

        atom* d0 = new divide();
        d0->set_arg(0, new inumber(4));
        d0->set_arg(1, p2);

        return d0;

    }
    else if(diff05 < margin)
    {

        atom* p0 = new power();
        p0->set_arg(0, new fraction(1,2));
        p0->set_arg(1, new inumber(3));

        atom* m0 = new minus();
        m0->set_arg(0, p0);
        m0->set_arg(1, new inumber(2));

        return m0;

    }
    else if(diff06 < margin)
    {

        atom* p0 = new power();
        p0->set_arg(0, new fraction(1,2));
        p0->set_arg(1, new inumber(3));

        atom* d0 = new divide();
        d0->set_arg(0, new inumber(3));
        d0->set_arg(1, p0);

        return d0;

    }
    else if(diff07 < margin)
    {

        atom* p0 = new power();
        p0->set_arg(0, new fraction(1,2));
        p0->set_arg(1, new inumber(3));
        return p0;

    }
    else if(diff08 < margin)
    {

        atom* p0 = new power();
        p0->set_arg(0, new fraction(1,2));
        p0->set_arg(1, new inumber(3));

        atom* p1 = new plus();
        p1->set_arg(0, p0);
        p1->set_arg(1, new inumber(2));

        return p1;

    }
    else if(diff09 < margin)
    {

        atom* p0 = new power();
        p0->set_arg(0, new fraction(1,2));
        p0->set_arg(1, new inumber(6));

        atom* p1 = new power();
        p1->set_arg(0, new fraction(1,2));
        p1->set_arg(1, new inumber(2));

        atom* m0 = new minus();
        m0->set_arg(0, p1);
        m0->set_arg(1, p0);

        return m0;

    }
    else if(diff10 < margin)
    {

        atom* p0 = new power();
        p0->set_arg(0, new fraction(1,2));
        p0->set_arg(1, new inumber(3));

        atom* m0 = new multiply();
        m0->set_arg(0, p0);
        m0->set_arg(1, new fraction(2,3));

        return m0;

    }
    else if(diff11 < margin)
    {

        power* r0 = new power();
        r0->set_arg(0, new fraction(1,2));
        r0->set_arg(1, new inumber(2));
        return r0;

    }
    else if(diff12 < margin)
    {

        return new inumber(2);

    }
    else if(diff13 < margin)
    {

        atom* p0 = new power();
        p0->set_arg(0, new fraction(1,2));
        p0->set_arg(1, new inumber(6));

        atom* p1 = new power();
        p1->set_arg(0, new fraction(1,2));
        p1->set_arg(1, new inumber(2));

        atom* p2 = new plus();
        p2->set_arg(0, p1);
        p2->set_arg(1, p0);

        return p2;

    }
    else if(diff14 < margin)
    {

        return new inumber(0);

    }
    else if (diff15 < margin)
    {

        return new inumber(1);

    }
    else if(diff16 < margin)
    {

        return new inumber(-1);

    }
    else
    {

        return new dnumber(d);

    }
}

