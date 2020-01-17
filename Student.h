#ifndef ACSE_STUDENT_H
#define ACSE_STUDENT_H

#include <iostream>		// input/output standard library
#include <string>			// standard string library
#include <vector>

using namespace std;

namespace spider
{
	//simple class
	enum DegreeType { Undergraduate = 10, MSc = 100, PhD = 1000 };
	enum Department { EarthScience, ComputerScience, Mathematics };

	typedef DegreeType ImperialDegree;

	//This function generates a completely random name of fixed length
	/*
	@Function randomName
	@Purpose aaa
	@Input bb
	@Output xx
	*/
	string randomName(int len);

	//Inheritance
	class HumanBeing
	{
	public:
		HumanBeing() // default constructor
		{
			std::cerr << "\nConstructor HumanBeing";
			name = "NONE"; //set default value
			age = 0;
		}

		//mutator - interfaces
		void SetName(std::string s);
		
		//accessor - interfaces
		std::string GetName() const;

		int Age() { return age; }
		
		///we will create the same functions in the base class
		virtual void Print() //declaring a pure virtual function 
		{
			cerr << "\n\n\nPRINTING HUMAN BEING aged " << age;
		}

		//virtual void Print() = 0; //declaring a pure virtual function 

	protected:
		std::string name;//is protected it is visible to the inherited class (Student)

	private:
		int age;
	};

	//Class Declaration
	class Student : public HumanBeing
	{
	public:
		Student();//Default constructor
		Student(const Student& s);//Copy Constructor
		static void AvailableDepartments();
		Student operator+ (const Student& other);//declared
		~Student();
		void Random();
		virtual void Print();
		
	private:
		//let's describe a student
		unsigned int student_id;
		DegreeType type_of_degree;
		//std::string degree;
		Department department;
		double float_that_we_forgot;

		//wont be copied correctly by the default (in built) copy constructor
		//int *grades;//somewhere in the code --> grades = new int[10];
	};


}

#endif
