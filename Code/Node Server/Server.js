
/*
var queue = [];
var currentTTS = null;

const say = require('say')

let voice = "Microsoft Zira Desktop";
let speed = 1.15;


//Adds dome text to the text to speech
//time the number of milliseconds after Jan 1 1970 when this text should be said
//importance 0-10 0 = least important mos likley to be skipped when there are conflicts
//bubble the number of seconds in which playing this text after its desired time would make sense (if this text is skipped due to having a low importance it will never be played if bubble + time already happened)
function addTTS(text, time, importance, bubble) {
	queue.push({text: text, time: time, importance: importance, bubble: (bubble * 1000)});
	ttsUpdate();
}

function playText(tts) {
	var text = tts["text"];
	var index = queue.indexOf(tts);
	if(index > -1) queue.splice(index, 1);
	if(currentTTS != null) {
		say.stop();
		console.log("Stopping " + currentTTS["text"]);
	}

	currentTTS = tts;
	say.speak(text, voice, speed, function (error) {
		if(error) {
			console.log("Error while playing \"" + text + "\" : " + error);
		} else {
			currentTTS = null;
		}
	});
}

function getBestTTSMessage() {
	var best = null;
	queue.forEach(function (item) {
		var now = Date.now();
		var itsTime = item["time"];
		var itsBuble = item["bubble"];
		if((now > (itsTime - 500)) && (now < (itsTime + itsBuble))) {
			if(best == null || item["importance"] > best["importance"] || (item["importance"] == best["importance"] && (Math.abs(now - itsTime) < Math.abs(now - best["time"])))) {
				best = item;
			}
		}
	});
	return best;
}

function getAltitude() {
	var time = getTimeAfterLaunch();
	if(time < 0) {
		return 0;
	} else if(time < 14.8) {
		return -14.623 * time * (time - 22);
	} else {
		return Math.max(2000 - 30 * time, 0);
	}
}

var emptyQueueCallBack = function () {
	var time = getTimeAfterLaunch();
	var height = getAltitude();
	if(time > 0 && height != 0) {
		var height;
		if(time < 14) {
			height = Math.floor(height / 100) * 100;
		} else {
			height = Math.round(height / 10) * 10;
		}
		addTTS(height + " feet", Date.now(), 5, 15);
	}
}

function ttsUpdate() {
	if(!currentTTS) {
		var best = getBestTTSMessage();
		if(best != null) {
			playText(best);
		} else {
			emptyQueueCallBack();
		}
	}
	queue.forEach(function (item, index) {
		var now = Date.now();
		var itsTime = item["time"];
		var itsBuble = item["bubble"];
		if(now > (itsTime + itsBuble)) {
			queue.splice(index, 1);
			//console.log("removing " + item["text"] + "its buble=" + itsBuble + "   since  now=" + now + " and it ended " + (now - (itsTime + itsBuble)) + "ms ago");
		}
	});
}

let launch = Date.now() + 20 * 1000;

function getTimeAfterLaunch() {
	return (Date.now() - launch) / 1000;
}

for(var i = 60; i > 10; i -= 5) {
	addTTS("t minus " + i + " seconds.", launch - (i * 1000) - 1000, 4, 2);
}
for(var i = 10; i > 0; i--) {
	addTTS("" + i, launch - (i * 1000) - 1000, 4, 2);
}

addTTS("Launch!", launch, 10, 5);

var lastHeight = getAltitude();
var apogee = false;

setInterval(function() {
	ttsUpdate();
	process.stdout.clearLine();
	process.stdout.cursorTo(0);
	var time = getTimeAfterLaunch();
	var height = getAltitude();
	process.stdout.write("T" + (time < 0 ? "-" : "+") + " " + Math.abs(time) + " height " + height + " queue length " + queue.length);  // write text
	if(lastHeight > height) {
		//console.log("coming down");
		if(apogee == false) {
			addTTS("Apogee of " + Math.round(height) + " feet at t equals " + (Math.round(time * 10) / 10) + " seconds", Date.now(), 10, 60);
			addTTS("eparation of the booster and of the payload confirmed", Date.now() + 4000, 4, 60);
			//console.log("APOGEE!");
			apogee = true;
		} else {
			if(height == 0) {
				//console.log("touchdown");
				addTTS("Touchdown, at t equals " + (Math.round(time * 10) / 10) + " seconds", Date.now(), 10, 60);
			}
		}
	}
	lastHeight = height;
}, 250);
*/

var http = require("http"),
    url = require("url"),
    path = require("path"),
    fs = require("fs")
    port = 8192;

server = http.createServer(function(request, response) {
	var contentTypesByExtension = {
		'.html': "text/html",
		'.css':  "text/css",
		'.js':   "text/javascript"
	};
	var uri = url.parse(request.url).pathname;
	var filename1 = path.join("../Website/", uri);
	var filename2 = path.join(process.env.APPDATA + "/npm/node_modules", uri);

	if(fs.existsSync(filename1)) {
		console.log("found " + uri + " at " + filename1);


		if (fs.statSync(filename1).isDirectory()) filename1 += '/index.html';

		fs.readFile(filename1, "binary", function(err, file) {
			if(err) {
				response.writeHead(500, {"Content-Type": "text/plain"});
				response.write(err + "\n");
				response.end();
				return;
			}

			var headers = {};
			var contentType = contentTypesByExtension[path.extname(filename1)];
			if (contentType) headers["Content-Type"] = contentType;
			response.writeHead(200, headers);
			response.write(file, "binary");
			response.end();
		});
	} else if(fs.existsSync(filename2)) {
		console.log("found " + uri + " at " + filename2 + " (location 2)");

		if (fs.statSync(filename2).isDirectory()) {
			response.writeHead(404, {"Content-Type": "text/plain"});
			response.write("404 Not Found\n");
			console.log("Coundnt file file: " + uri + " at " + filename2);
			response.end();
			return;
		} else {

			fs.readFile(filename2, "binary", function(err, file) {
				if(err) {
					response.writeHead(500, {"Content-Type": "text/plain"});
					response.write(err + "\n");
					response.end();
					return;
				}

				var headers = {};
				var contentType = contentTypesByExtension[path.extname(filename2)];
				if (contentType) headers["Content-Type"] = contentType;
				response.writeHead(200, headers);
				response.write(file, "binary");
				response.end();
			});
		}
	} else {
		response.writeHead(404, {"Content-Type": "text/plain"});
		response.write("404 Not Found\n");
		console.log("Coundnt file file: " + uri + " at " + filename1 + " or " + filename2);
		response.end();
		return;
	}
});

server.listen(port);

var io = require('socket.io').listen(server);

io.on('connection', function(socket) {
    io.emit('Server 2 Client Message', 'Welcome!' );
    socket.on('Client 2 Server Message', function(message) {
        console.log(message);
        io.emit('Server 2 Client Message', message.toUpperCase());     //upcase it
    });
});


var SerialPort = require('serialport');
var serialPort = new SerialPort('COM6', {
    baudRate: 115200
});

io.on('data', function(data) {
	console.log("data from client " + tostring(data));
});

serialPort.on('data', function (data) {
	var index = 0;
	while(typeof(data[index]) === "number") {
		io.emit('Socket', data[index]);
		index++;
		bytes++;
	}
});


var readline = require('readline');

var rl = readline.createInterface({
	input: process.stdin,
	output: process.stdout
});


var bytes = 0;

var secondTrackerID = setInterval(function(){
	console.log("Bytes per second " + bytes);

	bytes = 0;
}, 1000);

rl.question("Press enter to stop the server ", function(answer) {
	serialPort.close();
	server.close();
	rl.close();
	io.close();
	clearInterval(secondTrackerID);
});
