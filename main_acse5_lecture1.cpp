#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main()
{
	//streams
	cout << "\nHello world!\n"; //standard output
	cerr << "\nHello world ERROR!\n"; //standard error

	cout << "\nHey! what is your name and age? ...";

	int iAge(0);// , j(1), k(2);
	//k = i + j;

	string name, last_name;
	cin >> name >> last_name >> iAge;
	string full_name(name + " " + last_name);
	
	cout << "\n\nHey! Your name is " << name << " " << last_name;
	cout << "\n\nHey! Your FULL name is " << full_name << " and age " << iAge; 

	//streams
	ofstream ofs("output_file.txt", ios::trunc);
	ofs << "\n\nHey! Your name is " << name << " " << last_name;
	ofs.close();

	int a = -1;
	unsigned int ua = 2;
	double f = 2.3;
	float ff = 1.2;

	system("pause");

	return 0;
};


