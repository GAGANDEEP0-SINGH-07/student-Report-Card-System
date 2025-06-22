#ifndef MENU_H
#define MENU_H

#include <string>
#include <map>


void teacherMenu();
void studentMenu(const std::string& student_id);


extern std::map<std::string, std::string> teacher_credentials;
extern std::map<std::string, std::string> student_credentials;

#endif 