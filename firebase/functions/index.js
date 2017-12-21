'use strict';

process.env.DEBUG = 'actions-on-google:*';
const App = require('actions-on-google').DialogflowApp;
const functions = require('firebase-functions');
const admin = require('firebase-admin');
admin.initializeApp(functions.config().firebase);

const ACTION_WATER_LEVEL = "water_level";
const ACTION_NEEDS_WATERING = "plant_needs_watering";

exports.getWaterLevel = functions.https.onRequest((request, response) => {
	const app = new App({request, response});
	
	function getLevel (app) {
		admin.database().ref('plant_water_level').once('value', (snapshot) => {
			let level = snapshot.val();
			app.tell('Der Wasserstand beträgt ' + level + '%.'); 
		});
	}
	
	function needsWatering(app) {
		admin.database().ref('plant_water_level').once('value', (snapshot) => {
			let level = snapshot.val();
			if (level < 30) {
				app.tell('Die Pflanze sollte gegossen werden, der Wasserstand beträgt noch ' + level + '%');
			}
			else {
				app.tell('Die Pflanze muss noch nicht gegossen werden, der Wasserstand beträgt noch ' + level + '%');
			}
		});
	}
	
	let actionMap = new Map();
	actionMap.set(ACTION_WATER_LEVEL, getLevel);
	actionMap.set(ACTION_NEEDS_WATERING, needsWatering);
	app.handleRequest(actionMap);
 });
