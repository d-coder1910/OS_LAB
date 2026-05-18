#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;


void FCFS(vector<int> req, int head) {
    int seek = 0;
    cout << head << " -> ";
    for (int i = 0; i < req.size(); i++) {
        seek += abs(req[i] - head);
        head = req[i];
        cout << head << " -> ";
    }
    cout << "FCFS Seek Time: " << seek << endl;
}


void SSTF(vector<int> req, int head) {
    int n = req.size(), seek = 0;
    vector<bool> visited(n, false);
    cout << head << " -> ";

    for (int i = 0; i < n; i++) {
        int minDist = 1e9, index = -1;

        for (int j = 0; j < n; j++) {
            if (!visited[j]&&req[j] != head) {
                int dist = abs(req[j] - head);
                if (dist < minDist) {
                    minDist = dist;
                    index = j;
                }
            }
        }

        visited[index] = true;
        seek += minDist;
        head = req[index];
        cout << head << " -> ";
    }

    cout << "SSTF Seek Time: " << seek << endl;
}



void SCAN(vector<int> req, int head, int disk_size) {
    int seek = 0;
    vector<int> left, right;
    cout << head << " -> ";

    for (int i : req) {
        if (i == head)
            continue;
        if (i < head) left.push_back(i);
        else right.push_back(i);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    for (int i : right) {
        seek += abs(i - head);
        head = i;
        cout << head << " -> ";
    }

    seek += abs((disk_size - 1) - head);
    
    head = disk_size - 1;
    cout << head << " -> ";

    for (int i = (int)left.size() - 1; i >= 0; i--) {
        seek += abs(left[i] - head);
        head = left[i];
        cout << head << " -> ";
    }

    cout << "SCAN Seek Time: " << seek << endl;
}


void CSCAN(vector<int> req, int head, int disk_size) {
    int seek = 0;
    vector<int> left, right;
    cout << head << " -> ";

    for (int i : req) {
        if (i == head)
            continue;
        if (i < head) left.push_back(i);
        else right.push_back(i);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    for (int i : right) {
        seek += abs(i - head);
        head = i;
        cout << head << " -> ";
    }

    seek += abs((disk_size - 1) - head);
    cout << disk_size-1 << " -> ";
    head = 0;
    cout << head << " -> ";
    seek += (disk_size - 1);

    for (int i : left) {
        seek += abs(i - head);
        head = i;
        cout << head << " -> ";
    }

    cout << "C-SCAN Seek Time: " << seek << endl;
}

int main() {
    int n, head, disk_size;

    cout << "Enter number of requests: ";
    cin >> n;

    vector<int> req(n);
    cout << "Enter request queue:\n";
    for (int i = 0; i < n; i++)
        cin >> req[i];

    cout << "Enter initial head position: ";
    cin >> head;

    cout << "Enter disk size: ";
    cin >> disk_size;

    FCFS(req, head);
    SSTF(req, head);
    SCAN(req, head, disk_size);
    CSCAN(req, head, disk_size);

    return 0;
}
