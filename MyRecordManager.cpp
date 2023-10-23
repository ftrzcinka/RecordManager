#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "MyRecordManager.h"

//MyStudent Functions

MyStudent::MyStudent() {}

MyStudent::MyStudent(const int id, const std::string& name, const unsigned short age) {
	student_ID = id;
	studentName = name;
	studentAge = age;
}

int MyStudent::getID() const {
	return student_ID;
}

std::string MyStudent::getName() const {
	return studentName;
}

unsigned short MyStudent::getAge() const {
	return studentAge;
}

MyStudent::~MyStudent() {};



//MyParser Functions

MyParser::MyParser() {
	parsed_Seperators.push_back(' ');
	parsed_Seperators.push_back('\t');
}

MyParser::MyParser(const std::vector<char>& seperators) {
	parsed_Seperators = seperators;
}

std::vector<std::string> MyParser::operator()(std::string& input) {
	std::string tempString = "";
	std::string operatorString = "";
	bool foundLetter = false;
	bool isSeperator = false;
	bool foundQuotes = false;

	userInput.clear();

	for (int i = 0; i < input.length(); i++) {
		isSeperator = false;

		for (int j = 0; j < parsed_Seperators.size(); j++) {
			if (input[i] == parsed_Seperators[j]) {
				isSeperator = true;
				break;
			}
		}

		if (input[i] == '"' && foundQuotes) {
			userInput.push_back(tempString);
			tempString = "";
			foundLetter = false;
			foundQuotes = false;
		}
		else if (input[i] == '"' && !foundQuotes)
			foundQuotes = true;

		if (foundQuotes) {
			if(input[i] != '"')
				tempString += input[i];
		}
		else if (!isSeperator) {
			if (input[i] == '=' || input[i] == '>' || input[i] == '<') {
				if(tempString != "")
					userInput.push_back(tempString);

				tempString = input[i];
				userInput.push_back(tempString);
				tempString = "";
				foundLetter = false;
			}
			else {
				if (input[i] != '"') {
					tempString += input[i];
					foundLetter = true;
				}
			}
		}
		else {
			if (foundLetter && tempString != "") {
				userInput.push_back(tempString);
				tempString = "";
				foundLetter = false;
			}
		}
	}
	
	if(!isSeperator && input[input.size() - 1] != '"')
		userInput.push_back(tempString);

	return userInput;
}

MyParser::~MyParser() {}



//MyFileManager Functions

MyFileManager::MyFileManager(){}

std::vector<MyStudent*> MyFileManager::fileToVector(const std::string& fileName) {
	
	std::ifstream file(fileName);

	if (file.fail()) {
		std::vector<MyStudent*> empty;
		return empty;
	}

	std::string item;
	int id;
	std::string str;
	unsigned short age;

	std::vector<MyStudent*> tempDataBase;
	std::vector<std::string> parsedString;

	bool foundLetter = false;


	while (std::getline(file, item)) {

		for (int i = 0; i < item.length(); i++) {
			if (item[i] != '\t') {
				str += item[i];
				foundLetter = true;
			}
			else {
				if (foundLetter) {
					parsedString.push_back(str);
					foundLetter = false;
					str = "";
				}
			}
		}

		parsedString.push_back(str);

		id = std::stoi(parsedString[0]);
		str = parsedString[1];
		age = (unsigned short) std::stoi(parsedString[2]);

		tempDataBase.push_back(new MyStudent(id, str, age));

		parsedString.clear();
		str = "";
	}

	file.close();

	return tempDataBase;
}

void MyFileManager::copyVectorToFile(const std::string& fileName, std::vector<MyStudent*>& database) {
	std::ofstream outputFile;
	outputFile.open(fileName, std::ofstream::out | std::ofstream::trunc); //clears file

	if (outputFile.is_open()) {
		for (int i = 0; i < database.size(); i++) {
			outputFile << database[i]->getID() << "\t";
			outputFile << database[i]->getName() << "\t";
			outputFile << database[i]->getAge() << std::endl;
		}

		outputFile.close();
	}
	else {
		std::cerr << "File cannot be opened for writing." << std::endl;
		exit(1);
	}
}

MyFileManager::~MyFileManager(){}



//MyManager Functions

MyManager::MyManager() {
	fileOpened = false;
}

bool MyManager::isOnlyNumbers(const std::string& input) const {
	for (int i = 0; i < input.size(); i++) 
		if(!std::isdigit(input[i]))
			return false;
	
	return true;
}

void MyManager::addStudentsFromFile(const std::string& fileName) {
	studentDataBase = fileManager.fileToVector(fileName);
}

void MyManager::addStudent(const std::vector<std::string>& user_Input) {

	if (!isOnlyNumbers(user_Input[1]) || !isOnlyNumbers(user_Input[3])) {
		std::cout << "Improper Command" << std::endl;
		return;
	}

	int id = std::stoi(user_Input[1]);
	unsigned short age = (unsigned short) std::stoi(user_Input[3]);

	for (int i = 0; i < studentDataBase.size(); i++) {
		if (id == studentDataBase[i]->getID()) {
			std::cout << "ID Already In Use" << std::endl;
			return;
		}
	}

	MyStudent* newStudent = new MyStudent(id, user_Input[2], age);
	studentDataBase.push_back(newStudent);
}

void MyManager::removeStudentsByName(const std::string& name) {
	std::vector<MyStudent*> tempVector;

	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getName() != name)
			tempVector.push_back(studentDataBase[i]);
	}

	studentDataBase = tempVector;
}

void MyManager::removeStudentsByID(const int id) {
	std::vector<MyStudent*> tempVector;

	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getID() != id)
			tempVector.push_back(studentDataBase[i]);
	}

	studentDataBase = tempVector;
}

void MyManager::removeStudentsByIDGreater(const int id) {
	std::vector<MyStudent*> tempVector;

	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getID() <= id)
			tempVector.push_back(studentDataBase[i]);
	}

	studentDataBase = tempVector;
}

void MyManager::removeStudentsByIDLesser(const int id) {
	std::vector<MyStudent*> tempVector;

	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getID() >= id)
			tempVector.push_back(studentDataBase[i]);
	}

	studentDataBase = tempVector;
}

void MyManager::removeStudentsByAge(const unsigned short age) {
	std::vector<MyStudent*> tempVector;
	
	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getAge() != age)
			tempVector.push_back(studentDataBase[i]);
	}

	studentDataBase = tempVector;
}

void MyManager::removeStudentsByAgeGreater(const unsigned short age) {
	std::vector<MyStudent*> tempVector;

	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getAge() <= age)
			tempVector.push_back(studentDataBase[i]);
	}

	studentDataBase = tempVector;
}

void MyManager::removeStudentsByAgeLesser(const unsigned short age) {
	std::vector<MyStudent*> tempVector;

	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getAge() >= age)
			tempVector.push_back(studentDataBase[i]);
	}

	studentDataBase = tempVector;
}

void MyManager::printStudentInfo(MyStudent*& student) const {
	std::cout << student->getID() << "\t" << student->getName() << "\t" << student->getAge() << std::endl;
}

void MyManager::findStudentsByName(const std::string& name) {
	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getName() == name) {
			printStudentInfo(studentDataBase[i]);
		}
	}
}

void MyManager::findStudentsByID(const int id) {
	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getID() == id) {
			printStudentInfo(studentDataBase[i]);
		}
	}
}

void MyManager::findStudentsByIDGreater(const int id) {
	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getID() > id) {
			printStudentInfo(studentDataBase[i]);
		}
	}
}

void MyManager::findStudentsByIDLesser(const int id) {
	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getID() < id) {
			printStudentInfo(studentDataBase[i]);
		}
	}
}

void MyManager::findStudentsByAge(const unsigned short age) {
	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getAge() == age) {
			printStudentInfo(studentDataBase[i]);
		}
	}
}

void MyManager::findStudentsByAgeGreater(const unsigned short age) {
	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getAge() > age) {
			printStudentInfo(studentDataBase[i]);
		}
	}
}

void MyManager::findStudentsByAgeLesser(const unsigned short age) {
	for (int i = 0; i < studentDataBase.size(); i++) {
		if (studentDataBase[i]->getAge() < age) {
			printStudentInfo(studentDataBase[i]);
		}
	}
}

bool MyManager::userInputFindID(const std::vector<std::string>& user_Input) {
	if (!isOnlyNumbers(user_Input[3])) {
		std::cout << "Improper Command" << std::endl;
		return false;
	}

	int id = std::stoi(user_Input[3]);
	if (user_Input[2] == "=") {
		findStudentsByID(id);
		return false;
	}
	else if (user_Input[2] == ">") {
		findStudentsByIDGreater(id);
		return false;
	}
	else if (user_Input[2] == "<") {
		findStudentsByIDLesser(id);
		return false;
	}
	else {
		std::cout << "Improper Command" << std::endl;
		return false;
	}
}

bool MyManager::userInputFindAGE(const std::vector<std::string>& user_Input) {
	if (!isOnlyNumbers(user_Input[3])) {
		std::cout << "Improper Command" << std::endl;
		return false;
	}

	unsigned short age = (unsigned short) std::stoi(user_Input[3], nullptr, 0);
	if (user_Input[2] == "=") {
		findStudentsByAge(age);
		return false;
	}
	else if (user_Input[2] == ">") {
		findStudentsByAgeGreater(age);
		return false;
	}
	else if (user_Input[2] == "<") {
		findStudentsByAgeLesser(age);
		return false;
	}
	else {
		std::cout << "Improper Command" << std::endl;
		return false;
	}
}

bool MyManager::userInputRemoveID(const std::vector<std::string>& user_Input) {
	if (!isOnlyNumbers(user_Input[3])) {
		std::cout << "Improper Command" << std::endl;
		return false;
	}

	int id = std::stoi(user_Input[3]);
	if (user_Input[2] == "=") {
		removeStudentsByID(id);
		return false;
	}
	else if (user_Input[2] == ">") {
		removeStudentsByIDGreater(id);
		return false;
	}
	else if (user_Input[2] == "<") {
		removeStudentsByIDLesser(id);
		return false;
	}
	else {
		std::cout << "Improper Command" << std::endl;
		return false;
	}
}

bool MyManager::userInputRemoveAGE(const std::vector<std::string>& user_Input) {
	if (!isOnlyNumbers(user_Input[3])) {
		std::cout << "Improper Command" << std::endl;
		return false;
	}

	unsigned short age = (unsigned short) std::stoi(user_Input[3]);
	if (user_Input[2] == "=") {
		removeStudentsByAge(age);
		return false;
	}
	else if (user_Input[2] == ">") {
		removeStudentsByAgeGreater(age);
		return false;
	}
	else if (user_Input[2] == "<") {
		removeStudentsByAgeLesser(age);
		return false;
	}
	else {
		std::cout << "Improper Command" << std::endl;
		return false;
	}
}

bool MyManager::process(const std::vector<std::string>& user_Input) {

	if (user_Input.size() >= 5) {
		std::cout << "Improper Command" << std::endl;
		return false;
	}

	if (!fileOpened) {
		addStudentsFromFile("dataBase.txt");
		fileOpened = true;
	}

	if (user_Input[0] == "STOP") {
		fileManager.copyVectorToFile("dataBase.txt", studentDataBase);
		return true;
	}
	else if (user_Input[0] == "ADD") {
		if (user_Input.size() < 4) {
			std::cout << "Improper Command" << std::endl;
			return false;
		}

		addStudent(user_Input);
		return false;
	}
	else if (user_Input[0] == "FIND") {
		if (user_Input.size() < 4) {
			std::cout << "Improper Command" << std::endl;
			return false;
		}

		if (user_Input[1] == "name") {
			findStudentsByName(user_Input[3]);
			return false;

		}
		else if (user_Input[1] == "id") {

			return userInputFindID(user_Input);
		}
		else if (user_Input[1] == "age") {

			return userInputFindAGE(user_Input);
		}
		else {
			std::cout << "Improper Command" << std::endl;
			return false;
		}

	}
	else if (user_Input[0] == "REMOVE") {
		if (user_Input.size() < 4) {
			std::cout << "Improper Command" << std::endl;
			return false;
		}

		if (user_Input[1] == "name") {
			removeStudentsByName(user_Input[3]);
			return false;
		}
		else if (user_Input[1] == "id") {

			return userInputRemoveID(user_Input);
		}
		else if (user_Input[1] == "age") {

			return userInputRemoveAGE(user_Input);
		}
		else {
			std::cout << "Improper Command" << std::endl;
			return false;
		}
	}
	else {
		std::cout << "Improper Command" << std::endl;
		return false;
	}
}

MyManager::~MyManager() {
	for (int i = 0; i < studentDataBase.size(); i++) {
		delete studentDataBase[i];
		studentDataBase[i] = nullptr;
	}
}

int main()
{
	MyManager  engine;

	std::vector<char>  separators{ ' ', '\t' };
	MyParser  parser{ separators }; // its ok to not have a parametrized constructor and have a fixed set of separators inside a class
	std::string  userInput;
	bool  shouldStop{ false };

	while (!shouldStop)
	{
		getline(std::cin, userInput); // pay attention, pure cin doesn't allow spaces or tabs in userInput
		shouldStop = engine.process(parser(userInput));
	}

	return 0;
}