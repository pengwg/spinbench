/***************************************************************************
 
 Copyright 2006-2011 HeartVista Inc.  All rights reserved.
 Contact: HeartVista, Inc. <rthawk-info@heartvista.com>
 
 This file is part of the RTHawk system.
 
 $HEARTVISTA_BEGIN_LICENSE$
 
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF HEARTVISTA
 The copyright notice above does not evidence any	
 actual or intended publication of such source code.
 
 $HEARTVISTA_END_LICENSE$
 
 ***************************************************************************/

// this is needed to ensure that docs are generated for the enums
/** @file */

#import <Foundation/Foundation.h>
#import "SBConstants.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 *    @enum  SBEndianEnum
 *    @brief Provides choices for data endian-ness
 *    @note  Typedef'd to SBEndian
 */
typedef enum SBEndianEnum { 
  SBBigEndian=0x00,   /**<  Big Endian    */
  SBLittleEndian=0x01 /**<  Little Endian */
} SBEndian;
  

static const float SBFloatZerosArray[MAX_GRAD_AXES] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static const float SBFloatOnesArray[MAX_GRAD_AXES] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
void SBMessage(int debugLevel, NSString *format,...);
void SBErrorMessage(NSString *format,...);
// sinc function.  SBSinc(x) = sin(pi*x)/(pi*x)
double SBSinc(double x);

/**
 *    @brief     Concatenate two bytes into a short
 *    @details   Combines two bytes into a 16-bit short using the desired endianness.
 */
short SBMakeShort(unsigned char *bytes, SBEndian endian);

// calculates gradient scale factors for all pulse axes for a specific pulse using raw imaging parameters.  It's usually easier to use
//  SBPulsePlugin's - (float)gradientScaleFactorForInterval:... methods to get this information.
//   values are returned in the first argument, scaleFactors[0..pulseAxes-1]
//   set magLimitScaleFactors[*]=0.0 for no magnitude limit.
//   set attenuation[*]=0.0 for no attenuation of terms.
void getScaleFactorLimits(float *scaleFactors, float const * const * const rotmat, float const * const * const gradTransform, const float *overlaps,
                          const float *physLimitScaleFactors, const float *magLimitScaleFactors, const float *attenuation,
                          int pulseAxes, int waveformAxes, BOOL freelyRotatable);
  
// This gives a single scale factor that can be used in any single direction within span(gradTransform).
// This value cannot be used on all axes concurrently, however.
float getScaleFactorLimitMagnitude(float const * const * const rotmat, float const * const * const gradTransform, const float *overlaps,
                                   const float *physLimitScaleFactors, const float *magLimitScaleFactors, const float *attenuation,
                                   int pulseAxes, int waveformAxes, BOOL freelyRotatable);

// make sure you call processOverlaps on the overlaps vector, if you haven't already, before calling getGradScaleFac*()
void processOverlaps(float *out, const float *in, float const * const * const gradTransform, int pulseAxes, int waveformAxes);
  
double det3x3(double matrix[][3]);

// returns NO if not invertible; otherwise, replaces matrix with its inverse
BOOL invert2x2(double matrix[][2]);
BOOL invert3x3(double matrix[][3]);

 /* These root-finding functions are taken from Graphics Gems
  *
  * wro: found at http://tog.acm.org/resources/GraphicsGems/gems/Roots3And4.c
  *      permissive license found at http://tog.acm.org/resources/GraphicsGems/ (copied below)
  * EULA: The Graphics Gems code is copyright-protected. In other words, you cannot
  *       claim the text of the code as your own and resell it. Using the code is
  *       permitted in any program, product, or library, non-commercial or commercial.
  *       Giving credit is not required, though is a nice gesture. The code comes as-is,
  *       and if there are any flaws or problems with any Gems code, nobody involved 
  *       with Gems - authors, editors, publishers, or webmasters - are to be held
  *       responsible. Basically, don't be a jerk, and remember that anything free comes
  *       with no guarantee.
  *
  * Minor modifications and additions:
  * -- SolveDiminishedQuartic() has the same interface as SolveQuartic, but it assumes the X^3 coefficient is zero.  This is slightly faster than SolveQuartic
  * -- SolveQuadric has been renamed SolveQuadratic
  */  
  int SolveQuadratic          (double c[ 3 ],double s[ 2 ]);
  int SolveCubic              (double c[ 4 ],double s[ 3 ]);
  int SolveDiminishedQuartic  (double c[ 5 ],double s[ 4 ]);
  int SolveQuartic            (double c[ 5 ],double s[ 4 ]);

  int maxint( int a, int b );
    
  int minint( int a, int b );
    
  float bessi0( float x);   
    
#ifdef __cplusplus
}
#endif