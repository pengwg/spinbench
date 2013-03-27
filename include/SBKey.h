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

@class SBParamsObject;

/**
 *   @class       SBKey 
 *   @brief       Class for representing and manipulating SBKey references
 *   @details     SBKey provides a stable reference to a specific key for classes such as #SBVariableAxis.  When the reference is to a plugin, the reference follows a specific instance of the plugin even as its editableName changes.  When that happens, the SBKey sends SBKeyDidChangeNotification with itself as associated object.
 *   @author      HeartVista, Inc.
 *   @see         SBParamsObject
 *   @see         SBSpinModel
 */
@interface SBKey : NSObject {
@private
  NSString *keyPath;
  NSString *key;
  id owner;
  SBParamsObject *params;
  id parent;
  NSNotificationCenter *nc;
}

/**
 *    @brief     Initializes a new instance to default values
 *    @param     _params The #SBParamsObject associated with the key.  May not be nil.
 *    @param     _parent The parent object.  This parent may not be nil but may be the same object as _params.
 *    @result    An #SBKey object containing default-initialized parameters (by default, points to SBParamsObject#tr.
 *    @note      Designated initializer.
 */
- (id)initWithParams:(SBParamsObject *)_params parent:(id)_parent;

- (id)owner;
- (NSString *)key;
- (NSString *)keyPath;
- (NSMutableArray *)keyList;

- (NSString *)units;
- (BOOL)isAngle;

- (BOOL)validateValue:(id *)valObj error:(NSError **)outError;
- (void)setValue:(id)val;
- (id)value;
- (float)floatValue;

- (void)setKeyPath:(NSString *)val;

- (void)addViewObservers;
- (void)addObservers;
- (void)removeObservers;

@end
