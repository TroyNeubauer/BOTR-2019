

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


function findStructs(text) {
	var result = {};
	var lines = text.split('\n');
	var commented = false;
	for(var i = 0;i < lines.length;i++){
	    var line = lines[i];
		if(commented) {
			if(line.includes("*/")) {
				commented = false;
				console.log("uncommenting at  " + line);
			} else {
				console.log("ignmoring line " + line);
			}
		} else {
			if(line.includes("/*")) {
				commented = true;
				console.log("commented line " + line);
			} else if(line.startsWith("struct")) {
				console.log("normal line " + line);
				i = processStruct(lines, i, result) - 1;//We dont want to increment right into the next line!
			} else {
				console.log("non-struct normal line " + line);
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
	console.log("start name: " +structName );
	i++;
	var fieldMap = {};
	while(!(line = lines[i++]).startsWith("};")) {
		if(line.trim().length > 0 && !line.startsWith("{")) {
			line = line.trim();
			j = 0;
			len = line.length;
			while(j < len && line.charAt(j++) != ' '){ }//Find the space after the word struct
			fieldType =  line.substring(0, j - 1);
			console.log("\tType: " + fieldType);
			line = line.substring(j, line.length);
			line = line.replace(/\s/g, "");
			var semiIndex = line.indexOf(";");
			line = line.substring(0, semiIndex);
			var fields = line.split(",");
			for(var k = 0; k < fields.length; k++) {
				var fieldName = fields[k];
				console.log("\t\tField Name: " + fieldName);
				fieldMap[fieldName] = fieldType;
			}
		}
	}
	result[structName]= {name: structName, fields: fieldMap};
	return i;
}

/*
struct AccelData {
	int16_t ax, ay, az;
	int16_t gx, gy, gz;
	int16_t temp;
};

struct PitotTubeData
{
	uint16_t airSpeed;
};

*/
