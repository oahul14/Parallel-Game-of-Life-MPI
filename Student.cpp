#include "Student.h"

//Definition
namespace spider
{

string randomName(int len)
{
	const string a("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	string name;
	//pick a letter
	for (int i = 0; i < len; i++)
	{
		//create random number
		size_t r = (rand() % 62);
		//get the corresponding letter and add it to the name
		name.push_back(a.at(r));
	}
	return name;
}

Student::Student()//Default constructor
{
	static int counter(0);//

	std::cerr << "\nConstructor Student" << counter++;
	name = "NoName";
	student_id = 0;
	type_of_degree = Undergraduate;
	department = EarthScience;
}

Student::Student(const Student& s)//Copy Constructor
{
	static int counter(0);
	std::cerr << "\nCopy Constructor" << counter++;
	name = s.name;
	student_id = s.student_id;
	type_of_degree = s.type_of_degree;
	department = s.department;
}

void Student::AvailableDepartments()
{
	cout << "\tDepartments: ";
	cout << "EarthScience,";
	cout << "ComputerScience,";
	cout << "Mathematics";
}

Student::~Student()
{
	static int counter(0);
	std::cerr << "\nDestructor" << counter++;
}

void Student::Random()
{
	name = randomName(10);
	student_id = rand() % 100000;
	type_of_degree = DegreeType(rand() % 2);
	department = Department(rand() % 2);
}

void Student::Print()
{
	//first version
	//cout << "\nName: " << name;
	//cout << "\tID: " << student_id;
	//cout << "\tType of degree: " << type_of_degree;
	//cout << "\tDepartment: " << department;	
	//second version
	cout << "\n\nName: " << name;
	//cout << "\t" << float_that_we_forgot;
	cout << "\tID: " << student_id;
	cout << "\tType of degree: ";
	if (type_of_degree == Undergraduate) cout << "Undergraduate";
	if (type_of_degree == MSc) cout << "MSc";
	if (type_of_degree == PhD) cout << "PhD";

	cout << "\tDepartment: ";
	switch (department) //its a multiple selection if statement
	{
	case EarthScience:
		cout << "EarthScience";
		break;
	case ComputerScience:
		cout << "ComputerScience";
		break;
	case Mathematics:
		cout << "Mathematics";
		break;
	default:
		break;
	}
}

//define the operator 
Student Student::operator+ (const Student& other)
{
	Student new_student;

	//our data
	//name;//private
	//student_id;//private
	//type_of_degree;//private
	//department;//private
	//float_that_we_forgot;//private

	//other data
	//other.name;//private
	//other.student_id;//private
	//other.type_of_degree;//private
	//other.department;//private
	//other.float_that_we_forgot;//private

	//combine it
	new_student.name = name + other.name;
	//you want to be systematic
	new_student.student_id = student_id + other.student_id;
	//new_student.name = name + " added to " + other.name;
	//new_student.name = name + " added to " + other.name;
	//this might be confusing! and a recipe for disaster!
	//new_student.student_id = student_id;//private
	//new_student.type_of_degree = other.type_of_degree;//private


	//type_of_degree;//private
	//department;//private
	//float_that_we_forgot;//private
	return new_student;
}

//mutator - interfaces
void HumanBeing::SetName(std::string s) { name = s; }

//accessor - interfaces
std::string HumanBeing::GetName() const { return name; }


}