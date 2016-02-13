var primitiveTypes = ["number", "boolean", "string", "null", "undefined"];

var isPrimitive = function(type) {
	for (var i = 0; i < primitiveTypes.length; i++)
		if (type === primitiveTypes[i])
			return true;
	
	return false;
};

var isFunction = function(type) {
	return type === "function";
};

var indent = function(number) {
	var tabSet = "";
	
	for (var i = 0; i < number; i++)
		tabSet += "\t";
	
	return tabSet;
}

function Name(firstName, lastName) {
    this.firstName = firstName;
    this.lastName = lastName;
};

function Address(postalCode, country, city, street, building) {
    this.postalCode = postalCode;
    this.country = country;
    this.city = city;
    this.street = street;	
	this.building = building;
};

function Contacts(address, phoneNumber) {
    this.address = address;
    this.phoneNumber = phoneNumber;
};

function Person(name, age, contacts, titles) {
    this.name = name;
    this.age = age;
    this.contacts = contacts;
	this.titles = titles;
		
	this.log = function() {
		recursiveLog(0, this);
	};
	
	var recursiveLog = function(level, current) {
		var currentType = typeof(current);
		
		if (isFunction(currentType))
			return;
		
		var line = indent(level) + level + "\t" + currentType;
		
		if (isPrimitive(currentType)) {
			console.log(line + "\t" + current);
			return;
		}
		
		console.log(line);
		
		for (property in current)
			recursiveLog(level + 1, current[property]);
	};
};

var anne = new Person(
	new Name("Anne", "Boleyn"),
	28,
	new Contacts(
		new Address("EC3N 4AB", "England", "London", "Tower Hill", "Tower of London"),
		null),
	["Queen of England", "Marquess of Pembroke"]
	);

anne.log();