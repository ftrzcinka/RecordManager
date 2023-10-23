#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class MyStudent {
public:
	MyStudent();
	MyStudent(const int id, const std::string& name, const unsigned short age);
	
	int getID() const;
	std::string getName() const;
	unsigned short getAge() const;
	
	~MyStudent();

private:
	int student_ID;
	std::string studentName;
	unsigned short studentAge;

};

class MyParser {
public:
	MyParser();
	MyParser(const std::vector<char>& seperators);

	std::vector<std::string> operator()(std::string& input);

	~MyParser();

private:
	std::vector<char> parsed_Seperators;
	std::vector<std::string> userInput;
};

class MyFileManager {
public:
	MyFileManager();

	std::vector<MyStudent*> fileToVector(const std::string& fileName);

	void copyVectorToFile(const std::string& fileName, std::vector<MyStudent*>& database);

	~MyFileManager();
};

class MyManager {
public:
	MyManager();
	
	void addStudentsFromFile(const std::string& fileName);

	bool isOnlyNumbers(const std::string& input) const;
	
	void addStudent(const std::vector<std::string>& user_Input);
	
	void removeStudentsByName(const std::string& name);
	
	void removeStudentsByID(const int id);
	void removeStudentsByIDGreater(const int id);
	void removeStudentsByIDLesser(const int id);
	
	void removeStudentsByAge(const unsigned short age);
	void removeStudentsByAgeGreater(const unsigned short age);
	void removeStudentsByAgeLesser(const unsigned short age);
	
	void printStudentInfo(MyStudent*& student) const;

	void findStudentsByName(const std::string& name);

	void findStudentsByID(const int id);
	void findStudentsByIDGreater(const int id);
	void findStudentsByIDLesser(const int id);
	
	void findStudentsByAge(const unsigned short age);
	void findStudentsByAgeGreater(const unsigned short age);
	void findStudentsByAgeLesser(const unsigned short age);

	bool userInputFindID(const std::vector<std::string>& user_Input);
	bool userInputFindAGE(const std::vector<std::string>& user_Input);
	bool userInputRemoveID(const std::vector<std::string>& user_Input);
	bool userInputRemoveAGE(const std::vector<std::string>& user_Input);

	bool process(const std::vector<std::string>& user_Input);

	~MyManager();

private:
	std::vector<MyStudent*> studentDataBase;
	MyFileManager fileManager;
	bool fileOpened;
};