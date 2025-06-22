#include "student.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem> 
#include <cctype>
#include <string>
#include <map>
#include <vector>


// Define global constants
const std::string BASE_DATA_DIR = "SchoolData";
const std::vector<std::string> ALL_SUBJECTS = {"Maths", "Science", "English", "Hindi", "Computer"};

// Define global students map
std::map<std::string, student> students;

int student::total() const {
    int sum = 0;
    if (maths != -1) sum += maths;
    if (science != -1) sum += science;
    if (english != -1) sum += english;
    if (hindi != -1) sum += hindi;
    if (computer != -1) sum += computer;
    return sum;
}

float student::percentage() const {
    int count = 0;
    if (maths != -1) count++;
    if (science != -1) count++;
    if (english != -1) count++;
    if (hindi != -1) count++;
    if (computer != -1) count++;

    return count > 0 ? (total() / (count * 1.0)) : 0.0;
}

fs::path getSubjectFilePath(int class_no, char section, const std::string& subject) {
    fs::path class_dir = fs::path(BASE_DATA_DIR) / ("Class_" + std::to_string(class_no));
    fs::path section_dir = class_dir / ("Section_" + std::string(1, section));
    return section_dir / (subject + ".txt");
}

void saveAllStudentData() {
    if (!fs::exists(BASE_DATA_DIR)) {
        fs::create_directories(BASE_DATA_DIR);
    }

    std::map<std::pair<int, char>, std::map<std::string, student>> grouped_students;
    for (const auto& pair : students) {
        if (pair.second.class_no != 0 && pair.second.section != ' ') {
            grouped_students[{pair.second.class_no, pair.second.section}][pair.first] = pair.second;
        }
    }

    for (const auto& group_pair : grouped_students) {
        int class_num = group_pair.first.first;
        char section_char = group_pair.first.second;
        const auto& students_in_group = group_pair.second;

        fs::path class_dir = fs::path(BASE_DATA_DIR) / ("Class_" + std::to_string(class_num));
        fs::path section_dir = class_dir / ("Section_" + std::string(1, section_char));
        fs::create_directories(section_dir);

        for (const std::string& subject : ALL_SUBJECTS) {
            fs::path filePath = getSubjectFilePath(class_num, section_char, subject);
            std::ofstream outFile(filePath, std::ios::trunc);

            if (outFile.is_open()) {
                outFile << "------------------------------------------------------------------------------------\n";
                outFile << "| " << std::left << std::setw(10) << "Student ID"
                        << " | " << std::left << std::setw(8) << "Roll No"
                        << " | " << std::left << std::setw(25) << "Name"
                        << " | " << std::left << std::setw(12) << (subject + " Marks") << " |\n";
                outFile << "------------------------------------------------------------------------------------\n";

                for (const auto& student_entry : students_in_group) {
                    const std::string& student_id = student_entry.first;
                    const student& s = student_entry.second;

                    int mark = -1;
                    if (subject == "Maths") mark = s.maths;
                    else if (subject == "Science") mark = s.science;
                    else if (subject == "English") mark = s.english;
                    else if (subject == "Hindi") mark = s.hindi;
                    else if (subject == "Computer") mark = s.computer;

                    outFile << "| " << std::left << std::setw(10) << student_id
                            << " | " << std::left << std::setw(8) << s.roll_no
                            << " | " << std::left << std::setw(25) << s.name
                            << " | " << std::left << std::setw(12) << (mark == -1 ? "N/A" : std::to_string(mark)) << " |\n";
                }
                outFile << "------------------------------------------------------------------------------------\n";
                outFile.close();
            } else {
                std::cerr << "Error: Unable to open file " << filePath << " for writing.\n";
            }
        }
    }
}

void loadAllStudentData() {
    students.clear();

    if (!fs::exists(BASE_DATA_DIR) || !fs::is_directory(BASE_DATA_DIR)) {
        return;
    }

    for (const auto& class_entry : fs::directory_iterator(BASE_DATA_DIR)) {
        if (class_entry.is_directory() && class_entry.path().filename().string().rfind("Class_", 0) == 0) {
            int class_num = 0;
            try {
                std::string class_name_str = class_entry.path().filename().string();
                class_num = std::stoi(class_name_str.substr(6));
            } catch (const std::exception& e) {
                std::cerr << "Warning: Could not parse class number from " << class_entry.path() << ": " << e.what() << "\n";
                continue;
            }

            for (const auto& section_entry : fs::directory_iterator(class_entry.path())) {
                if (section_entry.is_directory() && section_entry.path().filename().string().rfind("Section_", 0) == 0) {
                    char section_char = ' ';
                    try {
                        std::string section_name_str = section_entry.path().filename().string();
                        section_char = section_name_str[8];
                    } catch (const std::exception& e) {
                        std::cerr << "Warning: Could not parse section from " << section_entry.path() << ": " << e.what() << "\n";
                        continue;
                    }

                    for (const auto& subject_file_entry : fs::directory_iterator(section_entry.path())) {
                        if (subject_file_entry.is_regular_file() && subject_file_entry.path().extension() == ".txt") {
                            std::string subject_name = subject_file_entry.path().stem().string();

                            bool recognized_subject = false;
                            for (const std::string& s_name : ALL_SUBJECTS) {
                                if (s_name == subject_name) {
                                    recognized_subject = true;
                                    break;
                                }
                            }
                            if (!recognized_subject) continue;

                            std::ifstream inFile(subject_file_entry.path());
                            if (inFile.is_open()) {
                                std::string line;
                                std::getline(inFile, line);
                                std::getline(inFile, line);
                                std::getline(inFile, line);

                                while (std::getline(inFile, line)) {
                                    if (line.find("---") != std::string::npos) continue;

                                    std::string student_id_str = line.substr(2, 10);
                                    std::string roll_no_str = line.substr(15, 8);
                                    std::string name_str = line.substr(26, 25);
                                    std::string mark_str = line.substr(54, 12);

                                    student_id_str.erase(student_id_str.find_last_not_of(" \t") + 1);
                                    roll_no_str.erase(roll_no_str.find_last_not_of(" \t") + 1);
                                    name_str.erase(name_str.find_last_not_of(" \t") + 1);
                                    mark_str.erase(mark_str.find_last_not_of(" \t") + 1);

                                    int roll_no = 0;
                                    try {
                                        roll_no = std::stoi(roll_no_str);
                                    } catch (const std::exception& e) {
                                        std::cerr << "Warning: Could not parse roll number '" << roll_no_str << "' in " << subject_file_entry.path() << ": " << e.what() << "\n";
                                        continue;
                                    }

                                    int mark = -1;
                                    if (mark_str != "N/A") {
                                        try {
                                            mark = std::stoi(mark_str);
                                        } catch (const std::exception& e) {
                                            std::cerr << "Warning: Could not parse mark '" << mark_str << "' in " << subject_file_entry.path() << ": " << e.what() << "\n";
                                            continue;
                                        }
                                    }

                                    student& s_ref = students[student_id_str];

                                    if (s_ref.name.empty() || s_ref.name != name_str) s_ref.name = name_str;
                                    if (s_ref.roll_no == 0 || s_ref.roll_no != roll_no) s_ref.roll_no = roll_no;
                                    if (s_ref.class_no == 0 || s_ref.class_no != class_num) s_ref.class_no = class_num;
                                    if (s_ref.section == ' ' || s_ref.section != section_char) s_ref.section = section_char;

                                    if (subject_name == "Maths") s_ref.maths = mark;
                                    else if (subject_name == "Science") s_ref.science = mark;
                                    else if (subject_name == "English") s_ref.english = mark;
                                    else if (subject_name == "Hindi") s_ref.hindi = mark;
                                    else if (subject_name == "Computer") s_ref.computer = mark;
                                }
                                inFile.close();
                            } else {
                                std::cerr << "Error: Unable to open file " << subject_file_entry.path() << " for reading.\n";
                            }
                        }
                    }
                }
            }
        }
    }
}