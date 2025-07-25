#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;

// Common structures and functions
string dayNames[7] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

int getDayIndex(string inputDay) {
    transform(inputDay.begin(), inputDay.end(), inputDay.begin(), ::tolower);
    string days[7] = { "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday" };
    for (int i = 0; i < 7; ++i) {
        if (inputDay == days[i]) return i;
    }
    return -1;
}

// Array implementation
struct TaskArray {
    string description;
};

vector<TaskArray> weeklyTasksArray[7];

void initializeDefaultTasksArray() {
    for (int i = 0; i < 7; ++i) {
        weeklyTasksArray[i].clear();
    }
}

void addTaskArray(int dayIndex, const string& position, int customPosition = -1) {
    TaskArray newTask;
    cout << "Enter task description: ";
    cin.ignore();
    getline(cin, newTask.description);

    auto& tasks = weeklyTasksArray[dayIndex];
    int shifts = 0;
    size_t originalSize = tasks.size();

    auto start = chrono::high_resolution_clock::now();

    if (position == "front") {
        shifts = originalSize;
        vector<TaskArray> tempTasks = tasks;
        tempTasks.push_back(TaskArray{ "TEMP" });
        for (int i = originalSize - 1; i >= 0; --i) {
            tempTasks[i + 1] = tempTasks[i];
            cout << "Step " << (originalSize - i) << "\n";
            for (int j = 0; j <= originalSize; ++j)
                cout << "  " << j + 1 << ". " << tempTasks[j].description << "\n";
            cout << "\n";
        }
        tempTasks[0] = newTask;
        cout << "Step " << (shifts + 1) << "\n";
        for (int j = 0; j <= originalSize; ++j)
            cout << "  " << j + 1 << ". " << tempTasks[j].description << "\n";
        cout << "\n";
        tasks.insert(tasks.begin(), newTask);
        shifts += 1;
    }
    else if (position == "end") {
        shifts = 1;
        vector<TaskArray> tempTasks = tasks;
        tempTasks.push_back(newTask);

        cout << "Step 1\n";
        for (int j = 0; j <= originalSize; ++j)
            cout << "  " << j + 1 << ". " << tempTasks[j].description << "\n";
        cout << "\n";

        tasks.push_back(newTask);
    }
    else if (position == "position") {
        if (customPosition < 0) customPosition = 0;
        if (customPosition > originalSize) customPosition = originalSize;
        shifts = (originalSize - customPosition);

        vector<TaskArray> tempTasks = tasks;
        tempTasks.push_back(TaskArray{ "TEMP" });

        for (int i = originalSize - 1; i >= customPosition; --i) {
            tempTasks[i + 1] = tempTasks[i];
            cout << "Step " << (originalSize - i) << "\n";
            for (int j = 0; j <= originalSize; ++j)
                cout << "  " << j + 1 << ". " << tempTasks[j].description << "\n";
            cout << "\n";
        }

        tempTasks[customPosition] = newTask;
        cout << "Step " << (shifts + 1) << "\n";
        for (int j = 0; j <= originalSize; ++j)
            cout << "  " << j + 1 << ". " << tempTasks[j].description << "\n";
        cout << "\n";

        tasks.insert(tasks.begin() + customPosition, newTask);
        shifts += 1;
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration<double, milli>(end - start).count();

    cout << fixed << setprecision(4);
    cout << "Task added to " << dayNames[dayIndex] << " at "
        << (position == "position" ? "position " + to_string(customPosition + 1) : position) << ".\n";
    cout << "Steps (shifts + insert): " << shifts << "\n";
    cout << "Time taken (shifts + insert): " << duration << " milliseconds\n\n";
}

void displayTasksArray() {
    for (int i = 0; i < 7; ++i) {
        cout << dayNames[i] << ":\n";
        if (weeklyTasksArray[i].empty()) {
            cout << "  No tasks.\n";
        }
        else {
            for (int j = 0; j < weeklyTasksArray[i].size(); ++j) {
                cout << "  " << j + 1 << ". " << weeklyTasksArray[i][j].description << "\n";
            }
        }
        cout << "\n";
    }
}

void clearAllArray() {
    for (int i = 0; i < 7; ++i) weeklyTasksArray[i].clear();
    cout << "All tasks cleared for the week.\n\n";
}

void bulkInsertArray(int dayIndex, const string& position, int index = -1) {
    auto& tasks = weeklyTasksArray[dayIndex];

    if (position == "front" || position == "end") {
        cout << "\n=== Bulk Inserting 10 tasks at " << position << " ===\n";
        for (int i = 0; i < 10; ++i) {
            string desc = "Benchmark Task " + to_string(i + 1);
            if (position == "front") {
                tasks.insert(tasks.begin(), TaskArray{ desc });
            }
            else {
                tasks.push_back(TaskArray{ desc });
            }
        }
        cout << "Bulk insert at " << position << " completed.\n";
    }
    else if (position == "position") {
        int count = tasks.size();
        if (index <= 0 || index > count + 1) {
            cout << "Invalid position, inserting at front.\n";
            index = 1;
        }

        cout << "\n=== Bulk Inserting 10 tasks at position " << index << " ===\n";
        int insertPos = index - 1;

        for (int i = 0; i < 10; ++i) {
            string desc = "Benchmark Task " + to_string(i + 1);
            if (insertPos > tasks.size()) {
                tasks.push_back(TaskArray{ desc });
            }
            else {
                tasks.insert(tasks.begin() + insertPos, TaskArray{ desc });
            }
        }
        cout << "Bulk insert at position " << index << " completed.\n";
    }
    else cout << "Invalid position.\n";
}

// Linked List implementation
struct TaskLinkedList {
    string description;
    TaskLinkedList* next;
    TaskLinkedList(string desc) : description(desc), next(nullptr) {}
};

TaskLinkedList* weeklyTasksLinkedList[7] = { nullptr };

void initializeDefaultTasksLinkedList() {
    for (int i = 0; i < 7; ++i) {
        weeklyTasksLinkedList[i] = nullptr;
    }
}

void displayDayTasksLinkedList(int dayIndex) {
    cout << dayNames[dayIndex] << " current tasks:\n";
    TaskLinkedList* temp = weeklyTasksLinkedList[dayIndex];
    if (!temp) cout << "  No tasks.\n";
    else {
        int j = 1;
        while (temp) {
            cout << "  [" << j++ << "] " << temp->description << "\n";
            temp = temp->next;
        }
    }
    cout << endl;
}

void displayTasksLinkedList() {
    for (int i = 0; i < 7; ++i) displayDayTasksLinkedList(i);
}

void addTaskLinkedList(int dayIndex, const string& position, int customPosition = -1) {
    cout << "Enter task description: ";
    cin.ignore();
    string desc;
    getline(cin, desc);

    auto start = chrono::high_resolution_clock::now();

    TaskLinkedList* newTask = new TaskLinkedList(desc);
    int steps = 0;
    TaskLinkedList*& head = weeklyTasksLinkedList[dayIndex];

    if (position == "front" || !head) {
        steps = 1;
        newTask->next = head;
        head = newTask;
        cout << "\nStep 1 (Insertion at front):\n";
        displayDayTasksLinkedList(dayIndex);
    }
    else if (position == "end") {
        TaskLinkedList* temp = head;
        int stepCount = 1;
        while (temp->next) {
            cout << "\nStep " << stepCount++ << " (traverse): Current node = " << temp->description << "\n";
            temp = temp->next;
            steps++;
            displayDayTasksLinkedList(dayIndex);
        }
        temp->next = newTask;
        steps++;
        cout << "\nStep " << stepCount++ << " (insert at end):\n";
        displayDayTasksLinkedList(dayIndex);
    }
    else if (position == "position") {
        TaskLinkedList* temp = head;
        int count = 0;
        while (temp) { temp = temp->next; count++; }

        displayDayTasksLinkedList(dayIndex);
        cout << "You can insert at position from 1 (front) to " << count + 1 << " (end).\n";

        if (customPosition <= 0 || customPosition > count + 1) {
            cout << "Invalid position, inserting at front.\n";
            steps = 1;
            newTask->next = head;
            head = newTask;
            cout << "\nStep 1 (Insertion at front):\n";
            displayDayTasksLinkedList(dayIndex);
        }
        else {
            temp = head;
            int stepCount = 1;
            for (int index = 1; index < customPosition - 1 && temp->next; index++, steps++, stepCount++) {
                cout << "\nStep " << stepCount << " (traverse): Current node = " << temp->description << "\n";
                temp = temp->next;
                displayDayTasksLinkedList(dayIndex);
            }
            newTask->next = temp->next;
            temp->next = newTask;
            steps++;
            cout << "\nStep " << stepCount + 1 << " (insert at position " << customPosition << "):\n";
            displayDayTasksLinkedList(dayIndex);
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double duration = chrono::duration<double, milli>(end - start).count();

    cout << fixed << setprecision(4);
    cout << "\nTask added at " << (position == "position" ? "position " + to_string(customPosition) : position) << ".\n";
    cout << "Steps (traversals + insert): " << steps << "\n";
    cout << "Time taken (shifting + insert): " << duration << " milliseconds\n\n";
}

void clearAllLinkedList() {
    for (int i = 0; i < 7; ++i) {
        TaskLinkedList* temp = weeklyTasksLinkedList[i];
        while (temp) {
            TaskLinkedList* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
        weeklyTasksLinkedList[i] = nullptr;
    }
    cout << "All tasks cleared for the week.\n\n";
}

void bulkInsertLinkedList(int dayIndex, const string& position, int index = -1) {
    TaskLinkedList*& head = weeklyTasksLinkedList[dayIndex];

    if (position == "front" || position == "end") {
        cout << "\n=== Bulk Inserting 10 tasks at " << position << " ===\n";
        for (int i = 0; i < 10; ++i) {
            TaskLinkedList* newTask = new TaskLinkedList("Benchmark Task " + to_string(i + 1));
            if (position == "front") {
                newTask->next = head;
                head = newTask;
            }
            else {
                if (!head) head = newTask;
                else {
                    TaskLinkedList* temp = head;
                    while (temp->next) temp = temp->next;
                    temp->next = newTask;
                }
            }
        }
        cout << "Bulk insert at " << position << " completed.\n";
    }
    else if (position == "position") {
        TaskLinkedList* temp = head;
        int count = 0;
        while (temp) { temp = temp->next; count++; }

        if (index <= 0 || index > count + 1) {
            cout << "Invalid position, inserting at front.\n";
            index = 1;
        }

        cout << "\n=== Bulk Inserting 10 tasks at position " << index << " ===\n";
        for (int i = 0; i < 10; ++i) {
            TaskLinkedList* newTask = new TaskLinkedList("Benchmark Task " + to_string(i + 1));
            if (index == 1 || !head) {
                newTask->next = head;
                head = newTask;
            }
            else {
                TaskLinkedList* curr = head;
                for (int j = 1; j < index - 1 && curr->next; j++) curr = curr->next;
                newTask->next = curr->next;
                curr->next = newTask;
            }
        }
        cout << "Bulk insert at position " << index << " completed.\n";
    }
    else cout << "Invalid position.\n";
}

// Main menu with implementation switching
int main() {
    int implChoice = 0;

    while (true) {
        cout << "\nChoose implementation:\n";
        cout << "1. Array\n";
        cout << "2. Linked List\n";
        cout << "3. Exit Program\n";
        cout << "Select option: ";
        cin >> implChoice;

        if (implChoice == 3) {
            cout << "Exiting program. Goodbye!\n";
            break;
        }
        else if (implChoice == 1) {
            initializeDefaultTasksArray();
            bool returnToMain = false;

            while (!returnToMain) {
                cout << "\nTo-Do List Menu (Array):\n";
                cout << "1. Add Task\n";
                cout << "2. View All Tasks\n";
                cout << "3. Clear All Tasks\n";
                cout << "4. Bulk Insert Benchmarking\n";
                cout << "5. Return to Implementation Selection\n";
                cout << "Select option: ";
                int choice;
                cin >> choice;

                switch (choice) {
                case 1: {
                    string day;
                    cout << "Enter day (Monday to Sunday): ";
                    cin >> day;
                    int dayIndex = getDayIndex(day);
                    if (dayIndex == -1) {
                        cout << "Invalid day entered.\n";
                        continue;
                    }

                    string pos;
                    cout << "Where to add? (front / end / position): ";
                    cin >> pos;
                    transform(pos.begin(), pos.end(), pos.begin(), ::tolower);

                    if (pos == "front" || pos == "end") {
                        addTaskArray(dayIndex, pos);
                    }
                    else if (pos == "position") {
                        auto& tasks = weeklyTasksArray[dayIndex];
                        int taskCount = tasks.size();

                        cout << dayNames[dayIndex] << " current tasks:\n";
                        for (int i = 0; i < taskCount; ++i) {
                            cout << "  [" << i + 1 << "] " << tasks[i].description << "\n";
                        }
                        cout << "You can insert at position from 1 (front) to " << taskCount + 1 << " (end).\n";

                        int index;
                        do {
                            cout << "Enter position index (1 to " << taskCount + 1 << "): ";
                            cin >> index;
                            if (index < 1 || index > taskCount + 1) {
                                cout << "Invalid position. Please try again.\n";
                            }
                        } while (index < 1 || index > taskCount + 1);

                        addTaskArray(dayIndex, "position", index - 1);
                    }
                    else {
                        cout << "Invalid position entered.\n";
                    }
                    break;
                }
                case 2: {
                    displayTasksArray();
                    break;
                }
                case 3: {
                    clearAllArray();
                    break;
                }
                case 4: {
                    string day, pos;
                    cout << "Enter day (Monday to Sunday): ";
                    cin >> day;
                    int dayIndex = getDayIndex(day);
                    if (dayIndex == -1) {
                        cout << "Invalid day.\n";
                        continue;
                    }

                    cout << "Where to bulk insert? (front / end / position): ";
                    cin >> pos;
                    transform(pos.begin(), pos.end(), pos.begin(), ::tolower);

                    if (pos == "front" || pos == "end") {
                        bulkInsertArray(dayIndex, pos);
                    }
                    else if (pos == "position") {
                        auto& tasks = weeklyTasksArray[dayIndex];
                        int count = tasks.size();

                        cout << dayNames[dayIndex] << " current tasks:\n";
                        if (tasks.empty()) {
                            cout << "  No tasks.\n";
                        }
                        else {
                            for (int j = 0; j < tasks.size(); ++j) {
                                cout << "  [" << j + 1 << "] " << tasks[j].description << "\n";
                            }
                        }
                        cout << "You can insert at position from 1 (front) to " << count + 1 << " (end).\n";
                        cout << "Enter position: ";
                        int index; cin >> index;

                        bulkInsertArray(dayIndex, pos, index);
                    }
                    else cout << "Invalid position.\n";
                    break;
                }
                case 5: {
                    returnToMain = true;
                    break;
                }
                default: {
                    cout << "Invalid option.\n";
                    break;
                }
                }
            }
        }
        else if (implChoice == 2) {
            initializeDefaultTasksLinkedList();
            bool returnToMain = false;

            while (!returnToMain) {
                cout << "\nTo-Do List Menu (Linked List):\n";
                cout << "1. Add Task\n";
                cout << "2. View All Tasks\n";
                cout << "3. Clear All Tasks\n";
                cout << "4. Bulk Insert Benchmarking\n";
                cout << "5. Return to Implementation Selection\n";
                cout << "Select option: ";
                int choice;
                cin >> choice;

                switch (choice) {
                case 1: {
                    string day;
                    cout << "Enter day (Monday to Sunday): ";
                    cin >> day;
                    int dayIndex = getDayIndex(day);
                    if (dayIndex == -1) {
                        cout << "Invalid day.\n";
                        continue;
                    }

                    string pos;
                    cout << "Where to add? (front / end / position): ";
                    cin >> pos;
                    transform(pos.begin(), pos.end(), pos.begin(), ::tolower);

                    if (pos == "front" || pos == "end") {
                        addTaskLinkedList(dayIndex, pos);
                    }
                    else if (pos == "position") {
                        TaskLinkedList* temp = weeklyTasksLinkedList[dayIndex];
                        int count = 0;
                        while (temp) { temp = temp->next; count++; }
                        displayDayTasksLinkedList(dayIndex);
                        cout << "Enter position (1 to " << count + 1 << "): ";
                        int index; cin >> index;
                        addTaskLinkedList(dayIndex, "position", index);
                    }
                    else cout << "Invalid position.\n";
                    break;
                }
                case 2: {
                    displayTasksLinkedList();
                    break;
                }
                case 3: {
                    clearAllLinkedList();
                    break;
                }
                case 4: {
                    string day, pos;
                    cout << "Enter day (Monday to Sunday): ";
                    cin >> day;
                    int dayIndex = getDayIndex(day);
                    if (dayIndex == -1) {
                        cout << "Invalid day.\n";
                        continue;
                    }

                    cout << "Where to bulk insert? (front / end / position): ";
                    cin >> pos;
                    transform(pos.begin(), pos.end(), pos.begin(), ::tolower);

                    if (pos == "front" || pos == "end") {
                        bulkInsertLinkedList(dayIndex, pos);
                    }
                    else if (pos == "position") {
                        TaskLinkedList* temp = weeklyTasksLinkedList[dayIndex];
                        int count = 0;
                        while (temp) { temp = temp->next; count++; }

                        displayDayTasksLinkedList(dayIndex);
                        cout << "You can insert at position from 1 (front) to " << count + 1 << " (end).\n";
                        cout << "Enter position: ";
                        int index; cin >> index;

                        bulkInsertLinkedList(dayIndex, pos, index);
                    }
                    else cout << "Invalid position.\n";
                    break;
                }
                case 5: {
                    returnToMain = true;
                    break;
                }
                default: {
                    cout << "Invalid option.\n";
                    break;
                }
                }
            }
        }
        else {
            cout << "Invalid implementation choice. Please try again.\n";
        }
    }

    return 0;
}