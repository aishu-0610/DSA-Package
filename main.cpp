#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

const int MAX_COURSES = 500;

struct Course {
    char code[10];
    char name[100];
    char prereqCodes[10][10];
    char prereqNames[10][100];
    int prereqCount;
    int semester;
};

class CourseGraph {
    Course courseList[MAX_COURSES];
    int courseCount;

public:
    CourseGraph() {
        courseCount = 0;
    }

    void addCourse(const Course& c) {
        courseList[courseCount++] = c;
    }

    void loadCoursesFromFile(const char* filename, int semester) {
        ifstream file(filename);
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            Course c;
            stringstream ss(line);
            string code, name, prereqCodeStr, prereqNameStr;

            getline(ss, code, '|');
            getline(ss, name, '|');
            getline(ss, prereqCodeStr, '|');
            getline(ss, prereqNameStr);

            strncpy(c.code, code.c_str(), sizeof(c.code));
            strncpy(c.name, name.c_str(), sizeof(c.name));
            c.semester = semester;

            c.prereqCount = 0;
            if (prereqCodeStr != "NA") {
                size_t pos = 0, lastPos = 0;
                size_t posName = 0, lastPosName = 0;
                int i = 0;

                while ((pos = prereqCodeStr.find(',', lastPos)) != string::npos &&
                       (posName = prereqNameStr.find(',', lastPosName)) != string::npos) {
                    string pc = prereqCodeStr.substr(lastPos, pos - lastPos);
                    string pn = prereqNameStr.substr(lastPosName, posName - lastPosName);
                    strncpy(c.prereqCodes[i], pc.c_str(), sizeof(c.prereqCodes[i]));
                    strncpy(c.prereqNames[i], pn.c_str(), sizeof(c.prereqNames[i]));
                    lastPos = pos + 1;
                    lastPosName = posName + 1;
                    i++;
                }

                if (lastPos < prereqCodeStr.length() && lastPosName < prereqNameStr.length()) {
                    strncpy(c.prereqCodes[i], prereqCodeStr.substr(lastPos).c_str(), sizeof(c.prereqCodes[i]));
                    strncpy(c.prereqNames[i], prereqNameStr.substr(lastPosName).c_str(), sizeof(c.prereqNames[i]));
                    i++;
                }

                c.prereqCount = i;
            }

            addCourse(c);
        }

        file.close();
    }

    void displaySemesterCourses(int semester) {
        cout << "\nCourses in Semester " << semester << ":\n";
        for (int i = 0; i < courseCount; ++i) {
            if (courseList[i].semester == semester) {
                cout << courseList[i].code << " - " << courseList[i].name << endl;
            }
        }
    }

   void displayElectives(const char* prefix) {
    for (int i = 0; i < courseCount; ++i) {
        if (strncmp(courseList[i].code, prefix, strlen(prefix)) == 0 &&
            strcmp(courseList[i].code, "23XTE_") != 0 &&
            strcmp(courseList[i].code, "23XTO_") != 0) {
            cout << courseList[i].code << " - " << courseList[i].name << endl;
        }
    }
}

    int findCourseIndex(const char* code) {
    for (int i = 0; i < courseCount; ++i) {
        if (strcmp(courseList[i].code, code) == 0) {
            return i;
        }
    }
    return -1;
}

    void printPrerequisites(const char* code, int level = 0) {
    if (strcmp(code, "NA") == 0) return;

    int index = findCourseIndex(code);
    if (index == -1) {
        cout << string(level * 4, ' ') << "Course not found: " << code << endl;
        return;    }

    const Course& course = courseList[index];
    cout << string(level * 4, ' ') << course.code << " - " << course.name << endl;

    for (int i = 0; i < course.prereqCount; i++) {
        if (strcmp(course.prereqCodes[i], "NA") == 0) {
            cout << string((level + 1) * 4, ' ') << course.prereqNames[i] << endl;
        } else {
            printPrerequisites(course.prereqCodes[i], level + 1);
        }
    }
}
};

int main() {
    CourseGraph graph;

    graph.loadCoursesFromFile("sem1.txt", 1);
    graph.loadCoursesFromFile("sem2.txt", 2);
    graph.loadCoursesFromFile("sem3.txt", 3);
    graph.loadCoursesFromFile("sem4.txt", 4);
    graph.loadCoursesFromFile("sem5.txt", 5);
    graph.loadCoursesFromFile("sem6.txt", 6);
    graph.loadCoursesFromFile("sem8.txt", 8);
    graph.loadCoursesFromFile("sem9.txt", 9);
    graph.loadCoursesFromFile("professional_electives.txt", 0);
    graph.loadCoursesFromFile("open_electives.txt", 0);

    int semester;
    cout << "Enter semester number (1-10): ";
    cin >> semester;

    if (semester < 1 || semester > 10) return 0;

    graph.displaySemesterCourses(semester);

    if (semester == 7 || semester == 10) return 0;

    char code[10];
    cout << "Enter course code to print prerequisites (or 'E' for electives): ";
    cin >> code;

    if (strcmp(code, "E") == 0) {
        int choice;
        cout << "\n1. Professional Electives\n2. Open Electives\nEnter choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "\nProfessional Electives:\n";
            graph.displayElectives("23XTE");
        } else if (choice == 2) {
            cout << "\nOpen Electives:\n";
            graph.displayElectives("23XTO");
        } else {
            return 0;
        }

        cout << "\nEnter elective course code to see prerequisites: ";
        cin >> code;
    }

    cout << "\nPrerequisite Graph:\n";
    graph.printPrerequisites(code);

    return 0;
}





