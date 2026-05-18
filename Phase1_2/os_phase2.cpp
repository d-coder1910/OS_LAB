#include <bits/stdc++.h>
using namespace std;

vector<string> M(300, "    ");
vector<int> free_frames;

string IR = "    ";
string R = "    ";

int IC = 0;
bool C = false;
int PTR = 0;

int SI = 0;
int PI = 0;
int TI = 0;

int TTC = 0;
int LLC = 0;
int TTL = 0;
int TLL = 0;

struct PCBStruct {
    string job_id;
    int TTL;
    int TLL;
} PCB;

ifstream input_file("input.txt");
ofstream output_file("output.txt");

bool terminated = false;
int operand_va = -1;

map<int, string> TERMINATE_MESSAGES = {
    {0, "NO ERROR"},
    {1, "OUT OF DATA"},
    {2, "LINE LIMIT EXCEEDED"},
    {3, "TIME LIMIT EXCEEDED"},
    {4, "OPERATION CODE ERROR"},
    {5, "OPERAND ERROR"},
    {6, "INVALID PAGE FAULT"}
};

string pad40(string s) {
    while (s.length() < 40)
        s += ' ';
    return s.substr(0, 40);
}

bool is_valid_operand(string op) {
    return op.length() == 2 &&
           isdigit(op[0]) &&
           isdigit(op[1]);
}

string format_frame(int frame) {
    stringstream ss;
    ss << setw(2) << setfill('0') << frame;
    return "  " + ss.str();
}

int allocate() {
    if (free_frames.empty())
        return -1;

    int idx = rand() % free_frames.size();
    int frame = free_frames[idx];
    free_frames.erase(free_frames.begin() + idx);
    return frame;
}

int address_map(int VA) {
    if (VA < 0 || VA >= 100) {
        PI = 2;
        return -1;
    }

    int page = VA / 10;
    int offset = VA % 10;

    string entry = M[PTR * 10 + page];
    entry.erase(remove(entry.begin(), entry.end(), ' '), entry.end());

    if (entry.empty() || entry == "-1") {
        PI = 3;
        return -1;
    }

    int frame = stoi(entry);
    return frame * 10 + offset;
}

void init(string job_id, int ttl, int tll) {
    for (int i = 0; i < 300; i++)
        M[i] = "    ";

    IR = "    ";
    R = "    ";

    IC = 0;
    C = false;

    SI = PI = TI = 0;
    TTC = LLC = 0;

    TTL = ttl;
    TLL = tll;

    operand_va = -1;
    terminated = false;

    PCB = {job_id, ttl, tll};

    PTR = allocate();

    for (int i = 0; i < 10; i++)
        M[PTR * 10 + i] = "  -1";
}

void terminate(int EM, int EM2 = -1) {
    output_file << "\n";
    output_file << "JOB ID : " << PCB.job_id << "\n";
    output_file << TERMINATE_MESSAGES[EM] << "\n";

    if (EM2 != -1)
        output_file << TERMINATE_MESSAGES[EM2] << "\n";

    output_file << "IC  : " << IC << "\n";
    output_file << "IR  : " << IR << "\n";
    output_file << "TTC : " << TTC << "\n";
    output_file << "LLC : " << LLC << "\n\n";

    for (int i = 0; i < 10; i++) {
        string entry = M[PTR * 10 + i];
        entry.erase(remove(entry.begin(), entry.end(), ' '), entry.end());

        if (!entry.empty() && entry != "-1") {
            free_frames.push_back(stoi(entry));
        }
    }

    free_frames.push_back(PTR);
    terminated = true;
}

void READ() {
    string line;

    if (!getline(input_file, line)) {
        terminate(1);
        return;
    }

    if (line.substr(0, 4) == "$END") {
        terminate(1);
        return;
    }

    line = pad40(line);

    int RA = address_map(operand_va);

    if (PI != 0)
        return;

    RA = (RA / 10) * 10;

    for (int i = 0; i < 10; i++) {
        M[RA + i] = line.substr(i * 4, 4);
    }

    SI = 0;
}

void WRITE() {
    if (LLC >= TLL) {
        terminate(2);
        return;
    }

    int RA = address_map(operand_va);

    if (PI != 0)
        return;

    RA = (RA / 10) * 10;

    string data = "";

    for (int i = 0; i < 10; i++)
        data += M[RA + i];

    output_file << data << "\n";

    LLC++;
    SI = 0;
}

void valid_page_fault_handler() {
    int page = operand_va / 10;

    int frame = allocate();

    if (frame == -1) {
        terminate(6);
        return;
    }

    M[PTR * 10 + page] = format_frame(frame);

    PI = 0;
    IC--;
}

void MOS() {
    if (TI == 2) {
        if (PI == 1)
            terminate(3, 4);
        else if (PI == 2)
            terminate(3, 5);
        else if (PI == 3)
            terminate(3);
        else if (SI == 1)
            terminate(3);
        else if (SI == 2) {
            WRITE();
            if (!terminated)
                terminate(3);
        }
        else if (SI == 3)
            terminate(0);
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

    if (PI == 3) {
        string opcode = IR.substr(0, 2);

        if (opcode == "GD" || opcode == "SR") {
            valid_page_fault_handler();
            return;
        }

        terminate(6);
        return;
    }

    if (SI == 1)
        READ();
    else if (SI == 2)
        WRITE();
    else if (SI == 3)
        terminate(0);
}

void simulation() {
    TTC++;

    if (TTC > TTL)
        TI = 2;

    if (SI || PI || TI)
        MOS();
}

void execute_user_program() {
    while (!terminated) {

        PI = 0;

        int RA = address_map(IC);

        if (PI != 0) {
            MOS();

            if (terminated)
                break;

            continue;
        }

        IR = M[RA];
        IC++;

        string opcode = IR.substr(0, 2);
        string operand = IR.substr(2, 2);

        if (IR == "H   ") {
            SI = 3;
            simulation();
            continue;
        }

        if (!is_valid_operand(operand)) {
            PI = 2;
            simulation();
            continue;
        }

        operand_va = stoi(operand);

        int RA_operand = -1;

        if (opcode == "LR" ||
            opcode == "SR" ||
            opcode == "CR" ||
            opcode == "GD" ||
            opcode == "PD") {

            RA_operand = address_map(operand_va);

            if (PI != 0) {
                MOS();

                if (terminated)
                    break;

                continue;
            }
        }

        if (opcode == "LR") {
            R = M[RA_operand];
        }
        else if (opcode == "SR") {
            M[RA_operand] = R;
        }
        else if (opcode == "CR") {
            C = (R == M[RA_operand]);
        }
        else if (opcode == "BT") {
            if (C)
                IC = operand_va;
        }
        else if (opcode == "GD") {
            SI = 1;
        }
        else if (opcode == "PD") {
            SI = 2;
        }
        else {
            PI = 1;
        }

        simulation();
    }
}

void start_execution() {
    IC = 0;
    execute_user_program();
}

void load() {
    string line;
    int page_no = 0;

    while (getline(input_file, line)) {

        line = pad40(line);

        if (line.substr(0, 4) == "$AMJ") {

            string job_id = line.substr(4, 4);
            int ttl = stoi(line.substr(8, 4));
            int tll = stoi(line.substr(12, 4));

            init(job_id, ttl, tll);
            page_no = 0;
        }
        else if (line.substr(0, 4) == "$DTA") {
            start_execution();
        }
        else if (line.substr(0, 4) == "$END") {
            continue;
        }
        else {

            if (page_no >= 10) {
                terminate(6);
                continue;
            }

            int frame = allocate();

            if (frame == -1) {
                terminate(6);
                continue;
            }

            M[PTR * 10 + page_no] = format_frame(frame);

            int RA = frame * 10;

            for (int i = 0; i < 10; i++) {
                M[RA + i] = line.substr(i * 4, 4);
            }

            page_no++;
        }
    }
}

int main() {
    srand(time(0));

    free_frames.resize(30);
    iota(free_frames.begin(), free_frames.end(), 0);

    load();

    input_file.close();
    output_file.close();

    cout << "PHASE-2 MOS EXECUTION COMPLETED\n";

    return 0;
}