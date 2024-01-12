/*
Example Sitemap which highlights adding a custom attribute
to a line item in an order.
This is not a complete Sitemap.
*/
//This Sitemap was tested on https://www.northerntrailoutfitters.com 


SalesforceInteractions.init({
    cookieDomain: "northerntrailoutfitters.com",
    consents: [
        {
            purpose: SalesforceInteractions.mcis.ConsentPurpose.Personalization,
            provider: "Example Consent Manager",
            status: SalesforceInteractions.ConsentStatus.OptIn,
        },
    ]
}).then(() => {


    // set the log level during sitemap development to see potential problems 
    SalesforceInteractions.setLoggingLevel('DEBUG'); 

    const sitemapConfig = {
        global: {},
        pageTypeDefault: {
            name: "default",
            interaction: {
                name: "Default Page"
            }
        },
        pageTypes: [
            {
                name: "order_confirmation",
                isMatch: ()=> /\/confirmation/.test(window.location.href),
                interaction: {
                    name: SalesforceInteractions.OrderInteractionName.Purchase,
                    order: {
                        id: SalesforceInteractions.DisplayUtils.pageElementLoaded(".order-number", "html").then((ele) =>{
                            return SalesforceInteractions.resolvers.fromSelector(".order-number");
                        }),
                        lineItems: SalesforceInteractions.DisplayUtils.pageElementLoaded(".product-line-item", "html").then(() =>{
                            let purchaseLineItems = [];
                            SalesforceInteractions.cashDom(".product-line-item").each((index, ele) =>{
                                let itemQuantity = SalesforceInteractions.cashDom(ele).find(".qty-card-quantity-count").text() ? parseInt(SalesforceInteractions.cashDom(ele).find(".qty-card-quantity-count").text().trim()) : null;
                                if (itemQuantity && itemQuantity > 0) {
                                    let lineItem = {
                                        catalogObjectType: "Product",
                                        catalogObjectId: "myItemId",
                                        price: SalesforceInteractions.cashDom(ele).find(".pricing").text().trim().replace(/[^0-9.]+/g, "") / itemQuantity,
                                        quantity: itemQuantity,
                                        //Custom attribute BEGIN
                                        attributes: {
                                            discount_name: "Test Discount from Sitemap"
                                        }
                                        //Custom attribute END
                                    };
                                    purchaseLineItems.push(lineItem);
                                }
                            }) 
                            return purchaseLineItems;
                        }),
                        totalValue: parseFloat(SalesforceInteractions.cashDom("span.sub-total").text().trim().replace("$", ""))
                    }
                }
            }
        ]
    };

    SalesforceInteractions.initSitemap(sitemapConfig); 

});