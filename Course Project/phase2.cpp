#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class job {
    char M[300][4], IR[4], R[4], buffer[40];
    string error[9] = {"No Error", "Out of Data", "Line Limit Exceeded", "Time Limit Exceeded", "Operation Code Error", "Operand Error", "Invalid Page Fault", "Time Limit Exceeded and Operation Code Error", "Time Limit Exceeded and Operand Error"};
    bool C, flag, flag2;
    bool allocatedFrames[30];
    int job_id, TTL, TLL, TTC, LLC, PTR, VA, RA;
    int IC, SI, PI, TI;

public:
    void INIT(), LOAD(), STARTEXECUTION(), EXECUTEUSERPROGRAM(), READ(int RA), WRITE(int RA), TERMINATE(int EM), SIMULATION();
    int MOS(), ALLOCATE(), ADDRESSMAP(int VA);
    fstream Input, Output;

    job() {
        IC = 0;
        SI = 0;
        PI = 0;
        TI = 0;
        TTL = 0;
        TLL = 0;
        TTC = 0;
        LLC = 0;
        C = false;
        flag = false;
        flag2 = false;
        VA = 0;
        memset(M, 0, sizeof(M));
        memset(IR, 0, sizeof(IR));
        memset(R, 0, sizeof(R));
        memset(buffer, 0, sizeof(buffer));
        memset(allocatedFrames, false, sizeof(allocatedFrames));  
    }

    void setPCB(int id, int ttl, int tll) {
        job_id = id;
        TTL = ttl;
        TLL = tll;
    }
};

void job::INIT() {
    IC = 0;
    SI = 0;
    PI = 0;
    TI = 0;
    TTL = 0;
    TLL = 0;
    TTC = 0;
    LLC = 0;
    job_id = 0;
    C = false;
    flag = false;
    flag2 = false;
    VA = 0;
    RA = 0;
    PTR = 0;
    memset(M, 0, sizeof(M));  
    memset(IR, 0, sizeof(IR));
    memset(R, 0, sizeof(R));
    memset(buffer, 0, sizeof(buffer));
    memset(allocatedFrames, false, sizeof(allocatedFrames));  
}


void job::LOAD() {
    string line;
    int mem_loc = 0;
    int currPTR = 0;
    while (getline(Input, line)) {
        if (line.substr(0, 4) == "$AMJ") {
            INIT();
            int job_id = stoi(line.substr(4, 4));
            int ttl = stoi(line.substr(8, 4));
            int tll = stoi(line.substr(12, 4));
            setPCB(job_id, ttl, tll);

            PTR = ALLOCATE() * 10;
            currPTR = PTR;

            for (int i = 0; i < 10; i++) {
                M[PTR + i][0] = '0';  
            }

        } else if (line.substr(0, 4) == "$DTA") {
            STARTEXECUTION();
        } else if (line.substr(0, 4) == "$END") {
            continue;
        } else {
            if(flag) continue; 

            mem_loc = ALLOCATE();

            M[currPTR][0] = '1';  
            M[currPTR][2] = (mem_loc / 10) + '0';  
            M[currPTR][3] = (mem_loc % 10) + '0';

            currPTR++;  

            int line_idx = 0;
            for (int i = 0; i < 10 && line_idx < line.size(); i++) {
                for (int j = 0; j < 4 && line_idx < line.size(); j++, line_idx++) {
                    M[mem_loc*10 + i][j] = line[line_idx];
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
    while (!flag) {  
        RA = ADDRESSMAP(IC);  

        if(M[RA][0] != 'H'){
            for (int i = 0; i < 4; i++)
                IR[i] = M[RA][i]; 
        } else {
            IR[0] = 'H';
            IR[1] = ' ';
            IR[2] = ' ';
            IR[3] = ' ';
        }

        IC++;  

        if (IR[0] == 'H') {
            SI = 3;
            MOS();
            break;
        } 

        if (isdigit(IR[2]) && isdigit(IR[3]))
            VA = (IR[2] - '0') * 10 + (IR[3] - '0');
        else {
            PI = 2;  
            MOS();
            return;
        }

        RA = ADDRESSMAP(VA);  
    
        if (IR[0] == 'G' && IR[1] == 'D') {
            SI = 1;
            MOS();
        } 
        else if (IR[0] == 'P' && IR[1] == 'D') {
            SI = 2;
            MOS();
        } 
        else if (IR[0] == 'L' && IR[1] == 'R') {
            for (int i = 0; i < 4; i++)
                R[i] = M[RA][i];  
        } 
        else if (IR[0] == 'S' && IR[1] == 'R') {
            for (int i = 0; i < 4; i++)
                M[RA][i] = R[i]; 
        } 
        else if (IR[0] == 'C' && IR[1] == 'R') {
            C = true;
            for (int i = 0; i < 4; i++) {
                if (M[RA][i] != R[i]) {
                    C = false;
                    break;
                }
            }
        } 
        else if (IR[0] == 'B' && IR[1] == 'T') {
            if (C) IC = VA;  
        } 
        else {
            PI = 1;  
            MOS();
        }

        SIMULATION();  

        cout << "IC: " << IC << " SI: " << SI << " IR: " << IR << " R: " << R << " C: " << C << endl;
        cout << "Memory: " << endl;
        for (int i = 0; i < 300; i++) {
            cout << "M" << i << " ";
            for (int j = 0; j < 4; j++) {
                cout << M[i][j] << " ";
            }
            cout << endl;
        }
        cout << "-----------------------------------------------" << endl;

        if(SI != 0 || PI != 0 || TI != 0)
            MOS();
    }
}


int job::MOS() {
    if(TI == 0){
        if(SI != 0){
            switch(SI){
                case 1:
                    READ(RA); break;
                case 2:
                    WRITE(RA); break;   
                case 3:
                    TERMINATE(0); flag = true; break;
            }
            SI = 0;
        }
        else if(PI != 0){
            switch(PI){
                case 1:
                    TERMINATE(4); flag = true; break;
                case 2:
                    TERMINATE(5); flag = true; break;
                case 3:
                    char temp[3];
                    memset(temp, '\0', 2);
                    memcpy(temp, IR, 2);
                    if(!strcmp(temp, "GD") || !strcmp(temp, "SR")){
                        int mem_loc = ALLOCATE();

                        M[PTR + (VA / 10)][0] = '1';  
                        M[PTR + (VA / 10)][2] = (mem_loc / 10) + '0';  
                        M[PTR + (VA / 10)][3] = (mem_loc % 10) + '0';  

                        PI = 0;  
                        RA = mem_loc * 10 + (VA % 10);  

                        return RA;
                         
                    }
                    else if(!strcmp(temp, "PD") || !strcmp(temp, "LR") || !strcmp(temp, "H") || !strcmp(temp, "CR") || !strcmp(temp, "BT")){
                        TERMINATE(6);
                        flag = true;
                    }
            }
            PI = 0;
        }
    } else if (TI == 2 && !flag2) {
        if (SI != 0) {
            switch (SI) {
                case 1:
                    TERMINATE(3); flag = true; break;
                case 2:
                    WRITE(RA); 
                    if (!flag) TERMINATE(3); 
                    break;
                case 3:
                    TERMINATE(0); flag = true; break;
            }
            SI = 0;
        }
        else if (PI != 0) {
            switch (PI) {
                case 1:
                    TERMINATE(7); flag = true; break;
                case 2:
                    TERMINATE(8); flag = true; break;
                case 3:
                    TERMINATE(3); flag = true; break;
            }
            PI = 0;
        }
    } 
    else if (TI == 2 && flag2) {  //when TLE occurs for other instructions(LR, SR, CR, BT)
        TERMINATE(3); 
        flag = true;  
        TI = 0;
    }
    return 0;
}

void job::READ(int RA) {
    Input.getline(buffer, 40);
    char temp[4];
    memset(temp, '\0', 5);
    memcpy(temp, buffer, 4);
    if(!strcmp(temp, "$END")){
        TERMINATE(1);
        flag = true;
        return;
    }
    else{
        for (int i = 0; i < 10; i++) {
            memset(M[RA + i], '\0', 4);
            for (int j = 0; j < 4 && buffer[i * 4 + j] != '\0'; j++)
                M[RA + i][j] = buffer[i * 4 + j];
        }
    }
}

void job::WRITE(int RA) {
    LLC++;
    if(LLC > TLL){
        TERMINATE(2);
        flag = true;
    }
    else{
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 4; j++) {
                if (M[RA + i][j] != '\0')
                    Output << M[RA + i][j];
            }
        }
        Output << endl;
    }
}

void job::TERMINATE(int EM) { 
    Output << "Job ID   :   " << job_id << endl;
    Output << "  " << error[EM] << endl;
    Output << "IC       :   " << IC << endl;
    Output << "IR       :   " << IR[0] << IR[1] << IR[2] << IR[3] << endl;
    //Output << "R        :   " << R << endl;
    Output << "TTL      :   " << TTL << endl;
    Output << "TTC      :   " << TTC << endl;
    Output << "TLL      :   " << TLL << endl;
    Output << "LLC      :   " << LLC << endl;
    Output << "\n\n";

    LOAD(); 
}


int job::ALLOCATE() {
    int frame;
    do {
        frame = rand() % 30;  
    } while (allocatedFrames[frame]); 

    allocatedFrames[frame] = true;  
    return frame;  
}


int job::ADDRESSMAP(int VA) {
    if (VA >= 0 && VA < 100) {
        int loc = PTR + VA / 10;
        int frameno = (M[loc][2] - '0') * 10 + (M[loc][3] - '0');

        if (M[loc][0] == '0') {  
            PI = 3;
            MOS();
            if (PI == 0)
                return RA;
        } else {  
            int RA = frameno * 10 + (VA % 10);
            return RA;
        }
    } else { 
        PI = 2;
        MOS();
    }

    return -1; 
}


void job::SIMULATION() {
    TTC++;
    if (TTC == TTL)
        TI = 2;
    else if (TTC > TTL) {
        flag2 = true; 
        //MOS();
    } 
    else
        TI = 0;
}


int main() {
    job myJob;
    myJob.Input.open("input_phase2.txt", ios::in);
    myJob.Output.open("output_phase2.txt", ios::out);

    if (!myJob.Input) {
        cerr << "Error: Input file not found!" << endl;
        return 1;
    }

    myJob.LOAD();

    myJob.Input.close();
    myJob.Output.close();

    return 0;
}