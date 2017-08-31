


sap.m.StandardTreeItem.extend('MySuperDuperTreeItem', {
    metadata: {
	properties: {
	    status: 'string'
	}
    },	     
    onAfterRendering: function() {
        console.log("after tree rendering");
	sap.m.StandardTreeItem.prototype.onAfterRendering.apply(this, arguments);
        var title = this.getTitle();
        this.$().removeClass("sapMLIBSelected");
        this.$().find('span').each(function() {
            if ($(this).hasClass('sapMLIBIconDet')) {
                if (jQuery.sap.startsWith(title, "Track")) {
                    $(this).addClass("myPurple");
                } else if (jQuery.sap.startsWith(title, "Muon")) {
                    $(this).addClass("myYellow");
                }
            }
        });
    },
    renderer:"sap.m.StandardTreeItemRenderer" 
});
	 


addTreeElement = function( tname) {
                 var tree_nodes = [];
                 var size = 3;
                 if ( tname == "Track")
                 {
                    size =   aData.length;
                 }
                 for (i = 0; i < size ; ++i) {
                                 var xx =  tname + " " + aData[i].Idx + ", pt = " +  aData[i].pt ;
                     tree_nodes.push({text: xx,  ref: 'sap-icon://color-fill', selected: aData[i].visible });
                 }
             return tree_nodes;
         }


