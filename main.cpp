#include <iostream>
#include "student.h" 
#include "menu.h"    

int main() {
    loadAllStudentData();

    while (true) {
        std::cout << "\n--- Main Menu ---\n";
        std::cout << "1. Teacher\n2. Student\n3. Exit\nEnter choice: ";
        int userType;
        std::cin >> userType;

        if (userType == 1) {
            std::string teacher_id, password;
            std::cout << "Enter User ID: ";
            std::cin >> teacher_id;
            std::cout << "Enter Password: ";
            std::cin >> password;

            if (teacher_credentials.count(teacher_id) && teacher_credentials[teacher_id] == password) {
                std::cout << "Login successful (Teacher)\n";
                teacherMenu();
            } else {
                std::cout << "Invalid Teacher credentials! Please try again.\n";
            }
        } else if (userType == 2) {
            std::string student_id_input, password;
            std::cout << "Enter User ID (e.g., S101): ";
            std::cin >> student_id_input;
            std::cout << "Enter Password: ";
            std::cin >> password;

            if (student_credentials.count(student_id_input) && student_credentials[student_id_input] == password) {
                std::cout << "Login successful (Student)\n";
                studentMenu(student_id_input);
            } else {
                std::cout << "Invalid Student credentials! Please try again.\n";
            }
        } else if (userType == 3) {
            std::cout << "Exiting system. Goodbye!\n";
            break;
        } else {
            std::cout << "Invalid choice! Please enter 1, 2, or 3.\n";
        }
    }

    return 0;
}