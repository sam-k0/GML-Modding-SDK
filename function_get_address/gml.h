/// Most of this is taken from
/// https://github.com/Saturnyoshi/UGMMS
/// I (sam-k0) took this from YellowAfterlife (who gave credit above)
/// and modified it for my needs

#pragma once
#include <string>
#include <iostream>
#include <map>
#define dllx extern "C" __declspec(dllexport)
#define gmtrue 1.0
#define gmfalse 0.0
#define gmbool double
#define gmint double
#define gmobject int
#define gmsprite int


class GMLInstanceBase;
class GMLInstance;

const int GML_TYPE_REAL = 0;
const int GML_TYPE_STRING = 1;
const int GML_TYPE_ARRAY = 2;
const int GML_TYPE_POINTER = 3;
const int GML_TYPE_UNDEFINED = 5;
const int GML_TYPE_INT32 = 7;
const int GML_TYPE_INT64 = 10;
const int GML_TYPE_BOOL = 13;

struct GMLStringRef {
	const char* string;
	int refCountGML;
	int size;

	GMLStringRef(const char* str) {
		string = str;
		size = std::strlen(str);
		refCountGML = 1;
	}

	void free() {
		if (string != NULL) {
			delete[] string;
		}
	}
};

#pragma pack(push, 4)
struct GMLVar {
	union {
		int valueInt32; // int32
		long long valueInt64; // int64
		double valueReal; // number
		GMLStringRef* valueString; // string
		void* valueArray; // array
		void* valuePointer; // ptr
	};
	int flags; // Not sure what this is used for
	int	type;

	inline void setReal(double value) {
		type = GML_TYPE_REAL;
		valueReal = value;
	}

	inline void setInt32(int value) {
		type = GML_TYPE_INT32;
		valueInt32 = value;
	}

	inline void setInt64(long long value) {
		type = GML_TYPE_INT64;
		valueInt64 = value;
	}

	inline void setBool(bool value) {
		type = GML_TYPE_BOOL;
		valueReal = value ? 1 : 0;
	}

	double getReal() {
		switch (type) {
		case GML_TYPE_REAL:
		case GML_TYPE_BOOL:
			return valueReal;
		case GML_TYPE_INT32:
			return (double)valueInt32;
		case GML_TYPE_INT64:
			return (double)valueInt64;
		default:
			return 0;
		}
	}

	const char* getCString() {
		if (type == GML_TYPE_STRING) {
			if (valueString->string != NULL) {
				return valueString->string;
			}
		}
		return "";
	}

	inline std::string getString() {
		return std::string(getCString());
	}

	GMLVar() {
		type = GML_TYPE_UNDEFINED;
		valuePointer = NULL;
	}

	GMLVar(double value) { setReal(value); }
	GMLVar(float value) { setReal(value); }
	GMLVar(int value) { setInt32(value); }
	GMLVar(long long value) { setInt64(value); }
	GMLVar(bool value) { setReal(value ? 1 : 0); }

	GMLVar(const char* value) {
		type = GML_TYPE_STRING;
		valueString = new GMLStringRef(_strdup(value));
	}

	GMLVar(std::string value) {
		type = GML_TYPE_STRING;
		valueString = new GMLStringRef(_strdup(value.c_str()));
	}

	std::string toString() {
		switch (type) {
		case GML_TYPE_REAL:
		case GML_TYPE_BOOL:
			return std::to_string(valueReal);
		case GML_TYPE_INT32:
			return std::to_string(valueInt32);
		case GML_TYPE_INT64:
			return std::to_string(valueInt64);
		case GML_TYPE_POINTER:
			return "*" + std::to_string((long long)valuePointer);
		case GML_TYPE_UNDEFINED:
			return "undefined";
		case GML_TYPE_STRING:
			return getString();
		case GML_TYPE_ARRAY:
			return "array";
		default:
			return "unkonwn";
		}
	}

	inline bool truthy() {
		// Returns whether the value casts to true in GML
		return getReal() > 0.5;
	}

	inline void freeString() {
		if (type == GML_TYPE_STRING) {
			valueString->free();
			delete valueString;
			type = GML_TYPE_UNDEFINED;
			valuePointer = NULL;
		}
	}

};
#pragma pack(pop)

typedef void(*GMLCall) (GMLVar& result, GMLInstance* self, GMLInstance* other, int argc, GMLVar* argv);
struct GMLClosure {
	int __unknown[26];
	GMLCall call;
	inline void ncall(GMLVar& result, int argc, GMLVar* argv) {
		call(result, nullptr, nullptr, argc, argv);
	}
};

// Stores all pointers to functions
struct GMLAddressTable
{
	// Variable definitions
	std::map<std::string , GMLClosure*> *AddressMap = NULL; // Pointer to a map holding <pointers to addresses , strings>
	bool doneInitializing = false;

	// Constructor
	GMLAddressTable()
	{
		AddressMap = new std::map<std::string , GMLClosure* >; // Allocate the map
		std::cout << "Created GMLAddr Table." << std::endl;
	}

	~GMLAddressTable()  // Override default destructor because of heap allocation
	{
		if(AddressMap != NULL)
		{
			delete AddressMap;
		}
	}

	// Functions
	bool functionExists(std::string key) // Checks if there already is a given entry for a specified function
	{
		if (AddressMap == NULL)
		{
			return false;
		}
		if (AddressMap->count(key) > 0) 
		{
			// Key entry exists
			// Check if key is bad
			if (AddressMap->at(key) != NULL)
			{
				std::cout << "Found entry for function " << key << std::endl;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	// Returns the Address or NULL for the given function 
	GMLClosure* getFunction(std::string key) // Returns Address to the requested function or NULL
	{
		if (functionExists(key))
		{
			return AddressMap->at(key);
		}
		return NULL;
	}

	// Adds a new function to the map
	// Returns true (Could be added), false (Already exists)
	bool addFunction(std::string key, GMLClosure* addr)
	{
		if (functionExists(key) || AddressMap == NULL)
		{
			return false; // entry already exists
		}

		AddressMap->insert(std::pair<std::string, GMLClosure*>(key, addr));
		return true;
	}

	void printAllFunctions()
	{

		for (auto const& x : *AddressMap)
		{
			std::cout << x.first  // string (key)
				<< ':'
				<< x.second // string's value 
				<< std::endl;
		}
	}
};

bool isdef(GMLClosure* c)
{
	return !(c == NULL);
}

// All addresses will be stored here
GMLAddressTable* gmlAddresses = new GMLAddressTable();

