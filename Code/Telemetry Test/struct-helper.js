

function readBinaryFile(fileName, onLoadFunction) {
	const xhr = new XMLHttpRequest();
    xhr.timeout = 2000;
    xhr.onreadystatechange = function(e) {
    	if (xhr.readyState === 4) {
			if (xhr.status === 200) {
				var raw = xhr.response;
				console.log(raw);
				if(raw) {
					onLoadFunction(new DataView(raw));
				} else {
					console.log("ERROR. Server reterned null" + xhr.status);
				}
    		} else {
				console.log("ERROR" + xhr.status);
			}
    	}

    };
    xhr.open('GET', fileName, true);
	xhr.responseType = "arraybuffer";
    xhr.send();

}

function readTextFile(fileName, onLoadFunction) {
    const xhr = new XMLHttpRequest();
    xhr.timeout = 2000;
    xhr.onreadystatechange = function(e) {
    	if (xhr.readyState === 4) {
			if (xhr.status === 200) {
				onLoadFunction(xhr.response);
    		} else {
				console.log("ERROR" + xhr.status);
			}
    	}

    };
    xhr.open('GET', fileName, true);
    xhr.send();
}

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
	for(var i = 0;i < lines.length;i++){
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
	while(!(line = lines[i++]).startsWith("};")) {
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
			console.log("found struct: " + stuName);
			console.log("looking for struct: " + name);
			if(stuName == name) {

				return structs[struct];
			}
		}
	}
	console.log("couldnt find struct with name " + name);
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
						console.log("setting index " + index + " to " + value + " type " + type);
						view.setUint8(index, value, littleEndian);
						index += 1;
						break;
					case "uint16_t":
						console.log("setting index " + index + " to " + value + " type " + type);
						view.setUint16(index, value, littleEndian);
						index += 2;
						break;
					case "uint32_t":
						console.log("setting index " + index + " to " + value + " type " + type);
						view.setUint32(index, value, littleEndian);
						index += 4;
						break;
					case "int8_t":
						console.log("setting index " + index + " to " + value + " type " + type);
						view.setInt8(index, value, littleEndian);
						index += 1;
						break;
					case "int16_t":
						console.log("setting index " + index + " to " + value + " type " + type);
						view.setInt16(index, value, littleEndian);
						index += 2;
						break;
					case "int32_t":
						console.log("setting index " + index + " to " + value + " type " + type);
						view.setInt32(index, value, littleEndian);
						index += 4;
						break;
					case "float":
						console.log("setting index " + index + " to " + value + " type " + type);
						view.setFloat32(index, value, littleEndian);
						index += 4;
						break;
					case "double":
						console.log("setting index " + index + " to " + value + " type " + type);
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
						console.log("uint8");
						result[fieldName] = view.getUint8(index, littleEndian);
						index += 1;
						break;
					case "uint16_t":
						result[fieldName] = view.getUint16(index, littleEndian);
						index += 2;
						console.log("uint16");
						break;
					case "uint32_t":
						result[fieldName] = view.getUint32(index, littleEndian);
						index += 4;
						console.log("uint32");
						break;
					case "int8_t":
						result[fieldName] = view.getInt8(index, littleEndian);
						index += 1;
						console.log("int8");
						break;
					case "int16_t":
						result[fieldName] = view.getInt16(index, littleEndian);
						index += 2;
						console.log("int16");
						break;
					case "int32_t":
						result[fieldName] = view.getInt32(index, littleEndian);
						index += 4;
						console.log("int32");
						break;
					case "float":
						result[fieldName] = view.getFloat32(index, littleEndian);
						index += 4;
						console.log("float");
						break;
					case "double":
						result[fieldName] = view.getFloat64(index, littleEndian);
						index += 8;
						console.log("double");
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
