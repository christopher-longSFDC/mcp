/*
Example Sitemap which highlights setting the locale in the global object.
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
        global: {
            locale: "en_US"
        },
        pageTypeDefault: {
            name: "default",
            interaction: {
                name: "Default Page"
            }
        },
        pageTypes: []
    };

    SalesforceInteractions.initSitemap(sitemapConfig); 

});