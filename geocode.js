var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
// Constants
var url = "https://maps.googleapis.com/maps/api/geocode/json?";

// Key storage
var key = "AIzaSyAyn7a1Yn847Ex4WgAZ17p4QAWeA1a-iWc";  // API key to use when processing

var addr = "Kepong";    

// Makes the request and on callback processes.
function doAddress() {
  var xhr = new XMLHttpRequest();

  // Generate the request.
  var local_url = url + "address=" + encodeURIComponent(addr) + "&key=" + encodeURIComponent(key);
  // Request
  xhr.open("GET", local_url, true);
  xhr.onreadystatechange = function (e) {
    if (this.readyState == 1 && this.status == 200) {  // Connected to Google
      var result = JSON.parse(xhr.responseText);
      if (result.status === "OK") {
        var geometry = result.results[0].geometry;
        var location = geometry.location;

        console.log(location.lat + ', ' + location.lng);

        return; // Processed!
      }
    }else { console.log("Sheeepiidddddd why dont you ask for hellpp");}
  };
  
  xhr.send();

}

doAddress();