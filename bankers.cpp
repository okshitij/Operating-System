#include <iostream>
#include <vector>
#include <array>
using namespace std;

// Function to print a 2D matrix
void printMatrix(vector<vector<int>> matrix)
{
    for (auto row : matrix) {
        cout << "[ ";
        for (auto val : row)
            cout << val << " ";
        cout << "]" << endl;
    }
}

int main()
{
    int processes, numberOfResources;
    
    cout << "\nEnter total Number of processes: ";
    cin >> processes;

    cout << "\nEnter total Number of resources: ";
    cin >> numberOfResources;

    // Matrices for allocated, max, and need
    vector<vector<int>> allocated(processes, vector<int>(numberOfResources + 1, 0));
    vector<vector<int>> max(processes, vector<int>(numberOfResources + 1, 0));
    vector<vector<int>> need(processes, vector<int>(numberOfResources + 1, 0));

    // Input for allocated resources
    for (int i = 0; i < processes; i++) {
        int processId;
        cout << "\nEnter process Id: ";
        cin >> processId;
        allocated[i][0] = processId;
        max[i][0] = processId;
        need[i][0] = processId;

        for (int j = 1; j <= numberOfResources; j++) {
            cout << "\nEnter allocated resource for R" << (j - 1) << ": ";
            cin >> allocated[i][j];
        }
    }

    // Input for maximum resources
    for (int i = 0; i < processes; i++){
        for (int j = 1; j <= numberOfResources; j++){
            cout << "\nFor process with processId " << max[i][0] << " max instances of resource R" << (j - 1) << ": ";
            cin >> max[i][j];
        }
    }

    // Input for total resources
    vector<int> totalResources(numberOfResources);
    for (int i = 0; i < numberOfResources; i++){
        cout << "Enter total instances of R" << i << ": ";
        cin >> totalResources[i];
    }

    // Calculate the need matrix
    for (int i = 0; i < processes; i++){
        for (int j = 1; j <= numberOfResources; j++)
            need[i][j] = max[i][j] - allocated[i][j];
    }

    // Print matrices
    cout << "\nAllocated Matrix:\n";
    printMatrix(allocated);

    cout << "\nMax Matrix:\n";
    printMatrix(max);

    cout << "\nNeed Matrix:\n";
    printMatrix(need);

    // Calculate allocated resources sum per resource type
    vector<int> allocatedResources(numberOfResources, 0);
    for (int i = 1; i <= numberOfResources; i++){
        for (int j = 0; j < processes; j++)
            allocatedResources[i - 1] += allocated[j][i];
    }

    // Calculate available resources
    vector<int> available(numberOfResources, 0);
    for (int i = 0; i < numberOfResources; i++)
        available[i] = totalResources[i] - allocatedResources[i];

    vector<bool> statusOfProcess(processes, false);

    cout << "\nAllocated: ";
    for (int i : allocatedResources)
        cout << i << " ";
    cout << "\nAvailable: ";
    for (int i : available)
        cout << i << " ";
    cout << endl;

    // Banker's algorithm for finding the safe sequence
    vector<int> safeSequence;
    bool flag1 = true;
    while (flag1){
        flag1 = false;
        bool flag2 = false;

        for (int i = 0; i < processes; i++){
            if (!statusOfProcess[i]){
                flag2 = false;
                for (int j = 1; j <= numberOfResources; j++){
                    if (need[i][j] > available[j - 1]){
                        flag2 = true;
                        break;
                    }
                }

                if (!flag2){
                    flag1 = true;
                    statusOfProcess[i] = true;
                    safeSequence.push_back(allocated[i][0]);

                    for (int j = 1; j <= numberOfResources; j++)
                        available[j - 1] += allocated[i][j];

                    cout << "Available resource after execution of process Id " << allocated[i][0] << ": ";
                    for (int j : available)
                        cout << j << " ";
                    cout << endl;
                }
            }
        }
    }

    // Check for unsafe state
    for (bool element : statusOfProcess){
        if (!element){
            cout << "The system is in unsafe state" << endl;
            return 0;
        }
    }

    cout << "Safe sequence: ";
    for (int seq : safeSequence)
        cout << seq << " ";
    cout << endl;

    return 0;
}
