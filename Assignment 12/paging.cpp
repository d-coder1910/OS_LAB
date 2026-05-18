#include <iostream>
#include <vector>

using namespace std;

int main() {


    int pageSize, logicalAddress;
    int numPages;

    cout << "===== PAGING =====\n";

    cout << "Enter Page Size: ";
    cin >> pageSize;


    if (pageSize <= 0) {
        cout << "Error: Invalid Page Size!\n";
        return 0;
    }

    cout << "Enter Number of Pages in Page Table: ";
    cin >> numPages;

    if (numPages <= 0) {
        cout << "Error: Invalid Number of Pages!\n";
        return 0;
    }

    vector<int> pageTable(numPages);

    cout << "\nEnter Frame Numbers for each Page:\n";

    for (int i = 0; i < numPages; i++) {
        cout << "Page " << i << " -> Frame Number: ";
        cin >> pageTable[i];

        if (pageTable[i] < 0) {
            cout << "Error: Frame Number cannot be negative!\n";
            return 0;
        }
    }

    cout << "\nEnter Logical Address: ";
    cin >> logicalAddress;

    if (logicalAddress < 0) {
        cout << "Error: Logical Address cannot be negative!\n";
        return 0;
    }

    int pageNumber = logicalAddress / pageSize;
    int offset = logicalAddress % pageSize;

    cout << "\n--- Address Breakdown ---\n";
    cout << "Page Number = " << pageNumber << endl;
    cout << "Offset = " << offset << endl;


    if (pageNumber >= numPages) {
        cout << "\nERROR: Page Fault! Page does not exist in Page Table.\n";
    }
    else {
        int frameNumber = pageTable[pageNumber];

        int physicalAddress = frameNumber * pageSize + offset;

        cout << "\n--- Paging Output ---\n";
        cout << "Frame Number = " << frameNumber << endl;
        cout << "Physical Address = " << physicalAddress << endl;
    }


    cout << "\n\n===== SEGMENTATION =====\n";

    int numSegments;

    cout << "Enter Number of Segments: ";
    cin >> numSegments;

    if (numSegments <= 0) {
        cout << "Error: Invalid Number of Segments!\n";
        return 0;
    }

    vector<int> base(numSegments);
    vector<int> limit(numSegments);

    cout << "\nEnter Segment Table:\n";

    for (int i = 0; i < numSegments; i++) {

        cout << "\nSegment " << i << " Base Address: ";
        cin >> base[i];

        cout << "Segment " << i << " Limit: ";
        cin >> limit[i];


        if (base[i] < 0 || limit[i] <= 0) {
            cout << "Error: Invalid Base or Limit Value!\n";
            return 0;
        }
    }

    int segmentNumber, segOffset;

    cout << "\nEnter Segment Number: ";
    cin >> segmentNumber;

    cout << "Enter Offset: ";
    cin >> segOffset;

    
    if (segmentNumber < 0 || segmentNumber >= numSegments) {
        cout << "\nERROR: Invalid Segment Number!\n";
    }

   
    else if (segOffset < 0) {
        cout << "\nERROR: Offset cannot be negative!\n";
    }


    else if (segOffset >= limit[segmentNumber]) {
        cout << "\nERROR: Segmentation Fault! Offset exceeds limit.\n";
    }

    else {
        int physicalAddress = base[segmentNumber] + segOffset;

        cout << "\n--- Segmentation Output ---\n";
        cout << "Segment Number = " << segmentNumber << endl;
        cout << "Offset = " << segOffset << endl;
        cout << "Base Address = " << base[segmentNumber] << endl;
        cout << "Physical Address = " << physicalAddress << endl;
    }

    return 0;
}
