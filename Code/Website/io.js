
var macros;
var structs;
var deviceConnected = false;
var packetsPerSecond = -1;

readTextFile("Opcodes.h", function(text) {
	structs = findStructs(text);
	macros = findMacros(text);
	console.log("macros " + JSON.stringify(macros, null, " "));
	console.log("structs " + JSON.stringify(structs, null, " "));
});

function handleStruct(obj, name) {
	switch(name) {
		case "packetheaderdata":
			console.log("packetID " + obj["packetCount"]);
		break;
		case "subheaderdata":
		break;
		case "arduinoduedata":
		break;
		case "cameradata":
		break;
		case "acceldata":
		break;
		case "gpsdata":
		break;
		case "pitottubedata":
		break;
		case "altimeterdata":
		break;
		case "sdcarddata":
		break;
		case "radiodata":
		break;
	}
}


function handleData(structs, macros, view, index) {
	var packetID = view.getUint8(index++);
	var struct = getStructWithName(macros["IDname"][packetID], structs);
	var result = readStruct(struct, view, index, true);
	var object = result[0];
	handleStruct(object, struct["name"]);
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
		while(buffer[index] != undefined) {
			index = handleData(structs, macros, view, index);
		}
	}
);

socket.on('Connect', function() {
	deviceConnected = socket.connected;
});

setInterval(function(){
	console.log("Packets Per Second " + packetsPerSecond);

	packetsPerSecond = 0;
}, 1000);
