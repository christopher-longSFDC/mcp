//
//  EVGClientConfigurationBuilder.h
//  Evergage
//
//  Copyright © 2018-2021 Salesforce.com, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class EVGClientConfigurationBuilder;
/**
 The block/closure used to populate the `<EVGClientConfigurationBuilder>`,
 in order to specify the Evergage iOS configuration in `<[Evergage startWithClientConfiguration:]>`.
 
 @see [Evergage startWithClientConfiguration:]
 @see EVGClientConfigurationBuilder
 @since 1.3.0
 */
typedef void(^EVGClientConfigurationBlock)(EVGClientConfigurationBuilder * _Nonnull builder);

#pragma mark -

/**
 Builds the Evergage iOS configuration, including some required settings,
 provided by the client in `<[Evergage startWithClientConfiguration:]>`.
 
 @see [Evergage startWithClientConfiguration:]
 @see EVGClientConfigurationBlock
 @since 1.3.0
 */
@interface EVGClientConfigurationBuilder : NSObject

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability"
#pragma clang diagnostic ignored "-Wnullability-completeness"
- (nullable instancetype)init __attribute__((unavailable("Provided in EVGClientConfigurationBlock in Evergage startWithClientConfiguration:")));
#pragma clang diagnostic pop

/// ---------------------------------
#pragma mark - Required Settings
/// @name Required Settings
/// ---------------------------------

/** Required - Account within Evergage to use. */
@property (nonnull, nonatomic, copy) NSString *account;

/** Required - Dataset within the Evergage account to use. */
@property (nonnull, nonatomic, copy) NSString *dataset;


/// ---------------------------------
#pragma mark - Optional Settings
/// @name Optional Settings
/// ---------------------------------

/**
 Optional - If the app uses push notifications (APNS and/or Firebase),
 allow Evergage to collect the app's APNS/Firebase token(s) in order to support
 Evergage push notification campaigns.
 
 Defaults to false.
 
 See the [Push Notifications](../docs/doc/Push-Notifications.html) Guide.
 @see [Evergage(Swizzling)]
 */
@property (nonatomic) BOOL usePushNotifications;

/**
 Optional - Allow user-initiated gesture to connect to the Evergage web console
 for action mapping screens.
 
 Defaults to false.
 Recommended to use only during development in DEBUG builds.
 
 See [Tracking Guide](../docs/doc/Tracking.html) for more info.
 */
@property (nonatomic) BOOL useDesignMode;

@end
