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

#import <Cocoa/Cocoa.h>
#import "AppKit/NSStringDrawing.h"

@class SBVariableAxis;

#define XMINTICKS 4.0
#define YMINTICKS 4.0
#define BL_EDGEPAD 5.0  //absolute
#define TR_EDGEPAD 10.0 //absolute
#define BETWEENPAD 5.0  //absolute
//#define AUTOSCALE_PAD 0.05 //percentage
#define VALUECONST 9.0 // minimum value size, in pts
#define LABELCONST 12.0 // minimum value size, in pts
#define VALUELINEAR 0.005 // percentage
#define LABELLINEAR 0.005 // percentage
#define LABELFILL 0.85 // fill factor for labels (of the full LABELLINEAR*diag+LABELCONST allotted)
#define LABELHORIZFILL 0.88 // when labels are tightly spaced, what percentage of the space to try to use 
#define LABELMINHEIGHT 6.0 // if things are tightly spaced, how small do you go before going no smaller?
#define SCROLLWHEELSPEED 3.0

#define GRAPH_SEPARATOR 1.0 // absolute

enum SBBarModeEnum { SBNoBar, SBVerticalBar, SBTwoBar };
enum SBGraphXLabelEnum { SBNoXValues=0, SBAllXValues=1, SBBottomXValues=2 };

typedef struct _GraphInfo {
	void *dataSource;
	void *rawData;
	id objects;
  id objectDrawingInfo;
  id plotIndex;
	int numSecondary;
	BOOL yWrap;
	BOOL xAxisEnabled;
	float yMin,yMax;
	float yOrigin;
	int xCount;
	int yCount;
	int linkedFrom,linkedTo;
	BOOL dirty;
	NSMutableArray *yTickValues;
	NSMutableArray *yTickLabels;
  NSMutableArray *areaRects;
  NSMutableArray *areaRectColors;

	NSTextField *yLabelTextBox;
	NSTrackingRectTag yLabelTrackingRect;
	NSRect yLabelRect;
	BOOL highlightYLabel;
	float yValueLabelSize;
	SBVariableAxis *yAxis;
	NSTextField *yMinTextBox;
	NSTextField *yMaxTextBox;
	NSTrackingRectTag yMinTrackingRect;
	NSTrackingRectTag yMaxTrackingRect;
	NSRect yMinRect;
	NSRect yMaxRect;
	BOOL highlightYMin;
	BOOL highlightYMax;
} GraphInfo_type;

@interface SBGenericPlotView : NSView <NSTextFieldDelegate>
{
	int nominal_xRes,nominal_yRes; // these are the nominal sizes of the data array to be used _only_ for calculating graph size params and xybar positions; members of graphinfo struct indicate actual data size.  These update immediately when params change; others update only when data is available there
	BOOL needsUpdateSizeParams;
	BOOL showYRightLabel;
	BOOL axesInFront;
	int numGraphs;
	float width,height,diag;
	float xMin,xMax;
	float graphXSize,graphYSize;
	NSRect bounding_rect;
	float yBarPos,xBarPos;
	CGFloat dash[2];
	float xSelectedRange, ySelectedRange;
	id _delegate;
	float xEdgePad;
	float yEdgePad;
	float yLabelSize;
	float yRightLabelSize;
	float xValueLabelSize;
	float yAxisLoc;
	enum SBBarModeEnum barMode;
	NSString *disclosureString;
	NSMutableDictionary *labelAttrs;
	NSMutableArray *xTickValues;
	NSMutableArray *xTickLabels;
	NSTextField *xLabelTextBox;
	NSTextField *xMinTextBox;
	NSTextField *xMaxTextBox;
	NSTrackingRectTag xLabelTrackingRect;
	NSTrackingRectTag xMinTrackingRect;
	NSTrackingRectTag xMaxTrackingRect;
	NSRect xLabelRect;
	NSRect xMinRect;
	NSRect xMaxRect;
	BOOL xAxisHasMenu;
	BOOL highlightXLabel;
	BOOL highlightXMin;
	BOOL highlightXMax;
	BOOL yAxisHasMenu;
	enum SBGraphXLabelEnum showXValues;
	BOOL showYValues;
	NSDictionary *validationDict;
	SBVariableAxis *xAxis;
	NSMutableString *xLabel;
	NSMutableArray *yLabels;
	NSMutableArray *yRightLabels;
	NSArray *pasteBoardTypes;
	GraphInfo_type *GraphInfo;
}

- (int)numGraphs;
- (void)setNumGraphs:(int)_numGraphs;
- (void)setShowXValues:(enum SBGraphXLabelEnum)show;
- (void)setShowYValues:(BOOL)show;
- (void)setWrap:(BOOL)ywrap forGraph:(int)idx;
- (void)updateSizeParams;
- (float)getX:(NSPoint)pt;
- (BOOL)drawGraphNum:(int)grNum inRect:(NSRect)rect; //must subclass
- (void)setNominalXRes:(int)_xres yRes:(int)_yres;
- (float)getBoundingBoxWidth;
- (float)getBoundingBoxHeight;

- (float)xRangeMin;
- (float)xRangeMax;
- (float)yRangeMin:(int)grNum;
- (float)yRangeMax:(int)grNum;

/**
 *    @brief     Adds an area rectangle to be displayed on a specific plot
 *    @details   Area rectangles are specified in the units of the underlying graph, and they appear in the color specified by this method.  Area rectangles acccumulate when added by this method until they are cleared using -clearAreaRectsForGraph:.
 *    @param rect Specifies the rectangle to be displayed, in graph units
 *    @param color Specifies the color and transparency of the given rectangle
 *    @param idx Specifies the graph index of the graph to be annotated with the rectangle.
 */
- (void)addAreaRect:(NSRect)rect color:(NSColor *)color forGraph:(int)idx;

/**
 *    @brief     Clears the collection of area rectangles for a specific plot
 *    @details   Area rectangles added using addAreaRect:color:forGraph: may be cleared using this method.  There is currently no way to remove individual area rectangles.
 *    @param idx Specifies the graph index of the graph whose area rectangles are to be removed.
 */
- (void)clearAreaRectsForGraph:(int)idx;

- (void)setXAxis:(id)_xAxis; // argument can be either an SBVariableAxis or an NSString.  If it's a SBVariableAxis, then name editing and min/max scaling are enabled
- (void)setYAxis:(id)_yAxis forGraph:(int)idx;
- (void)setYRightLabel:(NSString *)_label forGraph:(int)idx;
- (void)setXRangeMin:(float)min max:(float)max tickLocs:(NSArray *)_tickLocs tickLabels:(NSArray *)_tickLabels;
- (void)setXRangeMin:(float)min max:(float)max preferredSpacing:(float)ps;
- (void)setYRange:(int)grNum min:(float)min max:(float)max tickLocs:(NSArray *)_tickLocs tickLabels:(NSArray *)_tickLabels;
- (void)setYRange:(int)grNum min:(float)min max:(float)max preferredSpacing:(float)ps;
- (void)setBarMode:(enum SBBarModeEnum)mode;
- (float)xBarPos;
- (float)yBarPos;
- (float)xSelectedRange;
- (float)ySelectedRange;
- (void)setXBarPos:(float)val;
- (void)setYBarPos:(float)val;
- (void)setXSelectedRange:(float)val;
- (void)setYSelectedRange:(float)val;
//- (void)removeObservers;

//subclass these to provide functionality
- (BOOL)horizRangeIsSelectable;
- (BOOL)vertRangeIsSelectable;

- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent;
- (id)delegate;
- (void)setDelegate:(id)new_delegate;

@end

@interface NSObject (SBGenericPlotViewDelegate)

- (void)mouseSelection:(id)sender event:(NSEvent *)theEvent xPos:(float)xp yPos:(float)yp shiftDown:(BOOL)sd;
- (void)scrollWheelEvent:(NSEvent *)theEvent;
- (NSMenu *)xAxisContextualMenu;
- (NSMenu *)yAxisContextualMenuForGraph:(int)graphNo;

@end
