//
//  Recipe.h
//  CopelandiOS
//
//  Created by Brock Boland on 6/14/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>


@interface Recipe : NSManagedObject

@property (nonatomic, retain) NSString * body;
@property (nonatomic, retain) NSNumber * cookTime;
@property (nonatomic, retain) NSString * instructions;
@property (nonatomic, retain) NSString * nid;
@property (nonatomic, retain) NSNumber * prepTime;
@property (nonatomic, retain) NSNumber * servings;
@property (nonatomic, retain) NSString * source;
@property (nonatomic, retain) NSString * submittedBy;
@property (nonatomic, retain) NSString * title;
@property (nonatomic, retain) NSSet *ingredients;
@end

@interface Recipe (CoreDataGeneratedAccessors)

- (void)addIngredientsObject:(NSManagedObject *)value;
- (void)removeIngredientsObject:(NSManagedObject *)value;
- (void)addIngredients:(NSSet *)values;
- (void)removeIngredients:(NSSet *)values;

@end
