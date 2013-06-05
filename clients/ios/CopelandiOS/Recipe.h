//
//  Recipe.h
//  CopelandiOS
//
//  Created by Brock Boland on 5/5/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>


@interface Recipe : NSManagedObject

@property (nonatomic, retain) NSString * title;
@property (nonatomic, retain) NSString * body;
@property (nonatomic, retain) NSNumber * prepTime;
@property (nonatomic, retain) NSNumber * cookTime;
@property (nonatomic, retain) NSNumber * servings;
@property (nonatomic, retain) NSString * instructions;
@property (nonatomic, retain) NSString * source;
@property (nonatomic, retain) NSString * submittedBy;

@end
