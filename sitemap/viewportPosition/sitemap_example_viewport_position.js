/*
Example Sitemap which highlights observing when an HTML element is within
the browser viewport.
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

    // Function to check if an element is in the viewport
    function isElementInViewport(element) {
        var rect = element.getBoundingClientRect();
        return (
            rect.top >= 0 &&
            rect.left >= 0 &&
            rect.bottom <= (window.innerHeight || document.documentElement.clientHeight) &&
            rect.right <= (window.innerWidth || document.documentElement.clientWidth)
        );
    }

    // Function to be executed when the element is in view
    function doSomethingWhenInView() {
        /* Your code here, e.g., add a class or perform an action. 
        In this example, a custom event is dispatched */

        //Dispatch custom event announcing to the DOM the element is in view
        var customEvent = new Event("global_infobar_bottom_of_page-in-viewport");
        SalesforceInteractions.cashDom("body")[0].dispatchEvent(customEvent);
    }

    // Get the HTML element you want to track
    var elementToTrack = SalesforceInteractions.cashDom("footer.site-footer")[0];

    // Add a SCROLL event listener to continuously check for when the element is in view 
    window.addEventListener("scroll", function () {
        if (isElementInViewport(elementToTrack)) {
            doSomethingWhenInView();
        }
    });

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
                name: "homepage",
                isMatch: () =>/\/homepage/.test(window.location.href),
                interaction: {
                  name: "Homepage"
                },
                contentZones: [{
                    name: "global_infobar_bottom_of_page",
                    selector: "footer.site-footer"
                }],
                listeners: [
                    // Listen for your custom viewport event! 
                    SalesforceInteractions.listener("global_infobar_bottom_of_page-in-viewport", "body", (e) =>{
                        SalesforceInteractions.sendEvent({
                            interaction: {
                                name: "global_infobar_bottom_of_page in viewport",
                            },
                            /* If you want to target a campaign on this event,
                            be sure to include a content zone */
                            source:{
                                contentZones: [
                                {
                                    name: "global_infobar_bottom_of_page",
                                    selector: "footer.site-footer"
                                }]
                            }
                        });
                    })
                ]
            }
        ]
    }
    SalesforceInteractions.initSitemap(sitemapConfig);
    
});