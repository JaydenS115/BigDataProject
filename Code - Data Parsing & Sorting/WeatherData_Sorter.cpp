// Group8-CS43016-Final Project Code-Data Sorter
// 04/25/2022
// 
// Weather Data Sorter
// Reads from pre-parsed data file, sorting data entries
// into different forms for the purposes of statistical calculations
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
#include <unordered_map>
using std::unordered_map; using std::pair;
#include <set>
using std::set;


const string DATAFILE_NAME = "weatherData_Parsed.csv";
const char   DATAFILE_DELIMITER_CHARACTER = ',';
//const int  DATAFILE_NUM_ATTRIBUTES = 6;

const string OUTPUTFILE_NAME = "weatherData_Sorted.csv";
const string SPLIT_OUTPUTFILE_NAME = "weatherData_Sorted_Split.csv";


// starting year of the data, used for converting date fields
const short int EPOCH_YEAR = 1995;
// split string on given delimiter character, stored to given vector
void stringSplit(const string&, const char delim, vector<string>&);

typedef vector<pair<string, string>> dataSequence_t;
typedef unordered_map<string, dataSequence_t> sortedData_t;

int main() {

	ifstream fin(DATAFILE_NAME); // open input parsed data file
	if (fin.fail()) {
		std::cerr << "\n\tERROR: Unable to open file \"" << DATAFILE_NAME << "\". Exiting." << endl;
		exit(1);
	}

	ofstream fout(OUTPUTFILE_NAME, std::ios_base::app); // open output sorted data file
	if (fout.fail()) {
		std::cerr << "\n\tERROR: Unable to open file \"" << OUTPUTFILE_NAME << "\". Exiting." << endl;
		exit(2);
	}


	string weatherDataCSVEntry; // one line's entry of the CSV file

	getline(fin, weatherDataCSVEntry); // copy over new header format (below)
	weatherDataCSVEntry = "Region,Country,State,City,Entry1.DaysSince01-01-" + to_string(EPOCH_YEAR) + ",Entry1.AvgTemperature,...";
	fout << weatherDataCSVEntry << endl;


	vector<string> entryItems; // individual line items
	                           // split by delimiting character of input file
	sortedData_t sortedData; // set of all individual cities


	// Iterate thru entire CSV file
	long int entryNum = 0;
	while (getline(fin, weatherDataCSVEntry)) {
		std::cout << "Read Entry #" << entryNum++ << endl;

		// split string into individual items on comma character
		stringSplit(weatherDataCSVEntry, DATAFILE_DELIMITER_CHARACTER, entryItems);

		// re-built string of just the location elements
		const string locStr = entryItems[0] + ',' + entryItems[1] + ',' + entryItems[2] + ',' + entryItems[3];

		// to the map entry for this location, 
		//	add to the end of the location list the
		//	pair of values:  (daysSinceEpoch, avgTemperature)
		sortedData[locStr].push_back( pair<string, string>(entryItems[4], entryItems[5]) );

		// done with this entry, continue on to next line of input file
		entryItems.clear();
	}

	fin.close(); // done with input data file


	// write sorted data into output file
	int locNum = 1;
	for (sortedData_t::iterator it = sortedData.begin(); it != sortedData.end(); ++it) {

		fout << it->first; // write the location string itself

		// go thru each entry in this location's data
		entryNum = 1;
		for (dataSequence_t::iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {

			std::cout << "Write Location #" << locNum << " Entry #" << entryNum++ << endl;

			// if entry has valid temperature, add it to output file
			if(jt->second != "-99")
				fout << ',' << jt->first << ',' << jt->second;
		}
		
		fout << endl; // on to next entry in sorted data set
		++locNum;
	}

	fout.close();



	fout.open(SPLIT_OUTPUTFILE_NAME, std::ios_base::app); // open SPLIT output sorted data file, split by entry attribute
	if (fout.fail()) {
		std::cerr << "\n\tERROR: Unable to open file \"" << SPLIT_OUTPUTFILE_NAME << "\". Exiting." << endl;
		exit(3);
	}


	// write attribute 1 (days since epoch) to sorted data file
	fout << "Region,Country,State,City,Entry1.DaysSince01-01-" + to_string(EPOCH_YEAR) + ",Entry2.DaysSince01-01-" + to_string(EPOCH_YEAR) + ",..." << endl;
	locNum = 1;
	for (sortedData_t::iterator it = sortedData.begin(); it != sortedData.end(); ++it) {

		fout << it->first; // write the location string itself

		entryNum = 1;
		for (dataSequence_t::iterator jt = it->second.begin(); 
			  jt != it->second.end() && entryNum <= 1825; ++jt) {

			std::cout << "Write Location #" << locNum << " Time Attribute #" << entryNum++ << endl;

			// if entry has valid temperature, 
			// add TIME attribute to split output file
			if(jt->second != "-99")
				fout << ',' << jt->first;
		}

		fout << endl; // on to the next entry in the sorted data set
		++locNum;
	}

	// write attribute 2 (average temperature) to sorted data file
	fout << endl << "Region,Country,State,City,Entry1.AvgTemperature,Entry2.AvgTemperature,..." << endl;
	locNum = 1;
	for (sortedData_t::iterator it = sortedData.begin(); it != sortedData.end(); ++it) {

		fout << it->first; // write the location string itself

		entryNum = 1;
		for (dataSequence_t::iterator jt = it->second.begin();
			jt != it->second.end() && entryNum <= 1825; ++jt) {

			std::cout << "Write Location #" << locNum << " Time Attribute #" << entryNum++ << endl;

			// if entry has valid temperature, 
			// add TEMPERATURE attribute to split output file
			if (jt->second != "-99")
				fout << ',' << jt->second;
		}

		fout << endl; // on to the next entry in the sorted data set
		++locNum;
	}


	fout.close(); 
}


// split string on the CSV file's delimiting character
void stringSplit(const string& givenString, const char delim, vector<string>& vect) {

	std::stringstream strStream(givenString);
	string tmp;
	
	while (getline(strStream, tmp, delim)) {

		vect.push_back(std::move(tmp));
	}
}
