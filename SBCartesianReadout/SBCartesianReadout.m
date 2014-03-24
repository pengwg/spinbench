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

#import "SBCartesianReadout.h"
#import "SBPulseData.h"
#import "SBParamsObject.h"
#import "SBTrapezoid.h"
#import "SBOptimizedGradient.h"
#import "SBReadoutTag.h"
#import "SBFunctions.h"

#import <math.h>

static const float kxCoverageInit = 1.0f;
static const float kyCoverageInit = 1.0f;
static const float fovInit = 24.0f;
static const int xResInit = 256;
static const int yResInit = 256;
static const float netAreaInit = 0.0f;
static const float xComponentInit = 1.00f;
static const float yComponentInit = 0.00f;
static const float zComponentInit = 0.00f;
static const float samplingRateInit = 250.0f;
static const BOOL hasPhaseEncodingInit = YES;
static const BOOL hasRewinderInit = NO;

@implementation SBCartesianReadout

-(id)initWithPropertyList:(NSMutableDictionary *)_plist params:(SBParamsObject *)_params {
  /* moved here to allow loading files in which coverages are not specified */
  kxCoverage = kxCoverageInit;
  kyCoverage = kyCoverageInit;
  samplingRate = samplingRateInit;
	self = [super initWithPropertyList:_plist params:_params];
	if(self) {
		if(!_plist) {
      [pulseData setNoOverlapForInterval:@"readout"];
			float **gradTransform = [pulseData gradTransform];
			gradTransform[0][0] = xComponentInit; gradTransform[0][1] = yComponentInit; gradTransform[0][2] = zComponentInit;
			gradTransform[1][0] = zComponentInit; gradTransform[1][1] = xComponentInit; gradTransform[1][2] = yComponentInit;
			
			fov = fovInit;
			xRes = xResInit;
			yRes = yResInit;
			netArea = netAreaInit;
			hasPhaseEncoding = hasPhaseEncodingInit;
			hasRewinder = hasRewinderInit;
		} else {
      // load global read rate if necessary (pre-1.3.3 files)
      id thisVal = [_plist valueForKey:@"samplingRate"];
			if(!thisVal) {
				[self setValue:[NSNumber numberWithFloat:[params maxReadoutRate]] forKey:@"samplingRate"];
      }
      // translate keys for compatibility with old (pre-1.3.0) .spv files
      thisVal = [_plist valueForKey:@"kxCoverage"];
      if(!thisVal) {
				[self setValue:[NSNumber numberWithFloat:kxCoverageInit] forKey:@"kxCoverage"];
			}
      thisVal = [_plist valueForKey:@"kyCoverage"];
			if(!thisVal) {
				[self setValue:[NSNumber numberWithFloat:kyCoverageInit] forKey:@"kyCoverage"];
			}
    }
	}
	return self;
}

-(NSArray *)attributeKeys
{
	return [[NSArray arrayWithObjects:@"fov",@"samplingRate",@"xRes",@"yRes",@"netArea",@"hasPhaseEncoding",@"kxCoverage",@"kyCoverage",@"hasRewinder",nil] arrayByAddingObjectsFromArray:[super attributeKeys]];
}

-(NSArray *)derivedAttributeKeys
{
	return [[NSArray arrayWithObjects:@"duration",@"samples",nil] arrayByAddingObjectsFromArray:[super derivedAttributeKeys]];
}

-(NSString *)unitsOf:(NSString *)paramName
{
	if([paramName isEqualToString:@"fov"]) return @"cm";
	else if([paramName isEqualToString:@"samplingRate"]) return @"kSamp/sec";
	else if([paramName isEqualToString:@"netArea"]) return @"cyc/pixel";
	else if([paramName hasSuffix:@"Res"]) return @"";
	else if([paramName isEqualToString:@"hasPhaseEncoding"]) return @"";
	else if([paramName isEqualToString:@"hasRewinder"]) return @"";
  else if([paramName isEqualToString:@"kxCoverage"]) return @"fraction";
	else if([paramName isEqualToString:@"kyCoverage"]) return @"fraction";
	return [super unitsOf:paramName];
}

-(NSArray *)observedAttributeKeys
{
	return [[NSArray arrayWithObjects:@"gamma",@"maxReadoutRate",@"nominalGradLimitScale",@"receiverPhase",@"readoutSampleDivisor",nil] arrayByAddingObjectsFromArray:[super observedAttributeKeys]];
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
	return @"Cartesian Readout";
}

-(void)destroyNib
{
	if(nibIsLoaded){
        [kxPercentFormatter release];
        [kyPercentFormatter release];   
    }   
	[super destroyNib];
}

- (void)calculateOutput
{
  double readXGradLimit = [self gradLimitMagnitudeForInterval:@"readout"];

	double localGamma = [params gamma];

  if(samplingRate > [params maxReadoutRate]) {
		[self willChangeValueForKey:@"samplingRate"];
    samplingRate = [params maxReadoutRate];
		[self didChangeValueForKey:@"samplingRate"];
  }
	double readPlateauAmp = samplingRate*1000.0/fov/localGamma;

	BOOL savedAutoCalculateOnAttributeChange = autoCalculateOnAttributeChange;
	autoCalculateOnAttributeChange = NO;

	// if readplateauamp is too large -> increase fov
	if(readXGradLimit > 0.0 && readPlateauAmp > readXGradLimit) {
		[self willChangeValueForKey:@"fov"];
		fov =  samplingRate*1000.0/localGamma/readXGradLimit;
    readPlateauAmp = readXGradLimit;
		[self didChangeValueForKey:@"fov"];
	}
	
	autoCalculateOnAttributeChange = savedAutoCalculateOnAttributeChange;

  // CALCULATE FREQUENCY ENCODE TIMINGS
  const int rsd = [params readoutSampleDivisor];
  const int newSamples = ceilf(xRes*kxCoverage/rsd)*rsd;
  if(samples != newSamples) {
    [self willChangeValueForKey:@"samples"];
    samples = newSamples;
    [self didChangeValueForKey:@"samples"];
  }
  double kxCoverageRounded = (double)samples/xRes;
  double readPlateauDuration = (double)samples/samplingRate;


  // CALCULATE PHASE ENCODE TIMINGS
	double peArea = hasPhaseEncoding?1000.0/fov/localGamma*(double)yRes/2.0:0.0; 
   
  SBOptimizedGradient *prewinder = [SBOptimizedGradient gradientWithAxes:2];
  [prewinder setLimitsForPulse:self interval:@"preReadout" constraints:SBScalableConstraint | SBNominalGradLimitConstraint];
  //[prewinder setAxis:0 startValue:0.0 endValue:readPlateauAmp area:-readPlateauAmp*readPlateauDuration/2.0];
  [prewinder setAxis:0 startValue:0.0 endValue:readPlateauAmp area:-readPlateauAmp*readPlateauDuration*(kxCoverageRounded-0.5f)/kxCoverageRounded];  
  [prewinder setAxis:1 startValue:0.0 endValue:0.0 area:peArea];

  SBOptimizedGradient *rewinder = [SBOptimizedGradient gradientWithAxes:2];
  [rewinder setLimitsForPulse:self interval:@"postReadout" constraints:SBScalableConstraint | SBNominalGradLimitConstraint];
	if(hasRewinder) {
    double postArea = -readPlateauAmp*readPlateauDuration/kxCoverageRounded/2.0 - netArea*readPlateauAmp*readPlateauDuration;
    [rewinder setAxis:0 startValue:readPlateauAmp endValue:0.0 area:postArea];
  } else {
    [rewinder setAxis:0 startValue:readPlateauAmp endValue:0.0];
  }
  [rewinder setAxis:1 startValue:0.0 endValue:0.0 area:-peArea];
  
  // CALCULATE ANCHOR TIMINGS
  // constrain prewinder size to 2us boundaries (should be 2*systemclock)
	readoutStartOffset = ceil([prewinder duration]*500.0)/500.0f;
	readoutEndOffset = readoutStartOffset+readPlateauDuration;
  // constrain rewinder size to 2us boundaries (should be 2*systemclock)
	endOffset = readoutEndOffset+ceil([rewinder duration]*500.0)/500.0f;
	
  const double newDuration = endOffset;
  if(newDuration != duration) {
    [self willChangeValueForKey:@"duration"];
    duration = newDuration;
    [self didChangeValueForKey:@"duration"];
  }

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

-(BOOL)validateStart:(id *)valObj error:(NSError **)outError
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

-(BOOL)validateHasPhaseEncoding:(id *)valObj error:(NSError **)outError
{
	if(*valObj == nil) *valObj = [NSNumber numberWithDouble:hasPhaseEncoding];
	return YES;
}

-(BOOL)validateHasRewinder:(id *)valObj error:(NSError **)outError
{
	if(*valObj == nil) *valObj = [NSNumber numberWithDouble:hasRewinder];
	return YES;
}

-(BOOL)validateFov:(id *)valObj error:(NSError **)outError
{
	double readGradLimit = [self gradLimitMagnitudeForInterval:@"readout"];
	if(*valObj == nil) *valObj = [NSNumber numberWithDouble:fov];
	double limit = samplingRate*1000.0/(readGradLimit)/[params gamma];
	if([*valObj doubleValue] < limit)
		*valObj = [NSNumber numberWithDouble:limit];
	return YES;
}

-(BOOL)validateSamplingRate:(id *)valObj error:(NSError **)outError
{
	if((*valObj == nil) || ([*valObj doubleValue] <= 0.0))
    *valObj = [NSNumber numberWithDouble:samplingRate];
	double limit = [params maxReadoutRate];
	if([*valObj doubleValue] > limit)
		*valObj = [NSNumber numberWithDouble:limit];
	return YES;
}

-(BOOL)validateXRes:(id *)valObj error:(NSError **)outError
{
	if(*valObj == nil) *valObj = [NSNumber numberWithDouble:xRes];
	double val = [*valObj doubleValue];
  const int rsd = [params readoutSampleDivisor];
	if(val != maxint((int)rint(val/rsd)*rsd,rsd))
		*valObj = [NSNumber numberWithInt:maxint((int)rint(val/rsd)*rsd,rsd)];
	return YES;
}

-(BOOL)validateYRes:(id *)valObj error:(NSError **)outError
{
	if(*valObj == nil) *valObj = [NSNumber numberWithDouble:yRes];
	float val = [*valObj floatValue];
	if(val != maxint((int)rint(val),1))
		*valObj = [NSNumber numberWithInt:maxint((int)rint(val),1)];
	return YES;
}

-(BOOL)validateKxCoverage:(id *)valObj error:(NSError **)outError
{
	if(*valObj == nil) *valObj = [NSNumber numberWithFloat:kxCoverage];
	float val = [*valObj floatValue];

  if ([[NSNumber numberWithFloat:kyCoverage] floatValue] < 1.0f)
    *valObj = [NSNumber numberWithFloat:1.0f];
	else if(val < 0.5f)
		*valObj = [NSNumber numberWithFloat:0.5f];
  else if(val > 1.0f)
		*valObj = [NSNumber numberWithFloat:1.0f];
  return YES;
}

-(BOOL)validateKyCoverage:(id *)valObj error:(NSError **)outError
{
	if(*valObj == nil) *valObj = [NSNumber numberWithFloat:kyCoverage];
	float val = [*valObj floatValue];
 
  if ([[NSNumber numberWithFloat:kxCoverage] floatValue] < 1.0f)
    	*valObj = [NSNumber numberWithFloat:1.0f];
  else if(val < 0.5f)
		*valObj = [NSNumber numberWithFloat:0.5f];
  else if(val > 1.0f)
		*valObj = [NSNumber numberWithFloat:1.0f];
	return YES;
}

-(BOOL)validateNetArea:(id *)valObj error:(NSError **)outError
{
	if(*valObj == nil) *valObj = [NSNumber numberWithDouble:netArea];
	return YES;
}

-(double)duration
{
	return duration;
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
  float kxCoverageRounded = (double)samples/xRes;
	return start + (readoutStartOffset + (kxCoverageRounded-0.5f)*(readoutEndOffset-readoutStartOffset)/kxCoverageRounded);
}

- (double)readoutCenter
{
  float kxCoverageRounded = (double)samples/xRes;
	return start + (readoutStartOffset + (kxCoverageRounded-0.5f)*(readoutEndOffset-readoutStartOffset)/kxCoverageRounded);
}

- (void)setReadoutCenter:(double)val
{
  float kxCoverageRounded = (double)samples/xRes;
	start = val - (readoutStartOffset + (kxCoverageRounded-0.5f)*(readoutEndOffset-readoutStartOffset)/kxCoverageRounded);
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

- (int)samples
{
  return samples;
}

/*
-(double)plateauDuration
{
	return res/samplingRate;
}
*/
-(NSArray *)viewIndexKeys
{
	return [NSArray arrayWithObject:@"index"];
}

-(NSMutableArray *)tagsForTrNum:(int)trNum of:(int)numTr
{
	SBReadoutTag *tag = [SBReadoutTag tag];
	[tag setPluginName:[self editableName]];
	[tag setReadoutStart:[self readoutStart]];
	[tag setReadoutEnd:[self readoutEnd]];
	[tag setReadoutReferencePoint:[self readoutCenter]];
	[tag setPhase:[params receiverPhase]];
	[tag setSamplingRate:samplingRate];
	[tag setViewIndex:trNum of:numTr forKey:@"index" object:self];
	float *tagRes = [tag resolution];
	tagRes[0] = fov*10.0/xRes;
	tagRes[1] = fov*10.0/yRes;
	float *tagFov = [tag fov];
	tagFov[0] = fov;
	tagFov[1] = fov*numTr/yRes;
  float **kSpace = [tag allocateKSpaceWithLength:samples];
	float *kSpaceDensity = [tag kSpaceDensity];
  float kyCoverageRounded = ceilf(yRes*kyCoverage)/yRes;
	//double thisYPos = hasPhaseEncoding?((double)yRes/(double)xRes*0.5*((double)trNum/(((double)numTr/kyCoverageRounded)/2.0)-1.0)):0.0;
  // to match EPI convention, which allows for shorter TE
  // from -1 to 1-delta for full kspace, -0.xx to 1-delta for partial k-space
  double thisYPos;
  if(yRes < xRes) {
    thisYPos = hasPhaseEncoding?((double)yRes/(double)xRes*0.5*(1.0+2.0*kyCoverageRounded*(((double)trNum)/(double)numTr - 1.0))):0.0;
  } else {
    thisYPos = hasPhaseEncoding?(0.5*(1.0+2.0*kyCoverageRounded*(((double)trNum)/(double)numTr - 1.0))):0.0;
  }
  int i;
	for(i=0;i<samples;i++) {
    /* sampling time is defined at the center of the acquisition */
    if(yRes < xRes) {
      kSpace[0][i] = (double)(i+xRes-samples)/(double)xRes-0.5;
    } else {
      kSpace[0][i] = (double)xRes/(double)yRes*((double)(i+xRes-samples)/(double)xRes-0.5);
    }
    kSpace[1][i] = thisYPos;
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
	if(hasPhaseEncoding) {
		SBPulseData *outData = [pulseData subDataWithGradAxis:1];
    float kyCoverageRounded = ceilf(yRes*kyCoverage)/yRes;
    // to match EPI convention, which allows for shorter TE, see above
    //double thisScale = ((double)trNum/(((double)numTr/kyCoverageRounded)/2.0)-1.0);
    double thisScale = 1.0+2.0*kyCoverageRounded*(((double)trNum)/(double)numTr - 1.0);
    
    [outData setScaleFactor:thisScale forGradAxis:0];
    return outData;
	} else return nil;
}

- (void)setKxCoverage:(float)val
{
  if (val < 1.0f){
    [kyCoverageSlider setEnabled:NO];
    [kyCoverageTextValue setEnabled:NO];
  } else { 
    [kyCoverageSlider setEnabled:YES];
    [kyCoverageTextValue setEnabled:YES];
  }
  kxCoverage = val;
}

- (void)setKyCoverage:(float)val
{
  if (val < 1.0f){
    [kxCoverageSlider setEnabled:NO];
    [kxCoverageTextValue setEnabled:NO];
  } else {
    [kxCoverageSlider setEnabled:YES];
    [kxCoverageTextValue setEnabled:YES];
  }
  kyCoverage = val;
}



@end
