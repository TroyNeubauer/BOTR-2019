
var macros;
var structs;
var deviceConnected = false;
var packetsPerSecond = -1;
var GPS_ID, SUB_HEADER_DATA;
var interpolateData = true;

readTextFile("Opcodes.h", function(text) {
	structs = findStructs(text);
	macros = findMacros(text);
	GPS_ID = macros["nameID"]["gpsdata"];
	SUB_HEADER_DATA = macros["nameID"]["subheaderdata"];
	//console.log("macros " + JSON.stringify(macros, null, " "));
	//console.log("structs " + JSON.stringify(structs, null, " "));
});

function handleStruct(obj, name) {
	switch(name) {
		case "packetheaderdata":
			putData("Mission Time", obj["millis"]);
			//console.log("Packet Count: " + obj["packetCount"]);
		break;
		case "subheaderdata":
			putData("Mission Time", obj["millis"]);
		break;
		case "arduinoduedata":

		break;
		case "cameradata":

		break;
		case "acceldata":
			putData("AX", obj["ax"]);
			putData("AY", obj["ay"]);
			putData("AZ", obj["az"]);
			putData("VX", obj["vx"]);
			putData("VY", obj["vy"]);
			putData("VZ", obj["vz"]);
		break;
		case "gpsdata":

		break;
		case "pitottubedata":
			putData("Pitot Speed", obj["airSpeed"]);
		break;
		case "altimeterdata":

		break;
		case "sdcarddata":

		break;
		case "radiodata":

		break;
	}
}

var unixTimeAt0;
var currentSubDataTime;

function handleData(structs, macros, view, index) {
	//console.log("in handle data");
	var packetID = view.getUint8(index++);
	console.log("Packet ID: " + packetID);
	var struct = getStructWithName(macros["IDname"][packetID], structs);
	var result;
	if(packetID == GPS_ID) {
		result = readCString(view, index);
		if(result == null) {
			console.error("Failed to parse C-String! Null byte missing. Index: " + index + " view: "  + view);
			console.error("Throwing out rest of packet!");
			return 1 << 28;//Very large number so that we throw out the rest of this packet
		}
	} else {//Normal struct so we can read it as usual
		result = readStruct(struct, view, index, true);
	}
	var object = result[0];
	if(packetID == SUB_HEADER_DATA) {
		if(unixTimeAt0 === undefined) unixTimeAt0 = new Date().getTime() - object["millis"];
		if(interpolateData) {
			console.log(object["millis"] - currentSubDataTime);
			currentSubDataTime = object["millis"];
			//console.log("updating currentSubDataTime to " + currentSubDataTime);
		}
	}
	if(interpolateData) {
		const timeout = Math.max(0, unixTimeAt0 + currentSubDataTime - new Date().getTime());
		setTimeout(function() {
			handleStruct(object, struct["name"]);// this is null
			//console.log("handing struct! " + JSON.stringify(object, null, ""));
		}, timeout);
		//console.log("starting timer for " + timeout + " ms");
	} else {
		handleStruct(object, struct["name"]);
	}

	return result[1];//Return the new index

}

var socket = io.connect(':8192');
var magicCount = 0;
var index = 0;
socket.binary(true);
socket.on('Packet',
	function(buffer) {
		var length = 0;
		var i = 0;
		while(buffer[i++] != undefined) {
			length++;
		}
		var arrayBuffer = new ArrayBuffer(length);
		var view = new DataView(arrayBuffer);
		for(i = 0; i < length; i++) {
			view.setUint8(i, buffer[i]);
		}

		var index = 0;
		console.log("test");
		while(buffer[index] != undefined) {
			index = handleData(structs, macros, view, index);
		}
	}
);

socket.on('Log', 	text => console.log("Server: " + text));
socket.on('Error', 	text => console.error("Server: " + text));
socket.on('Warn', 	text => console.warn("Server: " + text));

socket.on('Connect', function() {
	deviceConnected = socket.connected;
});
