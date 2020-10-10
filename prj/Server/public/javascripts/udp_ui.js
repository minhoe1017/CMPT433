"use strict";
// Client-side interactions with the browser.
var errorTimer = setTimeout(function() {
	$('#messages').text("NodeJS server does not reply to a web-browser command for 1 second");
	$('#messages').show();
	setTimeout(function() {
		$('#messages').hide();
	}, 10000);
}, 1000);

// Make connection to server when web page is fully loaded.
var socket = io.connect();
$(document).ready(function() {

	$('#mode0').click(function(){
		sendPrimeCommand("mode0");
	});

//	sendPrimeCommand("start");
});



var xVal = 0;
var pref1 = 0;
var act1 = 0;
var updateInterval = 1000;
var dataLength = 60; // number of dataPoints visible at any point

socket.on('commandReply', function(result) {
//	var newDiv = $('<div></div>').text(result);

	console.log('incoming message: ' + result + '\n');
	if (result.indexOf("room1_pref_temp: ") >= 0) {
//			consold.log("Incoming message: " + result);
		let temp = result.slice(17);
		pref1 = parseInt(temp);
		$('#room1_pref_temp').text(temp);
		clearTimeout(errorTimer);
		$('#messages').hide();
	}
	if (result.indexOf("room1_act_temp: ") >= 0) {
//			consold.log("Incoming message: " + result);
		let temp = result.slice(16);
		act1 = Math.round(100 * parseFloat(temp)) / 100;
		$('#room1_act_temp').text(act1);
		if (act1 > pref1) {
			$('#fan1_status').text('on');
		} else {
			$('#fan1_status').text('off');
		}
		clearTimeout(errorTimer);
		$('#messages').hide();
	}
});

function sendPrimeCommand(message) {
	socket.emit('prime', message);
};

window.onload = function () {
	var dps_pref = []; // dataPoints
	var dps_act = []; // dataPoints
	var chart = new CanvasJS.Chart("room1_chart", {
		title :{
			text: "Room Temperature"
		},
		axisX: {
			title: "Seconds",
			includeZero: false
		},
		axisY: {
			title: "Temperature",
			includeZero: true
			//suffix: " °C"
		},
		legend:{
			cursor: "pointer",
			fontSize: 16
			//itemclick: toggleDataSeries
		},
		toolTip:{
			shared: true
		},
		data: [{
			name: "Preferred Temperature",
			type: "spline",
			//yValueFormatString: "#0.## °C",
			showInLegend: true,
			dataPoints: dps_pref
		},
		{
			name: "Actual Temperature",
			type: "spline",
			//yValueFormatString: "#0.## °C",
			showInLegend: true,
			dataPoints: dps_act
		}]
});

var updateChart = function (count) {

	dps_pref.push({
		x: xVal,
		y: pref1
	});
	dps_act.push({
		x: xVal,
		y: act1
	});
	xVal++;

	if (dps_pref.length > dataLength) {
		dps_pref.shift();
	}
	if (dps_act.length > dataLength) {
		dps_act.shift();
	}
	chart.render();
};


// Setup a repeating function (every 1s)
window.setInterval(function() {
	sendPrimeCommand('pref_temp1');
	setTimeout(function() {
		sendPrimeCommand('act_temp1');
	}, 10);
}, 980);

setInterval(function(){updateChart()}, updateInterval);
}
