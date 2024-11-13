#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include<limits.h>

using namespace std;

// Function to calculate and print FCFS Seek Time and Differences
int FCFS(vector<int>& requests, int head) {
    int seek_time = 0;
    vector<int> seek_sequence, differences;
    
    for (int i = 0; i < requests.size(); i++) {
        seek_sequence.push_back(requests[i]);
        differences.push_back(abs(requests[i] - head));
        seek_time += abs(requests[i] - head);
        head = requests[i];
    }
    
    cout << "FCFS\n";
    cout << "Sequence of requests: ";
    for (int r : seek_sequence) cout << r << " ";
    cout << "\nDifferences: ";
    for (int d : differences) cout << d << " ";
    cout << "\nTotal Seek Time: " << seek_time << "\n\n";
    
    return seek_time;
}

// Function to calculate and print SSTF Seek Time and Differences
int SSTF(vector<int>& requests, int head) {
    int seek_time = 0;
    vector<bool> visited(requests.size(), false);
    vector<int> seek_sequence, differences;
    int count = 0;
    
    while (count < requests.size()) {
        int closest = -1;
        int min_dist = INT_MAX;
        
        for (int i = 0; i < requests.size(); i++) {
            if (!visited[i] && abs(requests[i] - head) < min_dist) {
                min_dist = abs(requests[i] - head);
                closest = i;
            }
        }
        
        visited[closest] = true;
        seek_sequence.push_back(requests[closest]);
        differences.push_back(min_dist);
        seek_time += min_dist;
        head = requests[closest];
        count++;
    }
    
    cout << "SSTF\n";
    cout << "Sequence of requests: ";
    for (int r : seek_sequence) cout << r << " ";
    cout << "\nDifferences: ";
    for (int d : differences) cout << d << " ";
    cout << "\nTotal Seek Time: " << seek_time << "\n\n";
    
    return seek_time;
}

// Function to calculate and print SCAN Seek Time and Differences
int SCAN(vector<int>& requests, int head) {
    int seek_time = 0;
    vector<int> seek_sequence, differences;
    
    sort(requests.begin(), requests.end());
    int pos = distance(requests.begin(), lower_bound(requests.begin(), requests.end(), head));
    
    // Moving towards the end of the disk
    for (int i = pos; i < requests.size(); i++) {
        seek_sequence.push_back(requests[i]);
        differences.push_back(abs(requests[i] - head));
        seek_time += abs(requests[i] - head);
        head = requests[i];
    }
    
    // Moving back towards the beginning
    for (int i = pos - 1; i >= 0; i--) {
        seek_sequence.push_back(requests[i]);
        differences.push_back(abs(requests[i] - head));
        seek_time += abs(requests[i] - head);
        head = requests[i];
    }
    
    cout << "SCAN\n";
    cout << "Sequence of requests: ";
    for (int r : seek_sequence) cout << r << " ";
    cout << "\nDifferences: ";
    for (int d : differences) cout << d << " ";
    cout << "\nTotal Seek Time: " << seek_time << "\n\n";
    
    return seek_time;
}

// Function to calculate and print C-SCAN Seek Time and Differences
int CSCAN(vector<int>& requests, int head) {
    int seek_time = 0;
    vector<int> seek_sequence, differences;
    
    sort(requests.begin(), requests.end());
    int pos = distance(requests.begin(), lower_bound(requests.begin(), requests.end(), head));
    
    // Moving towards the end of the disk
    for (int i = pos; i < requests.size(); i++) {
        seek_sequence.push_back(requests[i]);
        differences.push_back(abs(requests[i] - head));
        seek_time += abs(requests[i] - head);
        head = requests[i];
    }
    
    // Wrap around to the beginning of the disk
    for (int i = 0; i < pos; i++) {
        seek_sequence.push_back(requests[i]);
        differences.push_back(abs(requests[i] - head));
        seek_time += abs(requests[i] - head);
        head = requests[i];
    }
    
    cout << "C-SCAN\n";
    cout << "Sequence of requests: ";
    for (int r : seek_sequence) cout << r << " ";
    cout << "\nDifferences: ";
    for (int d : differences) cout << d << " ";
    cout << "\nTotal Seek Time: " << seek_time << "\n\n";
    
    return seek_time;
}

int main() {
    int disk_size, head, n;
    
    cout << "Enter the size of the disk (max cylinder number): ";
    cin >> disk_size;
    
    cout << "Enter the initial head position: ";
    cin >> head;
    
    cout << "Enter the number of disk requests: ";
    cin >> n;
    
    vector<int> requests(n);
    cout << "Enter the disk requests: ";
    for (int i = 0; i < n; i++) {
        cin >> requests[i];
    }
    
    // FCFS
    vector<int> fcfs_requests = requests;
    FCFS(fcfs_requests, head);

    // SSTF
    vector<int> sstf_requests = requests;
    SSTF(sstf_requests, head);

    // SCAN
    vector<int> scan_requests = requests;
    SCAN(scan_requests, head);

    // C-SCAN
    vector<int> cscan_requests = requests;
    CSCAN(cscan_requests, head);

    return 0;
}
