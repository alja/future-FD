var gTable;

sap.ui.define([
	'sap/ui/core/mvc/Controller',
        'sap/ui/model/json/JSONModel',
        'sap/ui/table/Table',
        'sap/ui/commons/CheckBox',
	'sap/ui/table/Column'
], function( Controller, JSONModel) {
	"use strict";


    return Controller.extend("flexiblecolumnlayout.DetailDetail", {
	onInit: function () {
	    this.oRouter = this.getOwnerComponent().getRouter();
	    this.oModel  = this.getOwnerComponent().getModel();

            
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

            var tModel = new JSONModel({
		modelData: aData
	    });
	  // this.getView().setModel(oModel);

            
            var oTable = this.getView().byId("table");
            gTable = oTable;
	   // oTable.addStyleClass(sapUiTinyMargin);

   var oCcon = new sap.ui.commons.CheckBox("testCB", {change: colChecked}).bindProperty("checked", "visible");
	     oCcon.bindProperty("name", "Idx");
             oTable.addColumn(new sap.ui.table.Column({
	         label: new sap.ui.commons.Label({text: "Rnr"}),
	         template: oCcon,
	         width: "50px",
	         hAlign: "Center",
                 sortProperty: "visible",
                 filterProperty: "visible"
             }));

             
             oTable.addColumn(new sap.ui.table.Column({
                 label: new sap.ui.commons.Label({
                     text: "Col"
                 }),
                 template: new sap.ui.commons.TextView().bindProperty("text", "Idx", function(cellValue) {
                     this.addStyleClass('myPurple'); 
                     return cellValue;           
                 }),           
                 width: "40px"
             }));
	    
            // momentum, eta, phi
            var oControl = new sap.ui.commons.TextField().bindProperty("value", "pt");
	    var oColPt = new sap.ui.table.Column({label: new sap.ui.commons.Label({text: "pt", textAlign: "Left"}), template: oControl, sortProperty: "pt", filterProperty: "pt", width: "70px"});
            oTable.addColumn(oColPt);
           // addColumnSorterAndFilter(oColPt, compareFloats);

            oControl = new sap.ui.commons.TextField().bindProperty("value", "eta");
	    var oColEta = new sap.ui.table.Column({label: new sap.ui.commons.Label({text: "eta", textAlign: "Left"}), template: oControl, sortProperty: "eta", filterProperty: "eta", width: "70px"});
            oTable.addColumn(oColEta);
          //  addColumnSorterAndFilter(oColEta, compareFloats);

            oControl = new sap.ui.commons.TextField().bindProperty("value", "phi");
	    var oColPhi = new sap.ui.table.Column({label: new sap.ui.commons.Label({text: "phi", textAlign: "Left"}), template: oControl, sortProperty: "phi", filterProperty: "phi", width: "70px"});
            oTable.addColumn(oColPhi);
           // addColumnSorterAndFilter(oColPhi, compareFloats);


	    //Create a model and bind the table rows to this model
	    oTable.setModel(tModel);
	    oTable.bindRows("/modelData");
	    oTable.addStyleClass("sapUiSizeCondensed");
	},
	handleDetailDetailPress: function () {
            console.log("got ot page2");
	    var oNextUIState = this.getOwnerComponent().getHelper().getNextUIState(3);
	    this.oRouter.navTo("page2", {layout: oNextUIState.layout});
	},
	handleFullScreen: function () {
	    var sNextLayout = this.oModel.getProperty("/actionButtonsInfo/endColumn/fullScreen");
	    this.oRouter.navTo("detailDetail", {layout: sNextLayout});
	},
	handleExitFullScreen: function () {
	    var sNextLayout = this.oModel.getProperty("/actionButtonsInfo/endColumn/exitFullScreen");
	    this.oRouter.navTo("detailDetail", {layout: sNextLayout});
	},
	handleClose: function () {
	    var sNextLayout = this.oModel.getProperty("/actionButtonsInfo/endColumn/closeColumn");
	    this.oRouter.navTo("detail", {layout: sNextLayout});
	}
    });
});


 var colChecked = function(oEvent){
		 console.log(oEvent);
		 var xxx = $(this);
     var idx = Number(xxx[0].mProperties.name);


            
                 gTree.setSelectedItem(
                     gTree.getItems()[idx], xxx[0].mProperties.checked, Boolean(1));

                 gTree.getModel().refresh();


     
             };

/*
 *
 * @param oColumn Target table column to add custom menu
 * @param comparator Function to compare two values of column oColumn
 */
function addColumnSorterAndFilter(oColumn, comparator) {
    var oTable = oColumn.getParent();
    var oCustomMenu = new sap.ui.commons.Menu();
    
    oCustomMenu.addItem(new sap.ui.commons.MenuItem({
        text: 'Sort ascending',
        icon:"sap-icon://sort-ascending",
        select:function() {
            var oSorter = new sap.ui.model.Sorter(oColumn.getSortProperty(), false);
            oSorter.fnCompare=comparator;
            oTable.getBinding("rows").sort(oSorter);
            
            for (var i=0;i<oTable.getColumns().length; i++) oTable.getColumns()[i].setSorted(false);                
            oColumn.setSorted(true);
            oColumn.setSortOrder(sap.ui.table.SortOrder.Ascending);
        }
    }));
    oCustomMenu.addItem(new sap.ui.commons.MenuItem({
        text: 'Sort descending',
        icon:"sap-icon://sort-descending",
        select:function(oControlEvent) {
            var oSorter = new sap.ui.model.Sorter(oColumn.getSortProperty(), true);
            oSorter.fnCompare=comparator;
            oTable.getBinding("rows").sort(oSorter);
            
            for (var i=0;i<oTable.getColumns().length; i++) oTable.getColumns()[i].setSorted(false);
            
            oColumn.setSorted(true);
            oColumn.setSortOrder(sap.ui.table.SortOrder.Descending);
        }
    }));
    
    oCustomMenu.addItem(new sap.ui.commons.MenuTextFieldItem({
        text: 'Edit',
        icon: "sap-icon://edit",
        select: function(oControlEvent) {
            var filterValue = oControlEvent.getParameters().item.getValue();
            var filterProperty = oControlEvent.getSource().getParent().getParent().mProperties.sortProperty;
            var filters = [];
            if (filterValue.trim() != '') {
                var oFilter1 = new sap.ui.model.Filter(filterProperty, sap.ui.model.FilterOperator.EQ, filterValue);
                filters = [oFilter1];   
            }
            oTable.getBinding("rows").filter(filters, sap.ui.model.FilterType.Application);
        }
    }));
    
    
    oColumn.setMenu(oCustomMenu);
    return oColumn;
}


/**
 * Integer comparator
 */
function compareIntegers(value1, value2) {
    if ((value1 == null || value1 == undefined || value1 == '') &&
	(value2 == null || value2 == undefined || value2 == '')) return 0;
    if ((value1 == null || value1 == undefined || value1 == '')) return -1;
    if ((value2 == null || value2 == undefined || value2 == '')) return 1;
    if(parseInt(value1) < parseInt(value2)) return -1;
    if(parseInt(value1) == parseInt(value2)) return 0;
    if(parseInt(value1) > parseInt(value2)) return 1;           
};



/**
 * Floats comparator
 */
function compareFloats(value1, value2) {
    if ((value1 == null || value1 == undefined || value1 == '') &&
	(value2 == null || value2 == undefined || value2 == '')) return 0;
    if ((value1 == null || value1 == undefined || value1 == '')) return -1;
    if ((value2 == null || value2 == undefined || value2 == '')) return 1;
    if(parseFloat(value1) < parseFloat(value2)) return -1;
    if(parseFloat(value1) == parseFloat(value2)) return 0;
    if(parseFloat(value1) > parseFloat(value2)) return 1;           
};
