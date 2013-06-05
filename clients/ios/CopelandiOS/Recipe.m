//
//  Recipe.m
//  CopelandiOS
//
//  Created by Brock Boland on 5/5/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import "Recipe.h"


@implementation Recipe

@dynamic title;
@dynamic body;
@dynamic prepTime;
@dynamic cookTime;
@dynamic servings;
@dynamic instructions;
@dynamic source;
@dynamic submittedBy;

-(void)awakeFromInsert {
    [super awakeFromInsert];

    // custom initialization of object
}

@end
