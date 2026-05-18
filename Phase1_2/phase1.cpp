#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

char M[100][4];

char IR[4];   
char R[4];    
int IC;       
bool C;       

char buffer[40];

int SI;

ifstream fin("input4.txt");
ofstream fout("output.txt");

void LOAD();
void INIT();
void EXECUTE();
void MOS();
void READ();
void WRITE();
void TERMINATE();
void PRINTMEMORY();

void LOAD()
{
    int m = 0;

    while(fin.getline(buffer, 100))
    {
        if(strncmp(buffer, "$AMJ", 4) == 0)
        {
            INIT();
            m = 0;
        }

        else if(strncmp(buffer, "$DTA", 4) == 0)
        {
            IC = 0;
            EXECUTE();
        }

        else if(strncmp(buffer, "$END", 4) == 0)
        {
            continue;
        }

        else
        {
            int len = strlen(buffer);

            int k = 0;

            while(k < len)
            {
                
                for(int j = 0; j < 4; j++)
                    M[m][j] = ' ';

                
                for(int j = 0; j < 4 && k < len; j++)
                {
                    M[m][j] = buffer[k++];
                }

                m++;
            }
        }
    }
}

void INIT() {
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 4; j++)
            M[i][j] = ' ';

    IC = 0;
    C = false;
    memset(IR, ' ', 4);
    memset(R, ' ', 4);
}

void EXECUTE() {
    while (true) {
        for (int i = 0; i < 4; i++)
            IR[i] = M[IC][i];

        IC++;

        int addr = (IR[2] - '0') * 10 + (IR[3] - '0');

        if (IR[0] == 'L' && IR[1] == 'R') {
            for (int i = 0; i < 4; i++)
                R[i] = M[addr][i];
        }
        else if (IR[0] == 'S' && IR[1] == 'R') {
            for (int i = 0; i < 4; i++)
                M[addr][i] = R[i];
        }
        else if (IR[0] == 'C' && IR[1] == 'R') {
            C = true;
            for (int i = 0; i < 4; i++) {
                if (R[i] != M[addr][i]) {
                    C = false;
                    break;
                }
            }
        }
        else if (IR[0] == 'B' && IR[1] == 'T') {
            if (C == true)
                IC = addr;
        }
        else if (IR[0] == 'G' && IR[1] == 'D') {
            SI = 1;
            MOS();
        }
        else if (IR[0] == 'P' && IR[1] == 'D') {
            SI = 2;
            MOS();
        }
        else if (IR[0] == 'H' && IR[1] == ' ') {
            SI = 3;
            MOS();
            break;
        }
    }
}

void MOS() {
    switch (SI) {
        case 1: READ(); break;
        case 2: WRITE(); break;
        case 3: TERMINATE(); break;
    }
}

void READ() {
    fin.getline(buffer, 40);

    int addr = (IR[2] - '0') * 10 + (IR[3] - '0');

    int k = 0;
    for (int i = addr; i < addr + 10; i++) {
        for (int j = 0; j < 4; j++) {
            if (buffer[k] != '\0')
                M[i][j] = buffer[k++];
            else
                M[i][j] = ' ';
        }
    }
}

void WRITE() {
    int addr = (IR[2] - '0') * 10 + (IR[3] - '0');

    for (int i = addr; i < addr + 10; i++) {
        for (int j = 0; j < 4; j++)
            fout << M[i][j];
    }
    fout << endl;
}

void TERMINATE() {
    PRINTMEMORY();
    fout << endl << endl;
}

void PRINTMEMORY() {
    cout << "\nMEMORY DUMP\n\n";

    for (int i = 0; i < 100; i++) {
        cout << i << " : ";

        for (int j = 0; j < 4; j++) {
            if (M[i][j] >= 32 && M[i][j] <= 126)
                cout << M[i][j];
            else
                cout << ' ';
        }

        cout << endl;
    }
}

int main() {
    LOAD();
    fin.close();
    fout.close();
    cout << "Execution Completed. Check output.txt file." << endl;
    return 0;
}
