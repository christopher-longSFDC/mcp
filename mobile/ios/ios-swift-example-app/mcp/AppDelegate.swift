
import UIKit
import Evergage
import OSLog

@main
class AppDelegate: UIResponder, UIApplicationDelegate {


    func application(_ application: UIApplication, willFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        // Override point for customization after application launch.
        
        // Evergage SDK initialization
        let evergage = Evergage.sharedInstance()

        #if DEBUG
        evergage.logLevel = EVGLogLevel.all
        #endif

        // Set user ID if known
        // hardcoded here for testing purposes
        evergage.userId = "<>"

        // Start Evergage with your Evergage Configuration
        evergage.start { (clientConfigurationBuilder) in
            clientConfigurationBuilder.account = "<>"
            clientConfigurationBuilder.dataset = "<>"
            clientConfigurationBuilder.usePushNotifications = false // Not using push notifications for this demo app
            clientConfigurationBuilder.useDesignMode = false; // Enable Design Mode for debugging
        }
        
        print(" ********** Evergage initialization started *********** ")
        
        return true
    }

    func application(_ application: UIApplication, configurationForConnecting connectingSceneSession: UISceneSession, options: UIScene.ConnectionOptions) -> UISceneConfiguration {
        // Called when a new scene session is being created.
        // Use this method to select a configuration to create the new scene with.
        return UISceneConfiguration(name: "Default Configuration", sessionRole: connectingSceneSession.role)
    }

    func application(_ application: UIApplication, didDiscardSceneSessions sceneSessions: Set<UISceneSession>) {
        // Called when the user discards a scene session.
        // If any sessions were discarded while the application was not running, this will be called shortly after application:didFinishLaunchingWithOptions.
        // Use this method to release any resources that were specific to the discarded scenes, as they will not return.
    }

}
