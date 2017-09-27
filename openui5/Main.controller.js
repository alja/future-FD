sap.ui.controller("flexiblecolumnlayout.Main", {

  onPress: function (evt) {
    jQuery.sap.require("sap.m.MessageToast");
    sap.m.MessageToast.show(evt.getSource().getId() + " Pressed");
  }

});
