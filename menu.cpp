#include "menu.h"
#include "student.h" 
#include <iostream>
#include <iomanip>
#include <cctype> 

// Define global credentials
std::map<std::string, std::string> teacher_credentials = {{"T101", "1234"}, {"T102", "5678"}};
std::map<std::string, std::string> student_credentials = {{"S101", "1234"}, {"S102", "5678"}, {"S103", "1234"}};

void teacherMenu() {
    std::string current_teacher_subject;
    int current_teacher_class_no;
    char current_teacher_section;

    std::cout << "Enter your subject (e.g., Maths, English): ";
    std::cin >> current_teacher_subject;
    std::cout << "Enter class number you teach: ";
    std::cin >> current_teacher_class_no;
    std::cout << "Enter section: ";
    std::cin >> current_teacher_section;
    current_teacher_section = std::toupper(current_teacher_section);

    bool subject_recognized = false;
    for (const std::string& subj : ALL_SUBJECTS) {
        if (subj == current_teacher_subject) {
            subject_recognized = true;
            break;
        }
    }
    if (!subject_recognized) {
        std::cout << "Error: Subject '" << current_teacher_subject << "' is not a recognized subject. Please choose from: ";
        for (const std::string& subj : ALL_SUBJECTS) {
            std::cout << subj << " ";
        }
        std::cout << "\nReturning to main menu.\n";
        return;
    }

    while (true) {
        std::cout << "\n--- Teacher Menu for " << current_teacher_subject << ", Class "
             << current_teacher_class_no << "-" << current_teacher_section << " ---\n";
        std::cout << "1. Enter/Update Marks\n2. View Marks\n3. Logout\nEnter choice: ";
        int option;
        std::cin >> option;

        if (option == 1) {
            char choice;
            do {
                std::string reg_id;
                std::cout << "\nEnter student registration ID (e.g., S101): ";
                std::cin >> reg_id;

                student s_temp;

                if (students.count(reg_id)) {
                    s_temp = students.at(reg_id);
                    std::cout << "Existing student found. Updating marks for " << s_temp.name << ".\n";

                    char update_info_choice;
                    std::cout << "Update student name and roll number? (y/n): ";
                    std::cin >> update_info_choice;
                    if (std::tolower(update_info_choice) == 'y') {
                        std::cout << "Enter new student name (current: " << s_temp.name << "): ";
                        std::cin.ignore();
                        std::getline(std::cin, s_temp.name);
                        std::cout << "Enter new roll number (current: " << s_temp.roll_no << "): ";
                        std::cin >> s_temp.roll_no;
                    }
                } else {
                    std::cout << "New student. Enter student name: ";
                    std::cin.ignore();
                    std::getline(std::cin, s_temp.name);

                    std::cout << "Enter roll number: ";
                    std::cin >> s_temp.roll_no;
                }

                s_temp.class_no = current_teacher_class_no;
                s_temp.section = current_teacher_section;

                int mark;
                std::cout << "Enter marks in " << current_teacher_subject << " (0-100): ";
                std::cin >> mark;

                if (mark < 0 || mark > 100) {
                    std::cout << "Invalid marks. Marks should be between 0 and 100. Setting to N/A (-1).\n";
                    mark = -1;
                }

                if (current_teacher_subject == "Maths") s_temp.maths = mark;
                else if (current_teacher_subject == "Science") s_temp.science = mark;
                else if (current_teacher_subject == "English") s_temp.english = mark;
                else if (current_teacher_subject == "Hindi") s_temp.hindi = mark;
                else if (current_teacher_subject == "Computer") s_temp.computer = mark;

                students[reg_id] = s_temp;

                saveAllStudentData();

                std::cout << "Marks updated. Add another student? (y/n): ";
                std::cin >> choice;
            } while (std::tolower(choice) == 'y');
            std::cout << "Mark entry complete. All data saved.\n";
        } else if (option == 2) {
            if (students.empty()) {
                std::cout << "No student records found in the system to display.\n";
                continue;
            }

            std::cout << "\n--- Displaying " << current_teacher_subject << " Marks for Class "
                 << current_teacher_class_no << "-" << current_teacher_section << " ---\n";

            std::cout << "------------------------------------------------------------------------------------\n";
            std::cout << "| " << std::left << std::setw(10) << "Student ID"
                 << " | " << std::left << std::setw(8) << "Roll No"
                 << " | " << std::left << std::setw(25) << "Name"
                 << " | " << std::left << std::setw(12) << (current_teacher_subject + " Marks") << " |\n";
            std::cout << "------------------------------------------------------------------------------------\n";

            bool found_students_in_view = false;
            for (const auto& pair : students) {
                const student& s = pair.second;

                if (s.class_no == current_teacher_class_no && s.section == current_teacher_section) {
                    found_students_in_view = true;
                    int mark_to_display = -1;

                    if (current_teacher_subject == "Maths") mark_to_display = s.maths;
                    else if (current_teacher_subject == "Science") mark_to_display = s.science;
                    else if (current_teacher_subject == "English") mark_to_display = s.english;
                    else if (current_teacher_subject == "Hindi") mark_to_display = s.hindi;
                    else if (current_teacher_subject == "Computer") mark_to_display = s.computer;

                    std::cout << "| " << std::left << std::setw(10) << pair.first
                         << " | " << std::left << std::setw(8) << s.roll_no
                         << " | " << std::left << std::setw(25) << s.name
                         << " | " << std::left << std::setw(12) << (mark_to_display == -1 ? "N/A" : std::to_string(mark_to_display)) << " |\n";
                }
            }
            std::cout << "------------------------------------------------------------------------------------\n";

            if (!found_students_in_view) {
                std::cout << "(No students found for Class " << current_teacher_class_no << "-"
                     << current_teacher_section << " with data for " << current_teacher_subject << ".)\n";
            }
        } else if (option == 3) {
            std::cout << "Logging out...\n";
            saveAllStudentData();
            break;
        } else {
            std::cout << "Invalid choice! Please try again.\n";
        }
    }
}

void studentMenu(const std::string& student_id) {
    if (students.count(student_id)) {
        const student& s = students.at(student_id);

        std::cout << "\n--- Your Report Card ---\n";
        std::cout << "Name: " << s.name << "\n";
        std::cout << "Class: " << s.class_no << "-" << s.section << "\n";
        std::cout << "Roll No: " << s.roll_no << "\n";
        std::cout << "--------------------------\n";
        std::cout << "Maths:    " << (s.maths == -1 ? "N/A" : std::to_string(s.maths)) << "\n";
        std::cout << "Science:  " << (s.science == -1 ? "N/A" : std::to_string(s.science)) << "\n";
        std::cout << "English:  " << (s.english == -1 ? "N/A" : std::to_string(s.english)) << "\n";
        std::cout << "Hindi:    " << (s.hindi == -1 ? "N/A" : std::to_string(s.hindi)) << "\n";
        std::cout << "Computer: " << (s.computer == -1 ? "N/A" : std::to_string(s.computer)) << "\n";
        std::cout << "--------------------------\n";

        int subjects_entered_count = 0;
        if (s.maths != -1) subjects_entered_count++;
        if (s.science != -1) subjects_entered_count++;
        if (s.english != -1) subjects_entered_count++;
        if (s.hindi != -1) subjects_entered_count++;
        if (s.computer != -1) subjects_entered_count++;

        int total_possible = subjects_entered_count * 100;

        std::cout << "TOTAL: " << s.total() << "/" << total_possible << "\n";
        std::cout << "PERCENTAGE: " << std::fixed << std::setprecision(2) << s.percentage() << "%\n";
    } else {
        std::cout << "Your marks are not available yet. Please contact your teacher.\n";
    }
}