//
//  EVGContext.h
//  Evergage
//
//  Copyright © 2016-2021 Salesforce.com, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <Evergage/EVGCampaign.h>
#import <Evergage/EVGItems.h>

/**
 Provides APIs to handle data campaigns, track item views and activity, and track manual actions,
 all within a lifecycle-managed context such as `<EVGScreen>`.
 
 Typically used as `<[UIViewController(Evergage) evergageScreen]>` and if necessary `<[Evergage globalContext]>`.
 
 @since 1.2.0
 */
@interface EVGContext : NSObject

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability"
#pragma clang diagnostic ignored "-Wnullability-completeness"
- (nullable instancetype)init __attribute__((unavailable("Use UIViewController's 'evergageScreen' or Evergage's 'globalContext'")));
#pragma clang diagnostic pop

/// -----------------------------------
/// @name Lifecycle
/// -----------------------------------

/**
 If this context is currently active or not.
 
 A typical `<EVGScreen>` context is active when the screen is visible and the app is not backgrounded.
 
 Besides some initial setup just before becoming visible/active (`<setCampaignHandler:forTarget:>`, `<viewItem:>` etc),
 activity generally occurs while a context is active.
 
 Context activity, in addition to app state and user idleness, can affect campaign delivery, see `<setCampaignHandler:forTarget:>`
 
 @since 1.2.0
 */
@property (nonatomic, readonly) BOOL isActive;


/// -----------------------------------
/// @name Campaigns and Stats
/// -----------------------------------

/**
 To optionally support custom 'Data' campaigns, register a campaign handler for
 the app-defined target within this context. Can set `nil` to clear.
 
 Campaigns can potentially be received in response to actions (see `<trackAction:>`) and item interactions (see `<viewItem:>` etc) tracked on this context.
 
 The `target` is an app-defined string that uniquely identifies the payload data
 schema - what the data represents and its purpose.
 See `<[EVGCampaign target]>`.
 
 While the context, app, and user is active, the handler may receive asynchronous callbacks (on the main thread)
 with campaigns for the target, in response to actions/events sent. The handler code should show/update the campaign to the user,
 when appropriate, and follow best practices using weak references, see `<EVGCampaignHandler>` for example code.
 
 Campaigns may be held for delivery while either:
 
 - The context, app, or user is inactive
 - No handler is found for the campaign's target within this context
 
 For each context and target, only the most recent campaign will be held.
 
 Lifecycle details for an `<EVGScreen>` based context:
 
 - Recommended to always call this method just before becoming visible, specifically `UIViewController viewWillAppear:`.
 - In order to prevent accidental leaks in the handler block (see `weak` references in  `<EVGCampaignHandler>` example code to prevent this), a screen has a limited time to become visible, after which its handlers and held campaigns will be cleared.
 - When a screen transitions to not visible (`viewWillDisappear:`), all handlers and held campaigns are cleared.
 
 These limitations and automatic cleanup do not apply to `<[Evergage globalContext]>`.
 
 @param handler The campaign handler this context should use for the specified target.
 @param target App-defined string that uniquely identifies the payload data schema - what the data represents and its purpose. See `<[EVGCampaign target]>`
 @see EVGCampaignHandler
 @see EVGCampaign
 */
- (void)setCampaignHandler:(nullable EVGCampaignHandler)handler forTarget:(nonnull NSString *)target;

/**
 Tracks an impression for the provided campaign. Call this method after showing the campaign to the user or
 after the campaign would be shown but the user is in the control group.
 See [Mobile Data Campaigns](Mobile-Data-Campaigns.html) Guide and `<[EVGCampaignHandler]>`.
 @param campaign The campaign for which an impression should be tracked.
 @see EVGCampaignHandler
 */
- (void)trackImpression:(nonnull EVGCampaign *)campaign;

/**
 Tracks a dismissal for the provided campaign.
 Call this method after showing the campaign to the user and the user dismissed the campaign.
 See [Mobile Data Campaigns](Mobile-Data-Campaigns.html) Guide and `<[EVGCampaignHandler]>`.
 @param campaign The campaign for which a dismissal should be tracked.
 @see EVGCampaignHandler
 */
- (void)trackDismissal:(nonnull EVGCampaign *)campaign;

/**
 Tracks a clickthrough for the provided data campaign.
 Call this method after showing the campaign to the user and the user clicked campaign content.
 See [Mobile Data Campaigns](Mobile-Data-Campaigns.html) Guide and `<[EVGCampaignHandler]>`.
 @param campaign The campaign for which a clickthrough should be tracked.
 @see EVGCampaignHandler
 */
- (void)trackClickthrough:(nonnull EVGCampaign *)campaign;

/**
 You do not normally need to call this method. It tracks a click on custom UI
 you chose to render from a notification received in the foreground.
 
 Evergage automatically tracks clicks on push notifications. However, when the
 notification is received while the app is in the foreground, the notification
 is not automatically shown to the user. If you are using `UNUserNotificationDelegate`,
 you have some options with `userNotificationCenter:willPresentNotification:withCompletionHandler` :
 
 - If you don't show anything to the user, there is no click to track.
 - If you show the notification with UNNotificationPresentationOptionAlert,
   Evergage will automatically track any user click.
 - If you don't show the notification, but instead render some other UI based
   on the notification, and want to track user clicks, call this method when
   the user clicks on the associated UI.
 
 @param notification The notification/userInfo as received from iOS.
 @param actionIdentifier The actionIdentifier from the notification, if any.
 @see [EVGClientConfigurationBuilder usePushNotifications]
 @see [Evergage(Swizzling)]
 @since 1.3.0
 */
- (void)trackClickthrough:(nonnull NSDictionary *)notification
         actionIdentifier:(nullable NSString *)actionIdentifier;


/// --------------------------------
/// @name Tracking Item Views
/// --------------------------------

/**
 Tracks that an item is being viewed.
 Set `nil` to indicate no longer viewing any item/category/tag.
 
 Recommended to call just before visible, viewWillAppear for a view controller.
 
 Evergage will automatically track the time spent viewing the item while the context, app, and user is active.
 The item will remain the one viewed until this method or `<viewItemDetail:>` are called again.
 
 @param item The item being viewed.
 @see viewItem:actionName:
 */
- (void)viewItem:(nullable EVGItem *)item;

/**
 Same as `<viewItem:>` but with a different action name to distinguish this View Item.
 
 Only tracks an action if item is non-null.
 
 @param item The item being viewed.
 @param actionName Optional different action name.
 @since 1.3.0
 */
- (void)viewItem:(nullable EVGItem *)item actionName:(nullable NSString *)actionName;

/**
 Tracks that the details of an item are being viewed, such as viewing other product images or a specifications tab.
 Set `nil` to indicate no longer viewing any item/category/tag.
 
 Recommended to call just before visible, viewWillAppear for a view controller.
 
 Evergage will automatically track the time spent viewing the item while the context, app, and user is active.
 The item will remain the one viewed until this method or `<viewItem:>` are called again.
 
 @param item The item whose details are being viewed.
 @see viewItemDetail:actionName:
 */
- (void)viewItemDetail:(nullable EVGItem *)item;

/**
 Same as `<viewItemDetail:>` but with a different action name to distinguish this View Item Detail.
 
 Only tracks an action if item is non-null.
 
 @param item The item whose details are being viewed.
 @param actionName Optional different action name.
 @since 1.3.0
 */
- (void)viewItemDetail:(nullable EVGItem *)item actionName:(nullable NSString *)actionName;

/**
 Tracks that a category is being viewed.
 Set `nil` to indicate no longer viewing any item/category/tag.
 
 Recommended to call just before visible, viewWillAppear for a view controller.
 
 @param category The category being viewed.
 @see viewCategory:actionName:
 */
- (void)viewCategory:(nullable EVGCategory *)category;

/**
 Same as `<viewCategory:>` but with a different action name to distinguish this View Category.
 
 Only tracks an action if category is non-null.
 
 @param category The category being viewed.
 @param actionName Optional different action name.
 @since 1.3.0
 */
- (void)viewCategory:(nullable EVGCategory *)category actionName:(nullable NSString *)actionName;

/**
 Tracks that a tag is being viewed.
 Set `nil` to indicate no longer viewing any item/category/tag.
 
 Recommended to call just before visible, viewWillAppear for a view controller.
 
 @param tag The tag being viewed.
 @see viewTag:actionName:
 */
- (void)viewTag:(nullable EVGTag *)tag;

/**
 Same as `<viewTag:>` but with a different action name to distinguish this View Tag.
 
 Only tracks an action if tag is non-null.
 
 @param tag The tag being viewed.
 @param actionName Optional different action name.
 @since 1.3.0
 */
- (void)viewTag:(nullable EVGTag *)tag actionName:(nullable NSString *)actionName;


/// --------------------------------
/// @name Tracking Item Activity
/// --------------------------------

/**
 Tracks that a line item is being added to the shopping cart.
 
 @param lineItem The line item being added.  The `<[EVGLineItem item]>` should be a `<EVGProduct>` with `<[EVGProduct price]>` set.
 */
- (void)addToCart:(nonnull EVGLineItem *)lineItem;

/**
 Tracks that a line item is being removed from the shopping cart.  The `<[EVGLineItem item]>` should be a `<EVGProduct>` with `<[EVGProduct price]>` set.
 
 @param lineItem The line item being removed.
 @since 1.4.0
 */
- (void)removeFromCart:(nonnull EVGLineItem *)lineItem;

/**
 Tracks an entire shopping cart, which will update/replace the entire state of the mirrored cart in Evergage.
 
 A cart can be considered an 'open' order.  Most carts will not have an `orderId`, and so the field can be left un-set.
 
 @param order The entire cart being viewed.  Each `<[EVGLineItem item]>` should be a `<EVGProduct>` with `<[EVGProduct price]>` set.
 @since 1.4.0
 */
- (void)viewCart:(nonnull EVGOrder *)order;

/**
 Tracks that an order was purchased.
 
 If the order contains no `lineItems`, the lineItems currently in the cart will be
 used. If `orderId` is set and multiple purchase events are received for the same `orderId`, all but the first will be
 ignored.
 
 @param order The order that was purchased.  Each `<[EVGLineItem item]>` should be a `<EVGProduct>` with `<[EVGProduct price]>` set.
 */
- (void)purchase:(nonnull EVGOrder *)order;

/**
 Tracks that an item was reviewed.
 
 @param item The item that was reviewed.
 */
- (void)review:(nonnull EVGItem *)item;

/**
 Tracks that an item was reviewed, as well as the optional details of the review.
 
 @param item The item that was reviewed.
 @param reviewDetails The optional details of the review, such as the rating.
 */
- (void)review:(nonnull EVGItem *)item reviewDetails:(nullable EVGReview *)reviewDetails;

/**
 Tracks that an item was shared, for instance by email or on a social network.
 
 @param item The item that was shared.
 */
- (void)share:(nonnull EVGItem *)item;

/**
 Tracks that an item was commented on. For instance, an article or blog might accept comments.
 
 @param item The item that was commented on.
 */
- (void)comment:(nonnull EVGItem *)item;

/**
 Tracks that an item was marked by the user as a favorite item. This is an explicit action taken by the user (often
 indicated by a single star).
 
 @param item The item that was marked as favorite.
 */
- (void)favorite:(nonnull EVGItem *)item;


/// -------------------------------
/// @name Manually Tracking Actions
/// -------------------------------

/**
 Sends an event to Evergage describing an action to track.
 
 When considering the action name, remember that datasets encompass multiple platforms
 and apps. The name can match a corresponding action/behavior from another platform/app.
 A campaign can use a source rule to be limited to a specific source or set of sources.
 See [Mobile Data Campaigns](../docs/doc/Mobile-Data-Campaigns.html) Guide for recommended rules.
 
 @param action A short string that identifies the action.
 */
- (void)trackAction:(nonnull NSString *)action;

@end
