//
//  COPRecipeDisplayEditViewController.m
//  CopelandiOS
//
//  Created by Brock Boland on 5/5/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import "COPRecipeDisplayEditViewController.h"
#import "COPAppDelegate.h"

@interface COPRecipeDisplayEditViewController ()

@end

@implementation COPRecipeDisplayEditViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    
    self.titleField.text = self.currentRecipe.title;
    [self.bodyWebView loadHTMLString:self.currentRecipe.body baseURL:nil];
    self.submittedByField.text = self.currentRecipe.submittedBy;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
