import UIKit
import Evergage

/*
 // uiviewcontroller lifecycle methods in rough order when a View Controller is added to the view hierarchy:
 // https://developer.apple.com/documentation/uikit/uiviewcontroller
 
   . loadView()
   . loadViewIfNeeded()
   . viewDidLoad()
   . viewWillAppear(_ animated: Bool)
   . viewWillLayoutSubviews()
   . viewDidLayoutSubviews()
   . viewDidAppear(_ animated: Bool)
   . viewWillDisappear(_ animated: Bool)
   . viewDidDisappear(_ animated: Bool)
*/


class PageOneViewController: UIViewController {
    
    var activeCampaign: EVGCampaign?

    // Called after the controller's view is loaded into memory.
    override func viewDidLoad() {
    
        super.viewDidLoad()

        // Setting this to fill in the name in the Screen: log lines
        evergageName = "PageOneViewController"
        
        //custom action for this page
        evergageScreen?.trackAction("PageOneTrackActionFromViewDidLoad")
    }

    // Example from EVGCampaignHandler.html
    override func viewWillAppear(_ animated: Bool) {

        super.viewWillAppear(animated)

        // Note self is captured weakly
        let handler = { [weak self] (campaign: EVGCampaign) -> Void in
            
            // Safest to perform a single method/operation on weakSelf, which will simply be a no-op if weakSelf is nil:
            self?.handleCampaign(campaign: campaign)
        }

        // The target string uniquely identifies the expected data schema - here, a featured product:
        evergageScreen?.setCampaignHandler(handler, forTarget: "PageOne")
        
        evergageScreen?.trackAction("PageOneTrackActionFromViewWillAppear")
    }

    override func viewDidAppear(_ animated: Bool) {

        super.viewDidAppear(animated)

        evergageScreen?.trackAction("PageOneTrackActionFromViewDidAppear")
        
        
        if (evergageScreen?.isVisible != nil && evergageScreen?.isVisible == true) {
            evergageScreen?.trackAction("PageOne")
        }
    }

    // this is a good place to save changes or other state information
    override func viewWillDisappear(_ animated: Bool) {

        super.viewWillDisappear(animated)
      
        evergageScreen?.trackAction("PageOneTrackActionFromViewDidDisappear")
    }

    private func handleCampaign(campaign: EVGCampaign) {
        // Validate the campaign data since it's dynamic JSON. Avoid processing if fails.
        guard let featuredProductName = campaign.data["featuredProductName"] as? String
            else { return }
        if (featuredProductName.isEmpty) { return }

        print("********** inside handleCampaign for PageOneViewController **********")
        print("     campaign.campaignName:  ", campaign.campaignName)
        print("     campaign.campaignId:    ", campaign.campaignId)
        print("     campaign.data:          ", campaign.data)
        
        // Check if the same content is already visible/active
        if (activeCampaign != nil && activeCampaign!.isEqual(campaign)) {
        NSLog("Ignoring campaign name %@ since equivalent content is already active",
            campaign.campaignName)
            return
        }

        // Track the impression for statistics even if the user is in the control group.
        evergageScreen?.trackImpression(campaign)

        // Only display the campaign if the user is not in the control group.
        if (!campaign.isControlGroup) {
            // Keep active campaign as long as needed for (re)display and comparison
            activeCampaign = campaign
            NSLog("New active campaign name %@ for target %@ with data %@",
                campaign.campaignName, campaign.target, campaign.data)
        }
    }
}
