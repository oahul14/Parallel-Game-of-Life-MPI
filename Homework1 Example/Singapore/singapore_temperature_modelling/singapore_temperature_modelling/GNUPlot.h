#pragma once
#include <string>
#include <fstream>
#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;

class GNUPlot
{
	// A SIMPLE CLASS TO PRODUCE A DATA PLOT USING GNU PLOT
	// Constructor will take a data file and will read the first two columnns (X, Y)
	// Class will skip the first line, assuming it's a header
public:
	GNUPlot(vector<string> data_files);
	~GNUPlot();

	void setAxisLabels(string xlabel, string ylabel, string title);
	void setPlotLabels(vector<string> plot_labels);
	void makePlot();
	
private:
	string xlabel_;
	string ylabel_;
	string title_;
	vector<string> data_files_;
	vector<string> data_labels_;
};

