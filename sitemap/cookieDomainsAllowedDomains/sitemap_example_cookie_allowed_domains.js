/*
Example Sitemap which highlights setting the Cookie Domain for 
different URLs as well as incorporating Allowed Domains to limit
the domains where the Sitemap can be initialized.
This is not a complete Sitemap.
*/
//This Sitemap was tested on https://www.northerntrailoutfitters.com 

const getDomain = () => {
    let currentDomain = window.location.hostname;
    let _cookieDomain;
    if (currentDomain === "www.northerntrailoutfitters.com"){
      _cookieDomain = "northerntrailoutfitters.com";
    }else if (currentDomain === "staging-nto.demandware.net"){
      _cookieDomain = "staging-nto.demandware.net";
    }
    return _cookieDomain;
 }
 var allowedDomains = [
    "www.northerntrailoutfitters.com",
    "staging-nto.demandware.net"
 ];
 (allowedDomains.indexOf(window.location.hostname) >= 0) && SalesforceInteractions.init({
    cookieDomain: getDomain(),
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
         pageTypes: []
     };
 
     SalesforceInteractions.initSitemap(sitemapConfig); 
 
 });