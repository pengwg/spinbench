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

double SBTrapezoidEndTime(double startOffset, double riseTime, double plateauTime, double fallTime, double timeStep);
int SBTrapezoidBufferLength(double startOffset, double riseTime, double plateauTime, double fallTime, double timeStep);
void SBRamp(double startAmplitude, double endAmplitude, double startOffset, double duration, double timeStep, 
			float *allocatedBuffer, int bufferPts);
void SBPlateau(double amplitude, double startOffset, double duration, double timeStep, 
			float *allocatedBuffer, int bufferPts);
void SBTrapezoid(double startAmplitude, double plateauAmplitude, double endAmplitude, double startOffset, 
				 double riseTime, double plateauTime, double fallTime, double timeStep, float *allocatedBuffer, int bufferPts);

void SBMultidimensionalRamp(double startAmplitude, double endAmplitude, double startOffset, double duration, double timeStep,
                            const float *axisWeights, int numAxes, float **allocatedBuffer, int bufferPts);
void SBMultidimensionalPlateau(double amplitude, double startOffset, double duration, double timeStep,
                               const float *axisWeights, int numAxes, float **allocatedBuffer, int bufferPts);
void SBMultidimensionalTrapezoid(double startAmplitude, double plateauAmplitude, double endAmplitude, double startOffset,
                                 double riseTime, double plateauTime, double fallTime, double timeStep,
                                 const float *axisWeights, int numAxes, float **allocatedBuffer, int bufferPts);

double SBMomentForRamp(double startAmplitude, double endAmplitude, double startTime, double endTime);
double SBMomentForPlateau(double amplitude, double startTime, double endTime);
