sap.ui.define([
	"sap/ui/model/json/JSONModel",
	"sap/ui/core/mvc/Controller"
], function (JSONModel, Controller) {
	"use strict";

	return Controller.extend("flexiblecolumnlayout.Detail", {
		onInit: function () {
			this.oRouter = this.getOwnerComponent().getRouter();
		    this.oModel = this.getOwnerComponent().getModel();
		    console.log("set tank model");
/*
			var oModel3 = new sap.ui.model.json.JSONModel({ 
				filename: "http://xrd-cache-1.t2.ucsd.edu/alja/tank.root", 
				itemname: "geom", 
				opt: "z" 
			});
		    var pp = sap.ui.getCore().byId("__xmlview2--panel3");
		    console.log("whole ", pp);
		    console.log("ID whole ", pp0);
		    
	            pp.setModel(oModel3, "__xmlview2--panel3");
*/		    
		},
		handleDetailPress: function () {
			var oNextUIState = this.getOwnerComponent().getHelper().getNextUIState(2);
			this.oRouter.navTo("detailDetail", {layout: oNextUIState.layout});
		},
		handleFullScreen: function () {
			var sNextLayout = this.oModel.getProperty("/actionButtonsInfo/midColumn/fullScreen");
			this.oRouter.navTo("detail", {layout: sNextLayout});
		},
		handleExitFullScreen: function () {
			var sNextLayout = this.oModel.getProperty("/actionButtonsInfo/midColumn/exitFullScreen");
			this.oRouter.navTo("detail", {layout: sNextLayout});
		},
		handleClose: function () {
			var sNextLayout = this.oModel.getProperty("/actionButtonsInfo/midColumn/closeColumn");
			this.oRouter.navTo("master", {layout: sNextLayout});
		}
	});
}, true);
