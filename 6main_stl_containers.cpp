//#include <iostream>		// input/output standard library
//#include <string>			// standard string library
//#include <fstream>			
//
//#include <vector>//gives me the vector			
//#include <deque>			
//#include <list>			
//#include <set>			
//#include <map>			
//#include <unordered_set>	
//#include <unordered_map>	
//
//#include <iterator>	
//#include <algorithm>	
//#include <numeric>	
//#include <functional>	
//#include "7factorial.h"
//
//using namespace std;//stl
//
////#define ACSE_USE_TEMPLATES
//
//int main()
//{
//	/*double apple;
//	{//small scope
//		double pear(42.);
//		double d1(42.);
//		double c = d1 + d;//stack
//		c++;
//		{
//			c--;
//		}
//		//cerr << c;
//	}//d d1 c -> deleted
//	cerr << c;*/
//	//double arrayFactorial[1000];
//	//std::vector<double> vFactorials_(1000, 42.);//= resize
//	//vFactorials_.resize(1000);
////vFactorials_.reserve(1000);
//
//	/*std::vector<double> vFactorials_(100);
//	std::map<double, double> mNumber_to_factorials_;
//	for (double i = 0; i < 100; i++)
//	{
//		const double factorial(fact<double>(i));
//		vFactorials_.emplace_back(factorial);
//		//vFactorials_[i] = factorial;
//		mNumber_to_factorials_.insert(make_pair(i, factorial));
//	}*/
//
//	/*int c = 0;
//	{
//		double c = 1;
//	}
//	cerr << c;*/
//
//	/*std::vector<double> v1;
//	std::set<double> s1;//you can'T have duplicates
//	std::unordered_set<double> us1;//you can'T have duplicates
//	std::map<double, double> m1;//you can'T have duplicates
//	std::multimap<double, double> mm1;//you can have duplicates
//	for (double i = 0; i < 100; i++)
//	{
//		int j = rand() % 15;
//		const double fact(fact<double>(j));
//		v1.push_back(fact);
//		s1.insert(fact);
//		us1.insert(fact);
//		m1.insert(make_pair(j, fact));//if j already exists -> dont add
//		m1[j] = fact;//if j already exists -> OVERWRITE
//		
//		mm1.insert(make_pair(j, fact));//if j already exists -> ADD ADDITIONAL ENTRY
//
//	}
//
//	cerr << "\n\n****VECTOR:";
//	for(auto d : v1)
//		cerr << "\t" << d;
//	cerr << "\n\n****SET:";
//	for (auto d : s1)
//		cerr << "\t" << d;
//	cerr << "\n\n****UN-SET:";
//	for (auto d : us1)
//		cerr << "\t" << d;
//	cerr << "\n\n****MAP:";
//	for (auto d : m1)
//		cerr << "\t" << d.first << "-" << d.second;
//	cerr << "\n\n****MULTIMAP:";
//	for (auto d : mm1)
//		cerr << "\t" << d.first << "-" << d.second;
//
//	//system("pause"); - specific for windows*/
////	cerr << "\nLast One! ";
//	cin.get();//platform flexible
//}