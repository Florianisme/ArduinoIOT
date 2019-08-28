'use strict';
const {dialogflow} = require('actions-on-google');
const functions = require('firebase-functions');
const app = dialogflow({debug: true});
const admin = require('firebase-admin');
const {
  SimpleResponse
} = require('actions-on-google');
admin.initializeApp(functions.config().firebase);
 
app.intent('water_level', (conv) => {
	return new Promise( function( resolve, reject ){
	admin.database().ref('timestamp').once('value', (snapshot) => {
		var responseSentence = 'Die Wasserstände betrugen vor ' + getMinutesFromTimestamp(snapshot.val()) + ' Minuten: ';
		var plantsIndex = 0;
			for(var i = 1; i < 5; i++) {
				admin.database().ref('plant_' + i).once('value', (snapshot) => {
					let level = snapshot.val().water_level;
					let name = snapshot.val().name;
					responseSentence += 'Bei ' + name + ', ' + level + '%. ';
					plantsIndex++;
					if(checkAllDBRequestsFinished(plantsIndex)) {
						conv.ask(responseSentence.toString());
						conv.close('Vielen Dank');
						resolve();
					}
				});
			}
		});
	});
});

function checkAllDBRequestsFinished(counter) {
	return counter === 4;
}

function getMinutesFromTimestamp(timestamp) {
	var date = new Date(timestamp);
	var now = new Date();
	var difference = now - date;
	return Math.floor(difference / 60000);
}

exports.timestamp = functions.database.ref('/room_humidity').onUpdate(
    (change, context) => admin.database().ref('/timestamp').set(context.timestamp));

exports.getWaterLevel = functions.https.onRequest(app);
