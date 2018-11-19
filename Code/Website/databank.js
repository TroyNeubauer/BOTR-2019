var dataBank = new Map();//maps names to number values
var rawDataInfo = new Map();//maps names to the properties that that name has
var advancedMode = -1;

function add(longName, description, shortName, units, inRawData) {
	shortUnits = units.replace("seconds", "s");
	shortUnits = shortUnits.replace("second", "s");
	shortUnits = shortUnits.replace("feet", "ft");
	shortUnits = shortUnits.replace("degrees", "°");
	shortUnits = shortUnits.replace("miles", "mi.");
	rawDataInfo.set(longName, {description: description, shortName: shortName, longUnits: units, shortUnits: shortUnits, inRawData: inRawData});
}

$(document).ready(function() {
	add("Mission Time", "Time since launch", 									"TIME T:", "seconds", true);
	add("Apogee Height", "The rocket's projected apogee based on its velocity", "APOGEE ", "feet", true);
	add("Time to Apogee", "The time until the rocket's projected perigee", 		"TT APO ", "seconds", true);
	add("Semi Major Axis", "Desc...", 											"SMA    ", "miles", true);
	add("Eccentricity", "Desc...", 												"ECCEN  ", "", true);
	add("Inclination", "Desc...", 												"INC    ", "degrees", true);
	add("Orbital Period", "Desc...", 											"PERIOD ", "seconds", true);
	add("Total Velocity", "Desc...", 											"VEL    ", "feet/second", true);
	add("Terrain Elevation", "Desc...", 										"TRN HGT", "feet", true);
	add("Latitude", "Desc...", 													"LAT    ", "degrees", true);
	add("Longitude", "Desc...", 												"LNG    ", "degrees", true);
	add("Heading", "Desc...", 													"HEADING", "degrees", true);
	add("Vertical Speed", 	"Desc...", 											"VERTSPD", "feet/second", true);
	add("Horizontal Speed", "Desc...", 											"HORZSPD", "feet/second", true);
	add("Atmosphere Density", "Desc...", 										"ATO DEN", "IDK", true);
	add("Gs of Acceleration", "Desc...", 										"G LOAD ", "g", true);

	useAdvancedMode(true);
});

function useAdvancedMode(useAdvanced) {
	if(advancedMode == -1 || advancedMode != useAdvanced) {
		advancedMode = useAdvanced;
		buildUI();
	}
}

function getIDName(name) {
	return "RawDataID_" + name.replace(/ /g, "_");
}

function makeNCharacters(value, n) {
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

function buildUI() {
	var rawData = $("#raw_data");
	rawData.empty();
	rawData.append("<h3>Raw Data</h3>");
	rawDataInfo.forEach(function(value, key, map) {
		element = "<p class=\"datapoint\" id=\"" + getIDName(key) + "\">DEFAULT";
		element += "</p>";
		rawData.append(element);
		putData(key, "-");
	});
}

function getDataValue(name) {
	raw = dataBank.get(name);
	if(raw == null) return null;
	return raw.value;
}

function putData(name, newValue) {
	date = new Date();
	dataBank.set(name, {value: newValue, time: date});
	dataInfo = rawDataInfo.get(name);
	idName = getIDName(name);
	dataElement = $("#" + idName);
	dataElement.empty();
	element = (advancedMode ? dataInfo.shortName + " " : key + ": ");
	element += makeNCharacters(newValue, 8) + " " + (advancedMode ? dataInfo.shortUnits : dataInfo.longUnits);
	dataElement.append(element);
}
