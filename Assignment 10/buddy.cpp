#include <bits/stdc++.h>
using namespace std;

struct Block {
    int addr;
    int size;
};

class BuddySystem {
    int total_size;
    map<int, vector<int>> free_list; 
    map<int, int> allocated;         

public:
    BuddySystem(int size) {
        total_size = 1;
        while (total_size < size)
            total_size *= 2;

        free_list[total_size].push_back(0);

        cout << "Buddy System Initialized with " << total_size << " KB memory\n";
    }

    int nextPowerOf2(int n) {
        int p = 1;
        while (p < n) p *= 2;
        return p;
    }

    void allocate(int size) {
        if (size <= 0) {
            cout << "Invalid request size!\n";
            return;
        }

        int req = nextPowerOf2(size);
        int current = req;

        while (current <= total_size && free_list[current].empty()) {
            current *= 2;
        }

        if (current > total_size) {
            cout << "Memory not available!\n";
            return;
        }

        while (current > req) {
            int addr = free_list[current].back();
            free_list[current].pop_back();

            current /= 2;

            free_list[current].push_back(addr);
            free_list[current].push_back(addr + current);

            cout << "Split block into " << current << " KB each\n";
        }

        int alloc_addr = free_list[req].back();
        free_list[req].pop_back();

        allocated[alloc_addr] = req;

        cout << "Allocated " << req << " KB at address " << alloc_addr << "\n";
        cout << "Internal Fragmentation: " << req - size << " KB\n";
    }

    void deallocate(int addr) {
        if (allocated.find(addr) == allocated.end()) {
            cout << "Invalid address!\n";
            return;
        }

        int size = allocated[addr];
        allocated.erase(addr);

        int current_addr = addr; 
        int current_size = size;

        cout << "Deallocating block at " << addr << "\n";

        while (current_size < total_size) {
            int buddy = current_addr ^ current_size;

            auto &list = free_list[current_size];
            auto it = find(list.begin(), list.end(), buddy);

            if (it != list.end()) {
                list.erase(it);
                current_addr = min(current_addr, buddy);
                current_size *= 2;

                cout << "Merged to form " << current_size << " KB block\n";
            } else {
                break;
            }
        }

        free_list[current_size].push_back(current_addr);
    }

    void display() {
        cout << "\n--- Free Memory Blocks ---\n";
        for (auto &it : free_list) {
            if (!it.second.empty()) {
                cout << it.first << " KB : ";
                for (int addr : it.second)
                    cout << addr << " ";
                cout << endl;
            }
        }
    }

    void stats() {
        int total_free = 0;
        for (auto &it : free_list) {
            total_free += it.first * it.second.size();
        }

        int used = total_size - total_free;

        cout << "\n--- Memory Analysis ---\n";
        cout << "Total Memory: " << total_size << " KB\n";
        cout << "Used Memory : " << used << " KB\n";
        cout << "Free Memory : " << total_free << " KB\n";

        double utilization = (double)used * 100 / total_size;
        cout << "Memory Utilization: " << fixed << setprecision(2)
             << utilization << "%\n";
    }
};

int main() {
    int size;
    cout << "Enter total memory (KB): ";
    cin >> size;

    BuddySystem bs(size);

    int choice, req, addr;

    while (true) {
        cout << "\n1. Allocate\n2. Deallocate\n3. Display\n4. Stats\n5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter size: ";
            cin >> req;
            bs.allocate(req);
            break;

        case 2:
            cout << "Enter address to free: ";
            cin >> addr;
            bs.deallocate(addr);
            break;

        case 3:
            bs.display();
            break;

        case 4:
            bs.stats();
            break;

        case 5:
            return 0;

        default:
            cout << "Invalid choice!\n";
        }
    }
}