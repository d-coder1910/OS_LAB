#include <iostream>
using namespace std;

#define MAX 10

int n, m;

void calculateNeed(int need[MAX][MAX], int max[MAX][MAX], int allocation[MAX][MAX]) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

void bankersAlgorithm(int allocation[MAX][MAX], int max[MAX][MAX], int available[MAX]) {
    int need[MAX][MAX];
    calculateNeed(need, max, allocation);

    bool finish[MAX] = {false};
    int safeSeq[MAX];
    int work[MAX];

    for(int i = 0; i < m; i++)
        work[i] = available[i];

    int count = 0;

    while(count < n) {
        bool found = false;

        for(int i = 0; i < n; i++) {
            if(!finish[i]) {
                bool possible = true;

                for(int j = 0; j < m; j++) {
                    if(need[i][j] > work[j]) {
                        possible = false;
                        break;
                    }
                }

                if(possible) {
                    for(int j = 0; j < m; j++)
                        work[j] += allocation[i][j];

                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }
        

        if(!found) {
            cout << "\nSystem is in UNSAFE state (Deadlock possible)\n";
            return;
        }
    }

    cout << "\nNeed Matrix:\n";
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++)
            cout << need[i][j] << " ";
            cout << endl;
    }

    cout << "\nSystem is in SAFE state\nSafe Sequence: ";
    for(int i = 0; i < n; i++)
        cout << "P" << safeSeq[i] << " ";
    cout << endl;
}

void deadlockDetection(int allocation[MAX][MAX], int request[MAX][MAX], int available[MAX]) {
    bool finish[MAX];
    int work[MAX];

    for(int i = 0; i < m; i++)
        work[i] = available[i];

    for(int i = 0; i < n; i++) {
        bool zeroAlloc = true;
        for(int j = 0; j < m; j++) {
            if(allocation[i][j] != 0) {
                zeroAlloc = false;
                break;
            }
        }
        finish[i] = zeroAlloc;
    }

    while(true) {
        bool found = false;

        for(int i = 0; i < n; i++) {
            if(!finish[i]) {
                bool possible = true;

                for(int j = 0; j < m; j++) {
                    if(request[i][j] > work[j]) {
                        possible = false;
                        break;
                    }
                }

                if(possible) {
                    for(int j = 0; j < m; j++)
                        work[j] += allocation[i][j];

                    finish[i] = true;
                    found = true;
                }
            }
        }

        if(!found)
            break;
    }

    bool deadlock = false;
    cout << "\nDeadlocked Processes: ";
    for(int i = 0; i < n; i++) {
        if(!finish[i]) {
            cout << "P" << i << " ";
            deadlock = true;
        }
    }

    if(!deadlock)
        cout << "None";

    cout << endl;
}

int main() {
    int allocation[MAX][MAX], max[MAX][MAX], available[MAX], request[MAX][MAX];

    cout << "Enter number of processes: ";
    cin >> n;

    cout << "Enter number of resource types: ";
    cin >> m;

    cout << "\nEnter Allocation Matrix:\n";
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            cin >> allocation[i][j];

    cout << "\nEnter Max Matrix:\n";
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            cin >> max[i][j];

    cout << "\nEnter Available Resources:\n";
    for(int i = 0; i < m; i++)
        cin >> available[i];

    cout << "\nEnter Request Matrix (for Deadlock Detection):\n";
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            cin >> request[i][j];

    int choice;

    do {
        cout << "\nMenu:\n";
        cout << "1. Check Safe State (Banker's Algorithm)\n";
        cout << "2. Deadlock Detection\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1:
                bankersAlgorithm(allocation, max, available);
                break;
            case 2:
                deadlockDetection(allocation, request, available);
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice\n";
        }

    } while(choice != 3);

    return 0;
}