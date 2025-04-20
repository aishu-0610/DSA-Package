#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

const int MAX_COURSES = 500;
const int CODE_LEN = 10;
const int NAME_LEN = 100;

struct Course {
    char code[CODE_LEN];
    char name[NAME_LEN];
    int semester;
};

struct Node {
    int courseIndex;
    Node* next;
};

class CourseGraph {
    Course courseList[MAX_COURSES];
    Node* adjList[MAX_COURSES];
    int courseCount;

public:
    CourseGraph() {
        courseCount = 0;
        for (int i = 0; i < MAX_COURSES; i++) {
            adjList[i] = nullptr;
        }
    }

    ~CourseGraph() {
        for (int i = 0; i < MAX_COURSES; i++) {
            Node* curr = adjList[i];
            while (curr) {
                Node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
    }

    int findCourseIndex(const char* code) {
        for (int i = 0; i < courseCount; i++) {
            if (strcmp(courseList[i].code, code) == 0) {
                return i;
            }
        }
        return -1;
    }

    void addCourse(const Course& c) {
        courseList[courseCount++] = c;
    }

    void addPrerequisite(int courseIdx, int prereqIdx) {
        Node* newNode = new Node{prereqIdx, adjList[courseIdx]};
        adjList[courseIdx] = newNode;
    }

    void loadCoursesFromFile(const char* filename, int semester) {
        ifstream file(filename);
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            Course c;
            stringstream ss(line);
            string code, name, prereqCodeStr;

            getline(ss, code, '|');
            getline(ss, name, '|');
            getline(ss, prereqCodeStr, '|');

            strncpy(c.code, code.c_str(), CODE_LEN);
            strncpy(c.name, name.c_str(), NAME_LEN);
            c.semester = semester;

            int indexBeforeAdd = courseCount;
            addCourse(c);

            int courseIdx = indexBeforeAdd;
            if (prereqCodeStr != "NA") {
                size_t lastPos = 0, pos;
                while ((pos = prereqCodeStr.find(',', lastPos)) != string::npos) {
                    string prereqCode = prereqCodeStr.substr(lastPos, pos - lastPos);
                    int prereqIdx = findCourseIndex(prereqCode.c_str());
                    if (prereqIdx == -1) {
                        Course dummy;
                        strncpy(dummy.code, prereqCode.c_str(), CODE_LEN);
                        strncpy(dummy.name, "Unknown", NAME_LEN);
                        dummy.semester = 0;
                        prereqIdx = courseCount;
                        addCourse(dummy);
                    }
                    addPrerequisite(courseIdx, prereqIdx);
                    lastPos = pos + 1;
                }
                if (lastPos < prereqCodeStr.length()) {
                    string prereqCode = prereqCodeStr.substr(lastPos);
                    int prereqIdx = findCourseIndex(prereqCode.c_str());
                    if (prereqIdx == -1) {
                        Course dummy;
                        strncpy(dummy.code, prereqCode.c_str(), CODE_LEN);
                        strncpy(dummy.name, "Unknown", NAME_LEN);
                        dummy.semester = 0;
                        prereqIdx = courseCount;
                        addCourse(dummy);
                    }
                    addPrerequisite(courseIdx, prereqIdx);
                }
            }
        }
        file.close();
    }

    void displaySemesterCourses(int semester) {
        cout << "\nCourses in Semester " << semester << ":\n";
        for (int i = 0; i < courseCount; i++) {
            if (courseList[i].semester == semester) {
                cout << courseList[i].code << " - " << courseList[i].name << endl;
            }
        }
    }

    void displayElectives(const char* prefix) {
        for (int i = 0; i < courseCount; i++) {
            if (strncmp(courseList[i].code, prefix, strlen(prefix)) == 0 &&
                strcmp(courseList[i].code, "23XTE_") != 0 &&
                strcmp(courseList[i].code, "23XTO_") != 0) {
                cout << courseList[i].code << " - " << courseList[i].name << endl;
            }
        }
    }

    void printPrerequisites(const char* code, int level = 0) {
        int index = findCourseIndex(code);
        if (index == -1) {
            cout << string(level * 4, ' ') << "Course not found: " << code << endl;
            return;
        }

        cout << string(level * 4, ' ') << courseList[index].code << " - " << courseList[index].name << endl;
        for (Node* curr = adjList[index]; curr != nullptr; curr = curr->next) {
            printPrerequisites(courseList[curr->courseIndex].code, level + 1);
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

    char code[CODE_LEN];
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




