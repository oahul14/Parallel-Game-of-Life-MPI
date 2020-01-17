#include <iostream>		// input/output standard library
#include <string>			// standard string library
#include <vector>
#include <map>
#include "Student.h"

using namespace std;
using namespace spider;
//enum Color { RED, BLUE, GREEN, BLACK };
//
//struct Satellite
//{
//	//by default everything is public
//};
//
////create our own type
//class Earth
//{
//	//by default everything is private
//public:
//	//Constructor
//	Earth() : age_(0)
//	{
//		//age_ = 0;
//		cerr << "\nEarth:: Constructor!";
//	}
//	//Destructor
//	~Earth() 
//	{
//		cerr << "\nEarth:: Destructor!";
//	}
//	//accessor
//	const double& GetAge() const //without making a copy
//	{
//		return age_;
//	}
//	 double GetAge_MakeCopy() const // making a copy
//	{
//		return age_;
//	}
//	//accessor+mutator
//	double& Age()//without making a copy
//	{
//		return age_;
//	}
//	//mutator
//	void SetAge(const double& age)
//	{
//		age_ = age;
//	}
//
//private:
//	double age_;
//};
//
//int main()
//{
//	cerr << "\nHello Earth!";
//	{
//		Earth our_earth_;
//		our_earth_=Earth();
//		cerr << "\nThe age is: " << our_earth_.GetAge();
//		our_earth_.SetAge(4.5);
//		cerr << "\nThe age is: " << our_earth_.GetAge();
//
//		our_earth_.Age() = 10;
//		cerr << "\nThe age is: " << our_earth_.Age();
//	} //nested scope
//
//	system("pause");
//};
/*namespace otherLibrary {

	class Student {};

}*/

//spider::Student();
//otherLibrary::Student();

//void ChangeStudentName_To_PeterParker(Student& s)
void ChangeStudentName_To_PeterParker(Student s) //removed the &
{
	s.SetName("Peter Parker");
}

void VerifyStudentName_Is_PeterParker(const Student& s)
{
	if (s.GetName() == "Peter Parker")
		cerr << "\nTake a picture!";
}
int main()
{
	Student *student = new Student;
	//available and defined in the base class
	student->Random();
	student->Print(); //overwriting the base class
	delete student;

	/*HumanBeing *hb_student = new HumanBeing;
	hb_student->Age(); //available and defined in the base class
	hb_student->Print();
	delete hb_student;

	HumanBeing *new_student = new HumanBeing;
	new_student = new Student; //polymorphism
	new_student->Print();
	delete new_student;*/

	//HumanBeing *new_student = new HumanBeing;
	//dynamic_cast<Student*>(new_student)->Print();


	system("pause");
	//	Student s1;
			//cohort.push_back(s1);
		//}
		//cerr << "\nNow delete vector!";
	//}
	//why is the destructor called twice?
	Student::AvailableDepartments();
	{
		//std::vector<Student>  cohort(5);
		std::map<string, Student>  cohort;
		for (int i = 0; i < 3; i++)
		{
			Student s;
			s.Random();
			cohort[s.GetName()] = s;
		}
		/*for (int i = 0; i < cohort.size(); i++)
		{
			//Student s1;
			cohort[i].Random();
			if (i > 0)
				cohort[i] = cohort[i] + cohort[i - 1];
			//cohort[i] = s1;
			//cohort.push_back(s1);
		}
		ChangeStudentName_To_PeterParker(cohort[0]);*/
		cerr << "\nNow printing:";
		for (auto& s : cohort)
		{
			VerifyStudentName_Is_PeterParker(s.second);
			s.second.Print();
		}
		cerr << "\nNow delete vector!";
	} //internal representations may copy/destruct the objects more often than we think

	//why destructing so many times? what is happening???
	//the copy constructor is being called
	cin.get();
	return 0;
};