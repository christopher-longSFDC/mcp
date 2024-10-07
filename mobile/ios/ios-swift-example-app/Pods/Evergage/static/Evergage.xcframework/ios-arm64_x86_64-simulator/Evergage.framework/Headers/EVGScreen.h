//
//  EVGScreen.h
//  Evergage
//
//  Copyright © 2015-2021 Salesforce.com, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <Evergage/EVGContext.h>
#import <Evergage/EVGCampaign.h>
#import <Evergage/EVGItems.h>


/**
 A `<EVGContext>` associated with a `UIViewController` for tracking and personalization, when an app can use a `UIViewController` for each screen/page of the app.
 
 Retrieved via `<[UIViewController(Evergage) evergageScreen]>`.
 
 See `<EVGContext>` for APIs to track item views and activity, track manual actions, and handle data campaigns.
 
 See `<[Evergage globalContext]>` for an app that can not use a `UIViewController` for each screen/page of the app.
 */
@interface EVGScreen : EVGContext

/**
 If the screen is visible.
 
 Evergage automatically:
 
 - Updates visibility during `[super viewDidAppear:]` and `[super viewWillDisappear:]`, for eligible view controllers.
 - Tracks the 'view action' when the screen becomes visible and the app is/becomes active, if an action is dynamically mapped to the screen using the Evergage web app.
   See <Tracking> Guide for more details.
 */
@property (readonly, getter=isVisible) BOOL visible;

@end
