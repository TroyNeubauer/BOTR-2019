

function handleData(structs, macros, view, littleEndian) {
	var packetID = view.getUint8(0);
	var index = 1;
	var struct = getStructWithName(macros["IDname"][packetID], structs);
	var object = readStruct(struct, view, index, littleEndian);
	console.log(JSON.stringify(object, null, 2));

}
