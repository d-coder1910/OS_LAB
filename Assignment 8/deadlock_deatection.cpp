#include <iostream>
using namespace std;

#define MAX 10

int n, m;

void deadlockDetection(int allocation[MAX][MAX],
                       int request[MAX][MAX],
                       int available[MAX])
{
    bool finish[MAX];

    int work[MAX];

    for(int i = 0; i < m; i++)
    {
        work[i] = available[i];
    }

    for(int i = 0; i < n; i++)
    {
        bool zeroAlloc = true;

        for(int j = 0; j < m; j++)
        {
            if(allocation[i][j] != 0)
            {
                zeroAlloc = false;
                break;
            }
        }

        finish[i] = zeroAlloc;
    }

    while(true)
    {
        bool found = false;

        for(int i = 0; i < n; i++)
        {
            if(!finish[i])
            {
                bool possible = true;

                for(int j = 0; j < m; j++)
                {
                    if(request[i][j] > work[j])
                    {
                        possible = false;
                        break;
                    }
                }

                if(possible)
                {
                    for(int j = 0; j < m; j++)
                    {
                        work[j] += allocation[i][j];
                    }

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

    for(int i = 0; i < n; i++)
    {
        if(!finish[i])
        {
            cout << "P" << i << " ";
            deadlock = true;
        }
    }

    if(!deadlock)
    {
        cout << "None";
    }

    cout << endl;
}

int main()
{
    int allocation[MAX][MAX];
    int request[MAX][MAX];
    int available[MAX];

    cout << "Enter number of processes: ";
    cin >> n;

    cout << "Enter number of resource types: ";
    cin >> m;

    cout << "\nEnter Allocation Matrix:\n";

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            cin >> allocation[i][j];
        }
    }

    cout << "\nEnter Request Matrix:\n";

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            cin >> request[i][j];
        }
    }

    cout << "\nEnter Available Resources:\n";

    for(int i = 0; i < m; i++)
    {
        cin >> available[i];
    }

    deadlockDetection(allocation, request, available);

    return 0;
}