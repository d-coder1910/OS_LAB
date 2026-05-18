#include <bits/stdc++.h>
using namespace std;

vector<string> M(100, "    ");

string IR = "    ";
string R = "    ";

int IC = 0;
bool C = false;

int SI = 0;
int PI = 0;
int TI = 0;

int TTC = 0;
int LLC = 0;
int TTL = 50;
int TLL = 10;

bool terminated = false;

ifstream fin("input.txt");
ofstream fout("output.txt");

map<int, string> ERRORS = {
    {0, "NO ERROR"},
    {1, "OUT OF DATA"},
    {2, "LINE LIMIT EXCEEDED"},
    {3, "TIME LIMIT EXCEEDED"},
    {4, "OPERATION CODE ERROR"},
    {5, "OPERAND ERROR"}
};

bool validOperand(string op) {
    return op.length() == 2 &&
           isdigit(op[0]) &&
           isdigit(op[1]);
}

void terminate(int EM) {

    fout << "\nPROGRAM TERMINATED\n";
    fout << ERRORS[EM] << "\n";

    fout << "IC  : " << IC << "\n";
    fout << "IR  : " << IR << "\n";
    fout << "TTC : " << TTC << "\n";
    fout << "LLC : " << LLC << "\n\n";

    terminated = true;
}

void READ(int addr) {

    string line;

    if (!getline(fin, line)) {
        terminate(1);
        return;
    }

    if (line.substr(0, 4) == "$END") {
        terminate(1);
        return;
    }

    while (line.length() < 40)
        line += ' ';

    int k = 0;

    for (int i = addr; i < addr + 10; i++) {
        M[i] = line.substr(k, 4);
        k += 4;
    }

    SI = 0;
}

void WRITE(int addr) {

    if (LLC >= TLL) {
        terminate(2);
        return;
    }

    string data = "";

    for (int i = addr; i < addr + 10; i++)
        data += M[i];

    fout << data << "\n";

    LLC++;
    SI = 0;
}

void MOS(int addr) {

    if (TI == 2) {
        terminate(3);
        return;
    }

    if (PI == 1) {
        terminate(4);
        return;
    }

    if (PI == 2) {
        terminate(5);
        return;
    }

    if (SI == 1)
        READ(addr);

    else if (SI == 2)
        WRITE(addr);

    else if (SI == 3)
        terminate(0);
}

void simulation() {

    TTC++;

    if (TTC > TTL)
        TI = 2;

    if (SI || PI || TI)
        MOS(0);
}

void executeUserProgram() {

    while (!terminated) {

        IR = M[IC];
        IC++;

        string opcode = IR.substr(0, 2);
        string operand = IR.substr(2, 2);

        if (IR == "H   ") {
            SI = 3;
            simulation();
            continue;
        }

        if (!validOperand(operand)) {
            PI = 2;
            simulation();
            continue;
        }

        int addr = stoi(operand);

        if (opcode == "LR") {

            R = M[addr];
        }

        else if (opcode == "SR") {

            M[addr] = R;
        }

        else if (opcode == "CR") {

            C = (R == M[addr]);
        }

        else if (opcode == "BT") {

            if (C)
                IC = addr;
        }

        else if (opcode == "GD") {

            SI = 1;
            MOS(addr);
        }

        else if (opcode == "PD") {

            SI = 2;
            MOS(addr);
        }

        else {

            PI = 1;
        }
        if (terminated)
            break;

        simulation();
    }
}

void load() {

    string line;
    int mem_ptr = 0;

    while (getline(fin, line)) {

        if (line.substr(0, 4) == "$AMJ") {

            IC = 0;
            TTC = 0;
            LLC = 0;

            terminated = false;

            for (int i = 0; i < 100; i++)
                M[i] = "    ";

            mem_ptr = 0;

            TTL = stoi(line.substr(8, 4));
            TLL = stoi(line.substr(12, 4));
        }

        else if (line.substr(0, 4) == "$DTA") {

            executeUserProgram();
        }

        else if (line.substr(0, 4) == "$END") {

            continue;
        }

        else {

            while (line.length() < 40)
                line += ' ';

            int k = 0;

            for (int i = 0; i < 10; i++) {

                M[mem_ptr] = line.substr(k, 4);

                k += 4;
                mem_ptr++;
            }
        }
    }
}

int main() {

    load();

    fin.close();
    fout.close();

    cout << "EXECUTION COMPLETED\n";

    return 0;
}
