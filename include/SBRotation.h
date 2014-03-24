/***************************************************************************
 
 Copyright 2010-2011 HeartVista Inc.  All rights reserved.
 Contact: HeartVista, Inc. <rthawk-info@heartvista.com>
 
 This file is part of the RTHawk system.
 
 $HEARTVISTA_BEGIN_LICENSE$
 
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF HEARTVISTA
 The copyright notice above does not evidence any	
 actual or intended publication of such source code.
 
 $HEARTVISTA_END_LICENSE$
 
 ***************************************************************************/

#import <Cocoa/Cocoa.h>


/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBRotation
 *   @brief       Handles matrix rotations
 *   @author      HeartVista, Inc.
 */

@interface SBRotation : NSObject {
@private
  float **matrix;
  int numAxes;
}

// designated initializer
// initialized to NxN identity matrix
-(id)initWithAxes:(int)numAxes;
+(id)rotationWithAxes:(int)numAxes;
+(SBRotation *)rotationByAngle:(float)theta axisX:(float)x axisY:(float)y axisZ:(float)z;
+(SBRotation *)rotationFromMultiplying:(SBRotation *)r1 by:(SBRotation *)r2;
+(SBRotation *)rotationFromRotating:(SBRotation *)r1 byAngle:(float)theta axisX:(float)x axisY:(float)y axisZ:(float)z;
-(id)copy;

-(void)setToIdentity;
-(void)setEqualTo:(SBRotation *)other;
-(BOOL)isEqualTo:(SBRotation *)other;

-(int)numAxes;
-(float **)matrix;
-(float const * const *)constMatrix;
// for a pure rotation, the transpose is the inverse.
-(SBRotation *)transpose;
-(void)multiplyBy:(SBRotation *)r2;

// unrotate assumes this object contains a pure rotation
-(void)rotateVector:(float *)vec;
-(void)unrotateVector:(float *)vec;

@end
