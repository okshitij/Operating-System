#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class job {
    char M[100][4], IR[4], R[4], buffer[40];
    bool C;
    int IC, SI;

public:
    void INIT(), LOAD(), STARTEXECUTION(), EXECUTEUSERPROGRAM(), MOS(), READ(), WRITE(), TERMINATE();
    fstream Input, Output;

    job() {
        IC = 0;
        SI = 0;
        C = false;
        memset(M, 0, sizeof(M));
        memset(IR, 0, sizeof(IR));
        memset(R, 0, sizeof(R));
        memset(buffer, 0, sizeof(buffer));
    }
};

void job::INIT() {
    IC = 0;
    SI = 0;
    C = false;
    memset(M, 0, sizeof(M));
    memset(IR, 0, sizeof(IR));
    memset(R, 0, sizeof(R));
    memset(buffer, 0, sizeof(buffer));  
}

void job::LOAD() {
    string line;
    int mem_loc = 0;
    while (getline(Input, line)) {
        if (line.substr(0, 4) == "$AMJ") {
            INIT();
            mem_loc = 0; //reset mem location for each new job
        } else if (line.substr(0, 4) == "$DTA") {
            STARTEXECUTION();
        } else if (line.substr(0, 4) == "$END") {
            continue;
        } else {
                for (int i = 0; i < line.length(); i += 4, mem_loc++) {
                    for (int j = 0; j < 4 && i + j < line.length(); j++) {
                        M[mem_loc][j] = line[i + j];
                    }
                }
        }
    }
}

void job::STARTEXECUTION() {
    IC = 0;
    EXECUTEUSERPROGRAM();
}

void job::EXECUTEUSERPROGRAM() {
    while (true) {
        for (int i = 0; i < 4; i++) {
            IR[i] = M[IC][i];
        }

        IC++;

        int mem_loc = (IR[2] - '0') * 10 + (IR[3] - '0');

        if (IR[0] == 'G' && IR[1] == 'D') {
            SI = 1;
        } 
        else if (IR[0] == 'P' && IR[1] == 'D') {
            SI = 2;
        } 
        else if (IR[0] == 'H') {
            SI = 3;
        } 
        else if (IR[0] == 'L' && IR[1] == 'R') {
            for (int i = 0; i < 4; i++) {
                R[i] = M[mem_loc][i];
            }
        } 
        else if (IR[0] == 'S' && IR[1] == 'R') {
            for (int i = 0; i < 4; i++) {
                M[mem_loc][i] = R[i];
            }
        } 
        else if (IR[0] == 'C' && IR[1] == 'R') {
            C = true;
            for (int i = 0; i < 4; i++) {
                if (M[mem_loc][i] != R[i]) {
                    C = false;
                    break;
                }
            } 
        } 
        else if (IR[0] == 'B' && IR[1] == 'T') {
            if (C)
                IC = mem_loc;   
        }

        MOS();

        cout << "IC: " << IC << " SI: " << SI << " IR: " << IR << " R: " << R << " C: " << C << endl;
        cout << "Memory: " << endl;
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 4; j++) {
                cout << M[i][j] << " ";
            }
            cout << endl;
        }
        cout << "-----------------------------------------------" << endl;

       if (SI == 3) break;
    }
}

void job::MOS() {
    switch (SI) {
        case 1:
            READ();
            SI = 0;
            break;
        case 2:
            WRITE();
            SI = 0;
            break;
        case 3:
            TERMINATE();
            break;
    }
}

void job::READ() {
    Input.getline(buffer, 40);
    int mem_loc = (IR[2] - '0') * 10 + (IR[3] - '0');
    
    for (int i = 0; i < 10; i++) {
        memset(M[mem_loc + i], '\0', 4);  // Clear the memory location before writing
        for (int j = 0; j < 4 && buffer[i * 4 + j] != '\0'; j++) {
            M[mem_loc + i][j] = buffer[i * 4 + j];
        }
    }
    
    memset(buffer, '\0', sizeof(buffer));
}

void job::WRITE() {
    int mem_loc = (IR[2] - '0') * 10 + (IR[3] - '0');
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            if (M[mem_loc + i][j] != '\0') {
                Output << M[mem_loc + i][j];
            }
        }
    }
    Output << endl;
}


void job::TERMINATE() {
    Output << "\n\n";
}

int main() {
    job myJob;
    myJob.Input.open("input_Phase1.txt", ios::in);
    myJob.Output.open("output_Phase1.txt", ios::out);

    if (!myJob.Input) {
        cerr << "Error: Input file not found!" << endl;
        return 1;
    }

    myJob.LOAD();

    myJob.Input.close();
    myJob.Output.close();

    return 0;
}