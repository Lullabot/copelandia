//
//  Ingredient.h
//  CopelandiOS
//
//  Created by Brock Boland on 6/14/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

@class Recipe;

@interface Ingredient : NSManagedObject

@property (nonatomic, retain) NSString * amount;
@property (nonatomic, retain) NSString * ingredient;
@property (nonatomic, retain) Recipe *recipe;

@end
