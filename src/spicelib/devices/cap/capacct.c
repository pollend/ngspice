/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "capdefs.h"
#include "ngspice/trandefs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"
#include "ngspice/cpdefs.h"

void
soa_printf(GENinstance *, GENmodel *, CKTcircuit *, const char *, ...);

/* make SOA checks after NR has finished */

int
CAPaccept(CKTcircuit *ckt, GENmodel *inModel)
{
    CAPmodel *model = (CAPmodel *) inModel;
    CAPinstance *here;
    double vc;  /* current capacitor voltage */
    int maxwarns_bv = 0;
    static int warns_bv = 0;

    if (!ckt->CKTsoaCheck)
        return OK;

    if(!(ckt->CKTmode & (MODEDC | MODEDCOP | MODEDCTRANCURVE | MODETRAN | MODETRANOP)))
        return OK;

    for(; model; model = model->CAPnextModel) {

        maxwarns_bv = ckt->CKTsoaMaxWarns;

        for (here = model->CAPinstances; here; here = here->CAPnextInstance) {

            vc = ckt->CKTrhsOld [here->CAPposNode] -
                 ckt->CKTrhsOld [here->CAPnegNode];

            if (vc > here->CAPbv_max)
                if (warns_bv < maxwarns_bv) {
                    soa_printf((GENinstance*) here, (GENmodel*) model, ckt,
                               "|Vc|=%g has exceeded Bv_max=%g\n",
                               vc, here->CAPbv_max);
                    warns_bv++;
                }

        }

    }

    return OK;
}
