#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
using namespace std;

// Function for FIFO Page Replacement
int fifoPageReplacement(vector<int>& referenceString, int numberOfFrames) {
    queue<int> pageQueue;
    unordered_map<int, bool> inFrame;
    int pageFaults = 0;

    for (int page : referenceString) {
        if (!inFrame[page]) {
            pageFaults++;
            if (pageQueue.size() == numberOfFrames) {
                int removedPage = pageQueue.front();
                pageQueue.pop();
                inFrame[removedPage] = false;
            }
            pageQueue.push(page);
            inFrame[page] = true;
        }
    }

    return pageFaults;
}

// Function for LRU Page Replacement
int lruPageReplacement(vector<int>& referenceString, int numberOfFrames) {
    unordered_map<int, int> lastUsed;
    vector<int> frames;
    int pageFaults = 0;
    int currentTime = 0;

    for (int page : referenceString) {
        currentTime++;
        auto it = find(frames.begin(), frames.end(), page);

        if (it == frames.end()) { // Page not found in frames (page fault)
            pageFaults++;
            if (frames.size() == numberOfFrames) {
                int lruPage = *min_element(frames.begin(), frames.end(), [&](int a, int b) {
                    return lastUsed[a] < lastUsed[b];
                });
                auto lruIt = find(frames.begin(), frames.end(), lruPage);
                frames.erase(lruIt); // Remove LRU page
            }
            frames.push_back(page);
        }
        lastUsed[page] = currentTime;
    }

    return pageFaults;
}

// Function for Optimal Page Replacement
int optimalPageReplacement(vector<int>& referenceString, int numberOfFrames) {
    vector<int> frames;
    int pageFaults = 0;

    for (int i = 0; i < referenceString.size(); i++) {
        int page = referenceString[i];
        auto it = find(frames.begin(), frames.end(), page);

        if (it == frames.end()) { // Page not found in frames (page fault)
            pageFaults++;
            if (frames.size() == numberOfFrames) {
                int farthest = -1;
                int indexToReplace = -1;

                for (int j = 0; j < frames.size(); j++) {
                    int nextUse = -1;
                    for (int k = i + 1; k < referenceString.size(); k++) {
                        if (frames[j] == referenceString[k]) {
                            nextUse = k;
                            break;
                        }
                    }

                    if (nextUse == -1) { // Page is not used again, replace it
                        indexToReplace = j;
                        break;
                    }

                    if (nextUse > farthest) {
                        farthest = nextUse;
                        indexToReplace = j;
                    }
                }

                frames[indexToReplace] = page; // Replace optimal page
            } else {
                frames.push_back(page);
            }
        }
    }

    return pageFaults;
}

int main() {
    int numberOfFrames, numberOfPages;
    
    cout << "Enter the number of frames: ";
    cin >> numberOfFrames;

    cout << "Enter the number of pages in the reference string: ";
    cin >> numberOfPages;

    vector<int> referenceString(numberOfPages);
    cout << "Enter the reference string:\n";
    for (int i = 0; i < numberOfPages; i++) {
        cin >> referenceString[i];
    }

    // FIFO
    int fifoFaults = fifoPageReplacement(referenceString, numberOfFrames);
    cout << "\nFIFO Page Faults: " << fifoFaults;

    // LRU
    int lruFaults = lruPageReplacement(referenceString, numberOfFrames);
    cout << "\nLRU Page Faults: " << lruFaults;

    // Optimal
    int optimalFaults = optimalPageReplacement(referenceString, numberOfFrames);
    cout << "\nOptimal Page Faults: " << optimalFaults;

    return 0;
}
