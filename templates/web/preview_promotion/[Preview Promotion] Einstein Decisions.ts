import { ContextualBanditConfig, decide } from "corvus";
import { ItemReference } from "common";

function isCdnOrExternalImage(asset?: Asset) {
    return asset?.type === "CdnImage" || asset?.type === "ExternalImage";
}

export class PromotionSearchOptions implements Search<string> {

    search(context: GearLifecycleContext, searchString: string): ItemReference[] {
        if (!searchString) return [];

        const promos: Promotion[] = context.services.catalog.findByName("Promotion", searchString) as Promotion[];
        return promos.reduce((allPromos: ItemReference[], promo: Promotion) => {
            const promoItem = {
                itemType: "Promotion",
                id: promo.id,
                label: promo.attributes["name"] != null ? promo.attributes["name"].value : promo.id,
            } as ItemReference;
            allPromos.push(promoItem);
            return allPromos;
        }, []);
    }
}

//adding a new class for promotion search to use with our new Preview UI
export class AvailablePromotions implements Search<string> {

    search(context: GearLifecycleContext, searchString: string): ItemReference[] {
        if (!searchString) return [];
        const promos: Promotion[] = context.services.catalog.findByName("Promotion", searchString) as Promotion[];
        return promos.reduce((allPromos: ItemReference[], promo: Promotion) => {
            const promoItem = {
                itemType: "Promotion",
                id: promo.id,
                label: promo.attributes["name"] != null ? promo.attributes["name"].value : promo.id
            } as ItemReference;
            allPromos.push(promoItem);
            return allPromos;
        }, []);
    }
}

export class AssetLookupOptions implements Lookup<string> {

    fallbackArm: ItemReference

    constructor(fallbackArm: ItemReference) {
        this.fallbackArm = fallbackArm;
    }

    lookup(context: GearLifecycleContext): string[] {
        if (!this.fallbackArm) return [];

        const fullPromo: Promotion = context.services.catalog.findItem("Promotion", this.fallbackArm.id) as Promotion;
        if (!fullPromo || !fullPromo.assets) return [];

        return fullPromo.assets.reduce((contentZones: string[], asset: Asset) => {
            if (isCdnOrExternalImage(asset) && asset?.contentZones) {
                asset.contentZones.forEach(zone => { if (!(zone in contentZones)) contentZones.push(zone) });
            }
            return contentZones;
        }, []);
    }
}

export class EinsteinDecisionsTemplate implements CampaignTemplateComponent {

    @header(' ')
    @headerSubtitle('Only promotion assets tagged with the targeted web content zone will be eligible to return in the campaign.')

    @hidden(true)
    forHeaderSubtitle;

    @searchOptions((self) => new PromotionSearchOptions())
    @title("Optional Fallback Promotion Selector")
    @subtitle(`Search for a fallback promotion to display if there are no eligible promotions to show to the end user.
              If no fallback is selected, the default site experience would display. (NOTE: This field is case-sensitive.)`)
    fallbackArm: ItemReference;

    @title("Fallback Asset Selector")
    @lookupOptions((self) => new AssetLookupOptions(self.fallbackArm))
    @hidden(this, (self) => !self.fallbackArm)
    @subtitle(`Select a Content Zone or Tag to determine which asset on your selected fallback promotion is rendered in
              the targeted web content zone.`)
    fallbackAsset: string;

    //adding intstructions and notes to explain how the preview UI can be used
    @markdown(`
**Instructions**
- Below, select the checkbox next to "Preview Promotion"
- Search for the Promotion you want to preview by typing its name
- Alternatively, put your mouse cursor in the search box and press the SPACE BAR twice to see all promotions
- Choose a Promotion

**Notes**
- **Remember to uncheck the "Preview Promotion" checkbox before you publish the template !!**
- Promotions will not preview if they do not have image urls or they do not have an image asset assigned to a content zone used in this template
`)
    //adding checkbox that shows or hides the search box for promotions preview
    @header('Preview a Specific Promotion')
    previewPromotion: boolean = false
    //adding searchbox to choose which promoton to preview
    @searchOptions((self) => new AvailablePromotions())
    @title("Search for Promotion to Preview")
    @subtitle("NOTE: This field is case-sensitive.")
    @hidden(this, (self) => !self.previewPromotion)
    preview: ItemReference;
    //

    run(context: CampaignComponentContext) {
        const banditConfig: ContextualBanditConfig = {
            maxResults: 1,
            contentZone: context.contentZone,
            fallbackArms: this.fallbackArm ? [this.fallbackArm.id] : []
        } as ContextualBanditConfig;

        const promotion: Promotion = decide(context, banditConfig, null)[0] as Promotion;
        //GET PROMOTION BASED ON PREVIEW SELECTION - BEGIN
        const promotionForPreviewOnly: any = context.services.catalog.findItem("Promotion",this.preview?.id) ? context.services.catalog.findItem("Promotion",this.preview.id) : "";
        //GET PROMOTION BASED ON PREVIEW SELECTION - END

        const fetchImageUrl = (promotion: Promotion, contentZone: string): string => {
            if (!promotion || !promotion.assets) return "";

            for (const asset of promotion.assets) {
                if (!isCdnOrExternalImage(asset)) continue;
                if (asset.contentZones?.includes(contentZone)) {
                    return (asset as ImageAsset).imageUrl;
                }
            }
            if (this.fallbackAsset && this.fallbackArm?.id === promotion.id) {
                for (const asset of promotion.assets) {
                    if (!isCdnOrExternalImage(asset)) continue;
                    if (asset.contentZones?.includes(this.fallbackAsset)) {
                        return (asset as ImageAsset).imageUrl;
                    }
                }
            }
            return "";
        };

        const imageUrl: string = fetchImageUrl(promotion, context.contentZone);
        const url: string = promotion?.attributes?.url?.value ? promotion.attributes.url.value as string : "";
        const id: string = promotion.id;

        //SET PROPERTIES BASED ON PROMOTION FOR PREVIEW - BEGIN
        const previewImageUrl: string = fetchImageUrl(promotionForPreviewOnly, context.contentZone) ? fetchImageUrl(promotionForPreviewOnly, context.contentZone): null;
        const previewUrl: string = promotionForPreviewOnly?.attributes?.url?.value ? promotionForPreviewOnly.attributes.url.value as string : null;
        const previewId: string = promotionForPreviewOnly?.id ? promotionForPreviewOnly.id : null;
        //SET PROPERTIES BASED ON PROMOTION FOR PREVIEW - END

        //Commenting out because we want to include the properties from our promotion to preview,
        //not just the promotion automatically chosen by Einstein Decisions
        /*return { imageUrl, url, id };*/

        //ADD PROPERTIES BASED ON PROMOTION FOR PREVIEW AND PROMOTION CHOSEN BY EINSTEIN DECISIONS - BEGIN
        return { imageUrl, url, id, previewImageUrl, previewUrl, previewId};
        //ADD PROPERTIES BASED ON PROMOTION FOR PREVIEW AND PROMOTION CHOSEN BY EINSTEIN DECISIONS - END
    }

}
