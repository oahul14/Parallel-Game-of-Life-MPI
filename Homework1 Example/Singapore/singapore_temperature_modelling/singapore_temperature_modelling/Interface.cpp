#include "Interface.h"
#include "GNUPlot.h"
#include "LinearRegression.h"
#include "PopRegression.h"
#include "singapore_temperature.h"

using namespace std;


Interface::Interface()
{
	interfaceIntro();
	
	// m_all_ empty when initialising
	// m_all_ will hold all months for data analysis
	m_all_ = {};
}


Interface::~Interface()
{
}


void Interface::filterByMonth(vector<string> &months, vector<double> &temps, vector<string> &filt_months, vector<double> &filt_temps, int m)
{
	// From vector<string> months of format "YYYY-MM" and respective temperature values in vector<double> temps,
	// filters data for all years of a given month (m = 1 for January, m = 2 for February and so on...)
	// Filtered values are added to vector<string> filt_months and vector<double> filt_temps

	assert(m > 0 & m <= 12);
	assert(months.size() == temps.size());

	string search;
	if (m < 10)
	{
		search = "0" + to_string(m);
	}
	else
	{
		search = to_string(m);
	}

	for (int i = 0; i < months.size(); i++)
	{
		if (search == months[i].substr(5, 2))
		{
			filt_months.push_back(months[i]);
			filt_temps.push_back(temps[i]);
		}
	}

}

void Interface::interfaceIntro()
{
	system("CLS");
	cout << endl;
	cout << "-------------------------------" << endl;
	cout << " SINGAPORE TEMPERATURE PREDICTOR" << endl;
	cout << "-------------------------------" << endl;
	cout << endl;
	cout << " Deborah Pelacani Cruz" << endl << " John C Walding" << endl << endl;

	cout << "-------------------------------" << endl;
	cout << " This software is designed to make preliminary predictions \n of the surface temperature variation in Singapore" << endl;
	cout << "-------------------------------" << endl;

	cout << endl;
	cout << " Data Used:" << endl;
	cout << "-------------------------------" << endl;
	cout << " Average Surface Temperature of Singapore:" << endl;
	cout << " Changi Climate Station, Singapore, 1982 - 2018" << endl;
	cout << " World Bank Data, 1901 - 2018" << endl;
	cout << endl;
	cout << " Population of Singapore:" << endl;
	cout << " World Bank Data, 1960 - 2017 " << endl;
	cout << endl;

	cout << "------------------------------" << endl;
	cout << " Would you like to continue? (y/n)" << endl;
	cout << " >> ";
	char select;
	cin >> select;

	switch (select)
	{
	case 'y': break;
	case 'n': exit(0);
	default: interfaceIntro();
	}

	interfaceSelectData();
}


void Interface::interfaceSelectData()
{
	system("CLS");
	cin.clear();
	cout << endl;
	cout << " Select Data:" << endl << endl;
	cout << " 1: Surface Temperature" << endl;
	cout << " 2: Surface Temperature and Population" << endl;
	cout << " b: Back" << endl;
	cout << " x: Exit" << endl << endl;
	cout << " >> ";
	string select;
	cin >> select;

	// string to char
	char* select_char = new char[1];
	if (select.size() != 1) { *select_char = '#'; }
	else { copy(select.begin(), select.end(), select_char); }

	switch (*select_char)
	{
	case '1': interfaceSelectRange(); break;
	case '2': interfaceSelectRangePop(); break;
	case 'b': system("CLS"); cin.ignore(); interfaceIntro(); break;
	case 'x': exit(0);
	default: interfaceInvalid(""); interfaceSelectData();
	}
	delete[] select_char;
}


void Interface::interfaceInvalid(string message)
{
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	system("CLS"); 
	cout << endl << " Invalid Selection" << endl << endl;
	cout << "------------------------------" << endl;
	cout << message << endl;
	cout << endl;
	cout << " Press enter to return..." << endl;
	cin.get();
}


void Interface::interfaceSelectRange()
{
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;
	cout << " Insert date range to be analysed [1901 - 2018]: " << endl << endl;
	cout << " Example: \n  >> 1995 2006" << endl;

	cout << endl << " >> ";
	cin >> start_year_;
	cin >> end_year_;

	if (cin.fail()) {
		interfaceInvalid(" Please enter only integer characters");
		interfaceSelectData();
	}

	if (start_year_ < 1901 || start_year_ > 2017 || end_year_ < 1902 || end_year_ > 2018)
	{
		interfaceInvalid(" Date out of range, please select a date between 1901 - 2018");
		interfaceSelectData();
	}

	if (start_year_ >= end_year_)
	{
		interfaceInvalid(" Start year must be greater than end year");
		interfaceSelectData();
	}

	interfaceSelectMonth();
}


void Interface::interfaceSelectMonth()
{
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	system("CLS");
	cout << endl;
	cout << "Analysing data from " << start_year_ << "-" << end_year_ << endl;
	cout << endl;
	cout << " Select Month for Analysis:" << endl << endl;
	cout << " 1: January  \t 2: February" << endl;
	cout << " 3: March  \t 4: April" << endl;
	cout << " 5: May  \t 6: June" << endl;
	cout << " 7: July  \t 8: August" << endl;
	cout << " 9: September  \t 10: October" << endl;
	cout << " 11: November  \t 12: December" << endl;
	cout << " 0: All months " << endl;
	cout << " b: Back" << endl;
	cout << " x: Exit" << endl << endl;

	bool cont = true;
	int m;

	while (cont)
	{
		cout << " >> ";
		string select;
		cin >> select;
		
		if (select == "0") { m = 0; }
		else if (select == "1") { m = 1; }
		else if (select == "2") { m = 2; }
		else if (select == "3") { m = 3; }
		else if (select == "4") { m = 4; }
		else if (select == "5") { m = 5; }
		else if (select == "6") { m = 6; }
		else if (select == "7") { m = 7; }
		else if (select == "8") { m = 8; }
		else if (select == "9") { m = 9; }
		else if (select == "10") { m = 10; }
		else if (select == "11") { m = 11; }
		else if (select == "12") { m = 12; }
		else if (select == "x") { exit(0); }
		else if (select == "b") { system("CLS"); interfaceSelectData(); }
		else { interfaceInvalid(""); interfaceSelectMonth(); }
		
		//// string to char
		//char* select_char = new char[2];
		//if (select.size() > 2) { *select_char = '#'; }
		//else { copy(select.begin(), select.end(), select_char); }

		//switch (*select_char)
		//{
		//case '0': m = 0; break;
		//case '1': m = 1; break;
		//case '2': m = 2; break;
		//case '3': m = 3; break;
		//case '4': m = 4; break;
		//case '5': m = 5; break;
		//case '6': m = 6; break;
		//case '7': m = 7; break;
		//case '8': m = 8; break;
		//case '9': m = 9; break;
		//case '10': m = 10; break;
		//case '11': m = 11; break;
		//case '12': m = 12; break;
		//case 'x': exit(0);
		//case 'b':  system("CLS"); interfaceSelectData(); break;
		//default: interfaceInvalid(""); interfaceSelectMonth();
		//}
		//delete[] select_char;


		m_all_.push_back(m);
		//sort and remove duplicated
		sort(m_all_.begin(), m_all_.end());
		m_all_.erase(unique(m_all_.begin(), m_all_.end()), m_all_.end());
		
		cout << endl;
		for (int mm : m_all_)
			cout << " " << numberToMonth(mm) << " ";
		cout << " selected. " << endl << endl;

		cin.clear(); cin.ignore();
		char select2;
		cout << "Would you like to select another month? (y/n) >> ";
		cin >> select2;
		switch (select2)
		{
		case 'y': cont = true; break;
		case 'n': cont = false; break;
		default: interfaceInvalid(""); interfaceSelectMonth();
		}
	}
	mainTemp();

}


void Interface::mainTemp()
{
	// FUNCTION TO READ AVERAGE TEMPERATURE DATA, 
	// FILTER IT BY DATE AND SAVE FILTERED DATA AS A CSV FILE

	// create vectors to store data, for all months and for inidvidual months
	vector<string> months;
	vector<double> temps;

	// read file and store data in vectors
	string filename = "./data/surface-air-temperature-monthly-mean/surface-air-temperature-monthly-mean.csv";
	readDataFile(filename, months, temps, start_year_, end_year_);

	// filter by month; output monthly data to csv
	// if m = 0, store whole dataset
	vector<string> filt_month;
	vector<double> filt_temps;

	for (int mm : m_all_)
	{
		filt_month.clear();
		filt_temps.clear();

		if (mm != 0)
		{
			filterByMonth(months, temps, filt_month, filt_temps, mm);
		}
		else
		{
			for (int i = 0; i < months.size(); i++)
			{
				filt_temps.push_back(temps[i]);
				filt_month.push_back(to_string(dateStringToFloatSingle(months[i])));
			}
		}
		string outaddress = "./data/processed_data/avg_temp_data_" + to_string(mm) + ".csv";
		saveCsv(filt_month, filt_temps, outaddress, "year", "temperature(c)");
	}

	interfaceSelectOutput();
}



void Interface::interfaceSelectOutput()
{
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	system("CLS");
	cout << endl;
	cout << "Analysing ";
	for (int mm : m_all_)
	{
		cout << numberToMonth(mm) << " ";
	}
	cout << "data from " << start_year_ << "-" << end_year_ << endl;
	cout << endl << endl;

	cout << " Select Output:" << endl << endl;
	cout << " 1: Data Plot" << endl;
	cout << " 2: Data Plot with Linear Fit" << endl;
	cout << " 3: Data Plot with Linear Extrapolation" << endl;
	cout << " 4: Find temperature for specific time" << endl;
	cout << " 5: Find time for specific temperature " << endl;
	cout << " b. Back" << endl;
	cout << " x: Exit" << endl << endl;
	cout << " >> ";
	string select;
	cin >> select;

	char* select_char = new char[1];
	if (select.size() != 1) { *select_char = '#'; }
	else { copy(select.begin(), select.end(), select_char); }

	switch (*select_char)
	{
	case '1': plotData(); break;
	case '2': plotDataLinearFit(); break;
	case '3': plotDataLinearExtrap(); break;
	case '4': findTemperature(); break;
	case '5': findTime(); break;
	case 'b': m_all_.clear(); interfaceSelectMonth(); break;
	case 'x': exit(0);
	default: interfaceInvalid(""); interfaceSelectOutput();
	}
	delete[] select_char;
}



void Interface::plotData()
{
	// Plots temperature data

	vector<string> data_names;
	vector<string> data_labels;
	data_names.reserve(m_all_.size());
	data_labels.reserve(m_all_.size());

	for (int m : m_all_)
	{
		data_names.push_back("./data/processed_data/avg_temp_data_" + to_string(m) + ".csv");
		data_labels.push_back(numberToMonth(m));
	}

	GNUPlot Plot(data_names);
	Plot.setAxisLabels("Year", "Temperature(C)", "");
	Plot.setPlotLabels(data_labels);
	Plot.makePlot();

	interfaceSelectOutput();
}



void Interface::plotDataLinearFit()
{
	//Plots temperature data with a linear fit

	// Create array of times to fit line
	vector<double> times_extrapolate;
	for (int i = start_year_; i <= end_year_; i++)
	{
		times_extrapolate.push_back(i);
	}
	// Create linear fit
	lineFit(times_extrapolate);

	vector<string> data_names;
	vector<string> data_labels;
	data_names.reserve(m_all_.size());
	data_labels.reserve(m_all_.size());

	for (int m : m_all_)
	{
		data_names.push_back("./data/processed_data/avg_temp_data_" + to_string(m) + ".csv");
		data_names.push_back("./data/processed_data/extrap_temp_data_" + to_string(m) + ".csv");

		data_labels.push_back(numberToMonth(m));
		data_labels.push_back(numberToMonth(m) + " Least Squares");
		
	}

	GNUPlot Plot(data_names);
	Plot.setAxisLabels("Year", "Temperature(C)", "");
	Plot.setPlotLabels(data_labels);
	Plot.makePlot();

	interfaceSelectOutput();
}



void Interface::plotDataLinearExtrap()
{
	// Plots temperature data with a linear fit extrapolated to a user-specified year
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;
	cout << " Insert final year for extrapolation " << endl << endl;
	cout << " Example: \n  >> 2056" << endl;
	double extrapolation_year;
	cout << endl << " >> ";
	cin >> extrapolation_year;

	if (cin.fail()) {
		interfaceInvalid(" Please enter only integer or float characters");
		interfaceSelectOutput();
	}

	if (extrapolation_year <= end_year_)
	{
		interfaceInvalid(" Please select an extrapolation year greater than the time range of analysis [" + to_string(end_year_) + "]");
		interfaceSelectOutput();
	}

	// Create array of times to fit line
	vector<double> times_extrapolate;
	for (int i = start_year_; i <= extrapolation_year; i++)
	{
		times_extrapolate.push_back(i);
	}
	// Create linear fit
	lineFit(times_extrapolate);

	vector<string> data_names;
	vector<string> data_labels;
	data_names.reserve(m_all_.size());
	data_labels.reserve(m_all_.size());


	for (int m : m_all_)
	{
		data_names.push_back("./data/processed_data/avg_temp_data_" + to_string(m) + ".csv");
		data_names.push_back("./data/processed_data/extrap_temp_data_" + to_string(m) + ".csv");

		data_labels.push_back(numberToMonth(m));
		data_labels.push_back(numberToMonth(m) + " Least Squares");

	}

	GNUPlot Plot(data_names);
	Plot.setAxisLabels("Year", "Temperature(C)", "");
	Plot.setPlotLabels(data_labels);
	Plot.makePlot();

	interfaceSelectOutput();
}



void Interface::findTemperature()
{
	// Finds a temperature value for a user-specified year using extrapolated line regression
	// Year given in decimals and temperature in degrees celcius

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;
	cout << " Insert year" << endl << endl;
	cout << " Example: \n  >> 2056" << endl;
	double extrapolation_year;
	cout << endl << " >> ";
	cin >> extrapolation_year;

	if (cin.fail()) {
		interfaceInvalid(" Please enter only integer or float characters");
		interfaceSelectOutput();
	}

	vector<double> month_temps, years;
	vector<string> year_string;
	vector<LinearRegression> lines;

	for (int mm : m_all_)
	{
		month_temps.clear();
		year_string.clear();
		years.clear();

		// read monthly data
		string inaddress = "./data/processed_data/avg_temp_data_" + to_string(mm) + ".csv";
		readDataFile(inaddress, year_string, month_temps, start_year_, end_year_);

		// transform time data to years
		years = dateStringToFloat(year_string);

		// create linear regression object and initialise it
		LinearRegression Regression(mm);
		Regression.leastSquaresRegression(years, month_temps);

		// extrapolate at chosen times
		cout << endl;
		cout << numberToMonth(mm) + " " << extrapolation_year << " : " <<
			Regression.lineExtrapolationTempSingle(extrapolation_year) << " C " << endl;
	}
	cout << endl;
	system("pause");
	interfaceSelectOutput();
}


void Interface::findTime()
{
	// Finds the year  for a user-specified temperature value using extrapolated line regression
	// Year given in decimals and temperature in degree celcius

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;
	cout << " Insert temperature (C)" << endl << endl;
	cout << " Example: \n  >> 29.45" << endl;
	double temperature_find;
	cout << endl << " >> ";
	cin >> temperature_find;

	if (cin.fail()) {
		interfaceInvalid(" Please enter only integer or float characters");
		interfaceSelectOutput();
	}

	vector<double> month_temps, years;
	vector<string> year_string;
	vector<LinearRegression> lines;

	for (int mm : m_all_)
	{
		month_temps.clear();
		year_string.clear();
		years.clear();

		// read monthly data
		string inaddress = "./data/processed_data/avg_temp_data_" + to_string(mm) + ".csv";
		readDataFile(inaddress, year_string, month_temps, start_year_, end_year_);

		// transform time data to years
		years = dateStringToFloat(year_string);

		// create linear regression object and initialise it
		LinearRegression Regression(mm);
		Regression.leastSquaresRegression(years, month_temps);

		// extrapolate at chosen times
		cout << endl;
		cout << numberToMonth(mm) + " " << temperature_find << " C : " <<
			Regression.lineExtrapolationTimeSingle(temperature_find) << endl;
	}
	cout << endl;
	system("pause");
	interfaceSelectOutput();

}


void Interface::lineFit(vector<double> times_extrapolate)
{
	// Creates data points of the extrapolated line using linear regression of the temperature data.
	// Saves it into a CSV file using the saveCsv() function format

	vector<double> temperatures_extrapolate;
	vector<double> month_temps, years;
	vector<string> times_extrapolate_string;
	vector<string> year_string;
	vector<LinearRegression> lines;

	// Reserve vector spaces
	temperatures_extrapolate.reserve(times_extrapolate.size());
	times_extrapolate_string.reserve(times_extrapolate.size());
	month_temps.reserve(times_extrapolate.size());
	years.reserve(times_extrapolate.size());
	year_string.reserve(times_extrapolate.size());

	for (int mm : m_all_)
	{
		times_extrapolate_string.clear();
		temperatures_extrapolate.clear();
		month_temps.clear();
		year_string.clear();
		years.clear();

		// read monthly data
		string inaddress = "./data/processed_data/avg_temp_data_" + to_string(mm) + ".csv";
		readDataFile(inaddress, year_string, month_temps, start_year_, end_year_);

		// transform time data to years
		years = dateStringToFloat(year_string);

		// create linear regression object and initialise it
		LinearRegression Regression(mm);
		Regression.leastSquaresRegression(years, month_temps);

		// extrapolate at chosen times
		Regression.lineExtrapolationTemp(times_extrapolate, temperatures_extrapolate);
		cout << endl << "GRADIENT: " << Regression.getGradient() << endl;
		lines.push_back(Regression);

		//something quick to be fixed
		for (int i = 0; i < times_extrapolate.size(); i++)
		{
			times_extrapolate_string.push_back(to_string(times_extrapolate[i]));
		}

		// save extrapolation into file
		string outaddress = "./data/processed_data/extrap_temp_data_" + to_string(mm) + ".csv";
		saveCsv(times_extrapolate_string, temperatures_extrapolate, outaddress, "year", "temperature_prediction(c)");
	}
}


void Interface::interfaceSelectRangePop()
{
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;
	cout << " Insert date range to be analysed [1982 - 2017]: " << endl << endl;
	cout << " Example: \n  >> 1995 2006" << endl;

	cout << endl << " >> ";
	cin >> start_year_;
	cin >> end_year_;

	if (cin.fail()) {
		interfaceInvalid(" Please enter only integer characters");
		interfaceSelectData();
	}

	if (start_year_ < 1982 || start_year_ > 2016 || end_year_ < 1983 || end_year_ > 2017)
	{
		interfaceInvalid(" Date out of range, please select a date between 1982 - 2017");
		interfaceSelectData();
	}

	if (start_year_ >= end_year_)
	{
		interfaceInvalid(" Start year must be greater than end year");
		interfaceSelectData();
	}

	interfaceSelectOutputPop();
}

void Interface::interfaceSelectOutputPop()
{
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	system("CLS");
	cout << endl;
	cout << "Analysing ";
	for (int mm : m_all_)
	{
		cout << numberToMonth(mm) << " ";
	}
	cout << "data from " << start_year_ << "-" << end_year_ << endl;
	cout << endl << endl;

	cout << " Select Output:" << endl << endl;
	cout << " 1: Temperature Data Plot with Linear Extrapolation" << endl;
	cout << " 2: Find temperature for specific time" << endl;
	cout << " b. Back" << endl;
	cout << " x: Exit" << endl << endl;
	cout << " >> ";
	string select;
	cin >> select;

	char* select_char = new char[1];
	if (select.size() != 1) { *select_char = '#'; }
	else { copy(select.begin(), select.end(), select_char); }

	switch (*select_char)
	{
	case '1': multiRegressionFit(); break;
	case '2': findTempPop(); break;
	case 'b': m_all_.clear(); interfaceSelectData(); break;
	case 'x': exit(0);
	default: interfaceInvalid(""); interfaceSelectOutputPop();
	}
	delete[] select_char;
}

void Interface::multiRegressionFit()
{
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	system("CLS");
	cout << endl;
	cout << "Analysing data from " << start_year_ << "-" << end_year_ << endl;
	cout << endl;

	cout << " Insert final year for extrapolation " << endl << endl;
	cout << " Example: \n  >> 2056" << endl;
	double extrapolation_year;
	cout << endl << " >> ";
	cin >> extrapolation_year;

	if (cin.fail()) {
		interfaceInvalid(" Please enter only integer or float characters");
		interfaceSelectOutputPop();
	}

	if (extrapolation_year <= end_year_)
	{
		interfaceInvalid(" Please select an extrapolation year greater than the time range of analysis [" + to_string(end_year_) + "]");
		interfaceSelectOutputPop();
	}

	PopRegression PopFit;
	string filename = "./data/surface-air-temperature-monthly-mean/surface-air-temperature-monthly-mean82.csv";
	string filename2 = "./data/yearly_pop_data.csv";

	PopFit.solve(filename, filename2, start_year_, end_year_);
	PopFit.calculateIntercept();
	double ext_temp = PopFit.extrapolate(extrapolation_year);

	vector<string> data_names = {"./data/avg_temp_data.csv","./data/processed_data/multi_regression_extrp.csv" };
	vector<string> data_labels = {"TEMP", "TEMP-POP Regression" };
	GNUPlot Plot(data_names);
	Plot.setAxisLabels("Year", "Temperature(C)", "");
	Plot.setPlotLabels(data_labels);
	Plot.makePlot();

	interfaceSelectOutputPop();

}

void Interface::findTempPop()
{
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	system("CLS");
	cout << endl;
	cout << "Analysing data from " << start_year_ << "-" << end_year_ << endl;
	cout << endl;

	cout << " Insert  year " << endl << endl;
	cout << " Example: \n  >> 2056" << endl;
	double extrapolation_year;
	cout << endl << " >> ";
	cin >> extrapolation_year;

	if (cin.fail()) {
		interfaceInvalid(" Please enter only integer or float characters");
		interfaceSelectOutputPop();
	}

	if (extrapolation_year <= end_year_)
	{
		interfaceInvalid(" Please select an extrapolation year greater than the time range of analysis [" + to_string(end_year_) + "]");
		interfaceSelectOutputPop();
	}

	PopRegression PopFit;
	string filename = "./data/surface-air-temperature-monthly-mean/surface-air-temperature-monthly-mean82.csv";
	string filename2 = "./data/yearly_pop_data.csv";

	PopFit.solve(filename, filename2, start_year_, end_year_);
	PopFit.calculateIntercept();
	double extrapolated_temp = PopFit.extrapolate(extrapolation_year);

	cout << endl;
	cout << extrapolation_year << " : " << extrapolated_temp << " C " << endl;
	cout << endl;
	system("pause");

	interfaceSelectOutputPop();

}