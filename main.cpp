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
    string extraInfo;
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
            string code, name, prereqCodeStr, extraInfo;

            getline(ss, code, '|');
            getline(ss, name, '|');
            getline(ss, prereqCodeStr, '|');
            getline(ss, extraInfo);

            strncpy(c.code, code.c_str(), CODE_LEN);
            strncpy(c.name, name.c_str(), NAME_LEN);
            c.semester = semester;
            c.extraInfo = extraInfo;

            int indexBeforeAdd = courseCount;
            addCourse(c);

            int courseIdx = indexBeforeAdd;
            size_t lastPos = 0, pos;
            while ((pos = prereqCodeStr.find(',', lastPos)) != string::npos) {
                string prereqCode = prereqCodeStr.substr(lastPos, pos - lastPos);
                if (prereqCode != "NA") {
                    int prereqIdx = findCourseIndex(prereqCode.c_str());
                    if (prereqIdx != -1) {
                        addPrerequisite(courseIdx, prereqIdx);
                    }
                }
                lastPos = pos + 1;
            }
            if (lastPos < prereqCodeStr.length()) {
                string prereqCode = prereqCodeStr.substr(lastPos);
                if (prereqCode != "NA") {
                    int prereqIdx = findCourseIndex(prereqCode.c_str());
                    if (prereqIdx != -1) {
                        addPrerequisite(courseIdx, prereqIdx);
                    }
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
            if (strncmp(courseList[i].code, "23XTE_", 6) == 0 || strncmp(courseList[>
                continue;
        }
        if (strncmp(courseList[i].code, prefix, strlen(prefix)) == 0) {
            cout << courseList[i].code << " - " << courseList[i].name << endl;
        }
    }
}


    void printPrerequisites(const char* code, int level = 0) {
        int index = findCourseIndex(code);
        if (index == -1) return;

        cout << string(level * 4, ' ') << "- " << courseList[index].code << " " << c>
        bool hasPrereq = false;
        for (Node* curr = adjList[index]; curr != nullptr; curr = curr->next) {
            hasPrereq = true;
            printPrerequisites(courseList[curr->courseIndex].code, level + 1);
        }
        if (!hasPrereq && !courseList[index].extraInfo.empty()) {
            stringstream ss(courseList[index].extraInfo);
            string item;
            while (getline(ss, item, ',')) {
                cout << string((level + 1) * 4, ' ') << "- " << item << endl;
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



    char code[CODE_LEN];
    cout << "Choose the type of courses for prerequisites:\n";
    cout << "1. Semester Courses\n2. Professional Electives\n3. Open Electives\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
             int semester;
             cout << "Enter semester number (1-10): ";
             cin >> semester;

            if (semester < 1 || semester > 10){
                cout<<"Invalid choice!\n";
                break;
            }

            graph.displaySemesterCourses(semester);

            if (semester == 7 || semester == 10){
                cout<<"Project work - Industry/Research project\n";
                break;
            }

            cout << "\nEnter course code for Semester Courses: ";
            cin >> code;
            cout << "\nPrerequisite Graph for Semester Course:\n";
            graph.printPrerequisites(code);
            break;
        case 2:
            cout << "\nProfessional Electives:\n";
            graph.displayElectives("23XTE");
            cout << "\nEnter elective course code to see prerequisites: ";
            cin >> code;
            cout << "\nPrerequisite Graph for Professional Elective:\n";
            graph.printPrerequisites(code);
            break;
        case 3:
            cout << "\nOpen Electives:\n";
            graph.displayElectives("23XTO");
            cout << "\nEnter elective course code to see prerequisites: ";
            cin >> code;
            cout << "\nPrerequisite Graph for Open Elective:\n";
            graph.printPrerequisites(code);
            break;
        default:
            cout << "Invalid choice!\n";
            return 0;
    }

    return 0;
}





