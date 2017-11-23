'use strict';

process.env.DEBUG = 'actions-on-google:*';
const App = require('actions-on-google').DialogflowApp;
const functions = require('firebase-functions');
const admin = require('firebase-admin');
admin.initializeApp(functions.config().firebase);

const ACTION_WATER_LEVEL = "water_level";

exports.getWaterLevel = functions.https.onRequest((request, response) => {
	const app = new App({request, response});
	console.log('Request headers: ' + JSON.stringify(request.headers));
	console.log('Request body: ' + JSON.stringify(request.body));
	
	function getLevel (app) {
		admin.database().ref('plant_water_level').once('value', (snapshot) => {
			let level = snapshot.val();
			app.tell('Der Wasserstand beträgt ' + level + '%.'); 
		});
	}
	
	let actionMap = new Map();
	actionMap.set(ACTION_WATER_LEVEL, getLevel);
	
	app.handleRequest(actionMap);
 });
