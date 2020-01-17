#pragma once
#include <vector>
using namespace std;

class LinearRegression
{
	// A CLASS TO FIT A LINEAR REGRESSION GIVEN A SET OF DATA
	// USES LEAST-SQUARES METHOD

public:
	LinearRegression(int month);
	~LinearRegression();

	// Accessors
	int getMonth();
	double getIntercept();
	double getGradient();

	// Mutators
	void leastSquaresRegression(vector<double> time, vector<double> temperature);
	void lineExtrapolationTime(vector<double> temperatures, vector<double> &times);
	void lineExtrapolationTemp(vector<double> times, vector<double> &temperatures);
	double lineExtrapolationTimeSingle(double temperature);
	double lineExtrapolationTempSingle(double time);

private:
	int m_;              // month; If 0, selects whole dataset
	double c_, g_;	    // interception and gradient
	bool calc_ = false;  // flag is true if intercept and gradient exist
};

