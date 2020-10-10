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
	$('#mode1').click(function(){
		sendPrimeCommand("mode1");
	});
	$('#mode2').click(function(){
		sendPrimeCommand("mode2");
	});
	$('#sound0').click(function(){
		sendPrimeCommand("sound0");
	});
	$('#sound1').click(function(){
		sendPrimeCommand("sound1");
	});
	$('#sound2').click(function(){
		sendPrimeCommand("sound2");
	});
	$('#voldown').click(function(){
		sendPrimeCommand("voldown");
	});
	$('#volup').click(function(){
		sendPrimeCommand("volup");
	});
	$('#tempodown').click(function(){
		sendPrimeCommand("tempodown");
	});
	$('#tempoup').click(function(){
		sendPrimeCommand("tempoup");
	});
	
	socket.on('commandReply', function(result) {
		var newDiv = $('<div></div>').text(result);

		if (result.indexOf("mode: ") >= 0) {
			let temp = result.slice(6);
			$('#mode').text(temp);
			clearTimeout(errorTimer);
			$('#messages').hide();
		}
		if (result.indexOf("volume: ") >= 0) {
			let temp = result.slice(8);
			$('#volumeid').value(temp);
			clearTimeout(errorTimer);
			$('#messages').hide();
		}
		if (result.indexOf("tempo: ") >= 0) {
			let temp = result.slice(7);
			$('#tempoid').value(temp);
			clearTimeout(errorTimer);
			$('#messages').hide();
		}
		if (result.indexOf("uptime: ") >= 0) {
			let temp = result.slice(8);
			$('#uptime').text(temp);
			clearTimeout(errorTimer);
			$('#messages').hide();
		}
	});
	
});


// Setup a repeating function (every 1s)
window.setInterval(function() {
	sendPrimeCommand('uptime');
	setTimeout(function() {
		sendPrimeCommand('getMode');
	}, 150);
	setTimeout(function() {
		sendPrimeCommand('getVolume');
	}, 150);
	setTimeout(function() {
		sendPrimeCommand('getTempo');
	}, 150);
	setTimeout(function() {
		sendPrimeCommand('getUptime');
	}, 150);
}, 150);

function sendPrimeCommand(message) {
	socket.emit('prime', message);
};