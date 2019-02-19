
function macroNameToStructName(macroName) {
	macroName = macroName.replace(/_/g, "");
	return macroName.toLowerCase();
}

function findMacros(text) {
	var result = {nameID: {}, IDname: {}};
	var lines = text.split('\n');
	var commented = false;
	for(var i = 0;i < lines.length;i++) {
		var line = lines[i];
		if(commented) {
			if(line.includes("*/")) {
				commented = false;
				//console.log("uncommenting at  " + line);
			} else {
				//console.log("ignmoring line " + line);
			}
		} else {
			if(line.includes("/*")) {
				commented = true;
				//console.log("commented line " + line);
			} else if(line.startsWith("#define")) {
				j = 0;
				len = line.length;
				while(j < len && line.charAt(j++) != ' '){ }//Find the space after the word struct
				var nameStart = j;
				while(j < len && ((line.charAt(j) != ' ') && (line.charAt(j) != '\t') && (line.charAt(j++) != '\n'))){ }//Find the space after the word struct
				var name = line.substring(nameStart - 1, j);
				var value = line.substring(j, len);
				name = name.trim();
				name = macroNameToStructName(name);
				value = value.trim();
				result["nameID"][name] = value;
				result["IDname"][value] = name;
			} else {
				//console.log("non-struct normal line " + line);
			}
		}
	}
	return result;
}

function findStructs(text) {
	var result = {};
	var lines = text.split('\n');
	var commented = false;
	for(var i = 0;i < lines.length; i++){
	    var line = lines[i];
		if(commented) {
			if(line.includes("*/")) {
				commented = false;
				//console.log("uncommenting at  " + line);
			} else {
				//console.log("ignmoring line " + line);
			}
		} else {
			if(line.includes("/*")) {
				commented = true;
				//console.log("commented line " + line);
			} else if(line.startsWith("struct")) {
				//console.log("normal line " + line);
				i = processStruct(lines, i, result) - 1;//We dont want to increment right into the next line!
			} else {
				//console.log("non-struct normal line " + line);
			}
		}
	}
	return result;
}

function processStruct(lines, i, result) {
	var line = lines[i];
	var len = line.length;
	var j = 0;
	while(j < len && line.charAt(j++) != ' '){ }//Find the space after the word struct
	var end = j;
	while(end < len && line.charAt(end++) != ' '){ }//Find the space after the name
	var structName = line.substring(j, end).trim();
	//console.log("start name: " +structName );
	i++;
	var fieldMap = {};
	while(!(line = lines[i++]).startsWith("}")) {
		if(line.trim().length > 0 && !line.startsWith("{")) {
			line = line.trim();
			j = 0;
			len = line.length;
			while(j < len && line.charAt(j++) != ' '){ }//Find the space after the word struct
			fieldType =  line.substring(0, j - 1);
			//console.log("\tType: " + fieldType);
			line = line.substring(j, line.length);
			line = line.replace(/\s/g, "");
			var semiIndex = line.indexOf(";");
			line = line.substring(0, semiIndex);
			var fields = line.split(",");
			for(var k = 0; k < fields.length; k++) {
				var fieldName = fields[k];
				//console.log("\t\tField Name: " + fieldName);
				fieldMap[fieldName] = fieldType;
			}
		}
	}
	result[structName]= {name: structName.toLowerCase(), fields: fieldMap};
	return i;
}

function getStructWithName(name, structs) {
	for (var struct in structs) {
		if(structs.hasOwnProperty(struct)) {

			var stuName = structs[struct]["name"];
			//console.log("found struct: " + stuName);
			//console.log("looking for struct: " + name);
			if(stuName == name) {

				return structs[struct];
			}
		}
	}
	//console.log("couldnt find struct with name " + name);
	return null;
}

function writeStruct(struct, object, view, index, littleEndian) {
	if(struct == null) console.log("Cant be null!");
	else if(struct.hasOwnProperty("fields")) {
		var fields = struct["fields"];
		for (var fieldName in fields) {
			if (fields.hasOwnProperty(fieldName)) {
				var type = fields[fieldName];
				var value = object[fieldName];
				switch (type) {
					case "uint8_t":
						//console.log("setting index " + index + " to " + value + " type " + type);
						view.setUint8(index, value, littleEndian);
						index += 1;
						break;
					case "uint16_t":
						//console.log("setting index " + index + " to " + value + " type " + type);
						view.setUint16(index, value, littleEndian);
						index += 2;
						break;
					case "uint32_t":
						//console.log("setting index " + index + " to " + value + " type " + type);
						view.setUint32(index, value, littleEndian);
						index += 4;
						break;
					case "int8_t":
						//console.log("setting index " + index + " to " + value + " type " + type);
						view.setInt8(index, value, littleEndian);
						index += 1;
						break;
					case "int16_t":
						//console.log("setting index " + index + " to " + value + " type " + type);
						view.setInt16(index, value, littleEndian);
						index += 2;
						break;
					case "int32_t":
						//console.log("setting index " + index + " to " + value + " type " + type);
						view.setInt32(index, value, littleEndian);
						index += 4;
						break;
					case "float":
						//console.log("setting index " + index + " to " + value + " type " + type);
						view.setFloat32(index, value, littleEndian);
						index += 4;
						break;
					case "double":
						//console.log("setting index " + index + " to " + value + " type " + type);
						view.setFloat64(index, object[fieldName], littleEndian);
						index += 8;
						break;
					default:
						console.log("Invalid type " + type + "!");
				}
			}
		}
	} else {
		console.log("Invalid struct!");
	}
}

function readStruct(struct, view, index, littleEndian) {//struct type and dataview
	result = {};
	if(struct == null) console.log("Cant be null!");
	else if(struct.hasOwnProperty("fields")) {
		var fields = struct["fields"];
		for (var fieldName in fields) {
			if (fields.hasOwnProperty(fieldName)) {
				var type = fields[fieldName];
				switch (type) {
					case "uint8_t":
						//console.log("uint8");
						result[fieldName] = view.getUint8(index, littleEndian);
						index += 1;
						break;
					case "uint16_t":
						result[fieldName] = view.getUint16(index, littleEndian);
						index += 2;
						//console.log("uint16");
						break;
					case "uint32_t":
						result[fieldName] = view.getUint32(index, littleEndian);
						index += 4;
						//console.log("uint32");
						break;
					case "int8_t":
						result[fieldName] = view.getInt8(index, littleEndian);
						index += 1;
						//console.log("int8");
						break;
					case "int16_t":
						result[fieldName] = view.getInt16(index, littleEndian);
						index += 2;
						//console.log("int16");
						break;
					case "int32_t":
						result[fieldName] = view.getInt32(index, littleEndian);
						index += 4;
						//console.log("int32");
						break;
					case "float":
						result[fieldName] = view.getFloat32(index, littleEndian);
						index += 4;
						//console.log("float");
						break;
					case "double":
						result[fieldName] = view.getFloat64(index, littleEndian);
						index += 8;
						//console.log("double");
						break;
					default:
						console.log("Invalid type " + type + "!");
				}
			}
		}
	} else {
		console.log("Invalid struct!");
	}
	return result;
}

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
var shutdown = false;
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
server.on("error", function(error) {
	console.error("Another instance of the server is already running. Aborting!");
	console.error("Error is: " + error);
	process.exit();
});
server.listen(port);


var io = require('socket.io').listen(server);
var Buffer = require('buffer').Buffer;
const rimraf = require('rimraf');

var SerialPort = require('serialport');
var openSerial;
var serialPort;
var socket;

openSerial = setInterval(openSerialFunction, 1000);

clientLog 	= (text) => { console.log 	("To Client (PRE CONNECT): " + text); }
clientError = (text) => { console.error ("To Client (PRE CONNECT): " + text); }
clientWarn 	= (text) => { console.warn 	("To Client (PRE CONNECT): " + text); }

io.on("connection", function(newSocket) {
	clientLog 	= (text) => { newSocket.emit("Log", text); console.log("To Connected Client: " + text); }
	clientError = (text) => { newSocket.emit("Error", text); console.error("To Connected Client: " + text); }
	clientWarn 	= (text) => { newSocket.emit("Warn", text); console.warn("To Connected Client: " + text); }
	socket = newSocket;
	newSocket.on("data", function(data) {
		console.log("data from client " + tostring(data));
	});
});

var macros;
var structs;
var MAGIC_BYTE;
var MAGIC_BYTES_COUNT;

fs.readFile('../Arduino/Opcodes.h', 'utf8', function(err, contents) {
	structs = findStructs(contents);
	macros = findMacros(contents);
	MAGIC_BYTE = parseInt(macros["nameID"]["packetmagic"]);
	MAGIC_BYTES_COUNT = parseInt(macros["nameID"]["magiccount"]);
	//console.log(JSON.stringify(macros, null, " "));
	//console.log("magic " + MAGIC_BYTE + " count " + MAGIC_BYTES_COUNT);
});

const ReadStatus = {
    FIND_MAGIC: 'find_magic',
    FIND_END: 'find_end',
    READ_PAYLOAD: 'read_payload'
}

var magicCount = 0;
var end = 0;
var findEndStatus = -1;
var currentOffset = 0;
var status = ReadStatus.FIND_MAGIC;
var printedCount = 0;

var payloadBuffer = Buffer.alloc(50 * 1024);//50KB
var payloadIndex = 0;
var packetCount = 0;

var lastPacketMillis = new Date().getTime();

try {
	fs.mkdirSync("./packets/");
} catch(error) {
	//console.error("Failed to create packets directory! " + error);
}
rimraf('./packets/*', function () {  });//Delete old packets
fs.copyFileSync("../Arduino/Opcodes.h", "../Website/Opcodes.h");

var stream = fs.createWriteStream("./All-Serial.dat");
process.on("exit", (code) => {
	try {
		stream.end();
	} catch(error) {
		console.log("stream already closed");
	}
});

function nextByte(byte) {
	switch(status) {
		case ReadStatus.FIND_MAGIC:
			if(byte == MAGIC_BYTE) {
				magicCount++;
			} else {
				magicCount = 0;
			}
			if(magicCount == MAGIC_BYTES_COUNT) {
				magicCount = 0;
				findEndStatus = 0;
				status = ReadStatus.FIND_END;
				var now = new Date().getTime();
				var delta = now - lastPacketMillis;
				lastPacketMillis = now;
				console.log("last packet was " + (delta / 1000.0) + " seconds ago");
				console.log("Average blobs per packet: " + blobs);
				blobs = 0;
			}
		break;
		case ReadStatus.FIND_END:
			if(findEndStatus < 4) {
				var shift = 8 * findEndStatus++;
				end |= byte << shift;
			}
			if(findEndStatus >= 4) {
				if(end > payloadBuffer.length) {
					clientError("Expected Payload Buffer Overflow! end: " + end + " current byte " + byte);
					status = ReadStatus.FIND_MAGIC;//Somthing is wrong so fall back to finding the next magic
					payloadIndex = 0;
					findEndStatus = 0;
					end = 0;
					return true;
				}
				status = ReadStatus.READ_PAYLOAD;// We read all of end
			}
		break;
		case ReadStatus.READ_PAYLOAD:
			if(payloadIndex == payloadBuffer.length) {
				clientError("Payload Buffer Overflow! index: " + payloadIndex + " current byte " + byte + " payload " + payloadBuffer);
				payloadIndex = 0;
				status = ReadStatus.FIND_MAGIC;
				return true;
			}
			payloadBuffer.writeUInt8(byte, payloadIndex++);
			end--;
			if(end == 0) {
				var toSend = new Uint8Array(payloadIndex);
				for(var i = 0; i < payloadIndex; i++) {
					toSend[i] = payloadBuffer[i];
				}
				if(socket) socket.emit("Packet", toSend);
				status = ReadStatus.FIND_MAGIC;//The next byte should be the start of the next magic
				payloadIndex = 0;
				fs.writeFile("./packets/packet" + (packetCount++) + ".dat", toSend, function(err) {
				    if(err) {
				    	console.error("Failed to write packet file! " + err);
				    }
				});
			}
		break;
	}
	return false;
}


var readline = require('readline');

var rl = readline.createInterface({
	input: process.stdin,
	output: process.stdout
});

var bytes = 0, blobs = 0;

var secondChecks = setInterval(function() {

}, 1000);

function openSerialFunction() {
	if(!shutdown) {
		console.log("Attempting to open serialport...");
		SerialPort.list(function (err, ports) {
			if(err) {
				console.error("Error listing serial ports! " + err);
				return;
			}
			for(var port of ports) {
				if(port.manufacturer.includes("Arduino")) {
					serialPort = new SerialPort(port.comName, {
						baudRate: 115200
					});
					serialPort.on('error', (err) => clientWarn("Serial port error: " + err) );
					serialPort.on('open', () => {
						clientLog("Serial port is now open!");
						clearInterval(openSerial);
					});
					serialPort.on("close", () => {
						clientLog("Serial port closed");
						if(openSerial) clearInterval(openSerial);
						openSerial = setInterval(openSerialFunction, 1000);
					});
					serialPort.on('data', function (data) {
						stream.write(data);
						blobs++;
						for(var i = 0; i < data.length; i++) {//Process each byte
							var byte = data.readUInt8(i);
							if(nextByte(byte)) {
								break;
							}

							/*printedCount++;
							process.stdout.write(" " + byte.toString(16));
							if(printedCount % 48 == 0) {
								console.log();
							}*/

						}
					});

					console.log("Using serial port " + port.comName + " Manufacturer: \"" + port.manufacturer + "\" ID \"" + port.pnpId);
					break;//Make sure we done create mutiple serialports
				}
			}
		});
	}
}

rl.question("Press enter to stop the server ", function(answer) {
	shutdown = true;
	console.log("Stopping server...");
	server.close();
	rl.close();
	io.close();
	clearInterval(secondChecks);
	clearInterval(openSerial);
	if(serialPort && serialPort.isOpen())
		serialPort.close();
	console.log("closing serial port!");
	stream.end();

	setTimeout(function(){
	   process.exit();
   }, 1000);
});
console.log();
console.log();
