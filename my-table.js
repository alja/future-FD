


/**
 * Adds a custom sort menu for a given table
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
