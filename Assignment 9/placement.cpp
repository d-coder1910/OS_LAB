#include <iostream>
#include <vector>
using namespace std;

struct Block {
    int id;
    int size;
};
 
struct Process {
    int id;
    int size;
    int allocatedBlockId = -1;
};

void displayResults(vector<Block>& blocks, vector<Process>& processes) {
    cout << "\nProcess\tSize\tBlock\n";
    for (auto& p : processes) {
        cout << "P" << p.id << "\t" << p.size << "\t";
        if (p.allocatedBlockId != -1)
            cout << "B" << p.allocatedBlockId;
        else
            cout << "Not Allocated";
        cout << endl;
    }

    cout << "\nRemaining Block Sizes:\n";
    for (auto& b : blocks) {
        cout << "B" << b.id << " : " << b.size << endl;
    }
}

void firstFit(vector<Block> blocks, vector<Process> processes) {
    for (auto& p : processes) {
        for (auto& b : blocks) {
            if (b.size >= p.size) {
                p.allocatedBlockId = b.id;
                b.size -= p.size;
                break;
            }
        }
    }
    cout << "\nFirst Fit:\n";
    displayResults(blocks, processes);
}

void bestFit(vector<Block> blocks, vector<Process> processes) {
    for (auto& p : processes) {
        int best = -1;

        for (int i = 0; i < blocks.size(); i++) {
            if (blocks[i].size >= p.size) {
                if (best == -1 || blocks[i].size < blocks[best].size)
                    best = i;
            }
        }

        if (best != -1) {
            p.allocatedBlockId = blocks[best].id;
            blocks[best].size -= p.size;
        }
    }

    cout << "\nBest Fit:\n";
    displayResults(blocks, processes);
}

void worstFit(vector<Block> blocks, vector<Process> processes) {
    for (auto& p : processes) {
        int worst = -1;

        for (int i = 0; i < blocks.size(); i++) {
            if (blocks[i].size >= p.size) {
                if (worst == -1 || blocks[i].size > blocks[worst].size)
                    worst = i;
            }
        }

        if (worst != -1) {
            p.allocatedBlockId = blocks[worst].id;
            blocks[worst].size -= p.size;
        }
    }

    cout << "\nWorst Fit:\n";
    displayResults(blocks, processes);
}

void nextFit(vector<Block> blocks, vector<Process> processes) {
    int last = 0;

    for (auto& p : processes) {
        int count = 0;

        while (count < blocks.size()) {
            if (blocks[last].size >= p.size) {
                p.allocatedBlockId = blocks[last].id;
                blocks[last].size -= p.size;
                last = (last + 1) % blocks.size();
                break;
            }
            last = (last + 1) % blocks.size();
            count++;
        }
    }

    cout << "\nNext Fit:\n";
    displayResults(blocks, processes);
}

int main() {
    int nBlocks, nProcesses;

    cout << "Enter number of blocks: ";
    cin >> nBlocks;

    vector<Block> blocks(nBlocks);
    cout << "Enter block sizes:\n";
    for (int i = 0; i < nBlocks; i++) {
        blocks[i].id = i + 1;
        cin >> blocks[i].size;
    }

    cout << "Enter number of processes: ";
    cin >> nProcesses;

    vector<Process> processes(nProcesses);
    cout << "Enter process sizes:\n";
    for (int i = 0; i < nProcesses; i++) {
        processes[i].id = i + 1;
        cin >> processes[i].size;
    }

    int choice;

    while (true) {
        cout << "\n1. First Fit\n2. Best Fit\n3. Worst Fit\n4. Next Fit\n5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 5) break;

        switch (choice) {
            case 1: firstFit(blocks, processes); break;
            case 2: bestFit(blocks, processes); break;
            case 3: worstFit(blocks, processes); break;
            case 4: nextFit(blocks, processes); break;
            default: cout << "Invalid choice!\n";
        }
    }

    return 0;
}