//
//  UIViewController+Evergage.h
//  Evergage
//
//  Copyright © 2015-2021 Salesforce.com, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <Evergage/EVGItems.h>
#import <Evergage/EVGScreen.h>

// Reference to avoid app requiring -ObjC flag to link this category
FOUNDATION_EXPORT void * __nonnull const EVGCategoryUIViewControllerEvergage;


/**
 `UIViewController` category used to enable Evergage to automatically track screens, dynamically map them to actions,
 and provide APIs for tracking and campaigns.
 
 See `<evergageScreen>` for access to `<EVGContext>` APIs, and the <Tracking> Guide
 for more info about dynamically mapping screens to view actions.
 */
@interface UIViewController (Evergage)

/**
 A `<EVGSCreen>` `<EVGContext>` for tracking and personalization, when an app can use a `UIViewController` for each screen/page of the app.
 
 An app that can not use a `UIViewController` for each screen/page of the app can instead use `<[Evergage globalContext]>`.
 
 UI-related and lazily created on first access from main thread.
 
 Returns `nil` if Evergage is disabled, or not yet accessed from main thread.
 
 @see EVGScreen
 @see EVGContext
 */
@property (nullable, nonatomic, readonly, strong) EVGScreen *evergageScreen;

/**
 The optional 'Evergage Name' for this UIViewController.
 
 Only relevant if using "Code-less Screen Upload and Action Mapping", see the <Tracking> Guide for more info.
 
 Uniquely naming your UIViewControllers may make action-mapping screens in the Evergage web-app even faster and easier.
 
 Should be set before viewDidAppear, such as within Interface Builder (Attribute Inspector on the view controller)
 or programmatically in viewDidLoad or viewWillAppear.
 */
@property (nullable, atomic, copy) IBInspectable NSString *evergageName;

@end
