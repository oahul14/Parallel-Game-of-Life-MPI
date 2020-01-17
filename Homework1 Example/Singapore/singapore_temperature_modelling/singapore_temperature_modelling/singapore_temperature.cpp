#include "LinearRegression.h"
#include "singapore_temperature.h"

using namespace std;

void readDataFile(string filename, vector<string> &months, vector<double> &temps, int start_year, int end_year)
{
	// Reads a CSV data file of two columns (X,Y); First column is date format "YYYY-MM"; Second column is any double value
	// First column is added to vector<string> months; Second column added to vector<double> temps
	// Can specify what years to store data with int start_year and int end_year

	// Open file stream
	fstream myFile;
	myFile.open(filename, fstream::in);
	if (myFile.fail())
	{
		cerr << filename << endl << "File failed to open..." << endl;
		system("pause");
	}

	// Loop through lines, skip first line
	int no_data_points = 0;
	string line;
	getline(myFile, line);     // cursor on first line (header)
	getline(myFile, line);     // cursor on second line

	while (!myFile.eof())
	{
		// Create stream for the line
		stringstream stream(line);

		// Splitting string by delimeter, saving in vector as double
		bool flag = false;  // flag need to read every other line and store data in appropriate vectors
		int year;
		while (stream.good())
		{
			string substr;
			getline(stream, substr, ',');
			if (!flag)
			{
				year = stoi(substr.substr(0, 4));
				if (year >= start_year && year <= end_year)
				{
					months.push_back(substr);
				}
			}
			else
			{
				if (year >= start_year && year <= end_year)
				{
					temps.push_back(atof(substr.c_str()));
				}
			}
			flag = !flag;
		}
		getline(myFile, line);
		no_data_points++;
	}
	myFile.close();

}


void saveCsv(vector<string> xdata, vector<double> ydata, string out_address, string headerX, string headerY)
{
	// Creates a CSV file to string out_address using array of xdata and ydata in (X,Y) format;
	// First line is header specified by headerX and headerY

	assert(xdata.size() == ydata.size());
	fstream myFile;
	myFile.open(out_address, fstream::out);

	if (myFile.fail())
	{
		cerr << out_address << endl << "File failed to open..." << endl;
		system("pause");
	}

	else
	{
		cout << endl << "saving  to " << out_address << " ..." << endl << endl;
		myFile << headerX + "," + headerY << endl;
		for (int i = 0; i < xdata.size(); i++)
		{
			myFile << xdata[i] << ", " << ydata[i] << endl;
		}
	}



	myFile.close();
}


vector<double> dateStringToFloat(vector<string> date_string)
{

	vector<double> years;
	years.reserve(date_string.size());
	for (int i = 0; i < date_string.size(); i++)
	{
		string y = date_string[i].substr(0, 4);
		string m2 = date_string[i].substr(5, 2);
		double y_double = atof(y.c_str());
		double m2_double = atof(m2.c_str());
		years.push_back(y_double + m2_double / 12.0);
	}
	return years;
}


double dateStringToFloatSingle(string &date)
{
	// string format yyyy-mm
	// returns a double where decimal points refer to months
	// e.g. 2018-06 will return 2018.5

	string y_str = date.substr(0, 4);
	string m_str = date.substr(5, 6);
	double y_db = atof(y_str.c_str());
	double m_db = atof(m_str.c_str());

	return(y_db + (m_db/12.0));

}


string numberToMonth(int m)
{
	// converts a month number into a string

	vector<string> list = {"ALL", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };
	return list[m];
}

bool isInputValid(string input)
{
	// Checks if input is an integer or a set of valid characters *, x and b.
	int input_int = atoi(input.c_str());
	if (input_int == 0)
	{
		stringstream stream(input);
		string substr;
		getline(stream, substr, ' ');
		if (substr == "*" || substr == "x" or substr == "b")
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
	
}