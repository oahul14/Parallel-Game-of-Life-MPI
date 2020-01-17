#pragma once
#include <string>
#include <fstream>
#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;

class Interface
{

public:
	Interface();
	~Interface();

private:
	vector<int> m_all_;
	int start_year_, end_year_;


	// INTERFACE (terminal prompt) FUNCTIONS
	void interfaceIntro();

	void interfaceSelectData();

	void interfaceSelectRange();

	void interfaceSelectRangePop();

	void interfaceInvalid(string message);

	void interfaceSelectMonth();

	void interfaceSelectOutput();

	void interfaceSelectOutputPop();

	// ANALYSING DATA
	void mainTemp();

	void filterByMonth(vector<string> &months, vector<double> &temps, vector<string> &filt_months, vector<double> &filt_temps, int m);

	void lineFit(vector<double> times_extrapolate);

	void findTemperature();

	void findTempPop();

	void findTime();

	void multiRegressionFit();


	// PLOTTING

	void plotData();

	void plotDataLinearFit();

	void plotDataLinearExtrap();
};

