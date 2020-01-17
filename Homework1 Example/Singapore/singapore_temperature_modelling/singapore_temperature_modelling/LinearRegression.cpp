#include "singapore_temperature.h"
#include "LinearRegression.h"

using namespace std;


// Constructor and Destructor
LinearRegression::LinearRegression(int month)
{
	// month for the linear regression; if m=0, refers to all months.
	assert(month >= 0 && month <= 12);
	m_ = month;
}
 

LinearRegression::~LinearRegression()
{
}


// Accessors
int LinearRegression::getMonth()
{ 
	return m_;
}


double LinearRegression::getIntercept()
{
	if (!calc_)
	{
		cerr << "Gradient and interception must be calculated. Run function leastSquaresRegression()" << endl;
		return(-1);
	}

	return c_;
}


double LinearRegression::getGradient()
{
	if (!calc_)
	{
		cerr << "Gradient and interception must be calculated. Run function leastSquaresRegression()" << endl;
		return(-1);
	}
	return g_;
}


// Mutators
void LinearRegression::leastSquaresRegression(vector<double> time, vector<double> temperature)
{
	assert(time.size() == temperature.size());
	int N;
	double sum_x, sum_x2, sum_y, sum_xy;
	sum_x = 0;
	sum_y = 0;
	sum_x2 = 0;
	sum_xy = 0;
	N = time.size();

	// Compute gradient
	//cout << endl << "Fitting linear regression for month " << m_ << " ..." << endl << endl;
	for (int i = 0; i < N; i++)
	{
		sum_x += time[i];
		sum_x2 += (time[i])*(time[i]);
		sum_y += temperature[i];
		sum_xy += time[i] * temperature[i];
	}

	g_ = (N * sum_xy) - (sum_x * sum_y);
	g_ /= (N * sum_x2) - (sum_x * sum_x);

	// Compute intercept
	c_ = (sum_y - g_ * sum_x) / N;

	calc_ = true;  // set flag to true, meaning gradient and intercept of line object have been computed;
}


double LinearRegression::lineExtrapolationTimeSingle(double temperature)
{
	// Predicts the time at which a specific temperature is likely to happen
	if (!calc_)
	{
		cerr << "Gradient and interception must be calculated before attempting extrapolation. Run function leastSquaresRegression()" << endl;
		return(-1);
	}

	return ((temperature - c_) / g_);
}


double LinearRegression::lineExtrapolationTempSingle(double time)
{
	// Predicts the temperature at a specific time (year)
	if (!calc_)
	{
		cerr << "Gradient and interception must be calculated before attempting extrapolation. Run function leastSquaresRegression()" << endl;
		return(-1);
	}

	return (g_ * time + c_);
}


void LinearRegression::lineExtrapolationTime(vector<double> temperatures, vector<double> &times)
{
	// Predicts the times (years) at which a range of specific temperatures are likely to happen
	// User specifies vector<double> temperatures
	// Adds prediction to vector<double> times
	for (int i = 0; i < temperatures.size(); i++)
	{
		times.push_back(lineExtrapolationTimeSingle(temperatures[i]));
	}
}


void LinearRegression::lineExtrapolationTemp(vector<double> times, vector<double> &temperatures)
{ 
	// Predicts the temperatures at a range of specified times (year)
	// User specifies vector<double> times
	// Adds prediction to vector<double> temperatures
	for (int i = 0; i < times.size(); i++)
	{
		temperatures.push_back(lineExtrapolationTempSingle(times[i]));
	}
}