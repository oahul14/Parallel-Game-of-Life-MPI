#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <assert.h>
#include <cstdio>
#include <cassert>
#include <algorithm>

using namespace std;


// READING, SAVING AND ANALYSING FILES
void readDataFile(string filename, vector<string> &months, vector<double> &temps, int start_year, int end_year);

void saveCsv(vector<string>, vector<double>, string outAddress, string headerX, string headerY);

string numberToMonth(int m);


// DATA PARSING 
double dateStringToFloatSingle(string &date);

vector<double> dateStringToFloat(vector<string>);

bool isInputValid(string input);

