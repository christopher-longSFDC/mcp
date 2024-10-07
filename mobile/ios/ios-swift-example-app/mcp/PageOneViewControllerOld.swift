import UIKit
import Evergage

class PageOneViewControllerOld: UIViewController {

    override func viewDidLoad() {
        // https://developer.apple.com/documentation/uikit/uiviewcontroller
        // Called after the controller's view is loaded into memory.

        // Per docs and to fill in the name in the Screen: log lines
        evergageName = "PageOneViewController"
        
        super.viewDidLoad()

        // Example item
//        let item = EVGItem()
//        item.itemType = "product"
//        item.itemId = "12345"
//        item.name = "Example Product"
//
//        // Track viewing item details
//        let evergage = Evergage.sharedInstance().viewItemDetail(item)
//        
//        // You can log this for debugging purposes
//        print("Tracked viewItemDetail for item: \(item.name ?? "No name")")
        print("viewDidLoad and about to track action PageOne")
        evergageScreen?.trackAction("PageOne")

    }
    
//    override func viewWillAppear(_ animated: Bool) {
//        super.viewWillAppear(animated)
//    }

    override func viewWillDisappear(_ animated: Bool) {
        // https://developer.apple.com/documentation/uikit/uiviewcontroller
        // Notifies the view controller that its view is about to be removed from a view hierarchy.
        
        // this is a good place to save changes or other state information
        super.viewWillDisappear(animated)

        // Stop tracking item detail when the view disappears
        //Evergage.sharedInstance().viewItemDetail(nil)
        
    }
    
    override func viewDidAppear(_ animated: Bool) {
        //
        // Notifies the view controller that its view was added to a view hierarchy.
        print("viewDidAppear!")
        super.viewDidAppear(animated)
    }
    
    // Example from EVGCampaignHandler.html
    override func viewWillAppear(_ animated: Bool) {
        //
        // Notifies the view controller that its view is about to be added to a view hierarchy.
        
        print("top of viewWillAppear")
        super.viewWillAppear(animated)
        
        // Note self is captured weakly
        let handler = { [weak self] (campaign: EVGCampaign) -> Void in
            // Safest to perform a single method/operation on weakSelf, which will simply be a no-op if weakSelf is nil:
            self?.handleCampaign(campaign: campaign)
        }

        // The target string uniquely identifies the expected data schema - here, a featured product:
        evergageScreen?.setCampaignHandler(handler, forTarget: "PageOne")
    }

    func handleCampaign(campaign: EVGCampaign) {
        print("inside handleCampaign will track an impression after checking for featuredProductName")
        // Validate the campaign data since it's dynamic JSON. Avoid processing if fails.
        guard let featuredProductName = campaign.data["featuredProductName"] as? String
            else { return }
        if (featuredProductName.isEmpty) { return }

// TODO activeCampaign is not in scope
//        // Check if the same content is already visible/active (see Usage Details above).
//        if (EVGCampaign.activeCampaign && activeCampaign.equals(campaign)) {
//        NSLog("Ignoring campaign name %@ since equivalent content is already active",
//            campaign.campaignName)
//            return
//        }

        print("about to trackImpression for campaign")
        // Track the impression for statistics even if the user is in the control group.
        evergageScreen?.trackImpression(campaign)

// TODO activeCampaign is not in scope
//        // Only display the campaign if the user is not in the control group.
//        if (!campaign.isControlGroup) {
//            // Keep active campaign as long as needed for (re)display and comparison
//            activeCampaign = campaign
//            NSLog("New active campaign name %@ for target %@ with data %@",
//                campaign.campaignName, campaign.target, campaign.data)
//
//            featuredProductLabel.text = String(format: "Our featured product is %@!", featuredProductName)
//        }
    }
}

