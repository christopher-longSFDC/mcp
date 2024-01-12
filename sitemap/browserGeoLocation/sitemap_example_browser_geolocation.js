/*
Example Sitemap which highlights getting the geolocation from the browser 
and then comparing that location to a different, predetermined location.
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
            onActionEvent: (actionEvent) =>{
                //Gets geolocation from the browser and sends it to callback function
                if (navigator.geolocation) {
                    navigator.geolocation.getCurrentPosition(showPosition);
                }
                //Callback function for geolocation.getCurrentPosition
                function showPosition(position) {
                    //assigns browser geo points to variables
                    var latitude = position.coords.latitude;
                    var longitude = position.coords.longitude;
                
                    console.log("Latitude: " + latitude);
                    console.log("Longitude: " + longitude);
                
                    // Define the coordinates of Dallas, TX
                    var dallasLatitude = 32.7767;
                    var dallasLongitude = -96.7970;
                
                    // Calculate the distance between the user's position and Dallas using the Haversine formula
                    var distance = calculateDistance(latitude, longitude, dallasLatitude, dallasLongitude);
                
                    // Check if the user is near Dallas (within a certain distance threshold)
                    var distanceThreshold = 50; // You can adjust this threshold in kilometers
                
                    if (distance <= distanceThreshold) {
                        console.log("You are near Dallas, TX!");
                    } else {
                        console.log("You are not near Dallas, TX.");
                    }
                }
                
                // Function to calculate the distance between two sets of coordinates using the Haversine formula
                function calculateDistance(lat1, lon1, lat2, lon2) {
                    var R = 6371; // Radius of the Earth in kilometers
                    var dLat = (lat2 - lat1) * (Math.PI / 180);
                    var dLon = (lon2 - lon1) * (Math.PI / 180);
                    var a =
                        Math.sin(dLat / 2) * Math.sin(dLat / 2) +
                        Math.cos(lat1 * (Math.PI / 180)) * Math.cos(lat2 * (Math.PI / 180)) *
                        Math.sin(dLon / 2) * Math.sin(dLon / 2);
                    var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
                    var distance = R * c;
                
                    return distance;
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