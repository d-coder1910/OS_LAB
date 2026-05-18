#include<iostream>
#include<vector>
#include<algorithm>
#include<climits>
#include<queue>

using namespace std;

struct Process{
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int rt;
    int priority;
    bool completed;    
};

bool compareAT(Process a,Process b){
    return a.at <b.at;
}

void printTable(Process p[],int n)
{
    float totalWT=0,totalTAT=0;

    cout<<"\nPID\tAT\tBT\tCT\tTAT\tWT\n";

    for(int i=0;i<n;i++)
    {
        cout<<p[i].pid<<"\t"<<p[i].at<<"\t"<<p[i].bt<<"\t"
        <<p[i].ct<<"\t"<<p[i].tat<<"\t"<<p[i].wt<<endl;

        totalWT+=p[i].wt;
        totalTAT+=p[i].tat;
    }

    cout<<"\nAverage Waiting Time = "<<totalWT/n;
    cout<<"\nAverage Turnaround Time = "<<totalTAT/n<<endl;
}

void ganttChart(vector<int> order)
{
    cout<<"\nGantt Chart:\n|";

    int prev=-1;
    for(int x:order)
    {
        if(x!=prev)
            cout<<" P"<<x<<" |";
        prev=x;
    }
    cout<<endl;
}

void FCFS(Process p[],int n){
    vector<int> order ;
    int time=0;
    int completed=0;
    Process temp[n];
    copy(p,p+n,temp);
    sort(temp,temp+n,compareAT);
    for(int i=0; i<n ;i++){
        if(time<temp[i].at){
            time=temp[i].at;
        }
        time+=temp[i].bt;
        temp[i].completed=true;
        temp[i].ct=time;
        order.push_back(temp[i].pid);
        temp[i].tat=temp[i].ct-temp[i].at;
        temp[i].wt=temp[i].tat-temp[i].bt;

    }

    for(int i=0;i<n;i++){

        for(int j=0;j<n;j++){

            if(p[i].pid == temp[j].pid){

                p[i].ct = temp[j].ct;

                p[i].tat = temp[j].tat;

                p[i].wt = temp[j].wt;

                break;
            }
        }
    }
    ganttChart(order);
    printTable(p,n);
}

void SJF_NP(Process p[],int n){
    vector<int> order;
    int time=0;
    int completed=0;
    while(completed<n){
        int idx=-1;
        int minBT=INT_MAX;
        for(int i=0; i<n ; i++){
            if(p[i].at<=time && !p[i].completed && (p[i].bt<minBT ||(p[i].bt == minBT && (idx==-1 || p[i].at < p[idx].at)))){
                minBT=p[i].bt;
                idx=i;
            }
        }
        if(idx==-1){
            time++;
            continue;
        }
        order.push_back(p[idx].pid);
        time+=p[idx].bt;
        p[idx].ct=time;
        p[idx].completed=true;
        completed++;
        p[idx].tat=p[idx].ct-p[idx].at;
        p[idx].wt=p[idx].tat-p[idx].bt; 
    }

    ganttChart(order);
    printTable(p,n);
}

void SJF_P(Process p[], int n){
    vector<int> order;
    int time=0;
    int completed=0;

    while(completed<n){
        int idx=-1;
        int minRT=INT_MAX;

        for(int i=0; i<n;i++){
            if(p[i].at<=time && !p[i].completed && p[i].rt>0 ){
                if(p[i].rt<minRT || (p[i].rt==minRT &&(idx==-1 || p[i].at<p[idx].at))){
                    minRT=p[i].rt;
                    idx=i;
                }
            }
        }

        if(idx==-1){
            time++;
            continue;
        }
        order.push_back(p[idx].pid);
        time++;
        p[idx].rt--;

        if(p[idx].rt==0){
            p[idx].ct=time;
            p[idx].completed=true;
            p[idx].tat=p[idx].ct-p[idx].at;
            p[idx].wt=p[idx].tat-p[idx].bt; 
            completed++;
        }
        
    }

    ganttChart(order);
    printTable(p,n);
}  



void Priority_NP(Process p[],int n){
    vector<int> order;
    int time=0;
    int completed=0;

    while(completed<n){
        int idx=-1;
        int high=INT_MAX;

        for(int i=0; i<n;i++){
            if(p[i].at<=time && !p[i].completed && (p[i].priority<high || (p[i].priority == high && (idx==-1 || p[i].at < p[idx].at)))){
                high=p[i].priority;
                idx=i;
            }
        }

        if(idx==-1){
            time++;
            continue;
        }

        order.push_back(p[idx].pid);
        time+=p[idx].bt;
        p[idx].completed=true;
        p[idx].ct=time;
        p[idx].tat=p[idx].ct-p[idx].at;
        p[idx].wt=p[idx].tat-p[idx].bt; 
        completed++;
    }
    ganttChart(order);
    printTable(p,n);
}


void preemptive_p(Process p[] , int n){
    vector<int> order;
    int time=0;
    int completed=0;

    while(completed<n){
        int idx=-1;
        int high=INT_MAX;

        for(int i=0; i<n;i++){
            if(p[i].at<=time && !p[i].completed && p[i].rt>0){
                if(p[i].priority<high ||(p[i].priority==high&& (idx==-1 || p[i].at <p[idx].at))){
                    high=p[i].priority;
                    idx=i;
                }
            }
        }
        if(idx==-1){
            time++;
            continue;
        }

        time++;
        p[idx].rt--;

        order.push_back(p[idx].pid);
        if(p[idx].rt==0){
            p[idx].completed=true;
            p[idx].ct=time;
            p[idx].tat=p[idx].ct-p[idx].at;
            p[idx].wt=p[idx].tat-p[idx].bt; 
            completed++;
        }
    }
    ganttChart(order);
    printTable(p,n);
}

void roundRobin(Process p[], int n, int tq){
    queue<int> q;
    vector<int> order;
    int time=INT_MAX;
    int completed=0;
    bool visited[n]={false};
    for(int i=0; i<n ; i++){
        time=min(time,p[i].at);
    }
    for(int i=0; i<n ; i++){
        if(p[i].at<=time && !visited[i]){
            q.push(i);
            visited[i]=true;
        }
    }

    while(completed<n){

        if(q.empty()){
            time++;
            for(int j=0; j<n; j++){
                if(p[j].at<=time && !visited[j]){
                    q.push(j);
                    visited[j]=true;
                }
            }
            continue;
        }
        int i=q.front();
        q.pop();

        if(p[i].rt<=tq){
            time+=p[i].rt;
            p[i].rt=0;
            completed++;
            order.push_back(p[i].pid);
            p[i].completed=true;
            p[i].ct=time;
            p[i].tat=p[i].ct-p[i].at;
            p[i].wt=p[i].tat-p[i].bt;
        }

        else{
            order.push_back(p[i].pid);
            time+=tq;
            p[i].rt-=tq;
        }

        if(p[i].rt>0){
            q.push(i);
        }

        for(int j=0;j<n;j++){
            if(p[j].at<=time && !visited[j]){
                q.push(j);
                
                visited[j]=true;
            }
        }

    }
    ganttChart(order);
    printTable(p,n);

    
}



int main(){

    int n;

    cout<<"Enter number of processes: ";
    cin>>n;

    Process p[n];

    cout<<"\nEnter Process Details:\n";
    cout<<"PID AT BT PRIORITY\n";

    for(int i=0;i<n;i++){

        cin>>p[i].pid
           >>p[i].at
           >>p[i].bt
           >>p[i].priority;
    }

    int choice;

    do{

        cout<<"\n===== CPU Scheduling Algorithms =====\n";

        cout<<"1. FCFS\n";
        cout<<"2. SJF Non-Preemptive\n";
        cout<<"3. SJF Preemptive (SRTF)\n";
        cout<<"4. Priority Non-Preemptive\n";
        cout<<"5. Priority Preemptive\n";
        cout<<"6. Round Robin\n";
        cout<<"7. Exit\n";

        cout<<"\nEnter choice: ";
        cin>>choice;

        // Initialize before every algorithm
        for(int i=0;i<n;i++){

            p[i].completed=false;

            p[i].rt=p[i].bt;

            p[i].ct=0;

            p[i].tat=0;

            p[i].wt=0;
        }

        switch(choice){

            case 1:
                FCFS(p,n);
                break;

            case 2:
                SJF_NP(p,n);
                break;

            case 3:
                SJF_P(p,n);
                break;

            case 4:
                Priority_NP(p,n);
                break;

            case 5:
                preemptive_p(p,n);
                break;

            case 6:{

                int tq;

                cout<<"Enter Time Quantum: ";
                cin>>tq;

                roundRobin(p,n,tq);

                break;
            }

            case 7:
                cout<<"Exiting...\n";
                break;

            default:
                cout<<"Invalid Choice!\n";
        }

    }while(choice!=7);

    return 0;
}