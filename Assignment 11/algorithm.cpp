#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

void fifo(vector<int> pages, int frames) {
    queue<int> q;
    vector<int> mem;
    int faults = 0;

    for (int p : pages) {
        if (find(mem.begin(), mem.end(), p) == mem.end()) {
            faults++;
            if (mem.size() < frames) {
                mem.push_back(p);
                q.push(p);
            } else {
                int val = q.front(); q.pop();
                mem.erase(find(mem.begin(), mem.end(), val));
                mem.push_back(p);
                q.push(p);
            }
        }
    }

    int hits = pages.size() - faults;
    float ratio = (float)hits / pages.size();

    cout << "FIFO\t\t" << faults << "\t" << hits << "\t" << ratio << endl;
}

void lru(vector<int> pages, int frames) {
    vector<int> mem;
    vector<int> recent;
    int faults = 0;

    for (int i = 0; i < pages.size(); i++) {
        int p = pages[i];
        auto it = find(mem.begin(), mem.end(), p);

        if (it == mem.end()) {
            faults++;
            if (mem.size() < frames) {
                mem.push_back(p);
                recent.push_back(i);
            } else {
                int lru_index = 0;
                for (int j = 1; j < frames; j++) {
                    if (recent[j] < recent[lru_index])
                        lru_index = j;
                }
                mem[lru_index] = p;
                recent[lru_index] = i;
            }
        } else {
            int idx = it - mem.begin();
            recent[idx] = i;
        }
    }

    int hits = pages.size() - faults;
    float ratio = (float)hits / pages.size();

    cout << "LRU\t\t" << faults << "\t" << hits << "\t" << ratio << endl;
}
void optimal(vector<int> pages, int frames) {
    vector<int> mem;
    int faults = 0;

    for (int i = 0; i < pages.size(); i++) {
        int p = pages[i];

        if (find(mem.begin(), mem.end(), p) == mem.end()) {
            faults++;
            if (mem.size() < frames) {
                mem.push_back(p);
            } else {
                int farthest = -1, index = -1;

                for (int j = 0; j < frames; j++) {
                    int k;
                    for (k = i + 1; k < pages.size(); k++) {
                        if (mem[j] == pages[k]) break;
                    }
                    if (k > farthest) {
                        farthest = k;
                        index = j;
                    }
                }
                mem[index] = p;
            }
        }
    }

    int hits = pages.size() - faults;
    float ratio = (float)hits / pages.size();

    cout << "Optimal\t\t" << faults << "\t" << hits << "\t" << ratio << endl;
}

int main() {
    int n, frames;

    cout << "Enter number of pages: ";
    cin >> n;

    vector<int> pages(n);

    cout << "Enter reference string (space separated):\n";
    for (int i = 0; i < n; i++) {
        cin >> pages[i];
    }

    cout << "Enter number of frames: ";
    cin >> frames;

    cout << "\nReference String: ";
    for (int p : pages) cout << p << " ";
    cout << endl;

    cout << "\nAlgorithm\tFaults\tHits\tHit Ratio\n";

    fifo(pages, frames);
    lru(pages, frames);
    optimal(pages, frames);

    return 0;
}
