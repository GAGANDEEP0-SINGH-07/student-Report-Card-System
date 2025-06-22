#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <map>
#include <filesystem> 

// Namespace for filesystem operations
namespace fs = std::filesystem;

// Constants
extern const std::string BASE_DATA_DIR;
extern const std::vector<std::string> ALL_SUBJECTS;

// Student structure
struct student {
    std::string name;
    int roll_no;

    int maths = -1, science = -1, english = -1, hindi = -1, computer = -1;

    int class_no = 0;
    char section = ' ';

    int total() const;
    float percentage() const;
};

// Global map for students (declared extern, defined in student.cpp)
extern std::map<std::string, student> students;

// Function prototypes for data handling
fs::path getSubjectFilePath(int class_no, char section, const std::string& subject);
void saveAllStudentData();
void loadAllStudentData();

#endif 