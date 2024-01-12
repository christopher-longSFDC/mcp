/*
Example Sitemap which highlights getting the value of a querystring 
parameter and then setting a user attribute with that value.
This is not a complete Sitemap.
*/
//This Sitemap was tested on https://www.northerntrailoutfitters.com 

//
const getQueryStringParam = (paramName) => {
    const queryString = window.location.search;
    const urlParams = new URLSearchParams(queryString);
    return urlParams.get(paramName)
};
//

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
            onActionEvent: (actionEvent) =>{
                //
                const queryStringParam = getQueryStringParam("customerId");
                if (queryStringParam) {
                actionEvent.user = actionEvent.user || {};
                actionEvent.user.identities = actionEvent.user.identities || {};
                actionEvent.user.identities.customerId = queryStringParam;
                }
                //
                return actionEvent;
            }
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