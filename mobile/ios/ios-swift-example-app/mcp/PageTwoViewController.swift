import UIKit
import Evergage.EVGItems

class PageTwoViewController: UIViewController {
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Setting this to fill in the name in the Screen: log lines
        evergageName = "PageTwoViewController"
    }

    // code samples from:
    // https://developer.salesforce.com/docs/marketing/personalization/references/personalization-ios-sdk/tracking.html
    // Example using viewWillAppear, could consider viewDidAppear
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        let handler = { [weak self] (campaign: EVGCampaign)  -> Void in
            self?.handleCampaign(campaign: campaign)
        }
        
        evergageScreen?.setCampaignHandler(handler, forTarget: "PageTwo")
        
        refreshScreen()
    }

    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        
        if (evergageScreen?.isVisible != nil && evergageScreen?.isVisible == true) {
            evergageScreen?.trackAction("PageTwo")
        }
    }
    
    func handleCampaign(campaign: EVGCampaign) {
        print("********** inside handleCampaign for PageTwo view controller **********")
        print("campaign.campaignName in handleCampaign: ", campaign.campaignName)
        print("campaign.campaignId in handleCampaign: ", campaign.campaignId)
        print("campaign.data: ", campaign.data)
        
        evergageScreen?.trackImpression(campaign)
    }
    

    private func onPullToRefresh() {
        refreshScreen()
    }

    private func refreshScreen() {
        // If screen is viewing a product:
        evergageScreen?.viewItem(EVGProduct.init(id: "kiwi"))

        // If screen is viewing a category, like women's merchandise:
        evergageScreen?.viewCategory(EVGCategory.init(id: "Fruits"))

        // Or if screen is viewing a tag, like some specific brand:
        evergageScreen?.viewTag(EVGTag.init(id: "SomeBrand", type: EVGTagType.brand))

        // Or maybe screen isn't related to your catalog:
        evergageScreen?.trackAction("User Profile")
        
        // perhaps this is a page for just your favorite item
        evergageScreen?.favorite(EVGProduct.init(id: "kiwi"))

        // ... your content fetching/displaying, etc.
    }
}
