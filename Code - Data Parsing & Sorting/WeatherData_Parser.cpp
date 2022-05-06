// Group8-CS43016-Final Project Code-Data Parser
// 04/25/2022
// 
// Weather Data Parser
// Reads from data file, re-organizing and filtering
// data into a more quantitatively-useful form.
// 

#include <iostream>
using std::endl;
#include <fstream>
using std::ifstream; using std::ofstream;
#include <string>
using std::string; using std::to_string;
#include <sstream>
#include <vector>
using std::vector;
#include <unordered_set>
using std::unordered_set;


const string DATAFILE_NAME = "city_temperature.csv";
const char   DATAFILE_DELIMITER_CHARACTER = ',';
//const int  DATAFILE_NUM_ATTRIBUTES = 8;

const string OUTPUTFILE_NAME = "weatherData_Parsed.csv";
const string LOCATIONFILE_NAME = "locationsList.csv";


// split string on given delimiter character, stored to given vector
void stringSplit(const string&, const char delim, vector<string>&);

// starting year of the data, used for converting date fields
const short int EPOCH_YEAR = 1995;
const unsigned long int toEpochTime(short, short, short);


int main() {

	ifstream fin(DATAFILE_NAME); // open input data file
	if (fin.fail()) {
		std::cerr << "\n\tERROR: Unable to open file \"" << DATAFILE_NAME << "\". Exiting." << endl;
		exit(1);
	}

	ofstream fout(OUTPUTFILE_NAME, std::ios_base::app); // open output data file
	if (fout.fail()) {
		std::cerr << "\n\tERROR: Unable to open file \"" << OUTPUTFILE_NAME << "\". Exiting." << endl;
		exit(2);
	}


	string weatherDataCSVEntry; // one line's entry of the CSV file

	getline(fin, weatherDataCSVEntry); // copy over new header format (below)
	weatherDataCSVEntry = "Region,Country,State,City,DaysSince01-01-" + to_string(EPOCH_YEAR) + ",AvgTemperature";
	fout << weatherDataCSVEntry << endl;


	vector<string> entryItems; // individual line items, 
	                           //split by delimiting character of input file
	unordered_set<string> locations; // set of all individual cities


	// Iterate thru entire CSV file
	long int entryNum = 1;
	while (getline(fin, weatherDataCSVEntry)) {

		std::cout << "Read & Write Entry #" << entryNum++ << endl;

		// split string into individual items on the comma character
		stringSplit(weatherDataCSVEntry, DATAFILE_DELIMITER_CHARACTER, entryItems);

		// re-built string of just the location elements
		const string locStr = entryItems[0] + ',' + entryItems[1] + ',' + entryItems[2] + ',' + entryItems[3];

		fout << locStr << ','; // print location and the converted time since epoch
		fout << to_string( toEpochTime( stoi(entryItems[4]), stoi(entryItems[5]), stoi(entryItems[6]) ) ) << ',';
		fout << entryItems[7] << endl; // lastly, the measurement (temperature) value

		// place location into location set (if not there already)
		locations.insert(locStr);

		// done with this entry, continue on to next line in input file
		entryItems.clear();
	}

	fin.close();
	fout.close();


	ofstream foutLoc(LOCATIONFILE_NAME, std::ios_base::app); // open location list file
	if (foutLoc.fail()) {
		std::cerr << "\n\tERROR: Unable to open file \"" << LOCATIONFILE_NAME << "\". Exiting." << endl;
		exit(3);
	}

	// header of location list file
	foutLoc << "Region,Country,State,City" << endl;

	// place location strings to that file
	entryNum = 1;
	for (auto it = locations.begin(); it != locations.end(); ++it) {

		std::cout << "Write Location #" << entryNum++ << endl;

		foutLoc << *it << endl;
	}

	foutLoc.close(); 
}


// split string on the CSV file's delimiting character
void stringSplit(const string& givenString, const char delim, vector<string>& vect) {

	std::stringstream strStream(givenString);
	string tmp;

	while (getline(strStream, tmp, delim)) {

		vect.push_back(std::move(tmp));
	}
}


// helper function to determine if a given year is a leap year
bool isLeapYear(short yearNum) {

	if (yearNum % 4 == 0) {
		if (yearNum % 100 == 0) {
			if (yearNum % 400 == 0)
				return true;
			else
				return false;
		}
		return true;
	}
	return false;
}

// helper function to return the number of days in given month number
short numDaysInMonth(short monthNum, short yearNum) {

	switch (monthNum) {
	case 1:
		return 31;
	case 2: // February day count depends on what year it is
		if (isLeapYear(yearNum)) return 29;
		else return 28;
	case 3:
		return 31;
	case 4:
		return 30;
	case 5:
		return 31;
	case 6:
		return 30;
	case 7:
		return 31;
	case 8:
		return 31;
	case 9:
		return 30;
	case 10:
		return 31;
	case 11:
		return 30;
	case 12:
		return 31;
	}

	return 0;
}

// convert given month, day, year to epoch time 
// (number of days since 01/01 of EPOCH_YEAR global constant)
const unsigned long int toEpochTime(short month, short day, short year) {

	// if before EPOCH_YEAR, bad entry
	if (year < EPOCH_YEAR) return 0;

	long int numDaysSinceEpoch = 0;

	// account for days per past year
	for (short i = year-1; i >= EPOCH_YEAR; --i) {

		if (isLeapYear(i)) 
			numDaysSinceEpoch += 366;
		else 
			numDaysSinceEpoch += 365;
	}

	// account for days per past month of this year
	for (short i = month - 1; i > 0; --i) {
		numDaysSinceEpoch += numDaysInMonth(i, year);
	}

	// account for days past in current month
	numDaysSinceEpoch += (day - 1);

	return numDaysSinceEpoch;
}
