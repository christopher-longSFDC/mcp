
import UIKit
import Evergage

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Per docs and to fill in the name in the `Screen: log lines`
        evergageName = "MainViewController"
        
        let handler = { [weak self] (campaign: EVGCampaign) -> Void in
            // Safest to perform a single method/operation on weakSelf, which will simply be a no-op if weakSelf is nil:
            self?.handleCampaign(campaign: campaign)
        }

        // The target string uniquely identifies the expected data schema
        evergageScreen?.setCampaignHandler(handler, forTarget: "HelloWorld")

        // Do any additional setup after loading the view
    }


    @IBAction func sendHelloWorldTrackAction(_ sender: Any) {
        print("********** sending Hello World Track action to trigger a campaign on this screen **********")
        evergageScreen?.trackAction("Hello World")
    }

    @IBAction func unwindToMainVC(unwindSegue: UIStoryboardSegue) {
        print("********** unwinding to the main view controller **********")
    }

    @IBAction func unwindPageTwo(unwindSegue: UIStoryboardSegue) {
        print("********** unwinding to the main view from page two **********")
    }

    private func handleCampaign(campaign: EVGCampaign) {
        // see PageOneViewController's for more realistic campaign handling
        print("********** inside handleCampaign for our main ViewController **********")
        print("     campaign.campaignName:  ", campaign.campaignName)
        print("     campaign.campaignId:    ", campaign.campaignId)
        print("     campaign.data:          ", campaign.data)

        // calling trackImpression to see this in the event stream
        evergageScreen?.trackImpression(campaign)
    }
}
