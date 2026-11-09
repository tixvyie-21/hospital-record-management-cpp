// Hospital Patient Record System
// DSA concepts: Linked List, Searching, Sorting (Merge Sort)

#include <iostream>
#include <limits>
#include <string>

#include "PatientList.h"

using namespace std;

static const string DATA_FILE = "patients.txt";

// ---------- input helpers ----------
static string readLine(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

static string readNonEmpty(const string& prompt) {
    string s;
    do {
        s = readLine(prompt);
        if (s.empty()) cout << "  Value cannot be empty.\n";
    } while (s.empty());
    return s;
}

// Reads an integer in [lo, hi]. If allowBlank is true, a blank line returns -1.
static int readInt(const string& prompt, int lo, int hi, bool allowBlank = false) {
    while (true) {
        string s = readLine(prompt);
        if (s.empty() && allowBlank) return -1;
        try {
            size_t pos;
            int v = stoi(s, &pos);
            if (pos == s.size() && v >= lo && v <= hi) return v;
        } catch (...) {}
        cout << "  Enter a number between " << lo << " and " << hi << ".\n";
    }
}

static bool validDate(const string& d) {  // DD-MM-YYYY
    if (d.size() != 10 || d[2] != '-' || d[5] != '-') return false;
    for (size_t i = 0; i < d.size(); ++i)
        if (i != 2 && i != 5 && !isdigit(static_cast<unsigned char>(d[i]))) return false;
    int day = stoi(d.substr(0, 2)), month = stoi(d.substr(3, 2));
    return day >= 1 && day <= 31 && month >= 1 && month <= 12;
}

static string readDate(const string& prompt) {
    while (true) {
        string d = readNonEmpty(prompt);
        if (validDate(d)) return d;
        cout << "  Use the format DD-MM-YYYY.\n";
    }
}

// ---------- menu actions ----------
static void registerPatient(PatientList& list) {
    cout << "\n--- Register Patient ---\n";
    int id = readInt("Patient ID: ", 1, 999999);
    if (list.searchById(id)) { cout << "A patient with this ID already exists.\n"; return; }
    string name    = readNonEmpty("Name: ");
    int age        = readInt("Age: ", 0, 130);
    string disease = readNonEmpty("Disease: ");
    string doctor  = readNonEmpty("Doctor: ");
    string date    = readDate("Admission date (DD-MM-YYYY): ");

    list.addPatient(id, name, age, disease, doctor, date);
    cout << "Patient registered successfully.\n";
}

static void updatePatient(PatientList& list) {
    cout << "\n--- Update Patient ---\n";
    int id = readInt("Patient ID: ", 1, 999999);
    if (!list.searchById(id)) { cout << "Patient not found.\n"; return; }

    cout << "(Press Enter to keep the current value)\n";
    string name    = readLine("New name: ");
    int age        = readInt("New age: ", 0, 130, true);
    string disease = readLine("New disease: ");
    string doctor  = readLine("New doctor: ");

    list.updatePatient(id, name, age, disease, doctor);
    cout << "Record updated.\n";
}

static void dischargePatient(PatientList& list) {
    cout << "\n--- Discharge Patient ---\n";
    int id = readInt("Patient ID: ", 1, 999999);
    cout << (list.dischargePatient(id) ? "Patient discharged and record removed.\n"
                                       : "Patient not found.\n");
}

static void searchMenu(const PatientList& list) {
    cout << "\n--- Search ---\n1. By ID\n2. By Name\n3. By Disease\n";
    int ch = readInt("Choice: ", 1, 3);

    if (ch == 1) {
        int id = readInt("Patient ID: ", 1, 999999);
        Patient* p = list.searchById(id);
        if (!p) { cout << "Patient not found.\n"; return; }
        cout << "\nID: " << p->id << "\nName: " << p->name << "\nAge: " << p->age
             << "\nDisease: " << p->disease << "\nDoctor: " << p->doctor
             << "\nAdmitted: " << p->admissionDate << "\n";
    } else if (ch == 2) {
        if (!list.searchByName(readNonEmpty("Name (partial allowed): "))) cout << "No match found.\n";
    } else {
        if (!list.searchByDisease(readNonEmpty("Disease (partial allowed): "))) cout << "No match found.\n";
    }
}

static void sortMenu(PatientList& list) {
    cout << "\n--- Sort Patients ---\n1. By ID\n2. By Name\n3. By Age\n";
    int ch = readInt("Choice: ", 1, 3);
    list.sortBy(ch == 1 ? SortKey::ID : ch == 2 ? SortKey::NAME : SortKey::AGE);
    cout << "Sorted.\n\n";
    list.displayAll();
}

int main() {
    PatientList list;
    if (list.loadFromFile(DATA_FILE)) cout << "Loaded " << list.size() << " record(s) from " << DATA_FILE << ".\n";

    while (true) {
        cout << "\n===== HOSPITAL PATIENT RECORD SYSTEM =====\n"
             << "1. Register patient\n"
             << "2. Update patient details\n"
             << "3. Discharge patient\n"
             << "4. Search patient\n"
             << "5. Sort patients\n"
             << "6. Doctor-wise report\n"
             << "7. Display all patients\n"
             << "0. Save and exit\n";
        int choice = readInt("Choice: ", 0, 7);

        switch (choice) {
            case 1: registerPatient(list); break;
            case 2: updatePatient(list); break;
            case 3: dischargePatient(list); break;
            case 4: searchMenu(list); break;
            case 5: sortMenu(list); break;
            case 6: list.doctorReport(); break;
            case 7: list.displayAll(); break;
            case 0:
                cout << (list.saveToFile(DATA_FILE) ? "Records saved. Goodbye!\n" : "Could not save records.\n");
                return 0;
        }
    }
}
