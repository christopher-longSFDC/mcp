
export class LookbackActivity {

        //adjust the lookback period as needed
        //type is number representing days
        static LOOKBACK_DURAION = 30 as number;

        getLookbackActivityPayload(context: CampaignComponentContext){

        //set the lookback date for activity in the past
        const lookback = new Date();
        lookback.setDate(lookback.getDate() - LookbackActivity.LOOKBACK_DURAION);
        const startDateEpoch = lookback.getTime();
        const formattedUTCStardDate = new Date(startDateEpoch + lookback.getTimezoneOffset() * 60 * 1000);

        //set the start date for the lookback period to 'right now'
        const now = Date.now();
        const endDateEpoch = new Date(now);
        const formattedUTCEndDate = new Date(endDateEpoch.getTime() + endDateEpoch.getTimezoneOffset() * 60 * 1000);
        
        //actionStatRequest for Product Detail pages
        //--This action_stat example assumes your events are captured with the SalesforceInteractions
        //--namespace set actionName equal to the Interaction Name from the event. 
        //--If your sitemap uses the old Evergage namespace and defines an 'action' for the event,
        //--set actionName equal to that "action" instead of the Interaction Name.
        let action_stat = {actionName:"View Item",start:formattedUTCStardDate,end:formattedUTCEndDate};
        //count of Product Detail views based on actionStatRequest
        let _userActionStatCount = context.user.actionCount(action_stat);

        //activity history for products (using same start and end date as the actionStatRequest but it doesn't have to)
        let _product_activity = JSON.stringify(context?.user.getDimensionActivity("Product",formattedUTCStardDate,formattedUTCEndDate));

        return {
            lookback_activity:{
                duration_in_days: LookbackActivity.LOOKBACK_DURAION,
                "start_date" : formattedUTCStardDate.toDateString(),
                "end_date" : formattedUTCEndDate.toDateString(),
                "total_product_views" : _userActionStatCount,
                "product_activity_history" : JSON.parse(_product_activity) ? JSON.parse(_product_activity) : null
            }
        }
    }
}

export class UserInfo {

    //set as true to include history of visits (could be a lot of data)
    //set as false to not include history of visits
    static INCLUDE_ALL_TIME_VISIT_HISTORY = true;

    getUserInfoPayload(context: CampaignComponentContext){

        //user attributes
        let _user_attributes = context?.user?.attributes ? context?.user?.attributes : null;

        //user location
        let _location = UserInfo.INCLUDE_ALL_TIME_VISIT_HISTORY ? JSON.parse(JSON.stringify(context?.user?.location)) : null;

        //profile objects
        let _user_profile_objects = context?.user?.profileObjects ? context?.user?.profileObjects : null;

        //user segments
        let _user_segments = context?.user?.segmentMembership ? context?.user?.segmentMembership : null ;

        //order history - complete history including 'Open','Purchased' or 'Cancelled' orders
        let _order_history = context?.user?.orderHistory ? context?.user?.orderHistory : null;

        //most recent purchase
        let _most_recent_order = context?.user?.getLatestOrderByStatus("Purchased") ? context?.user?.getLatestOrderByStatus("Purchased") : null;

        //current cart
        let _current_cart = context?.user?.currentCart ? context?.user?.currentCart : null; 

        //all time visit history
        let _all_time_visit_history = UserInfo.INCLUDE_ALL_TIME_VISIT_HISTORY ? context?.user?.visits.map((visit)=>{

            let _formattedVisit = {};

            //visit event date and times
            let _visit_date = new Date(visit.start)
            let _visit_first_event = _visit_date.toTimeString();
            let _visit_last_event = new Date(visit.lastEventTime).toTimeString();
            _formattedVisit['visit_date'] = _visit_date.toDateString();
            _formattedVisit['visit_first_event'] = _visit_first_event;
            _formattedVisit['visit_last_event'] = _visit_last_event;

            //device
            _formattedVisit['device_type'] = visit.deviceType;

            //operating system
            _formattedVisit['operating_system'] = visit.operatingSystem;

            //browser
            _formattedVisit['browser'] = visit.browser;

            //referrer
            _formattedVisit['referrer'] = JSON.parse(JSON.stringify(visit.referrer));

            //weather
            _formattedVisit['weather'] = JSON.parse(JSON.stringify(visit.weather));

            return _formattedVisit;

        }) : null;

        const result = {
            "attributes" : _user_attributes,
            "location" : _location,
            "segments" : _user_segments,
            "profile_objects" : _user_profile_objects,
            "most_recent_purchase" : _most_recent_order,
            "current_cart" : _current_cart,
            "order_history" : _order_history,
            "visit_history" : _all_time_visit_history

        };

        return result;
    }
}

export class ServerSideMiscUserInfoTemplate implements CampaignTemplateComponent {

    run(context: CampaignComponentContext) {

        const _lookbackActivity = new LookbackActivity();
        const _userInfo = new UserInfo();

        return {
            "user" : {
                ..._lookbackActivity.getLookbackActivityPayload(context),
                ..._userInfo.getUserInfoPayload(context)
            }
        };

    }
}
