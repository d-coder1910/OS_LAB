#include<iostream>
#include<vector>
#include<algorithm>
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
    float totalWT = 0, totalTAT = 0;
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

    cout << "\nAverage Waiting Time = " << totalWT / n;
    cout << "\nAverage Turnaround Time = " << totalTAT / n << endl;
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



void SJF_NP(vector<Process> &p)
{
    resetProcesses(p);

    vector<int> order;

    int time = 0;
    int completed = 0;
    int n = p.size();

    while(completed < n)
    {
        int idx = -1;
        int minBT = INT_MAX;

        for(int i = 0; i < n; i++)
        {
            if(p[i].at <= time &&
               !p[i].completed)
            {
                if(p[i].bt < minBT ||
                  (p[i].bt == minBT &&
                  (idx == -1 || p[i].at < p[idx].at)))
                {
                    minBT = p[i].bt;
                    idx = i;
                }
            }
        }

        if(idx == -1)
        {
            time++;
            continue;
        }

        order.push_back(p[idx].pid);

        time += p[idx].bt;

        p[idx].ct = time;
        p[idx].completed = true;

        completed++;

        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
    }

    cout << "\n===== SJF Non-Preemptive =====\n";

    ganttChart(order);
    printTable(p);
}


void SJF_P(vector<Process> &p)
{
    resetProcesses(p);

    vector<int> order;

    int time = 0;
    int completed = 0;
    int n = p.size();

    while(completed < n)
    {
        int idx = -1;
        int minRT = INT_MAX;

        for(int i = 0; i < n; i++)
        {
            if(p[i].at <= time &&
               !p[i].completed &&
               p[i].rt > 0)
            {
                if(p[i].rt < minRT ||
                  (p[i].rt == minRT &&
                  (idx == -1 || p[i].at < p[idx].at)))
                {
                    minRT = p[i].rt;
                    idx = i;
                }
            }
        }

        if(idx == -1)
        {
            time++;
            continue;
        }

        order.push_back(p[idx].pid);

        time++;

        p[idx].rt--;

        if(p[idx].rt == 0)
        {
            p[idx].ct = time;
            p[idx].completed = true;

            completed++;

            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
        }
    }

    cout << "\n===== SJF Preemptive (SRTF) =====\n";

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

    SJF_NP(p);

    SJF_P(p);

    return 0;
}