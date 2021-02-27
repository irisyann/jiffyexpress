var distance = require('google-distance-matrix');

var origins = ['3.214, 101.6356'];
var destinations = ['3.0781, 101.7111'];

distance.key('AIzaSyAyn7a1Yn847Ex4WgAZ17p4QAWeA1a-iWc');
distance.units('metric');

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

const prompt = require('prompt-sync')({sigint: true});
 
// Random number from 1 - 10
const numberToGuess = Math.floor(Math.random() * 10) + 1;
// This variable is used to determine if the app should continue prompting the user for input
let foundCorrectNumber = false;
 
while (!foundCorrectNumber) {
  // Get user input
  let guess = prompt('Guess a number from 1 to 10: ');
  // Convert the string input to a number
  guess = Number(guess);
 
  // Compare the guess to the secret answer and let the user know.
  if (guess === numberToGuess) {
    console.log('Congrats, you got it!');
    foundCorrectNumber = true;
  } else {
    console.log('Sorry, guess again!');
  }
}

 
// Random number from 1 - 10
const numberToGuess = Math.floor(Math.random() * 10) + 1;
// This variable is used to determine if the app should continue prompting the user for input

let userType = prompt(
`>> Enter your identity: 
[1] Sender (I want to deliver a parcel)
[2] Rider
[3] Bus driver`);
userType = String(userType);

while (userType != "1" && userType != "2" && userType != "3")
{
  console.log(`[X] Invalid input! Please  try again.
>> Enter your identity: 
[1] Sender (I want to deliver a parcel)
[2] Rider
[3] Bus driver`);
  userType = String(userType);
}

if (userType == "1")


 
while (!foundCorrectNumber) {
  // Get user input
  let guess = prompt('Guess a number from 1 to 10: ');
  // Convert the string input to a number
  guess = Number(guess);
 
  // Compare the guess to the secret answer and let the user know.
  if (guess === numberToGuess) {
    console.log('Congrats, you got it!');
    foundCorrectNumber = true;
  } else {
    console.log('Sorry, guess again!');
  }
}



// const options = {
//   key: 'AIzaSyAyn7a1Yn847Ex4WgAZ17p4QAWeA1a-iWc',
//   origins: ['3.214, 101.6356'],
//   destinations: ['3.0781, 101.7111']
// }


// const data  = GoogleDistanceApi.distance(options, (err, data) => {
//   if(err) {
//       return console.log(err);
//   }
// //   var dist = data.distance.text;
//  console.log(data.distance);

// });