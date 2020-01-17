#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

//n!
template<typename T>
T fact(T number)
{
	T factorial = 1;
	for (T i = 1; i <= number; i++)
	{
		factorial *= i;
	}
	return factorial;
}

int main()
{
	//look at the differences in the output when we change the types
	//what is happening?
	/*cerr << "\nFactorials ";
	for (double i = 0; i < 100; i++)//try 500
	{
		//cerr << "\n" << i << ":\t" << fact(i);
		cerr << "\n" << i << ":\t" << fact<int>(i);
		cerr << ":" << fact<unsigned int>(i);
		cerr << ":" << fact<long long>(i);
		cerr << ":" << fact<float>(i);
		cerr << ":" << fact<double>(i);
		cerr << ":" << fact<long double>(i);
	}
	cin.get();*/

	return 0;
	return 0;
};


