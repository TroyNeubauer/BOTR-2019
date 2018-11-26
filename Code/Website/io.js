
var littleEndian = true;
var macros;
var structs;
var bigBuffer = new ArrayBuffer(1000000);
var bigView = new DataView(bigBuffer);
var endByte;
var endByteCount;
var deviceConnected = false;
var packetsPerSecond = -1;

readTextFile("/Opcodes.h", function(text) {
	structs = findStructs(text);
	macros = findMacros(text);
	endByte = macros["nameID"]["endbyte"];
	endByteCount = macros["nameID"]["endbytecount"];
	console.log("found thing!");

	var nameIDMacros = macros["nameID"];
	for(var macroName in nameIDMacros) {
		console.log(macroName + " = " + nameIDMacros[macroName]);
	}
});


function handleData(structs, macros, view, le) {
	var packetID = view.getUint8(0);
	var index = 1;
	var struct = getStructWithName(macros["IDname"][packetID], structs);
	var object = readStruct(struct, view, index, le);
	//console.log(JSON.stringify(object, null, 2));
}

var socket = io.connect(':8192');
var endCount = 0;
var index = 0;
socket.on('Socket',
	function(byte) {
		bigView.setUint8(index++, byte);
		//console.log("byte: " + byte);
		if(byte == endByte) {
			endCount++;
		} else {
			endCount = 0;
		}
		if(endCount == endByteCount) {
			//console.log("found struct ending!");
			handleData(structs, macros, bigView, littleEndian);
			if(packetsPerSecond == -1) {
				packetsPerSecond = 1;
			} else {
				packetsPerSecond++;
			}
			index = 0;
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
