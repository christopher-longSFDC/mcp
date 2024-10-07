# MCP Mobile Native iOS SDK Demo App with Swift

The Marketing Cloud Personalization (MCP) iOS SDK integrates MCP with native iOS apps/code for both deep tracking and delivery of personalization content into the app.

### Dev Docs
https://developer.salesforce.com/docs/marketing/personalization/references/personalization-ios-sdk/release-notes.html

## Requirements
- XCode - latest version
    - Simulated Devices (latest version or select what you want)
    - iOS >= 12.0 (>15 to support grey button in View, or change to a normal button for lower level)
- [Cocoapods](https://cocoapods.org/) to install dependencies
    - [Pod Install Docs](https://guides.cocoapods.org/using/pod-install-vs-update.html#pod-install) 
- Salesforce MCP account and dataset (testing dataset suggested)
- The code is for demo purposes only

## Install
- Download and unzip app.
- Open a terminal at that location.
- Assuming you have cocoapods installed, run `pod install` to load the pods (dependencies) that the app needs to run.
    - (`pod 'Evergage'`) is the only one necessary for this app and that will download the latest version (`1.4.1` as of Oct 2024).
    - You can specify a version there if you want to test a prior release.

## Setup
#### Local Env
- From the unzipped `mcp/` folder, Open the XCode Workspace (mcp.xcworkspace) which will open up XCode for you.
- In the AppDelgate swift file, update the `<>` variables with your account/dataset/userID.
- You will need a device to run this on configured.  See XCode docs for details.
- Click the play button to build and open up the simulator with the specified device type.  App will launch at this time and debugger will start.
- You should be able to see the default events in the Reports > [Activity section] Event Stream.  Filter on type mobile if you have lots of other events in this dataset.
    - Search the code for `evergageScreen?.` to see or edit what is sent to your account.
    - May need to update a Setting > [Integration Setup section] General Setup > Expand the Advanced Options > `Check Process Item Data from Native Mobile Apps` AND also check 
        `Also allow the events to create new items and update existing item metadata.` 
- Additional setup below will trigger campaign handlers and show impression tracking.  We need a few events to look for, so play around with the app to start sending events/items/tags/categories.

#### MCP
- From MCP Admin > Mobile > Select your App 
    - Confirm Bundle ID matches code or update/create a new app for testing this app
- In a new tab, open Mobile > Mobile Data Campaigns 
    - Create a campaign called `Hello World`
        - Click the Setup Cog in orange to configure
        - On the Campaign Settings Tab, Select this new app as a Source
        - In the Target Pages section > Add Rule > Action > INCLUDES > type "Hello World" in the box and select it as an action.
            - Note you need to have sent that event to see it in the dropdown to select it.  You can play around with the app or call the api with curl or postman to accomplish this as well.
        - Enter in the Target box the value of `HelloWorld`.  This will match up to what is in the code.
        - Add whatever static values you'd like to test this campaign. 
            example: `Name: Hello` and `Value: World` `Type: Text`
            - Message rules aren't necessary here since it's static content here
            - Click Save & Close and Publish.
    - Create a campaign called `PageOne` with a Target of `PageOne` and select your Target Application and Target Pages (`PageOne`) for this campaign.
        - The code checks for a key of `featuredProductName` in this campaign, so be sure to add that with a static value of type text.
        - Add other static values if you wish.
        - Click Save & Close and Publish.
    - Create a campaign called `PageTwo` with a Target of `PageTwo` and select your Target Application and Target Pages (`PageTwo`) for this campaign.
        - This campaign exercises some Dynamic content sending the `${user.id}`.
            - Create an attribute called `userID` with a value of `${user.id}` of type Text.
            - Click Save & Close and Publish.

## Running / Validating
- Click the Play button from the XCode header bar to build the app and run it on the specified device (simulated or real).
    - In Xcode you can see the logs from the SDK as well and some extra print() statements in the code to help follow along and show campaignd data being received.
- Open the MCP > Reports > [Activity section] Event Stream to watch as traffic flows into the system.
    - selecting a row item shows additional details about the event received.
- Campaign list page will show stats collected as well (keep in mind they are near-ish real time but can be delayed to show in the UI)


## Questions?
Start a thread in #help-technical-personalization
