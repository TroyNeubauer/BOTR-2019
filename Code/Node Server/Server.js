var queue = [];

//Adds dome text to the text to speech
//time the number of milliseconds after Jan 1 1970 when this text should be said
//importance 0-10 0 = least important mos likley to be skipped when there are conflicts
//bubble the number of seconds in which playing this text after its desired time would make sense (if this text is skipped due to having a low importance it will never be played if bubble + time already happened)
function addTTS(text, time, importance, bubble) {

}

const say = require('say')




function submitTTS() {

}

const delay = require('delay');
let voice = "Microsoft Zira Desktop";
let speed = 1.15;


say.speak("500 feet", voice, speed, function (error) {
	say.speak("1000 feet", voice, speed, function (error) {
		say.speak("1300 feet", voice, speed, function (error) {
			say.speak("1500 feet", voice, speed, function (error) {
				say.speak("1700 feet", voice, speed, function (error) {
					say.speak("1757 feet", voice, speed, function (error) {
						say.speak("Apogee at 1793 feet", voice, speed, function (error) {
							say.speak("Activating descent mode", voice, speed, function (error) {
								say.speak("Separation of the booster and of the payload confirmed", voice, speed, function (error) {
								})
							})
						})
					})
				})
			})
		})
	})
})


return;

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
	var filename1 = path.join("C:/Users/Troy Neubauer/Documents/Rocketry/Code/Website", uri);
	var filename2 = path.join("C:/Users/Troy Neubauer/AppData/Roaming/npm/node_modules", uri);

	if(fs.existsSync(filename1)) {
		console.log("#### found " + uri);


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
		console.log(".... found in location 2 " + uri);

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
