<!DOCTYPE html>
<html lang="en">
<head>
   <meta charset="utf-8">

   <title>ALICE VSD example with THttpServer</title>

   <!-- script src="/jsrootsys/scripts/JSRootCore.js" type="text/javascript"></script>  -->

   <!--  Use dev version due to several fixes compare to current version in ROOT -->
   <script src="http://jsroot.gsi.de/dev/scripts/JSRootCore.js" type="text/javascript"></script>

   <style>
     #main {
        overflow: hidden;
        position: absolute;
        left: 0px;
        top: 0px;
        right: 0px;
        bottom: 0px;
     }
     
     #custom_buttons {
        position: absolute;
        left: 10px;
        top: 10px;
     }

     #custom_table {
     position: absolute;
     left:10px;
     top: 100px;
     }
     
   </style>

</head>

<body>

  <div id="main"></div>
  

  
  <script type='text/javascript'>
    
  <div id="custom_buttons">
     <input id="btn_prev" type="button" title="Get previous event" value="Prev"/>
     <input id="btn_get" type="button" title="Get event" value="Get"/>
     <input id="btn_next" type="button" title="Get next event" value="Next"/>
     <input id="btn_table" type="button" title="popup table entries" value="GetTable"/>
  </div>

  
  <div id="custom_table">
     <input id="demo" type="table"/>
  </div>


  
var obj, dbParam, xmlhttp, myObj, x, txt = "";
      var geo_painter = null;
      
      function GetEvent() {
         if (!geo_painter) return;
         JSROOT.progress("Request and render EVE event");
         JSROOT.NewHttpRequest("eve/exe.json.gz?method=GetEvent&compact=23", 'object', function(lst) {
            geo_painter.clearExtras(); // remove old three.js container with tracks and hits
            geo_painter.drawExtras(lst);
            geo_painter.Render3D(); // not really needed, already toggled with clearExtras
            JSROOT.progress();
         }).send();
      }


      
      function GetTable() {
         JSROOT.NewHttpRequest("eve/exe.json.gz?method=GetTable&compact=23", 'object', function(lst) {

        JSON.parse(this.responseText);
        txt += "<table border='1'>"
        for (x in myObj) {
            txt += "<tr><td>" + myObj[x].name + "</td></tr>";
        }
        txt += "</table>"        
        document.getElementById("demo").innerHTML = txt;
      
         console.log("txt");
         }).send();
      }
      
      function Execute(method) {
         JSROOT.NewHttpRequest("eve/exe.json?method="+method, 'text', function(res) {
            console.log('Execute ', method, 'result', res);
            if (res==="1") GetEvent(); 
         }).send();
      }

      document.getElementById('btn_prev').onclick = function() {
         Execute("PrevEvent");
      }

      document.getElementById('btn_get').onclick = GetEvent;

      document.getElementById('btn_next').onclick = function() {
         Execute("NextEvent");
      }

      document.getElementById('btn_table').onclick = GetTable;
      
     JSROOT.NewHttpRequest("eve/exe.json.gz?method=GetGeometry&compact=23", 'object', function(res) {
        if (!res) return;
        JSROOT.draw("main", res, "", function(painter) {
           geo_painter = painter;
        });
     }).send();

  </script>

</body>

</html>
