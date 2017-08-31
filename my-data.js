
	     //Define some sample data
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



var aTestTreeData = [
	 {
		  text: "Node0",
		  ref: "images/action.png",
 				nodes: [
 					{
 						text: "Node0-1",
                  ref: "images/e-care.png",
 					},
 					{
 						text: "Node0-2",
 						ref: "images/e-care.png"
 					}
 				]
			},
 			{
 				text: "Node1",
 				ref: "sap-icon://list",
 				nodes: [
 					{
 						text: "Node1-1",
                  ref: "sap-icon://e-care"
 					},
 					{
 						text: "Node1-2",
 						ref: "images/action.png"
 					}
 				]
 			}
 		];
