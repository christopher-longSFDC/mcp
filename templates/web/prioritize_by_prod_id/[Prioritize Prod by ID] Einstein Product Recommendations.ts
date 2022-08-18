import { RecommendationsConfig, recommend } from "recs";

export class EinsteinProductRecsTemplate implements CampaignTemplateComponent {

    /**
     * Developer Controls
     */

    @hidden(true)
    maximumNumberOfProducts: 2 | 4 | 6 | 8 = 4;

    @hidden(true)
    maxRatingBound: number = 5;

    /**
     * Business-User Controls
     */

    @title("Recommendations Block Title")
    header: string = "Title Text";

    @title(" ")
    recsConfig: RecommendationsConfig = new RecommendationsConfig()
        .restrictItemType("Product")
        .restrictMaxResults(this.maximumNumberOfProducts);

    //Add an input to set the Product ID to prioritize
    //The value set is used later in the run() function
    @title("Product ID to Prioritize")
    prioritizeProdId: string = ""

    @header("Recommendation Display Attributes")

    @title("Show product name")
    nameVisibility: boolean = true;

    @title("Show product description")
    descriptionVisibility: boolean = true;

    @title("Show product price")
    priceVisibility: boolean = true;

    @title("Show product rating")
    ratingVisibility: boolean = false;

    run(context: CampaignComponentContext) {
        this.recsConfig.maxResults = this.maximumNumberOfProducts;

        //CREATE NEW PROD ARRAY WITH PRIORITIZED PROD - BEGIN
        let specialId = this.prioritizeProdId;
        let tempProds = recommend(context, this.recsConfig);
        let obj = tempProds.find(o => o.id == specialId);
        if(obj){
            tempProds.sort(function(x,y){ return x.id == specialId ? -1 : y.id == specialId ? 1 : 0; });
        }
        //CREATE NEW PROD ARRAY WITH PRIORITIZED PROD - END

        return {
            itemType: this.recsConfig.itemType,
            //Commenting out becuase we want to use our "prioritized" array instead of the original aray
            /*products: recommend(context, this.recsConfig)*/

            //Assign our "prioritized" array to the products array property in the payload
            products: tempProds
        };
    }

}
