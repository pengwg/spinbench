/***************************************************************************
 
 Copyright 2011-2012 HeartVista Inc.  All rights reserved.
 Contact: HeartVista, Inc. <rthawk-info@heartvista.com>
 
 This file is part of the RTHawk system.
 
 $HEARTVISTA_BEGIN_LICENSE$
 
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF HEARTVISTA
 The copyright notice above does not evidence any	
 actual or intended publication of such source code.
 
 $HEARTVISTA_END_LICENSE$
 
 ***************************************************************************/

void calc_vds(const double slewmax, const double gradmax, const double gamma,
              const double Tgsample, const double Tdsample,
              const double * const Ninterleaves, const double * const fov,
              void (*fovNfunc)(const double,const double * const,const double * const,const int,
                               double *,double *,double *,double *,double *,double *),
              const int numCoeff, const double krmax,
              const int ngmax, double **xgrad, double**ygrad,
              int *numgrad, double* effectiveKMax, double *initialValues);

void calc_fov_N_polynomial(const double kr, const double * const Nintl, const double * const fov,
                           const int numCoeff, double *fovval, double *dfovdrval, double *d2fovdr2val,
                           double *Nval, double *dNdrval, double *d2Ndr2val);

void calc_fov_N_exponential(const double kr, const double * const Nintl, const double * const fov,
                            const int numCoeff, double *fovval, double *dfovdrval, double *d2fovdr2val,
                            double *Nval, double *dNdrval, double *d2Ndr2val);

void calc_fov_N_sigmoid(const double kr, const double * const Nintl, const double * const fov,
                        const int numCoeff, double *fovval, double *dfovdrval, double *d2fovdr2val,
                        double *Nval, double *dNdrval, double *d2Ndr2val);

