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

#import "SBCathTrackGradient.h"
#import "SBPulseData.h"
#import "SBParamsObject.h"
#import "SBTrapezoid.h"
#import "SBOptimizedGradient.h"
#import "SBReadoutTag.h"
#import "SBFunctions.h"

#import <math.h>


#define fovInit 40.0f
#define samplesInit 256
#define numShotsInit 3

#define xComponentInit 1.00f
#define yComponentInit 0.00f
#define zComponentInit 0.00f


@implementation SBCathTrackGradient

- (id)initWithPropertyList:(NSMutableDictionary *)_plist params:(SBParamsObject *)_params {
    self = [super initWithPropertyList:_plist params:_params];
    if(self) {
        if(!_plist) {
            [pulseData setNoOverlapForInterval:@"readout"];
            float **gradTransform = [pulseData gradTransform];
            gradTransform[0][0] = xComponentInit; gradTransform[0][1] = yComponentInit; gradTransform[0][2] = zComponentInit;
            gradTransform[1][0] = zComponentInit; gradTransform[1][1] = xComponentInit; gradTransform[1][2] = yComponentInit;

            fov = fovInit;
            samples = samplesInit;
            numShots = numShotsInit;
        }

    }
    return self;
}

- (NSArray *)attributeKeys
{
    return [[NSArray arrayWithObjects:@"fov",@"samples",@"numShots",nil] arrayByAddingObjectsFromArray:[super attributeKeys]];
}

- (NSString *)unitsOf:(NSString *)paramName
{
    if([paramName isEqualToString:@"fov"]) return @"cm";
    else if([paramName isEqualToString:@"samples"]) return @"";
    else if([paramName isEqualToString:@"numShots"]) return @"";
    return [super unitsOf:paramName];
}

- (NSArray *)observedAttributeKeys
{
    return [[NSArray arrayWithObjects:@"gamma",@"readSamplingRate",@"receiverPhase",nil] arrayByAddingObjectsFromArray:[super observedAttributeKeys]];
}

- (NSArray *)anchorKeys
{
    return [NSArray arrayWithObjects:@"start",@"readoutStart",@"readoutCenter",@"readoutEnd",@"end",nil];
}

- (NSArray *)intervalKeys
{
    return [NSArray arrayWithObjects:@"preReadout",@"readout",@"postReadout",nil];
}

+ (NSString *)humanReadablePluginName
{
    return @"CathTrack Readout";
}

- (void)destroyNib
{
    [super destroyNib];
}

- (void)calculateOutput
{
    double readXGradLimit = [self gradLimitMagnitudeForInterval:@"readout"];

    double localGamma = [params gamma];
    double localReadSamplingRate = [params readSamplingRate];

    double readPlateauAmp = localReadSamplingRate*1000.0/fov/localGamma;

    BOOL savedAutoCalculateOnAttributeChange = autoCalculateOnAttributeChange;
    autoCalculateOnAttributeChange = NO;

    // if readplateauamp is too large -> increase fov
    if(readXGradLimit > 0.0 && readPlateauAmp > readXGradLimit) {
        [self willChangeValueForKey:@"fov"];
        fov =  localReadSamplingRate*1000.0/localGamma/readXGradLimit;
        readPlateauAmp = readXGradLimit;
        [self didChangeValueForKey:@"fov"];
    }

    autoCalculateOnAttributeChange = savedAutoCalculateOnAttributeChange;

    double readPlateauDuration = samples / localReadSamplingRate;
    double plateauArea = readPlateauAmp * readPlateauDuration;

    SBOptimizedGradient *prewinder = [SBOptimizedGradient gradientWithAxes:1];
    [prewinder setLimitsForPulse:self interval:@"preReadout" constraints:SBScalableConstraint];
    [prewinder setAxis:0 startValue:0.0 endValue:readPlateauAmp area:-plateauArea/2.0];
//    [prewinder setAxis:1 startValue:0.0 endValue:0.0 area:peArea];

    SBOptimizedGradient *rewinder = [SBOptimizedGradient gradientWithAxes:1];
    [rewinder setLimitsForPulse:self interval:@"postReadout" constraints:SBScalableConstraint];
    [rewinder setAxis:0 startValue:readPlateauAmp endValue:0.0 area:-plateauArea/2.0];
//    [rewinder setAxis:1 startValue:0.0 endValue:0.0 area:-peArea];

    // constrain to 2us boundaries
    readoutStartOffset = ceil([prewinder duration]*500.0)/500.0f;
    readoutEndOffset = readoutStartOffset+readPlateauDuration;
    endOffset = readoutEndOffset+ceil([rewinder duration]*500.0)/500.0f;

    //changing anchor timings might make it necessary to update the position.  do that here.
    [self setPulsePositionFromAnchor:anchor];

    double timeStep = 1.0/[params waveSamplingRate];
    int startPosition = (int)floor(start/timeStep); // resel
    double startOffset = timeStep*fmax(start/timeStep - (double)startPosition,0.0); // ms
    int numPoints = ceil((startOffset+endOffset)/timeStep);

    [pulseData setNumPoints:numPoints startPosition:startPosition setToZero:YES];
    float **data = [pulseData gradData];
    [prewinder generateWaveformsWithStartTime:startOffset+readoutStartOffset-[prewinder duration] dataArray:data dataArrayLength:numPoints];
    SBPlateau(readPlateauAmp, startOffset+readoutStartOffset, readPlateauDuration, timeStep, data[0], numPoints);
    [rewinder generateWaveformsWithStartTime:startOffset+readoutEndOffset dataArray:data dataArrayLength:numPoints];
    [super calculateOutput];
}

- (BOOL)validateStart:(id *)valObj error:(NSError **)outError
{
    if([*valObj doubleValue] < 0.0)
        *valObj = [NSNumber numberWithDouble:0.0];
    return YES;
}

- (int)numAxesForPulseType:(enum SBPulseTypeEnum)type
{
    // override this function to indicate the number of unique axes you need for each pulse type
    switch(type) {
    case SBGrad:
        return 2;
        break;
    default:
        return 0;
        break;
    }
}

- (BOOL)validateFov:(id *)valObj error:(NSError **)outError
{
    double readGradLimit = [self gradLimitMagnitudeForInterval:@"readout"];
    if(*valObj == nil) *valObj = [NSNumber numberWithDouble:fov];
    double limit = [params readSamplingRate]*1000.0/(readGradLimit)/[params gamma];
    if([*valObj doubleValue] < limit)
        *valObj = [NSNumber numberWithDouble:limit];
    return YES;
}

- (BOOL)validateSamples:(id *)valObj error:(NSError **)outError
{
    if(*valObj == nil) *valObj = [NSNumber numberWithDouble:samples];
    double val = [*valObj doubleValue];
    if(val != maxint((int)rint(val),1))
        *valObj = [NSNumber numberWithInt:maxint((int)rint(val),1)];
    return YES;
}

- (double)readoutStart
{
    return start+readoutStartOffset;
}

- (void)setReadoutStart:(double)val
{
    start = val - readoutStartOffset;
}

- (double)readoutReferencePoint
{
    //float kxCoverageRounded = ceilf(xRes*kxCoverage)/xRes;
    //return start + (readoutStartOffset + (kxCoverageRounded-0.5f)*(readoutEndOffset-readoutStartOffset)/kxCoverageRounded);
    return 0;
}

- (double)readoutCenter
{
    //float kxCoverageRounded = ceilf(xRes*kxCoverage)/xRes;
    //return start + (readoutStartOffset + (kxCoverageRounded-0.5f)*(readoutEndOffset-readoutStartOffset)/kxCoverageRounded);
    return 0;
}

- (void)setReadoutCenter:(double)val
{
    //float kxCoverageRounded = ceilf(xRes*kxCoverage)/xRes;
    //start = val - (readoutStartOffset + (kxCoverageRounded-0.5f)*(readoutEndOffset-readoutStartOffset)/kxCoverageRounded);
}

- (double)readoutEnd
{
    return start+readoutEndOffset;
}

- (void)setReadoutEnd:(double)val
{
    start = val - readoutEndOffset;
}

- (double)end
{
    return start+endOffset;
}

- (void)setEnd:(double)val
{
    start = val - endOffset;
}

- (int)readoutRes
{
    return samples;
}

/*
-(double)plateauDuration
{
    return res/[params readSamplingRate];
}
*/
- (NSArray *)viewIndexKeys
{
    return [NSArray arrayWithObject:@"index"];
}

- (NSMutableArray *)tagsForTrNum:(int)trNum of:(int)numTr
{
    SBReadoutTag *tag = [SBReadoutTag tag];
    [tag setPluginName:[self editableName]];
    [tag setReadoutStart:[self readoutStart]];
    [tag setReadoutEnd:[self readoutEnd]];
    [tag setReadoutReferencePoint:[self readoutCenter]];
    [tag setPhase:[params receiverPhase]];
    [tag setSamplingRate:[params readSamplingRate]];
    [tag setViewIndex:trNum of:numTr forKey:@"index" object:self];
    float *tagRes = [tag resolution];
    tagRes[0] = fov*10.0/samples;
    tagRes[1] = fov*10.0/samples;
    float *tagFov = [tag fov];
    tagFov[0] = fov;
    tagFov[1] = fov*numTr/samples;
    float **kSpace = [tag allocateKSpaceWithLength:samples];
    float *kSpaceDensity = [tag kSpaceDensity];
    //float kyCoverageRounded = ceilf(yRes*kyCoverage)/yRes;
    //double thisYPos = hasPhaseEncoding?((double)yRes/(double)xRes*0.5*((double)trNum/(((double)numTr/kyCoverageRounded)/2.0)-1.0)):0.0;
    // to match EPI convention, which allows for shorter TE
    // from -1 to 1-delta for full kspace, -0.xx to 1-delta for partial k-space
    //double thisYPos = hasPhaseEncoding?((double)yRes/(double)xRes*0.5*(1.0+2.0*kyCoverageRounded*(((double)trNum)/(double)numTr - 1.0))):0.0;

    int i;
     for(i=0;i<samples;i++) {
        /* sampling time is defined at the center of the acquisition */
        kSpace[0][i] = 0.5*((double)i/(((double)samples)/2.0)-1.0);
    //    kSpace[1][i] = thisYPos;
        kSpace[2][i] = 0.0;
        kSpaceDensity[i] = 1.0;
    }

    NSMutableArray *outArray = [super tagsForTrNum:trNum of:numTr];
    [outArray addObject:tag];
    return outArray;
}

- (SBPulseData *)trIndependentPulseData
{
    return [pulseData subDataWithGradAxis:0];
}

- (SBPulseData *)trDependentPulseDataForTrNum:(int)trNum of:(int)numTr
{
    /*if (hasPhaseEncoding) {
        SBPulseData *outData = [pulseData subDataWithGradAxis:1];
        float kyCoverageRounded = ceilf(yRes*kyCoverage)/yRes;
        // to match EPI convention, which allows for shorter TE, see above
        //double thisScale = ((double)trNum/(((double)numTr/kyCoverageRounded)/2.0)-1.0);
        double thisScale = 1.0+2.0*kyCoverageRounded*(((double)trNum)/(double)numTr - 1.0);

        [outData setScaleFactor:thisScale forGradAxis:0];
        return outData;
    } else return nil;*/
    return nil;
}




@end
