#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int remainingTime;
    int finishTime;
    int turnaroundTime;
    int waitingTime;
};

// Function to display the results
void displayResults(vector<Process>& processes) {
    cout << "\nProcess ID | Finish Time | Turnaround Time | Waiting Time\n";
    for (auto& p : processes) {
        cout << "    " << p.id << "       |      " << p.finishTime << "       |       " << p.turnaroundTime << "       |     " << p.waitingTime << "\n";
    }
}

// FCFS Scheduling
void FCFS(vector<Process>& processes) {
    int currentTime = 0;
    for (auto& p : processes) {
        if (currentTime < p.arrivalTime) 
            currentTime = p.arrivalTime;
        p.finishTime = currentTime + p.burstTime;
        p.turnaroundTime = p.finishTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        currentTime = p.finishTime;
    }
    cout << "\nFirst Come First Serve (FCFS):";
    displayResults(processes);
}

// Non-Preemptive Priority
void priorityNonPreemptive(vector<Process> processes) {
    vector<Process> result;
    int currentTime = 0;

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    while (!processes.empty()) {
        vector<Process> readyQueue;
        for (const auto& p : processes) {
            if (p.arrivalTime <= currentTime) {
                readyQueue.push_back(p);
            }
        }

        if (readyQueue.empty()) {
            currentTime++;
            continue;
        }

        auto it = min_element(readyQueue.begin(), readyQueue.end(), [](const Process& a, const Process& b) {
            return a.priority < b.priority;
        });
        
        Process currentProcess = *it;
        currentProcess.finishTime = currentTime + currentProcess.burstTime;
        currentProcess.turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;
        currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;
        
        currentTime = currentProcess.finishTime;
        
        result.push_back(currentProcess);

        processes.erase(remove_if(processes.begin(), processes.end(), [&](const Process& p) {
            return p.id == currentProcess.id;
        }), processes.end());
    }
    
    cout << "\nPriority (Non-Preemptive):";
    displayResults(result);
}

// Preemptive Priority
void priorityPreemptive(vector<Process> processes) {
    vector<Process> result;
    int currentTime = 0, completed = 0;
    int n = processes.size();
    vector<int> remainingBurstTime(n);
    for (int i = 0; i < n; i++) remainingBurstTime[i] = processes[i].burstTime;

    while (completed != n) {
        int idx = -1, highestPriority = 1e9;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && remainingBurstTime[i] > 0 && processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                idx = i;
            }
        }

        if (idx != -1) {
            remainingBurstTime[idx]--;
            currentTime++;
            if (remainingBurstTime[idx] == 0) {
                completed++;
                processes[idx].finishTime = currentTime;
                processes[idx].turnaroundTime = currentTime - processes[idx].arrivalTime;
                processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
                result.push_back(processes[idx]);
            }
        } else {
            currentTime++;
        }
    }
    cout << "\nPriority Scheduling (Preemptive):";
    displayResults(result);
}

// Non-Preemptive SJF
void SJFNonPreemptive(vector<Process> processes) {
    vector<Process> result;
    int currentTime = 0;

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    while (!processes.empty()) {
        vector<Process> readyQueue;
        for (const auto& p : processes) {
            if (p.arrivalTime <= currentTime) {
                readyQueue.push_back(p);
            }
        }

        if (readyQueue.empty()) {
            currentTime++;
            continue;
        }

        auto it = min_element(readyQueue.begin(), readyQueue.end(), [](const Process& a, const Process& b) {
            return a.burstTime < b.burstTime;
        });
        
        Process currentProcess = *it;
        currentProcess.finishTime = currentTime + currentProcess.burstTime;
        currentProcess.turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;
        currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;
        
        currentTime = currentProcess.finishTime;
        
        result.push_back(currentProcess);

        processes.erase(remove_if(processes.begin(), processes.end(), [&](const Process& p) {
            return p.id == currentProcess.id;
        }), processes.end());
    }
    
    cout << "\nShortest Job First (Non-Preemptive):";
    displayResults(result);
}

// Preemptive SJF
void SJFPreemptive(vector<Process> processes) {
    vector<Process> result;
    int currentTime = 0, completed = 0;
    int n = processes.size();
    vector<int> remainingBurstTime(n);
    for (int i = 0; i < n; i++) remainingBurstTime[i] = processes[i].burstTime;

    while (completed != n) {
        int idx = -1, minRemainingTime = 1e9;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && remainingBurstTime[i] > 0 && remainingBurstTime[i] < minRemainingTime) {
                minRemainingTime = remainingBurstTime[i];
                idx = i;
            }
        }

        if (idx != -1) {
            remainingBurstTime[idx]--;
            currentTime++;
            if (remainingBurstTime[idx] == 0) {
                completed++;
                processes[idx].finishTime = currentTime;
                processes[idx].turnaroundTime = currentTime - processes[idx].arrivalTime;
                processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
                result.push_back(processes[idx]);
            }
        } else {
            currentTime++;
        }
    }
    cout << "\nShortest Job First (Preemptive):";
    displayResults(result);
}

// Round Robin Scheduling function
void roundRobin(vector<Process>& processes, int quantum) {
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();
    queue<int> q;

    vector<bool> inQueue(n, false);  // Track if a process is already in the queue
    vector<int> remainingBurstTime(n);
    
    for (int i = 0; i < n; i++) 
        remainingBurstTime[i] = processes[i].burstTime;

    // Start with processes that have arrived at time 0
    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime == 0) {
            q.push(i);
            inQueue[i] = true;
        }
    }

    while (completed < n) {
        if (q.empty()) {
            currentTime++;
            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && processes[i].arrivalTime <= currentTime) {
                    q.push(i);
                    inQueue[i] = true;
                }
            }
            continue;
        }

        int i = q.front();
        q.pop();

        // Execute the current process
        if (remainingBurstTime[i] > quantum) {
            currentTime += quantum;
            remainingBurstTime[i] -= quantum;
        } else {
            currentTime += remainingBurstTime[i];
            processes[i].finishTime = currentTime;
            processes[i].turnaroundTime = currentTime - processes[i].arrivalTime;
            processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
            remainingBurstTime[i] = 0;
            completed++;
        }

        // Check for newly arrived processes and add them to the queue
        for (int j = 0; j < n; j++) {
            if (!inQueue[j] && processes[j].arrivalTime <= currentTime && remainingBurstTime[j] > 0) {
                q.push(j);
                inQueue[j] = true;
            }
        }

        // Re-add current process if it still has remaining burst time
        if (remainingBurstTime[i] > 0) {
            q.push(i);
        }
    }

    cout << "\nRound Robin Scheduling:";
    displayResults(processes);
}


int main() {
    int n, quantum;
    cout << "Enter the number of processes: ";
    cin >> n;
    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        cout << "\nEnter arrival time, burst time, and priority for process " << i + 1 << ": ";
        processes[i].id = i;
        cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
        processes[i].remainingTime = processes[i].burstTime;
    }

    cout << "Enter the time quantum for Round Robin: ";
    cin >> quantum;

    FCFS(processes);
    SJFNonPreemptive(processes);
    SJFPreemptive(processes);
    priorityPreemptive(processes);
    priorityNonPreemptive(processes);
    roundRobin(processes, quantum);

    return 0;
}