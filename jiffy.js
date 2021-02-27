var senderName, senderContact, senderEmail;
var receiverName, receiverContact, receiverEmail;
var senderLocation, receiverState, receiverLocation;
var itemSize = 'S'; // default is S (smallest size)

var distance_senderToBusTerminal = 0;
var distance_busTerminalToReceiver = 0;
var distance_terminalToTerminal = 0;
var parcelCost = 0;
var totalDistance = 0;
var itemType;
var itemProperty;
var insuranceOption;

// hide/show respective receiver areas when receiver state selected
$(document).ready(function() {
    $('.form-switch').on('change', function() {
      $('.area').removeClass('active');
      var formToShow = '.area-' + $(this).data('id');
      $(formToShow).addClass('active');
    });
});

document.getElementById("Ampang").disabled = true;
document.getElementById("Johor").disabled = true;
document.getElementById("Ipoh").disabled = true;
document.getElementById("Johor Bahru").disabled = true;
document.getElementById("Batu Pahat").disabled = true;


function calcTotalCost() {
    var totalCost = 0;
    const BASE_FARE = 5.0;
    // Kepong to TBS: 24.2km - RM36.3
    // TBS to Taiping Terminal: 280km - BUS TICKET RM27/2 = RM13.5
    // Taiping Terminal to Taiping: 4.9km - RM7.35

    distance_senderToBusTerminal = 24.2;
    distance_terminalToTerminal = 280;
    distance_busTerminalToReceiver = 4.9;

    cost_senderToBusTerminal = BASE_FARE + (24.2 * 1.5);
    cost_busTerminalToReceiver = BASE_FARE + (4.9 * 1.5);
    cost_terminalToTerminal = 27/2; 
    
    if (itemProperty == 'S') parcelCost = 10.0;
    else if (itemProperty == 'M') parcelCost = 15.0;
    else if (itemProperty == 'L') parcelCost = 20.0;
    
    if (insuranceOption == 'Y') totalCost += 3.0;

    totalCost += parcelCost + cost_senderToBusTerminal + cost_busTerminalToReceiver + cost_terminalToTerminal;

    return totalCost;
}

function showInput() {
    // display order details
    document.getElementById('show_senderName').innerHTML = document.getElementById("senderName").value;
    document.getElementById('show_senderPhone').innerHTML = document.getElementById("senderPhone").value;
    document.getElementById('show_senderEmail').innerHTML = document.getElementById("senderEmail").value;
    document.getElementById('show_senderLocation').innerHTML = document.getElementsByName("senderLocation")[0].value;
    document.getElementById('show_receiverName').innerHTML = document.getElementById("receiverName").value;
    document.getElementById('show_receiverPhone').innerHTML = document.getElementById("receiverPhone").value;
    document.getElementById('show_receiverEmail').innerHTML = document.getElementById("receiverEmail").value;
    document.getElementById('show_receiverState').innerHTML = document.getElementsByName("receiverState")[0].value;
    document.getElementById('show_receiverArea').innerHTML = document.getElementsByName("receiverArea")[0].value;
    document.getElementById('show_itemType').innerHTML = document.getElementById("itemType").value;

//     var addr = document.getElementsByName("senderLocation")[0].value;  
//     var values = doAddress();

//     var lat = parseFloat(values[0]);
//     document.getElementById('show_senderLat').innerHTML = lat;

    document.getElementById('show_itemSizeWeight').innerHTML = document.getElementsByName("itemSizeWeight")[0].value;
    document.getElementById('show_insuranceOption').innerHTML = document.getElementsByName("insuranceOption")[0].value;
    document.getElementById('show_totalCost').innerHTML = calcTotalCost();


    var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
    // Constants
    var url = "https://maps.googleapis.com/maps/api/geocode/json?";

    // Key storage
    var key = "AIzaSyAyn7a1Yn847Ex4WgAZ17p4QAWeA1a-iWc";  // API key to use when processing   

    // Makes the request and on callback processes. Will call doAddress with an incremented
    function doAddress() {
        var xhr = new XMLHttpRequest();
        alert("IM HERE");
        // Generate the request.
        var local_url = url + "address=" + encodeURIComponent(addr) + "&key=" + encodeURIComponent(key);
        // Request
        xhr.open("GET", local_url, true);
        xhr.onreadystatechange = function (e) {
        if (this.readyState == 4 && this.status === 200) {  // Connected to Google
            var result = JSON.parse(xhr.responseText);
            if (result.status === "OK") {
            var geometry = result.results[0].geometry;
            var location = geometry.location;

            console.log(location.lat + ', ' + location.lng);
            var lat = location.lat;
            var lng = location.lng;
            return[lat, lng]; // Processed!
            }
            // JSON result from google failed.
            if (result.status === "ZERO_RESULTS") { 
            console.log("no results");
            return;
            }
            if (result.status === "OVER_QUERY_LIMIT") {
            console.log("over query limit");
            return;
            }
            // Other Google error.
            console.log(result.status);
            return;
            }
            
        };

        xhr.send();

        }
}


var distance = require('google-distance-matrix');

var origins = ['3.214, 101.6356'];
var destinations = ['3.0781, 101.7111'];

distance.key('AIzaSyAyn7a1Yn847Ex4WgAZ17p4QAWeA1a-iWc');
distance.units('metric');


function calculateDistance() {

    distance.matrix(origins, destinations, function (err, distances) {
        if (err) {
            return console.log(err);
        }
        if(!distances) {
            return console.log('no distances');
        }
        if (distances.status == 'OK') {
            for (var i=0; i < origins.length; i++) {
                for (var j = 0; j < destinations.length; j++) {
                    var origin = distances.origin_addresses[i];
                    var destination = distances.destination_addresses[j];
                    if (distances.rows[0].elements[j].status == 'OK') {
                        var distance = distances.rows[i].elements[j].distance.text;
                        console.log('Distance from ' + origin + ' to ' + destination + ' is ' + distance);
                    } else {
                        console.log(destination + ' is not reachable by land from ' + origin);
                    }
                }
            }
        }
    });
}
