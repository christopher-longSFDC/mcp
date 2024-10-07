//
//  EVGItems.h
//  Evergage
//
//  Copyright © 2015-2021 Salesforce.com, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 Modifies how items will be selected and ordered for promotion.
 
 @see EVGItem
 */
typedef NS_ENUM(NSInteger, EVGPromotionState) {

    /**
     * Don't make any changes to the standard promoted content behavior. The default promotion state.
     */
    EVGPromotionStateUndefined = 0,

    /**
     * Give the associated item a higher priority when evaluating campaign dynamic promoted content rules; ignored for
     * static rules.
    */
    EVGPromotionStatePrioritized,

    /**
     * Never promote the associated item, even if it otherwise matches promoted content rules; honored for both static
     * and dynamic rules.
     */
    EVGPromotionStateExcluded
};

/**
 Specifies which type a tag belongs to.
 
 @see EVGTag
 */
typedef NS_ENUM(NSInteger, EVGTagType) {

    /**
     * A manufacturer, for example "Nike".
     */
    EVGTagTypeBrand,

    /**
     * A custom type of item in addition to the ones predefined by Evergage. For instance, "Concert".
     */
    EVGTagTypeItemClass,

    /**
     * The gender the item is intended for. For instance, "Male", "Female", "Unisex".
     * @since 1.2.0
     */
    EVGTagTypeGender,

    /**
     * A distinctive appearance. For example, furniture styles might be "Retro", "Art Deco" and "Shaker".
     */
    EVGTagTypeStyle,

    /**
     * A person who wrote an article or blog.
     */
    EVGTagTypeAuthor,

    /**
     * A word or short phrase describing an item. These might be curated or added by individual users. For example, if
     * the topic was sewing there might be a keyword tag for "needlepoint" and another for "patterns".
     */
    EVGTagTypeKeyword,

    /**
     * The type or format of an article or blog. For example, "ebook", "pdf", "whitepaper".
     * @since 1.2.0
     */
    EVGTagTypeContentClass
};

/**
 * Location to associate with an Item, for example for filtering reasons if a product only available in an area.
 * @since 1.3.0
 */
@interface EVGLocation : NSObject

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability"
#pragma clang diagnostic ignored "-Wnullability-completeness"
- (nullable instancetype)init __attribute__((unavailable("Use class factory methods")));
#pragma clang diagnostic pop

/**
 * Latitudinal coordinate of the location, should be from -90 to 90.
 */
@property (nullable, nonatomic, strong) NSNumber *latitude;

/**
 * Longitudinal coordinate of the location, should be from -180 to 180.
 */
@property (nullable, nonatomic, strong) NSNumber *longitude;

/**
 * Name of the city.
 */
@property (nullable, nonatomic, copy) NSString *city;

/**
 * Name of the state or province.
 */
@property (nullable, nonatomic, copy) NSString *stateProvinceCode;

/**
 * Postal code of the location.
 */
@property (nullable, nonatomic, copy) NSString *postalCode;

/**
 * Creates a location with geographic coordinates.
 * @param latitude Latitudinal coordinate, should be from -90 to 90
 * @param longitude Longitudinal coordinate, should be from -180 to 180
 */
+ (nullable instancetype)locationWithLatitude:(double)latitude longitude:(double)longitude;

/**
 * Creates a location with city and state.
 * @param city City name
 * @param stateProvinceCode State or Province Code (e.g. MA)
 */
+ (nullable instancetype)locationWithCity:(nonnull NSString *)city stateProvinceCode:(nonnull NSString *)stateProvinceCode;

/**
 * Creates a location with postal code.
 * @param postalCode Postal code
 */
+ (nullable instancetype)locationWithPostalCode:(nonnull NSString *)postalCode;

- (nullable NSMutableDictionary<NSString *,id> *)toJSONDictionary;

/**
 Creates a location from the provided JSON.
 
 @param json A location in JSON form
 @return A location which can be further modified, or nil if JSON is invalid
 */
+ (nullable instancetype)locationFromJSONDictionary:(nonnull NSDictionary<NSString *,id> *)json;

@end

/**
 Abstract base class. An item is something in a site or app that users can view or otherwise engage with. Typical
 examples are products, articles and blogs. Evergage tracks descriptive fields and user activity against items. This
 activity can be used to build reports and to promote relevant items to users.
 
 All fields are optional unless otherwise stated. Generally, optional fields should only be sent if they are specific
 to this transaction (e.g. lineItem.item.price) or if they aren't already being sent along with web activity on the same
 dataset.
 
 See `<EVGCategory>`, `<EVGTag>`, `<EVGProduct>`, `<EVGArticle>`, and `<EVGBlog>`.
 */
@class EVGTag;
@interface EVGItem : NSObject

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability"
#pragma clang diagnostic ignored "-Wnullability-completeness"
- (nonnull instancetype)init DEPRECATED_MSG_ATTRIBUTE("Use class factory methods");
#pragma clang diagnostic pop

/**
 * Required. Uniquely identifies an Item. The ID of a category must also identify its parents, separated by pipe | characters.
 * If a category has the ID `"parent"` and another category is the child of the first, the child's ID might be `"parent|child"`.
 */
@property (nonnull, nonatomic, copy) NSString *evgId;

/**
 * Name of a product, category or tag. For an article or blog this is the title.
 */
@property (nullable, nonatomic, copy) NSString *name;

/**
 * Overview text about the item.
 */
@property (nullable, nonatomic, copy) NSString *evgDescription;

/**
 * Link to this item.
 */
@property (nullable, nonatomic, copy) NSString *url;

/**
 * Fully-qualified URL for the image representing this item.
 */
@property (nullable, nonatomic, copy) NSString *imageUrl;

/**
 * If set, this item will be published/available on or after this time. Unpublished items will be excluded from
 * promotion campaigns.
 */
@property (nullable, nonatomic, strong) NSDate *published;

/**
 * If set, this item will be expired/unavailable on or after this time. Expired items will be excluded from
 * promotion campaigns.
 */
@property (nullable, nonatomic, strong) NSDate *expiration;

/**
 * Additional descriptive tags about an item such as Brand for products or Author and Keywords for articles.
 */
@property (nullable, nonatomic, strong) NSArray<EVGTag *> *tags;

/**
 * An optional promotion state that modifies how items will be selected and ordered for promotion. See
 * `<EVGPromotionState>` for allowed values and their meanings.
 */
@property (nonatomic) EVGPromotionState promotionState;

/**
 * Location related to this item.
 * @since 1.3.0
 */
@property (nullable, nonatomic, strong) EVGLocation *location;

/**
 * Overall score for the item.
 * @since 1.3.0
 */
@property (nullable, nonatomic, strong) NSNumber *rating;

/**
 * Total number of ratings given to this item, >=0
 * @since 1.3.0
 */
@property (nullable, nonatomic, strong) NSNumber *numRatings;

- (nonnull NSMutableDictionary<NSString *,id> *)toJSONDictionary;

/**
 Create an EVGItem subclass of the appropriate type from the provided JSON.
 
 @param json A type of item in JSON form
 @return An EVGItem subclass, or nil if JSON is invalid
 */
+ (nullable instancetype)fromJSONDictionary:(nonnull NSDictionary<NSString *,id> *)json;

/**
 Create an array of EVGItem subclasses from the provided JSON array.
 
 @param json A JSON array of items to convert to an array of EVGItem subclasses
 @return An array of EVGItem subclasses, or nil if JSON is invalid
 */
+ (nullable NSArray<EVGItem *> *)fromJSONArray:(nonnull NSArray<NSDictionary<NSString *,id> *> *)json;

@end


/**
 A category organizes other items and is contained in a hierarchy.
 */
@interface EVGCategory : EVGItem

/**
 If YES, this category is a department. Dynamic promoted content can be configured to only choose categories that are also departments.
 */
@property (nonatomic) BOOL isDepartment;

/**
 Creates an EVGCategory with the required id only.

 @param evgId See `<[EVGItem evgId]>`. This is the only required field.
 @return A category which can be further modified.
 */
+ (nonnull instancetype)categoryWithId:(nonnull NSString *)evgId;

/**
 Creates an EVGCategory with the required id and most common optionals.
 
 @param evgId See `<[EVGItem evgId]>`. This is the only required field.
 @param isDepartment See `<isDepartment>`
 @param name See `<[EVGItem name]>`
 @param evgDescription See `<[EVGItem evgDescription]>`
 @return A category which can be further modified.
 */
+ (nonnull instancetype)categoryWithId:(nonnull NSString *)evgId
                          isDepartment:(BOOL)isDepartment
                                  name:(nullable NSString *)name
                        evgDescription:(nullable NSString *)evgDescription;

/**
 Creates an EVGCategory from the provided JSON.
 
 @param json A category in JSON form
 @return A category which can be further modified, or nil if JSON is invalid
 */
+ (nullable instancetype)categoryFromJSONDictionary:(nonnull NSDictionary<NSString *,id> *)json;

@end


/**
 * Additional tags about an item such as Brand for products or Author and Keywords for articles.
 */
@interface EVGTag : EVGItem

/**
 * Required. The type of a tag. See `<EVGTagType>` for values.
 */
@property (nonatomic) EVGTagType tagType;

/**
 Creates an EVGTag with the required id and type only.

 @param evgId Required, see `<[EVGItem evgId]>`
 @param type Required, see `<EVGTagType>`
 @return A tag which can be further modified.
 */
+ (nonnull instancetype)tagWithId:(nonnull NSString *)evgId
                             type:(EVGTagType)type;

/**
 Creates an EVGTag with the required id and type, and the most common optionals.
 
 @param evgId Required, see `<[EVGItem evgId]>`
 @param type Required, see `<EVGTagType>`
 @param name See `<[EVGItem name]>`
 @param evgDescription See `<[EVGItem evgDescription]>`
 @return A tag which can be further modified.
 */
+ (nonnull instancetype)tagWithId:(nonnull NSString *)evgId
                             type:(EVGTagType)type
                             name:(nullable NSString *)name
                   evgDescription:(nullable NSString *)evgDescription;

/**
 Creates a tag from the provided JSON.
 
 @param json A tag in JSON form
 @return A tag which can be further modified, or nil if JSON is invalid
 */
+ (nullable instancetype)tagFromJSONDictionary:(nonnull NSDictionary<NSString *,id> *)json;

@end


/**
 Abstract class. A categorized item belongs to one or more categories.
 See `<EVGProduct>`, `<EVGArticle>`, and `<EVGBlog>`.
 */
@interface EVGCategorizedItem : EVGItem

/**
 The categories that this categorized item belongs to.
 */
@property (nullable, nonatomic, strong) NSArray<EVGCategory *> *categories;

@end


/**
 A product is an item that can be sold to users. Products can be added to `<EVGLineItem>` objects
 when they have been ordered by the user.
 
 See `<productWithId:name:price:url:imageUrl:evgDescription:>`
 
 ##Initialization
 
 Here is an example of EVGProduct initialization:
 
    EVGProduct *product = [EVGproduct productWithId:@"B25787"
                                               name:@"Road Runners"
                                              price:@110.00
                                                url:@"http://www.shoes4u.com/us/road-runners/B25787.html"
                                           imageUrl:@"http://www.shoes4u.com/us/road-runners/B25787.png"
                                     evgDescription:@"Road Runners shoes are great for running!"];
 */
@interface EVGProduct : EVGCategorizedItem

/**
 The unit price of the product. This is required when the product is being ordered but optional otherwise.
 For instance, if the product has variable pricing there may be no correct price for `<[EVGContext viewItem:]>` events.
 */
@property (nullable, nonatomic, strong) NSNumber *price;

/** Optional field for descriptive text for the price, e.g. 'Sale Price'. */
@property (nullable, nonatomic, copy) NSString *priceDescription;

/** Optional field for capturing e.g. MSRP in order to illustrate discount. */
@property (nullable, nonatomic, strong) NSNumber *listPrice;

/**
 An ISO 4217 currency code for the currency of this item's price such as `"USD"`, `"EUR"`, `"GBP"`. If absent,
 the currency is assumed to be the dataset default currency.
 */
@property (nullable, nonatomic, copy) NSString *currency;

/**
 * A count of available units. Alternatively, this may be simply 1 or 0 to indicate available or unavailable, respectively.
 */
@property (nullable, nonatomic, strong) NSNumber *inventoryCount;

/**
 * An identifier for a product other than the one stored in `evgId`.
 */
@property (nullable, nonatomic, copy) NSString *alternateId;

/**
 Builds an EVGProduct with the required id only.

 @param evgId See `<[EVGItem evgId]>`. This is the only required field.

 @return A product which can be further modified.
 */
+ (nonnull instancetype)productWithId:(nonnull NSString *)evgId;

/**
 Builds an EVGProduct, including many of the commonly used fields. This method only builds a data object. To use it,
 pass it to one of the tracking methods like `<[EVGContext viewItem:]>`.
 
 @param evgId See `<[EVGItem evgId]>`. This is the only required field.
 @param name The name of the product. It should be provided whenever possible.
 @param price The unit price of the product. This is required when the product is being ordered but optional otherwise.
 For instance, if the product has variable pricing there may be no correct price for `-viewItem:` events.
 @param url A URL where the user can view information about the product.
 @param imageUrl A URL containing an image of the product.
 @param evgDescription A text description of the product.
 
 @return A product which can be further modified.
 */
+ (nonnull instancetype)productWithId:(nonnull NSString *)evgId
                                 name:(nullable NSString *)name
                                price:(nullable NSNumber *)price
                                  url:(nullable NSString *)url
                             imageUrl:(nullable NSString *)imageUrl
                       evgDescription:(nullable NSString *)evgDescription;

/**
 Creates an EVGProduct from the provided JSON.
 
 @param json A product in JSON form
 @return A product which can be further modified, or nil if JSON is invalid
 */
+ (nullable instancetype)productFromJSONDictionary:(nonnull NSDictionary<NSString *,id> *)json;

@end


/**
 * An article is content that is typically organized by category or keyword.
 */
@interface EVGArticle : EVGCategorizedItem

/**
 Date and time when this article was last modified.
 @see [EVGItem published]
 */
@property (nullable, nonatomic, strong) NSDate *modifiedDate;

/**
 * Subtitle for this article.
 */
@property (nullable, nonatomic, copy) NSString *subTitle;

/**
 Creates an EVGArticle with the required id only.

 @param evgId See `<[EVGItem evgId]>`. This is the only required field.
 @return An article which can be further modified.
 */
+ (nonnull instancetype)articleWithId:(nonnull NSString *)evgId;

/**
 Creates an EVGArticle with the required id and most common optionals.
 
 @param evgId See `<[EVGItem evgId]>`. This is the only required field.
 @param name See `<[EVGItem name]>`
 @param subTitle See `<subTitle>`
 @param url See `<[EVGItem url]>`
 @param evgDescription See `<[EVGItem evgDescription]>`
 @return An article which can be further modified.
 */
+ (nonnull instancetype)articleWithId:(nonnull NSString *)evgId
                                 name:(nullable NSString *)name
                             subTitle:(nullable NSString *)subTitle
                                  url:(nullable NSString *)url
                       evgDescription:(nullable NSString *)evgDescription;

/**
 Creates an EVGArticle from the provided JSON.
 
 @param json An article in JSON form
 @return An article which can be further modified, or nil if JSON is invalid
 */
+ (nullable instancetype)articleFromJSONDictionary:(nonnull NSDictionary<NSString *,id> *)json;

@end


/**
 * A blog is content that is typically presented as reverse chronologically sorted on a single topic.
 */
@interface EVGBlog : EVGCategorizedItem

/**
 Date and time when this blog was last modified.
 @see [EVGItem published]
 */
@property (nullable, nonatomic, strong) NSDate *modifiedDate;

/**
 * Subtitle for this blog.
 */
@property (nullable, nonatomic, copy) NSString *subTitle;

/**
 Creates an EVGBlog with the required id only.

 @param evgId See `<[EVGItem evgId]>`. This is the only required field.
 @return A blog which can be further modified.
 */
+ (nonnull instancetype)blogWithId:(nonnull NSString *)evgId;

/**
 Creates an EVGBlog with the required id and most common optionals.
 
 @param evgId See `<[EVGItem evgId]>`. This is the only required field.
 @param name See `<[EVGItem name]>`
 @param subTitle See `<subTitle>`
 @param url See `<[EVGItem url]>`
 @param evgDescription See `<[EVGItem evgDescription]>`
 @return A blog which can be further modified.
 */
+ (nonnull instancetype)blogWithId:(nonnull NSString *)evgId
                              name:(nullable NSString *)name
                          subTitle:(nullable NSString *)subTitle
                               url:(nullable NSString *)url
                    evgDescription:(nullable NSString *)evgDescription;

/**
 Creates a blog from the provided JSON.
 
 @param json A blog in JSON form
 @return A blog which can be further modified, or nil if JSON is invalid
 */
+ (nullable instancetype)blogFromJSONDictionary:(nonnull NSDictionary<NSString *,id> *)json;

@end


/**
 An order sent upon user checkout. The order contains a total purchase value and line items representing each product
 and quantity.
 
 ##Initialization
 
 Here is a convenient way to initialize an EVGOrder
 
    EVGOrder *order = [EVGOrder orderWithId:@"order123" lineItems:@[lineItem] totalValue:@100];
 */
@class EVGLineItem;
@interface EVGOrder : NSObject

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability"
#pragma clang diagnostic ignored "-Wnullability-completeness"
- (nonnull instancetype)init DEPRECATED_MSG_ATTRIBUTE("Use class factory methods");
#pragma clang diagnostic pop

/**
 Optional array of `<EVGLineItem>`s ordered by the user.
 */
@property (nullable, nonatomic, strong) NSArray<EVGLineItem *> *lineItems;

/** Optional order ID uniquely identifies an order to avoid duplication if the same order is resent. */
@property (nullable, nonatomic, copy) NSString *orderId;

/**
 The total value of the order. If present and nonzero this value overrides the normal order
 value calculation which is quantity x price for all the line items.
 */
@property (nullable, nonatomic, strong) NSNumber *totalValue;

/**
 ISO code for the pricing currency of this order's total value such as `"USD"`, `"EUR"`, `"GBP"`.
 */
@property (nullable, nonatomic, copy) NSString *totalValueCurrency;

/**
 Builds an order. This method only builds a data object. To use it, pass it to one of the tracking methods like
 `<[EVGContext purchase:]>`.
 
 @param orderId The optional unique ID of this order.
 @param lineItems The optional items being ordered along with their quantities. All entries must be of type `<EVGLineItem>`.
 @param totalValue The optional total value of the order. If `nil` or `0` it will be treated as the sum of the revenue
        from each line item (price x quantity).
 
 @return An order which can be further modified.
 */
+ (nonnull instancetype)orderWithId:(nullable NSString *)orderId
                          lineItems:(nullable NSArray<EVGLineItem *> *)lineItems
                         totalValue:(nullable NSNumber *)totalValue;

- (nonnull NSMutableDictionary<NSString *,id> *)toJSONDictionary;

@end


/**
 A line item contains a single `<EVGItem>` subclass and the quantity that were ordered.
 
 ##Initialization
 
 Here is an example of how to initialize a single EVGLineItem:
 
    EVGLineItem *lineItem = [EVGLineItem lineItemWithProductId:@"product001" productName:@"Road Runner" price:@100 quantity:@2];
 */
@interface EVGLineItem : NSObject

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability"
#pragma clang diagnostic ignored "-Wnullability-completeness"
- (nonnull instancetype)init DEPRECATED_MSG_ATTRIBUTE("Use class factory methods");
#pragma clang diagnostic pop

/**
 * The item being ordered, subclass of EVGItem.
 */
@property (nonnull, nonatomic, strong) EVGItem *item;

/**
 * The quantity of the item being ordered.  Defaults to 1
 */
@property (nullable, nonatomic, strong) NSNumber *quantity;

/**
 Builds a line item which contains one or more units of a product in an order. This method only builds a data object. To
 use it, pass it to one of the tracking methods like `<[EVGContext addToCart:]>`.
 
 @param item The required `<EVGItem>` subclass being ordered
 @param quantity The optional quantity, defaults to 1
 @return A line item which can be further modified.
 */
+ (nonnull instancetype)lineItemWithItem:(nonnull EVGItem *)item quantity:(nullable NSNumber *)quantity;

/**
 Builds a line item which contains one or more units of a product in an order. This method only builds a data object. To
 use it, pass it to one of the tracking methods like `<[EVGContext addToCart:]>`.
 
 @param productId The ID of the product in this line item. This is the only required field.
 @param productName The name of the product. It should be provided whenever possible.
 @param price The unit price of the product. The revenue for this line item will be price x quantity. This is required
 when the product is being ordered but optional otherwise.
 @param quantity The number of units of the product this line item represents.
 
 @return A line item which can be further modified.
 */
+ (nonnull instancetype)lineItemWithProductId:(nonnull NSString *)productId
                                  productName:(nullable NSString *)productName
                                        price:(nullable NSNumber *)price
                                     quantity:(nullable NSNumber *)quantity;

- (nonnull NSMutableDictionary<NSString *,id> *)toJSONDictionary;

@end


/**
 A user-created review of an item.
 */
@interface EVGReview : NSObject

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability"
#pragma clang diagnostic ignored "-Wnullability-completeness"
- (nonnull instancetype)init DEPRECATED_MSG_ATTRIBUTE("Use class factory methods");
#pragma clang diagnostic pop

/** The ID (see `<[EVGItem evgId]>`) of the item being reviewed. */
@property (nullable, nonatomic, copy) NSString *itemId;

/**
 * Date and time when the review was submitted.
 */
@property (nullable, nonatomic, strong) NSDate *date;

/** Score given to the item in this review, must be normalized to the range 1-100 inclusive. */
@property (nullable, nonatomic, strong) NSNumber *rating;

/**
 * The title of the review.
 */
@property (nullable, nonatomic, copy) NSString *title;

/**
 * The text content of the review.
 */
@property (nullable, nonatomic, copy) NSString *content;

/** Name to be displayed along with review, may have no relation to actual user ID. */
@property (nullable, nonatomic, copy) NSString *userDisplayName;

/** Whether the product was purchased by this user before being reviewed. */
@property (nonatomic) BOOL purchased;

/**
 Builds review details for an item. This method only builds a data object. To
 use it, pass it to one of the tracking methods like `<[EVGContext review:reviewDetails:]>`.
 
 @param itemId The required `<[EVGItem evgId]>` of the item being reviewed
 @param date See `<date>`
 @param rating See `<rating>`
 @param title See `<title>`
 @param purchased See `<purchased>`
 @return Review details which can be further modified.
 */
+ (nonnull instancetype)reviewWithItemId:(nonnull NSString *)itemId
                                    date:(nullable NSDate *)date
                                  rating:(nullable NSNumber *)rating
                                   title:(nullable NSString *)title
                               purchased:(BOOL)purchased;

- (nonnull NSMutableDictionary<NSString *,id> *)toJSONDictionary;

@end
