//
//  Evergage.h
//  Evergage
//
//  Copyright © 2015-2021 Salesforce.com, Inc. All rights reserved.
//

// Required system frameworks:
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
// SystemConfiguration for monitoring network connectivity.
#import <SystemConfiguration/SystemConfiguration.h>
// CoreGraphics and QuartzCore for In-App-Message campaign rendering.
#import <CoreGraphics/CoreGraphics.h>
#import <QuartzCore/QuartzCore.h>
// libz (libz.tbd) will be used for gzip compression to reduce bandwidth
#import <zlib.h>

// Optional frameworks:
// UserNotifications for apps using it for push notifications.
// Can be weak linked. Safely ignored for iOS <10.
#import <UserNotifications/UserNotifications.h>
// FirebaseMessaging for apps using it for push notifications.
// Safely ignored for apps not using FirebaseMessaging.
//@import FirebaseMessaging;

#import <Availability.h>
#if defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 120000
#error "Evergage Framework requires iOS Deployment Target >= 12.0"
#endif

// Other public Evergage headers:
#import <Evergage/EVGClientConfigurationBuilder.h>
#import <Evergage/UIViewController+Evergage.h>
#import <Evergage/EVGScreen.h>
#import <Evergage/EVGContext.h>
#import <Evergage/EVGItems.h>
#import <Evergage/EVGCampaign.h>
#import <Evergage/EVGConstants.h>
// Also see <Evergage/Evergage+Swizzling.h>

FOUNDATION_EXPORT NSString * _Nonnull const EvergageVersion; // 1.4.1

/**
 This class is primarily used to configure and start Evergage.
 
 Tracking user interactions should ideally be done from a `UIViewController`,
 see `<[UIViewController(Evergage) evergageScreen]>` vs `<globalContext>`.  See `<EVGContext>` for the APIs.
 
 ## Startup
 
 As early as possible, ideally within `UIApplicationDelegate -willFinishLaunchingWithOptions:` :
 
 - Enable logging in DEBUG builds
 - Determine if the user is a returning known/authenticated user. If so, set `<userId>` as soon as possible, ideally before 'start' (next step).
 - Determine whether your user has consented to services. If so, provide the app's Evergage configuration and start Evergage using `<-startWithClientConfiguration:>`. Ideally this is before the user begins interacting with your app.

 ### Obj-C:
    // In AppDelegate.m
    #import <Evergage/Evergage.h>
 
    - (BOOL)application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
 
        Evergage *evergage = [Evergage sharedInstance];

        #ifdef DEBUG
        evergage.logLevel = EVGLogLevelWarn;
        #endif
 
        // Recommended to set the authenticated user's ID as soon as known:
        evergage.userId = @"theAuthenticatedUserId";
 
        // If using Firebase Messaging, be sure to uncomment and do these before starting Evergage:
        // [FIRApp configure];
        // [FIRMessaging messaging].delegate = self; // Or whatever your delegate is
 
        // Start Evergage with your Evergage Configuration:
        [evergage startWithClientConfiguration:^(EVGClientConfigurationBuilder * _Nonnull builder) {
            builder.account = @"yourEvergageAccount";
            builder.dataset = @"yourEvergageDataset";
            builder.usePushNotifications = YES; // If you want to use Evergage push notification campaigns
        }];

        // ... existing code from your app starts here

        return YES;
    }
 
 ### Swift:
    // In AppDelegate.swift:
    import Evergage

    func application(_ application: UIApplication, willFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey : Any]? = nil) -> Bool {
 
        let evergage = Evergage.sharedInstance()
 
        // For the DEBUG conditional to work in Swift, you must edit your project/target's Build Settings:
        // Expand "Other Swift Flags" and add "-DDEBUG" under Debug. Do not add to Release!
        #if DEBUG
        evergage.logLevel = EVGLogLevel.warn
        #endif
 
        // Recommended to set the authenticated user's ID as soon as known:
        evergage.userId = "theAuthenticatedUserId"
 
        // If using Firebase Messaging, be sure to uncomment and do these before starting Evergage:
        // FirebaseApp.configure()
        // Messaging.messaging().delegate = self // Or whatever your delegate is
 
        // Start Evergage with your Evergage Configuration:
        evergage.start { (clientConfigurationBuilder) in
            clientConfigurationBuilder.account = "yourEvergageAccount"
            clientConfigurationBuilder.dataset = "yourEvergageDataset"
            clientConfigurationBuilder.usePushNotifications = true // If you want to use Evergage push notification campaigns
        }
 
        // ... existing code from your app starts here
 
        return true
    }
 
 ## User Info
 To add information about the user, see `<userId>` and `<setUserAttribute:forName:>`.
 In the examples above, setting the userId provides Evergage with the user's authenticated ID. This could also
 have been called later, if the user's ID was not known at startup, and would then be sent with subsequent events.
 Likewise, if your users belong to an account, see `<accountId>` and `<setAccountAttribute:forName:>`.
 
 ## Testing
 To easily and codelessly manage test campaigns from the device, the app can add an
 Evergage URL scheme and define one of the iOS open-url delegate methods, see `<[Evergage(Swizzling) handleOpenURL:]>`.
 See <Testing> Guide for more info.
 
 ## Lifecycle
 Once `<-startWithClientConfiguration:>` has been called at app launch, the Evergage client will
 track user activity, send any applicable events to the Evergage server, and support campaigns in response.
 Evergage will monitor network availability and store events if necessary, sending them when the network becomes available again.
 
 ## Tracking Screens
 The Evergage client supports automatically tracking UIViewControllers and dynamically mapping and re-mapping them
 to view actions using the Evergage web app.
 See the <Tracking> Guide, `<UIViewController(Evergage)>`, and `<EVGScreen>`.

 ## Tracking Item Views and Activity
 Evergage can track how the user views and interacts with articles, blogs and products which are collectively called items.
 Evergage understands the actions that are possible on these items (View, Comment, Purchase, etc.) and also how they
 relate to each other (categories, brands, keywords, etc.).  See <Tracking> Guide and `<EVGContext>`.
 
 ## Campaigns
 Campaigns may be served in response to actions generated by the user interacting with the app:
 
 - 'In-App Message' campaigns are automatically handled by Evergage.
 - 'Push Notification' campaigns require APNS setup, and optionally Firebase.
   See [Push Notifications](../docs/doc/Push-Notifications.html) Guide
 - Custom 'Data' campaigns are handled by app-defined code. See
   [Mobile Data Campaigns](../docs/doc/Mobile-Data-Campaigns.html) Guide and `<EVGCampaign>`.
 
 */
@interface Evergage : NSObject

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability"
#pragma clang diagnostic ignored "-Wnullability-completeness"
- (nullable instancetype)init __attribute__((unavailable("Use sharedInstance to obtain Evergage singleton")));
#pragma clang diagnostic pop


/// ---------------------------------
#pragma mark - Startup
/// @name Startup
/// ---------------------------------

/**
 @return The Evergage singleton instance.
 */
+ (nonnull instancetype)sharedInstance;

/**
 Starts Evergage with the specified configuration.
 
 Before calling this, the app should determine that the user has consented to services.
 
 Recommended to call from `willFinishLaunchingWithOptions:`, see Evergage
 class/header doc for example code.
 
 Once started, subsequent calls will have no effect, unless `<reset>` has been
 called due to an environment change.
 
 @param configurationBlock Client-specific Evergage configuration defined via EVGClientConfigurationBuilder
 @see EVGClientConfigurationBuilder
 @see reset
 @since 1.3.0
 */
- (void)startWithClientConfiguration:(nonnull EVGClientConfigurationBlock)configurationBlock;

/**
 Deprecated, use `<startWithClientConfiguration:>` instead.
 
 @param accountKey Account within Evergage to use.
 @param dataset Dataset within the Evergage account to use.
 @deprecated As of 1.3.0
 @see startWithClientConfiguration:
 */
- (void)startWithEvergageAccountKey:(nonnull NSString *)accountKey dataset:(nonnull NSString *)dataset DEPRECATED_MSG_ATTRIBUTE("Instead use startWithClientConfiguration:");


/// ---------------------------------
#pragma mark - User Info
/// @name User Info
/// ---------------------------------

/**
 The user's authenticated ID. Setting this property is critical to correlate the same user's activity
 across devices and platforms, and also makes it easier to find a user in Evergage.
 
 When the authenticated ID is null, Evergage identifies the user with a generated anonymous ID.
 See `<anonymousId>` for details, including how activity may be merged.
 
 You may call this method with null to make the user anonymous again. At that point, all new
 activity will be attributed to the anonymous user, and push notifications can only be sent
 to the anonymous user (based off the anonymous activity). So after a simple 'log out',
 you may wish to continue to call this method with the previously-authenticated user ID,
 to continue to be able to send push notifications to the authenticated user (based off the
 authenticated activity) etc. You decide when the user becomes truly anonymous again.
 
 Recommended to set this property:
 
 - With the ID, when the user successfully authenticates (logs in).
 - With the ID, on app launch as soon as possible, if the user is still authenticated,
   or previously authenticated but needs to log back in again.
   Evergage does not persist the ID across app restarts.
 - With nil, when the app decides the user should be anonymous again. This may not be on
   logout - see note about anonymous activity above.
   Calling this method with nil will also set `<accountId>` to nil.
 
 @see anonymousId
 */
@property (nullable, atomic, copy) NSString *userId;

/**
 The user's anonymous ID, via `identifierForVendor`, which will be used if no
 authenticated ID is specified for the user via `<userId>`.
 
 May return nil if Evergage is disabled.
 
 When a user transitions from anonymous to authenticated (via `<userId>`),
 the previous activity while anonymous will be merged into the authenticated user
 if 'Merge Anonymous Users' is enabled in the Evergage web app.
 
 This ID should be passed in SmartSearch requests if `<userId>` returns nil.
 
 @see userId
 @since 1.2.1
 */
@property (nullable, readonly) NSString *anonymousId;

/**
 The optional account this user belongs to. Set this property to track which of your accounts users belong to inside the Evergage
 dataset and account.  If account is no longer applicable, it can be set to nil.
 Note this is not the main Evergage `account` passed in to `<startWithClientConfiguration:>`
 */
@property (nullable, atomic, copy) NSString *accountId;

/**
 Sets an attribute (a name/value pair) on the user. The new value will be sent to the Evergage server with the next
 event.
 
 @param attributeValue The new value of the user attribute, which replaces any existing value.  Set nil to clear.
 @param attributeName The name for the user attribute.
 */
- (void)setUserAttribute:(nullable NSString *)attributeValue forName:(nonnull NSString *)attributeName;

/**
 Sets an attribute (a name/value pair) on the account. The new value will be sent to the Evergage server with the next
 event.
 
 @param attributeValue The new value of the account attribute, which replaces any existing value.  Set nil to clear.
 @param attributeName The name for the account attribute.
 */
- (void)setAccountAttribute:(nullable NSString *)attributeValue forName:(nonnull NSString *)attributeName;


/// ---------------------------------
#pragma mark - Tracking Non-Screen Actions
/// @name Tracking Non-Screen Actions
/// ---------------------------------

/**
 A `<EVGContext>` that is not a `<EVGScreen>` for tracking and personalization, when an app can not use a `UIViewController` for each screen/page of the app.
 
 Property returns `nil` if the SDK is disabled.
 
 An app that can use a `UIViewController` for each screen/page of the app should instead use the `<EVGScreen>` returned by `<[UIViewController(Evergage) evergageScreen]>`, which automatically cleans up resources and stops timing/tracking on `UIViewController` disappear.
 
 However, an app might use a development framework that does not produce an accessible `UIViewController` per screen, or use a decoupled architecture where no `UIViewController` is easily accessible. In such scenarios, this `globalContext` can be used.

 Using the `globalContext` means losing some cleanup/safety mechanisms the SDK can automatically provide for a `<EVGScreen>`, so you must manually do these operations, as applicable:

 - Stop accumulating view time on an item.
   - After calling any of the View APIs (`<[EVGContext viewItem:]>` etc), you'll need to indicate when the item is no longer being viewed by calling a View API method again, supplying `nil` for the item parameter: `viewItem:nil`
 - Remove the `<EVGCampaignHandler>` when its lifecycle should end.
   - After using `<[EVGContext setCampaignHandler:forTarget:]>`, you'll need to indicate when the handler should be removed by calling it again, supplying `nil` for the handler: `setCampaignHandler:nil forTarget:@"sameTargetUsedEarlier"`
   - If the handler concerns a particular screen/UI of the app, it should be removed when the screen/UI would be removed. Otherwise the handler might contain strong references, preventing objects from being deallocated/released, and possibly even trying to handle a campaign which could result in crashes or unexpected behavior from UI-related objects being used beyond their expected lifecycle.
 
 @see [UIViewController(Evergage) evergageScreen]
 @since 1.2.0
 */
@property (nullable, readonly) EVGContext *globalContext;


/// ---------------------------------
#pragma mark - Developer Utilities
/// @name Developer Utilities
/// ---------------------------------

/**
 Configures the threshold `<EVGLogLevel>` of messages to log. Will only log messages at this level of severity or greater.
 
 Recommended to set only during development in DEBUG builds.
 */
@property EVGLogLevel logLevel;

/**
 Resets the SDK so `<startWithClientConfiguration:>` can be called again with a different dataset, in order to
 support apps that change environments (production, demo, QA, etc) and want to change Evergage datasets as well.
 
 Reset will clear settings, unsent actions, held campaigns, test campaign settings, allowDesignConnections, userId, accountId, and unsent attributes.
 
 Reset will keep existing campaign handlers, screen visibility, and items being viewed, since it would potentially be cumbersome/confusing to require
 once again setting handlers, potentially mocking viewWillAppear or re-navigating to screens, re-setting items being viewed, etc.

 Ideally the app will avoid environment churn:
 
 - On app launch, if the user must select/confirm the environment before using the app, consider delaying the
   call to `<startWithClientConfiguration:>` to when the environment is chosen and avoid calling reset.
 - Otherwise, if the user can immediately use the app with the current environment, call `<startWithClientConfiguration:>`
   as normal with the corresponding dataset.  And when the environment later changes, and a different Evergage dataset
   is desired, call reset as demonstrated below.
 
 #### Recommended Usage:
    // If the app is changing to a different server environment and a different Evergage dataset is desired..
    
    // Reset Evergage SDK just before changing the environment:
    Evergage *evergage = [Evergage sharedInstance];
    [evergage reset];
 
    // ... app changes server environment
 
    // Start Evergage SDK again with the dataset to use with the negitw environment:
    // See Evergage class doc for full example code.
    evergage.userId = ...;
    [evergage startWithClientConfiguration:...];
 */
- (void)reset;

/**
 Deprecated, instead use `<startWithClientConfiguration:>` with `<[EVGClientConfigurationBuilder useDesignMode]>`.
 
 Recommended to use only during development in DEBUG builds.
 @deprecated as of 1.3.0
 @see startWithClientConfiguration:
 @see [EVGClientConfigurationBuilder useDesignMode]
 */
- (void)allowDesignConnections DEPRECATED_MSG_ATTRIBUTE("Instead use startWithClientConfiguration:");

@end
