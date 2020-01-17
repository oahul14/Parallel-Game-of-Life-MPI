#pragma once
#include "singapore_temperature.h"
#include <vector>

using namespace std;

class PopRegression
{
public:
	PopRegression();
	~PopRegression();
	void ReadData(string filename, string filename2, int start_year, int end_year);
	void MatrixConstruct();
	void Transpose();
	void solve(string filename, string filename2, int start_year, int end_year);
	double extrapolate(double extrapolation_year);
	void calculateIntercept();
	vector<vector<double>> MatMult(vector<vector<double>> A, vector<vector<double>> B);
	vector<vector<double>> calcInv(vector<vector<double>> A);
	vector<double> years;
	vector<double> temps;
	vector<double> pops;
	vector<vector<double>> X;
	vector<vector<double>> X_T;
	vector<vector<double>> C;
	vector<vector<double>> Y;
	vector<vector<double>> C_inv;
	vector<vector<double>> beta;
	vector<vector<double>> T;
	double intercept;
	int start_year;
	int end_year;
	double beta_t;
	double beta_p;
	
};

