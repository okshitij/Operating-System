//g++ matrixthreads.cpp -o matrixthreads -lpthread
//./matrixthreads

#include <iostream>
#include <pthread.h>
#include <vector>

using namespace std;

vector<vector<int>> A, B, C, D;
int rows, cols;

struct ThreadData {
    int row;
    int col;
};

void* add(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    C[data->row][data->col] = A[data->row][data->col] + B[data->row][data->col];
    pthread_exit(0);
}

void* subtract(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    C[data->row][data->col] = A[data->row][data->col] - B[data->row][data->col];
    pthread_exit(0);
}

void* multiply(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    int sum = 0;
    for (int i = 0; i < cols; i++) {
        sum += A[data->row][i] * B[i][data->col];
    }
    D[data->row][data->col] = sum;
    pthread_exit(0);
}

int main() {
    cout << "Enter the number of rows and columns of the matrices: ";
    cin >> rows >> cols;

    A.resize(rows, vector<int>(cols));
    B.resize(rows, vector<int>(cols));
    C.resize(rows, vector<int>(cols));
    D.resize(rows, vector<int>(cols));

    cout << "Enter elements of matrix A:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cin >> A[i][j];
        }
    }

    cout << "Enter elements of matrix B:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cin >> B[i][j];
        }
    }

    pthread_t threads[rows][cols];
    ThreadData threadData[rows][cols];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            threadData[i][j].row = i;
            threadData[i][j].col = j;
            pthread_create(&threads[i][j], NULL, add, (void*) &threadData[i][j]);
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    cout << "\nResult of Matrix Addition:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << C[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            pthread_create(&threads[i][j], NULL, subtract, (void*) &threadData[i][j]);
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    cout << "Result of Matrix Subtraction:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << C[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            pthread_create(&threads[i][j], NULL, multiply, (void*) &threadData[i][j]);
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    cout << "Result of Matrix Multiplication:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << D[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}
