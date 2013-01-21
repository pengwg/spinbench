/*
 *  rf_tools.h
 *  C program versions of John Pauly's RF tools.
 * 
 *
 *  Functions:
 *  -------
 *  b2rf (rf, b, npoints, isComplex)
 *  computes complex rf given complex polynomial b.
 *  Returns an RF pulse using the inverse SLR transform in the (pre-allocated) RF array, 
 *    given a Beta polynomial in the array B.   If isComplex=0, then B is assumed to be 
 *    npoints real values, and the output will be complex and 2*npoints long.  Otherwise, 
 *    both B and RF are assumed to be 2*npoints in length, with real values on even indices 
 *    and imaginary on odd.
 *
 *  lp2mp (beta, npoints, isComplex)
 *  Returns the npoints-length minimum-phase polynomial in-place,
 *    given as input a linear-phase polynomial (length: npoints*2-1).  
 *    Values of extra samples from input array are undefined afterward.
 *    ** wro: only works for betas with max val = 1.0 (i.e., prior to tip-scaling).  
 *    ** wro: not tested with complex inputs
 *
 *  dInf = dInfinity (passbandRipple_dB, stopbandRipple_dB)
 *  computes D Infinity for a given stopband and passband ripple (in dB); dInfinity is defined
 *    as in Pauly IEEE XMI 10(1)53-65, 1991.
 *
 *  stopbandRipple_dB ()  /  passbandRipple_dB ()
 *  calculates {stopbandRipple/passbandRipple} in dB, given {passbandRipple/stopbandRipple} and dInf
 *
 *  Modified by William R. Overall 2007; added more functions, improved length allocation, optimized FT
 *  Written by Adam Kerr, October 1992
 *  Modified from John Pauly's code.
 *  (c) Board of Trustees, Leland Stanford Junior University
 */


void b2rf(double *RF, double *B, int npoints, int isComplex);
void lp2mp(double *beta, int npoints, int isComplex);
double dInfinity(double passbandRipple_dB, double stopbandRipple_dB);
double stopbandRipple_dB(double passbandRipple_dB,double dInfinity);
double passbandRipple_dB(double stopbandRipple_dB,double dInfinity);
