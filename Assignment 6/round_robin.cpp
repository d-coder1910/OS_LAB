#include<iostream>
#include<vector>
#include<queue>
#include<climits>

using namespace std;

struct Process
{
    int pid;
    int at;
    int bt;

    int ct;
    int tat;
    int wt;
    int rt;

    bool completed;
};



void printTable(vector<Process> &p)
{
    float totalWT = 0;
    float totalTAT = 0;

    int n = p.size();

    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";

    for(int i = 0; i < n; i++)
    {
        cout << p[i].pid << "\t"
             << p[i].at << "\t"
             << p[i].bt << "\t"
             << p[i].ct << "\t"
             << p[i].tat << "\t"
             << p[i].wt << endl;

        totalWT += p[i].wt;
        totalTAT += p[i].tat;
    }

    cout << "\nAverage Waiting Time = " 
         << totalWT / n;

    cout << "\nAverage Turnaround Time = " 
         << totalTAT / n << endl;
}



void ganttChart(vector<int> order)
{
    cout << "\nGantt Chart:\n|";

    int prev = -1;

    for(int x : order)
    {
        if(x != prev)
            cout << " P" << x << " |";

        prev = x;
    }

    cout << endl;
}



void resetProcesses(vector<Process> &p)
{
    for(int i = 0; i < p.size(); i++)
    {
        p[i].completed = false;

        p[i].rt = p[i].bt;

        p[i].ct = 0;
        p[i].tat = 0;
        p[i].wt = 0;
    }
}



void RoundRobin(vector<Process> &p, int tq)
{
    resetProcesses(p);

    int n = p.size();

    queue<int> q;

    vector<int> order;

    int completed = 0;

    int time = INT_MAX;

    vector<bool> visited(n, false);

  
    for(int i = 0; i < n; i++)
    {
        time = min(time, p[i].at);
    }

    
    for(int i = 0; i < n; i++)
    {
        if(p[i].at <= time && !visited[i])
        {
            q.push(i);
            visited[i] = true;
        }
    }

    while(completed < n)
    {
    
        if(q.empty())
        {
            time++;

            for(int j = 0; j < n; j++)
            {
                if(p[j].at <= time && !visited[j])
                {
                    q.push(j);
                    visited[j] = true;
                }
            }

            continue;
        }

        int i = q.front();
        q.pop();

        order.push_back(p[i].pid);

   
        if(p[i].rt <= tq)
        {
            time += p[i].rt;

            p[i].rt = 0;

            p[i].completed = true;

            completed++;

            p[i].ct = time;

            p[i].tat = p[i].ct - p[i].at;

            p[i].wt = p[i].tat - p[i].bt;
        }

       
        else
        {
            time += tq;

            p[i].rt -= tq;
        }

      
        for(int j = 0; j < n; j++)
        {
            if(p[j].at <= time && !visited[j])
            {
                q.push(j);

                visited[j] = true;
            }
        }

       
        if(p[i].rt > 0)
        {
            q.push(i);
        }
    }

    cout << "\n===== Round Robin Scheduling =====\n";

    ganttChart(order);

    printTable(p);
}



int main()
{
    int n;

    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);

    cout << "\nEnter Process Details:\n";
    cout << "PID AT BT\n";

    for(int i = 0; i < n; i++)
    {
        cin >> p[i].pid
            >> p[i].at
            >> p[i].bt;
    }

    int tq;

    cout << "\nEnter Time Quantum: ";
    cin >> tq;

    RoundRobin(p, tq);

    return 0;
}