#include "GNUPlot.h"

using namespace std;


GNUPlot::GNUPlot(vector<string> data_files)
{
	xlabel_ = "X";
	ylabel_ = "Y";
	title_ = "";
	data_files_ = data_files;
	
	// Default if data_labels_ not specified
	data_labels_.reserve(data_files_.size());
	for (int i = 0; i < data_files_.size(); i++)
	{
		data_labels_.push_back("");
	}
}


GNUPlot::~GNUPlot()
{
}


void GNUPlot::setAxisLabels(string xlabel, string ylabel, string title)
{
	xlabel_ = xlabel;
	ylabel_ = ylabel;
	title_ = title;
}


void GNUPlot::setPlotLabels(vector<string> data_labels)
{
	data_labels_ = data_labels;
}


void GNUPlot::makePlot()
{
	//create a pipe to send information to gnuplot
	FILE *gnuplotPipe = _popen("gnuplot -persist", "w");  // Open a pipe to gnuplot

	if (gnuplotPipe) {   // If gnuplot is found
		//Set labels
		fprintf(gnuplotPipe, ("set key autotitle columnhead\n"));
		fprintf(gnuplotPipe, ("set xlabel '" + xlabel_ + "'\n").c_str());
		fprintf(gnuplotPipe, ("set ylabel '" + ylabel_ + "'\n").c_str());
		fprintf(gnuplotPipe, ("set title '" + title_ + "'\n").c_str());
		fprintf(gnuplotPipe, ("set style data lines\n"));

		//Plot:
		fprintf(gnuplotPipe, ("plot \"" + data_files_[0] + "\" title \"" + data_labels_[0] + "\" \n").c_str());
		for (int i = 1; i < data_files_.size(); i++)
		{
			fprintf(gnuplotPipe, ("replot \"" + data_files_[i] + "\" title \"" + data_labels_[i] + "\" \n").c_str());

		}

		fprintf(gnuplotPipe, "show label \n");
		fprintf(gnuplotPipe, "\nexit \n");   // exit gnuplot
		_pclose(gnuplotPipe);    //close pipe
	}
}
