#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

struct Process{
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    bool completed=false;
};

bool compareAT(Process a, Process b){
    return a.at < b.at;
}

void printTable(vector<Process> &p)
{
    float totalWT = 0, totalTAT = 0;
    int n = p.size();

    cout<<"\nPID\tAT\tBT\tCT\tTAT\tWT\n";

    for(int i=0;i<n;i++)
    {
        cout<<p[i].pid<<"\t"
            <<p[i].at<<"\t"
            <<p[i].bt<<"\t"
            <<p[i].ct<<"\t"
            <<p[i].tat<<"\t"
            <<p[i].wt<<endl;

        totalWT += p[i].wt;
        totalTAT += p[i].tat;
    }

    cout<<"\nAverage Waiting Time = "<<totalWT/n;
    cout<<"\nAverage Turnaround Time = "<<totalTAT/n<<endl;
}

void ganttChart(vector<int> order)
{
    cout<<"\nGantt Chart:\n|";

    int prev = -1;

    for(int x : order)
    {
        if(x != prev)
            cout<<" P"<<x<<" |";

        prev = x;
    }

    cout<<endl;
}

void FCFS(vector<Process> &p)
{
    vector<int> order;
    int time = 0;
    int n = p.size();

    vector<Process> temp = p;

    sort(temp.begin(), temp.end(), compareAT);

    for(int i=0; i<n; i++)
    {
        if(time < temp[i].at)
            time = temp[i].at;

        time += temp[i].bt;

        temp[i].completed = true;
        temp[i].ct = time;

        order.push_back(temp[i].pid);

        temp[i].tat = temp[i].ct - temp[i].at;
        temp[i].wt = temp[i].tat - temp[i].bt;
    }

    // Copy results back
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(p[i].pid == temp[j].pid)
            {
                p[i].ct = temp[j].ct;
                p[i].tat = temp[j].tat;
                p[i].wt = temp[j].wt;
                break;
            }
        }
    }

    ganttChart(order);
    printTable(p);
}

int main()
{
    int n;

    cout<<"Enter number of processes: ";
    cin>>n;

    vector<Process> p(n);

    cout<<"\nEnter Process Details:\n";
    cout<<"PID AT BT\n";

    for(int i=0;i<n;i++)
    {
        cin>>p[i].pid
           >>p[i].at
           >>p[i].bt;
    }

    FCFS(p);

    return 0;
}