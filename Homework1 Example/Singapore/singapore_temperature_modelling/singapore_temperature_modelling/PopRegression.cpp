#include "PopRegression.h"
#include "singapore_temperature.h"
#include "LinearRegression.h"

using namespace std;

PopRegression::PopRegression()
{
}


PopRegression::~PopRegression()
{
}

void PopRegression::ReadData(string filename, string filename2, int start_year, int end_year)
{
	// Reads two CSV data files of two columns (X,Y); First column is date format "YYYY-MM"; Second column is any double value
	// First column is added to vector<double> years; Second column from the first file added to vector<double> temps
	// Second column from the second file added to vector<double> pops
	// Can specify what years to store data with int start_year and int end_year

	// Open file stream
	fstream myFile;
	fstream myFile2;
	myFile.open(filename, fstream::in);
	myFile2.open(filename2, fstream::in);
	if (myFile.fail() || myFile2.fail())
	{
		cerr << filename << endl << "File failed to open..." << endl;
		system("pause");
	}

	// Loop through lines, skip first line
	int no_data_points = 0;
	string line;
	string line2;
	getline(myFile, line);     // cursor on first line (header)
	getline(myFile, line);     // cursor on second line
	getline(myFile2, line2);     // cursor on first line (header)
	getline(myFile2, line2);     // cursor on second line

	int i = 1;

	while (!myFile.eof() && !myFile2.eof())
	{
		// Create stream for the line
		stringstream stream(line);
		stringstream stream2(line2);

		// Splitting string by delimeter, saving in vector as double
		bool flag = false;  // flag need to read every other line and store data in appropriate vectors
		int year;
		while (stream.good() && stream2.good())
		{
			string substr;
			string substr2;
			getline(stream, substr, ',');
			getline(stream2, substr2, ',');
			if (!flag)
			{
				year = stoi(substr.substr(0, 4));
				if (year >= start_year && year <= end_year)
				{
					years.push_back(year);
				}
			}
			else
			{
				if (year >= start_year && year <= end_year)
				{
					temps.push_back(atof(substr.c_str()));
					pops.push_back(atof(substr2.c_str()));
				}
			}
			flag = !flag;
		}
		getline(myFile, line);
		if (i == 12) // this machinery accounts for the yearly nature of the population data
		{
			getline(myFile2, line2);
			i = 0;
		}
		no_data_points++;
		i++;
	}
	myFile.close();
	myFile2.close();
}

void PopRegression::MatrixConstruct()
{
	// this creates the observation matrix
	X.push_back(years);
	X.push_back(pops);
}

void PopRegression::Transpose()
{
	// this is a simple function to perform a transpose of X
	vector<double> temp;
	X_T.assign(X[0].size(), temp);
	for (int i = 0; i < X.size(); i++)
	{
		for (int j = 0; j < X[0].size(); j++)
		{
			X_T[j].push_back(X[i][j]);
		}
	}
}

vector<vector<double>> PopRegression::MatMult(vector<vector<double>> A, vector<vector<double>> B)
{
	// this is a function to perform matrix multiplication, returns C from A * B = C
	// all matrices must be in vector<vector<double>> format
	vector<double> temp; // temporary variable to fill C
	temp.assign(B[0].size(), 0);
	vector<vector<double>> C;
	C.assign(A.size(), temp);

	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < B[0].size(); j++)
		{
			for (int k = 0; k < B.size(); k++)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return C;
	
}


vector<vector<double>> PopRegression::calcInv(vector<vector<double>> A) 
{
	// as the only inversion required is for a 2x2 matrix, this function can be the simple case
	double det;
	det = (A[0][0] * A[1][1]) - (A[1][0] * A[0][1]);
	double temp = A[0][0];
	A[0][0] = A[1][1] / det;
	A[1][1] = temp / det;
	A[1][0] *= -1 / det;
	A[0][1] *= -1 / det;

	return A;
}

void PopRegression::solve(string filename, string filename2, int start_year_, int end_year_)
{
	// this function evaluates the regression coefficients
	// it is also the top level call for the class, and so takes the filenames and years to be analysed
	// the equation being solved is beta = (X.transpose * X).inverse * (X.transpose * Y)
	start_year = start_year_;
	end_year = end_year_;
	ReadData(filename, filename2, start_year, end_year);
	MatrixConstruct();
	Transpose();
	C = MatMult(X, X_T);
	C_inv = calcInv(C);
	vector<double> p;
	for (int i = 0; i < temps.size(); i++)
	{
		p.push_back(temps[i]);
	}
	T.push_back(p); // p is an intermediary to turn the vector<double> temps into the required vector<vector<double>> for MatMult
	Y = MatMult(T, X_T);
	beta = MatMult(Y, C_inv);
}

void PopRegression::calculateIntercept()
{
	// function to calculate the intercept
	// finds the average of the intercepts for all known configurations
	beta_t = beta[0][0];
	beta_p = beta[0][1];

	double beta_0 = 0;
	for (int i = 0; i < temps.size(); i++)
	{
		beta_0 += temps[i] - beta_t * years[i] - beta_p * pops[i];
	}

	intercept = beta_0 / temps.size();
}


double PopRegression::extrapolate(double extrapolation_year)
{
	// this function extrapolates the temperature to a given year
	// this requires the simple linear extrapolation of population data
	LinearRegression Population(0);
	Population.leastSquaresRegression(years, pops);

	double pop_gradient = Population.getGradient();
	double pop_intercept = Population.getIntercept();
	vector<double> pop_extrapolated;
	vector<double> temp_extrapolated;
	vector<string> string_years;

	for (double i = start_year; i < extrapolation_year; i++)
	{
		double pop_ext = pop_gradient * i + pop_intercept;
		pop_extrapolated.push_back(pop_ext);
		temp_extrapolated.push_back(beta_t * i + beta_p * pop_ext + intercept);
		string_years.push_back(to_string(i));
	}

	string out_address = "./data/processed_data/multi_regression_extrp.csv";
	saveCsv(string_years, temp_extrapolated, out_address, "years", "temperature(C)");

	return temp_extrapolated[temp_extrapolated.size() - 1];
}




