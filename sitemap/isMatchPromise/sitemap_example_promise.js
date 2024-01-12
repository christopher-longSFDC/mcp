/*
Example Sitemap which highlights the use of a Promise for isMatch functions.
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
        pageTypes: [{
            name: "product_detail",
            /* The best practice for isMatch is to match as quickly as possible.
            If matching immediately is not an option, you can use a Promise.
            The Promise should resolve true or false and not run indefinitely.
            This Promise example uses an interval to prevent the isMatch from running
            indefinitely if the match condition is not met fast enough. */
            isMatch: () => {
                return new Promise((resolve, reject) => {
                    let maxChecks = 10;//max number of times the Promise will check to see if elmenent is avaialble
                    let currentChecks = 0;
                    let elementCheckInterval = window.setInterval(function() {
                        if (currentChecks === maxChecks) {
                            window.clearInterval(elementCheckInterval);
                            reject(new Error("Element for PDP not found in time."));
                        }
                        currentChecks++;
                        //This is where you put your logic for the Promise to return true or false
                        /*This example check to see if a specific element exists before confirming
                        true or false*/
                        if ( SalesforceInteractions.cashDom('div.page[data-action="Product-Show"]').length > 0 )
                        {
                            console.log("FOUNDIT")
                            resolve(true);
                            
                        }else{
                            reject(new Error("Not on a PDP page."));
                        }
                            window.clearInterval(elementCheckInterval);
                        }, 250); //time to wait in milliseconds before checking again
                }).then(()=>{return true;}).catch(()=>{return false;});
            },
            interaction: {
                name: SalesforceInteractions.CatalogObjectInteractionName.ViewCatalogObject,
                catalogObject: {
                    type: "Product",
                    id: "temp product id",
                }
            }     
        }]
    };

    SalesforceInteractions.initSitemap(sitemapConfig); 

});