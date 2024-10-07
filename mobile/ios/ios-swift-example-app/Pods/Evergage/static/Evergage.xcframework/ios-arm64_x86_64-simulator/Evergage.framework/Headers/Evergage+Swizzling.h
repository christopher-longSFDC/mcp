//
//  Evergage+Swizzling.h
//  Evergage
//
//  Copyright © 2018-2021 Salesforce.com, Inc. All rights reserved.
//

#import <Evergage/Evergage.h>

// Reference to avoid app requiring -ObjC flag to link this category
FOUNDATION_EXPORT void * __nonnull const EVGCategoryEvergageSwizzling;

/**
 'Swizzling' is a technique that Evergage can use to automatically listen for
 info it needs to support Evergage features you've enabled. Swizzling eliminates
 the need for you to manually forward info to Evergage via API calls.
 
 ## What Evergage Swizzles
 
 - Open URL for Testing
    - See `<handleOpenURL:>` for details. Your app must still declare one of the
      iOS open-URL delegate methods to allow Evergage to 'listen' as well.
    - This swizzling can be disabled, see `<swizzlingEnabled>`.
 - Push Notifications
    - APNS token
       - See `<setAPNSToken:>` for details. Your app must still declare the
         delegate method to allow Evergage to 'listen' as well.
    - Firebase token
       - See `<setFirebaseToken:>` for details. Your app must still declare the
         delegate method to allow Evergage to 'listen' as well.
    - Notification received
       - See `<handleNotification:actionIdentifier:>` for details. Your app must
         still declare one of the iOS notification-received methods to allow
         Evergage to 'listen' as well.
    - This swizzling can be disabled, see `<swizzlingEnabled>`.
 - UIViewController
    - `viewDidAppear:` and `viewWillDisappear:`. To manage the lifecycle of the
      associated `<[UIViewController(Evergage) evergageScreen]>`, stopping view
      time tracking and releasing resources as appropriate.
    - Note iOS unfortunately does not enforce classes to call `[super viewDidAppear:]`
      and `[super viewWillDisappear:]`, despite documentation stating "you must call super
      at some point in your implementation". Be sure your classes comply.
    - This swizzling currently cannot be disabled.
 
 ## Swizzle Safety
 Evergage takes care to swizzle safely:
 
 - Evergage will only extend found methods - If your app isn't declaring some
   method to discover info, we won't add a listener for you. While this approach
   may require slightly more app code, it makes clear what Evergage can listen to,
   and avoids potential issues where adding a new method could change behavior
   (for example, adding a 'higher-priority' callback resulting in some other
   callback not getting called at all).
 - Swizzle exactly once, using dispatch_once
 - For static classes, swizzle in a category's +load method
 - For dynamic delegate classes discovered at runtime:
    - If the delegate already exists, we dispatch_once swizzle the delegate class
    - Otherwise, we dispatch_once swizzle `setDelegate:`, and when we see the
      first non-nil delegate, we dispatch_once swizzle the delegate class
    - We only ever swizzle the first non-nil delegate. We noticed some other 3rd
      party libraries replacing a delegate with a proxy to the original delegate,
      and swizzling both classes led to double-processing.
 
 ## Swizzle Code
 Below you can see a simplified version of how Evergage swizzles a method.
 Evergage also takes precautions to try/catch NSExceptions, etc.
 
    @implementation SomeClass (Evergage)
    + (void)load {
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            __block IMP originalIMP = [EVGSwizzler replaceMethod:@selector(someMethod:) inClass:[self class] withBlock:^(id _self, id someParam) {
                // Call original implementation:
                ((void(*)(id,SEL,id))originalIMP)(_self, @selector(someMethod:), someParam);
 
                // ... Evergage processing ...
            }];
        });
    }
    @end
 
    @implementation EVGSwizzler
    + (nullable IMP)replaceMethod:(nonnull SEL)selector inClass:(nonnull Class)cls withBlock:(nonnull id)block {
        // Finds method on this class or superclass
        Method originalMethod = class_getInstanceMethod(cls, selector);
        if (!originalMethod) {
            // No orignal method found, abort
            return nil;
        }
 
        IMP originalIMP = method_getImplementation(originalMethod);
        if (!originalIMP) {
            // No original implementation found, abort
            return nil;
        }
 
        // Replaces the implementation with the block, which calls the returned original implementation
        IMP newIMP = imp_implementationWithBlock(block);
        class_replaceMethod(cls, selector, newIMP, method_getTypeEncoding(originalMethod));
        return originalIMP;
    }
    @end
 
 @see swizzlingEnabled
 @since 1.3.0
 */
@interface Evergage (Swizzling)

/**
 If swizzling is enabled for Evergage iOS.
 
 Defaults to YES (enabled). To disable, add "EvergageSwizzlingEnabled" to the app's
 Info.plist file with a boolean value of NO, and see Evergage(Swizzling) category/header
 doc for more details, including additional code you must write to forward info to Evergage.
 @since 1.3.0
 */
+ (BOOL)swizzlingEnabled;


/// ---------------------------------
#pragma mark - URLs for Testing
/// @name URLs for Testing
/// ---------------------------------

/**
 You do not normally need to call this method, but simply ensure your
 UIApplicationDelegate defines an open-URL method, and add your app's Evergage URL scheme.
 
 By default, Evergage will swizzle any of these open-URL methods you define:
 
 - `application:openURL:options:`
 - `application:openURL:sourceApplication:annotation:`
 - `application:handleOpenURL:`
 
 However, if you disable swizzling in the Info.plist (see `<swizzlingEnabled>`),
 then you will need to manually call this method from your open-URL method.
 
 This method enables Evergage to handle Evergage-related URLs. Used so test campaigns can easily
 and codelessly be managed by opening URLs in mobile Safari on the device.
 
 Your app must also add support for this Evergage URL scheme, see <Testing> Guide for more info.
 
 For the URL formats below:
 
 - `<URLScheme>` is the Evergage-generated URL scheme for the app, which is located
 in the Evergage web app: Select Dataset, then
 Settings > Sources > Apps > (this app) > URL Scheme: (format "evgxxxxx").
 - `<ExpID>` is the ID of a specific campaign experience, which can be found in the
 Evergage web app: Select Dataset, then Campaigns > Campaign Summary of the campaign the experience is in.
 - These URLs do not aggregate or combine. The most recent one determines the behavior.
 - Testing lasts for 30 minutes, or until app termination or another test URL is entered.
 
 There are three test URL formats:
 
 - Test All Campaigns:
 
 `<URLScheme>://test/all`
 
 In addition to default behavior of showing mobile campaigns in Published state,
 also show mobile campaigns that are in Test state.  All campaign rules will still be active.
 
 - Test a Specific Experience:
 
 `<URLScheme>://test/<ExpId>`
 
 In addition to default behavior of showing mobile campaigns in Published state,
 also show the particular experience of a mobile campaign, regardless of that
 campaign's state and rules that normally determine when its shown.
 
 - Stop Testing:
 
 `<URLScheme>://test/none`
 
 Return to the default behavior of showing mobile campaigns in Published state only.
 
 @param url The URL that the app is opening via `application:openURL:options:` (iOS9+),
 `application:openURL:sourceApplicatoin:annotation:`, or `application:handleOpenURL:`.
 @return YES if the URL was Evergage specific
 @see Testing Guide
 @since 1.2.0 (moved from Evergage.h)
 */
- (BOOL)handleOpenURL:(nonnull NSURL *)url;


/// ---------------------------------
#pragma mark - Push Notifications
/// @name Push Notifications
/// ---------------------------------

/**
 You do not normally need to call this method. If your app uses Apple Push
 Notification Service (APNS), simply ensure:
 
 - The app's UIApplicationDelegate defines the method
  `application:didRegisterForRemoteNotificationsWithDeviceToken:`
 - The app calls `registerForRemoteNotifications` on `UIApplication` regularly on/after launch,
   as applicable (perhaps after user has seen notification onboarding and opted-in, etc).  This will lead to
   periodic calls to the above delegate method, which provides the ability to react off of permission changes etc.
 - For iOS 10+ you use the `UNUserNotificationCenter` to get user permission for displaying notifications.
 
 By default, Evergage will swizzle the method application:didRegisterForRemoteNotificationsWithDeviceToken:`,
 if defined. However, if you disable swizzling in the Info.plist (see `<swizzlingEnabled>`), then you will
 need to call this method directly to provide the APNS token.
 
 This method notifies Evergage when the app's APNS token changes, in order to
 support Evergage push notification campaigns.
 
 This method will have no effect if you do not enable support for push notifications,
 see `<[EVGClientConfigurationBuilder usePushNotifications]>`.
 
 Evergage will only send push notifications to the most recent user.
 See `<[Evergage userId]>` for details.
 
 While Evergage automatically tracks clicks/opens for typical push notifications, see
 `<[EVGContext trackClickthrough:actionIdentifier:]>` for when
 you must call a method to track the click.
 
 This method can automatically determine if the token is for APNS production vs APNS sandbox.
 
 See [Push Notifications](../docs/doc/Push-Notifications.html) Guide.
 
 @param token The APNS token as received from iOS.
 @see [EVGClientConfigurationBuilder usePushNotifications]
 @see [Evergage userId]
 @see [EVGContext trackClickthrough:actionIdentifier:]
 @see setFirebaseToken:
 @since 1.3.0
 */
- (void)setAPNSToken:(nonnull NSData *)token;

/**
 You do not normally need to call this method. If the app uses Firebase
 Messaging, simply ensure your FIRMessagingDelegate defines the method
 `messaging:didReceiveRegistrationToken:`.
 
 This method notifies Evergage when the app's Firebase Messaging token changes, in order
 to support Evergage push notification campaigns using Firebase. Note Evergage
 also supports APNS with/without Firebase, and requires an APNS token even when using Firebase,
 see `<setAPNSToken:>`.
 
 By default, Evergage will swizzle your FIRMessagingDelegate's `messaging:didReceiveRegistrationToken:`
 method, if defined. However, if you disable swizzling in the Info.plist (see `<swizzlingEnabled>`),
 you will need to call this method directly to provide the Firebase Messaging token.
 
 This method will have no effect if you do not enable support for push notifications,
 see `<[EVGClientConfigurationBuilder usePushNotifications]>`.
 
 Evergage will only send push notifications to the most recent user.
 See `<[Evergage userId]>` for details.
 
 While Evergage automatically tracks clicks/opens for typical push notifications, see
 `<[EVGContext trackClickthrough:actionIdentifier:]>` for when
 you must call a method to track the click.
 
 See [Push Notifications](../docs/doc/Push-Notifications.html) Guide.
 
 @param token The Firebase token.
 @see [EVGClientConfigurationBuilder usePushNotifications]
 @see [Evergage userId]
 @see [EVGContext trackClickthrough:actionIdentifier:]
 @see setAPNSToken:
 @since 1.3.0
 */
- (void)setFirebaseToken:(nonnull NSString *)token;

/**
 You do not normally need to call this method. If your app uses push notifications,
 simply ensure you receive them by defining a notification-received method.
 
 By default, Evergage will swizzle any of these notification-received methods you define:
 
 - `UNUserNotificationCenterDelegate userNotificationCenter:didReceiveNotificationResponse:withCompletionHandler:`
 - `UIApplicationDelegate application:didReceiveRemoteNotification:`
 - `UIApplicationDelegate application:didReceiveRemoteNotification:fetchCompletionHandler:`
 - `UIApplicationDelegate application:handleActionWithIdentifier:forRemoteNotification:completionHandler:`
 - `UIApplicationDelegate application:handleActionWithIdentifier:forRemoteNotification:withResponseInfo:completionHandler:`
 
 However, if you disable swizzling in the Info.plist (see `<swizzlingEnabled>`),
 then you will need to manually call this method from your notification-received method.
 
 This method notifies Evergage when the app receives/processes a notification,
 so Evergage can update campaign attribution statistics.
 
 @param notification The notification/userInfo as received from iOS.
 @param actionIdentifier The actionIdentifier from the notification, if any.
 @since 1.3.0
 */
- (void)handleNotification:(nullable NSDictionary *)notification actionIdentifier:(nullable NSString *)actionIdentifier;

@end
