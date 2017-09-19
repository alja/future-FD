var gTree;

sap.ui.define([
    "sap/ui/model/json/JSONModel",
    "sap/ui/core/mvc/Controller"
], function (JSONModel, Controller) {
    "use strict";

    return Controller.extend("flexiblecolumnlayout.Master", {
	onInit: function () {
	    this.oRouter = this.getOwnerComponent().getRouter();

            var oTreeModel = new sap.ui.model.json.JSONModel();
	    var oTree = this.getView().byId("tree");
            gTree = oTree;
            oTree.setModel(oTreeModel);
            oTree.setMode(sap.m.ListMode.MultiSelect);

            var oTreeData = [];
	    var track_nodes = addTreeElement("Track" );
	    var muon_nodes = addTreeElement( "Muon" );     
	    oTreeData.push({ text:"Tracks", ref: "sap-icon://color-fill", nodes: track_nodes }); 
	    oTreeData.push({ text:"Muons" , ref: "sap-icon://color-fill", nodes: muon_nodes });
	    oTreeModel.setData(oTreeData);
            
	    var oStandardTreeItem = new MySuperDuperTreeItem ({
		title: "{text}",
		selected: "{selected}",
                type: sap.m.ListType.Detail
   	    });

	    
//            var oTable = gTable;
            oTree.bindItems("/", oStandardTreeItem);
            oTree.expandToLevel(1);
            // set the data to the model
            var oTVToolbar = new sap.m.Toolbar("toolbar", {
		content: [
                    new sap.m.Button({
			tooltip : "Add Collection",
			icon: "sap-icon://add"
                    })
                ]
            });
	    
	},
	handleMasterPress: function () {
	    var oNextUIState = this.getOwnerComponent().getHelper().getNextUIState(1);
	    this.oRouter.navTo("detail", {layout: oNextUIState.layout});
	}
    });
}, true);



sap.m.StandardTreeItem.extend('MySuperDuperTreeItem', {
    metadata: {
	properties: {
	    status: 'string'
	}
    },	     
    onAfterRendering: function() {
	sap.m.StandardTreeItem.prototype.onAfterRendering.apply(this, arguments);
        var title = this.getTitle();
        this.$().removeClass("sapMLIBSelected");
        this.$().find('span').each(function() {
            if ($(this).hasClass('sapMLIBIconDet')) {
        console.log("after tree rendering");
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


var aData = [
    {Idx: "1", eta: "1.1", phi: "3.1", pt: "1.1", color: "purple", visible:true},
    {Idx: "2", eta: "1.1", phi: "3.1", pt: "1.2", color: "purple", visible: true},
    {Idx: "3", eta: "1.1", phi: "3.1", pt: "1.0", color: "purple", visible: false},
    {Idx: "4", eta: "1.1", phi: "3.1", pt: "1.0", color: "purple", visible: true},
    {Idx: "5", eta: "1.1", phi: "3.1", pt: "1.0", color: "purple", visible: true},
    {Idx: "6", eta: "1.1", phi: "3.1", pt: "12.0", color: "purple", visible: false},
    {Idx: "7", eta: "1.1", phi: "3.1", pt: "3.0", color: "purple", visible: true},
    {Idx: "8", eta: "1.1", phi: "3.1", pt: "3.0", color: "purple", visible: true},
    {Idx: "9", eta: "1.1", phi: "3.1", pt: "3.4", color: "purple", visible: true},
    {Idx: "10", eta: "1.1", phi: "3.1", pt: "3.4", color: "purple", visible: true},
    {Idx: "11", eta: "1.1", phi: "3.1", pt: "3.4", color: "purple", visible: false},
    {Idx: "12", eta: "1.1", phi: "3.1", pt: "55.0", color: "purple", visible: true},
    {Idx: "13", eta: "1.1", phi: "3.1", pt: "7.0", color: "purple", visible: true},
    {Idx: "14", eta: "1.1", phi: "3.1", pt: "100.0", color: "purple", visible: true},
    {Idx: "15", eta: "1.1", phi: "3.1", pt: "3.3", color: "purple", visible: true}

];

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




onSelectionChange = function(oEvent) {
    console.log("tree selection BEGIN ");
    var items = oEvent.getParameter('listItems');

    items.forEach(function(item){
        item.$().removeClass('sapMLIBSelected');
    });

    var oTree = this.getView().byId("tree");
    var oModel = oTree.getModel();
    var item =  oEvent.getParameter('listItem');
    
    if (item) {
        //item.$().removeClass('sapMLIBSelected');

	var xx = item.mProperties.title.split(",", 2);
        if (xx.length > 1) {
	    var yy = xx[0].split(" ", 2);
            var yyn = parseInt(yy[1]);
            aData[yyn-1].visible =  item.mProperties.selected;                     
            gTable.getModel().setData([]);      
            gTable.getModel().setData({modelData: aData});
            gTable.getModel().refresh(true);
        }
    }
}
