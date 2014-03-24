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
              id gMaxSRDelegate,
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

/**
 *   @protocol    SBVDSSlewRateGMaxDelegateProtocol
 *   @brief       Formal protocol for classes that use vds (variable density sampling) to dynamically shape slew-rate and gradient constraints
 *   @details     Classes which conform to this protocol can relay pertinent information to vds during the design process to implement dynamic limits on slew rate and gradient max.
 *   @author      HeartVista, Inc.
 *   @see         SBCylindricalExcitation
 */
@protocol SBVDSSlewRateGMaxDelegateProtocol

/**   @brief Helper function for vds.m to permit limiting of peak B1 values.
 *    @param[out] srmax The maximum slew rate permitted, in G/cm/s
 *    @param[out] gmax The maximum gradient amplitude permitted, in G/cm
 *    @param kRadius The current k-space radius, in cm^-1
 *    @param kDensity The current k-space density, in cm^-2
 *    @param absG The current gradient magnitude, in G/cm
 *    @param fov The current FOV, in cm
 */
- (void)calcVDSSlewRate:(double *)srmax andGradMax:(double *)gmax forRadius:(double)kRadius density:(double)kDensity gradientMagnitude:(double)absG fieldOfView:(double)fov;

@end
