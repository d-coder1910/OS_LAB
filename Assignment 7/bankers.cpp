#include <iostream>
using namespace std;

#define MAX 10

int n, m;

void calculateNeed(int need[MAX][MAX],
                   int maxm[MAX][MAX],
                   int allocation[MAX][MAX])
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            need[i][j] = maxm[i][j] - allocation[i][j];
        }
    }
}

void bankersAlgorithm(int allocation[MAX][MAX],
                      int maxm[MAX][MAX],
                      int available[MAX])
{
    int need[MAX][MAX];

    calculateNeed(need, maxm, allocation);

    bool finish[MAX] = {false};

    int safeSeq[MAX];

    int work[MAX];

    for(int i = 0; i < m; i++)
    {
        work[i] = available[i];
    }

    int count = 0;

    while(count < n)
    {
        bool found = false;

        for(int i = 0; i < n; i++)
        {
            if(!finish[i])
            {
                bool possible = true;

                for(int j = 0; j < m; j++)
                {
                    if(need[i][j] > work[j])
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

                    safeSeq[count++] = i;

                    finish[i] = true;

                    found = true;
                }
            }
        }

        if(!found)
        {
            cout << "\nSystem is in UNSAFE state";
            cout << "\nDeadlock may occur.\n";

            return;
        }
    }

    cout << "\nNeed Matrix:\n";

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            cout << need[i][j] << " ";
        }

        cout << endl;
    }

    cout << "\nSystem is in SAFE state\n";

    cout << "Safe Sequence: ";

    for(int i = 0; i < n; i++)
    {
        cout << "P" << safeSeq[i];

        if(i != n - 1)
            cout << " -> ";
    }

    cout << endl;
}

int main()
{
    int allocation[MAX][MAX];
    int maxm[MAX][MAX];
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

    cout << "\nEnter Max Matrix:\n";

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            cin >> maxm[i][j];
        }
    }

    cout << "\nEnter Available Resources:\n";

    for(int i = 0; i < m; i++)
    {
        cin >> available[i];
    }

    bankersAlgorithm(allocation, maxm, available);

    return 0;
}