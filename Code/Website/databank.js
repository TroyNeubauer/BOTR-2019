var dataBank = new Map();//maps names to number values
var rawDataInfo = new Map();//maps names to the properties that that name has
var advancedMode = true;
var needsUpdate = true;

function add(longName, description, shortName, units, parent) {
	shortUnits = units.replace("seconds", "s");
	shortUnits = shortUnits.replace("second", "s");
	shortUnits = shortUnits.replace("feet", "ft");
	shortUnits = shortUnits.replace("degrees", "°");
	shortUnits = shortUnits.replace("miles", "mi.");
	rawDataInfo.set(longName, {description: description, shortName: shortName, longUnits: units, shortUnits: shortUnits, parent: parent});
}

add("Mission Time", "Time since launch", 									"TIME T:", "seconds", 		"orbit");
add("Apogee Height", "The rocket's projected apogee based on its velocity", "E APOGE", "feet", 			"orbit");
add("Time to Apogee", "The time until the rocket's projected perigee", 		"TT APO ", "seconds", 		"orbit");
add("Semi Major Axis", "Desc...", 											"SMA    ", "miles",			"orbit");
add("Eccentricity", "Desc...", 												"ECCEN  ", "", 				"orbit");
add("Inclination", "Desc...", 												"INC    ", "degrees", 		"orbit");
add("Orbital Period", "Desc...", 											"PERIOD ", "seconds", 		"orbit");
add("Total Velocity", "Desc...", 											"VEL    ", "feet/second", 	"orbit");
add("Terrain Elevation", "Desc...", 										"TRN HGT", "feet", 			"orbit");
add("Latitude", "Desc...", 													"LAT    ", "degrees", 		"orbit");
add("Longitude", "Desc...", 												"LNG    ", "degrees", 		"orbit");
add("Heading", "Desc...", 													"HEADING", "degrees", 		"orbit");
add("Vertical Speed", 	"Desc...", 											"VERTSPD", "feet/second", 	"orbit");
add("Horizontal Speed", "Desc...", 											"HORZSPD", "feet/second", 	"orbit");

add("Atmosphere Density", "Desc...", 			"ATN DEN", "IDK", 	"sensors");
add("Altimeter Altitude", "Desc...", 			"ALTIALT", "g", 	"sensors");
add("GPS Altitude" , "Desc...", 				"GPS ALT", "e", 	"sensors");
add("Pitot Speed" , "Desc...", 					"PIT SPD", "e", 	"sensors");
add("GPS Speed" , "Desc...", 					"GPS SPD", "e", 	"sensors");
add("Altimeter Speed" , "Desc...", 				"ALTISPD", "e", 	"sensors");
add("Accelerometer Accerlation" , "Desc...",	"ACC ACC", "e", 	"sensors");
add("Altimeter Accerlation" , "Desc...", 		"ALTIACC", "e", 	"sensors");
add("Temp7" , "Desc...", 						"TEMP7  ", "e", 	"sensors");

add("Temp8" , "Desc...", 					"TEMP8  ", "e", 			"onboard");
add("Temp9" , "Desc...", 					"TEMP9 " , "e", 			"onboard");
add("Temp10", "Desc...", 					"TEMP10 ", "e", 			"onboard");
add("Temp11", "Desc...", 					"TEMP11 ", "e", 			"onboard");
add("Temp12", "Desc...", 					"TEMP12 ", "e", 			"onboard");
add("Temp13", "Desc...", 					"TEMP13 ", "e", 			"onboard");
add("Temp14", "Desc...", 					"TEMP14 ", "e", 			"onboard");
add("Temp15", "Desc...", 					"TEMP15 ", "e", 			"onboard");
add("Temp16", "Desc...", 					"TEMP16 ", "e", 			"onboard");
add("Temp17", "Desc...", 					"TEMP17 ", "g", 			"onboard");
add("Temp18", "Desc...", 					"TEMP18 ", "e", 			"onboard");
add("Temp19", "Desc...", 					"TEMP19 ", "e", 			"onboard");
add("Temp20", "Desc...", 					"TEMP20 ", "e", 			"onboard");
add("Temp21", "Desc...", 					"TEMP21 ", "e", 			"onboard");


function makeNCharactersPre(value, n) {
	asString = value + "";
	if(asString.length == n) {
		return asString;
	} else if(asString.length > n) {
		return asString.substring(0, n);
	} else {
		while(asString.length < n) {
			asString = " " + asString;
		}
		return asString;
	}
}

function makeNCharactersPost(value, n) {
	asString = value + "";
	if(asString.length == n) {
		return asString;
	} else if(asString.length > n) {
		return asString.substring(0, n);
	} else {
		while(asString.length < n) {
			asString = asString + " ";
		}
		return asString;
	}
}

function dataRender() {
	if(needsUpdate) {
		var temp = new Map();
		rawDataInfo.forEach(function(rawInfo, name, map) {
			var parent = rawInfo["parent"];
			var string = temp.get(parent);
			if(string == undefined) {
				//string = "<p class=\"data\">";
				string = "";
			}
			if(advancedMode) string += makeNCharactersPost(rawInfo["shortName"], 7);
			else string += name;

			string += " ";
			string += "<span style=\"color:white;\">";
			var value = getDataValue(name);
			if(typeof(value) == "number" && value < 0) string += "-";
			else string += " ";

			value = Math.abs(value);

			if(advancedMode) string += makeNCharactersPre(value, 6);
			else string += value;
			string += "</span> ";
			if(advancedMode) string += rawInfo["shortUnits"];
			else string += rawInfo["longUnits"];
			string += "\n";
			temp.set(parent, string);
		});
		temp.forEach(function(value, key) {
			var element = $("#" + key);
			element.html(value);
		});
		needsUpdate = true;
	}
}
dataRender();

function getDataValue(name) {
	raw = dataBank.get(name);
	if(raw == null) return null;
	return raw.value;
}

function putData(name, newValue) {
	date = new Date();
	needsUpdate = true;
	dataBank.set(name, {value: newValue, time: date});
}
