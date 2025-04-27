#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <limits>
#include <iomanip>
#include <cctype>

using namespace std;

class Graph {
    unordered_map<string, vector<string>> adj_list;
    unordered_map<string, int> Execution_time;
    unordered_map<string, int> dependency_count;
    string to_lower(const string &srg) {
        string str = srg;
        for (char &c : str) {
            c = tolower(c);
        }
        return str;
    }

public:
    void Graph_form(const string &task, int time, const vector<string> &dependencies) {
        string task_lower = to_lower(task);
        if (dependency_count.find(task_lower) == dependency_count.end()) {
            dependency_count[task_lower] = 0;
        }
        
        if (adj_list.find(task_lower) == adj_list.end()) {
            adj_list[task_lower] = {};
        }
        
        if (time < 0) {
            cout << "Invalid execution time for task " << task << ". It should be a positive integer.\n";
            return;
        }
        
        Execution_time[task_lower] = time;
        
        for (const auto &dep : dependencies) {
            string dep_lower = to_lower(dep);
            if (adj_list.find(dep_lower) == adj_list.end()) {
                adj_list[dep_lower] = {};
            }

            adj_list[dep_lower].push_back(task_lower);
            dependency_count[task_lower]++;
        }
    }

    vector<string> topological_sort() {
        queue<string> q;
        vector<string> sorted_order;

        for (const auto &entry : dependency_count) {
            if (entry.second == 0) {
                q.push(entry.first);
            }
        }

        while (!q.empty()) {
            string task = q.front();
            q.pop();
            sorted_order.push_back(task);

            for (const auto &neighbour : adj_list[task]) {
                dependency_count[neighbour]--;
                if (dependency_count[neighbour] == 0) {
                    q.push(neighbour);
                }
            }
        }

        if (sorted_order.size() != dependency_count.size()) {
            cout << "\nCycle detected! Cannot perform a topological sort.\n";
            return {};  
        }

        return sorted_order;
    }

    unordered_map<string, pair<int, int>> calculate_earliest_start_times(const vector<string> &sorted_tasks) {
        unordered_map<string, pair<int, int>> earliest_start_time;
        int start_time = 0;

        for (const auto &task : sorted_tasks) {
            if (Execution_time[task] == 0) {
                continue;
            }

            earliest_start_time[task] = {start_time, start_time + Execution_time[task]};
            start_time += Execution_time[task];
        }

        return earliest_start_time;
    }

    void display_graph() {
        cout << "\nDependency Graph:\n";
        for (const auto &pair : adj_list) {
            cout << pair.first << " (" << dependency_count[pair.first] << " dependencies, Execution time: " << Execution_time[pair.first] << "min) -> ";
            for (const auto &task : pair.second) {
                cout << task << ", ";
            }
            cout << endl;
        }

        cout << "\nIndependent Tasks:\n";
        for (const auto &pair : dependency_count) {
            if (pair.second == 0) {
                cout << pair.first << " (Execution Time: " << Execution_time[pair.first] << " min)\n";
            }
        }
    }

    void display_schedule(const unordered_map<string, pair<int, int>> &schedule, const vector<string> &sorted_tasks, int start_hour) {
        cout << "\n===== Task Execution Schedule =====\n";
        cout << "+------------+------------+------------+------------+\n";
        cout << "| Task       | Start Time | End Time   | Duration   |\n";
        cout << "+------------+------------+------------+------------+\n";

        for (const string &task : sorted_tasks) {
            if (Execution_time[task] == 0) {
                printf("| %-10s | %-10s | %-10s | %-10d |\n", 
                    task.c_str(), "N/A", "N/A", Execution_time[task]);
                continue; 
            }

            auto [start_time, end_time] = schedule.at(task);
            int start_h = start_hour + (start_time) / 60;
            int start_m = (start_time) % 60;
            int end_h = start_hour + (end_time) / 60;
            int end_m = (end_time) % 60;

            printf("| %-10s | %02d:%02d      | %02d:%02d      | %-10d |\n", 
                task.c_str(), start_h, start_m, end_h, end_m, Execution_time[task]);
        }

        cout << "+------------+------------+------------+------------+\n";
    }
};

int main() {
    Graph g;
    string task;

    cout << "===== Task Scheduling System =====\n";
    int start_time;
    cout << "\nEnter your preferred start time (in hours): ";
    cin >> start_time;
    cin.ignore();

    while (true) {
        cout << "\nEnter task name (or 'q' to quit): ";
        getline(cin, task);
        if (task == "q")
            break;

        cout << "Enter execution time (minutes): ";
        int time;
        while (!(cin >> time) || time <= 0) {
            cout << "Invalid input! Enter a positive integer: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        vector<string> dependencies;
        string dep;
        cin.ignore();

        cout << "Enter dependencies (one per line, enter 'e' to stop):\n";
        while (true) {
            getline(cin, dep);
            if (dep == "e")
                break;
            dependencies.push_back(dep);
        }

        g.Graph_form(task, time, dependencies);
    }

    g.display_graph();
    vector<string> sorted_tasks = g.topological_sort();

    if (sorted_tasks.empty()) {
        cout << "\nNo valid task execution order found (cycle detected or empty tasks).\n";
        return 1;
    }

    unordered_map<string, pair<int, int>> schedule = g.calculate_earliest_start_times(sorted_tasks);
    g.display_schedule(schedule, sorted_tasks, start_time);

    return 0;
}
