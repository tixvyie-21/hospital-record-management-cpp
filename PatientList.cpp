#include "PatientList.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// ---------- helpers ----------
static string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
    return s;
}

static bool contains(const string& text, const string& pattern) {
    return toLower(text).find(toLower(pattern)) != string::npos;
}

static bool lessById(const Patient* a, const Patient* b)   { return a->id < b->id; }
static bool lessByAge(const Patient* a, const Patient* b)  { return a->age < b->age; }
static bool lessByName(const Patient* a, const Patient* b) { return toLower(a->name) < toLower(b->name); }

// ---------- lifecycle ----------
PatientList::PatientList() : head(nullptr), count(0) {}
PatientList::~PatientList() { clear(); }

void PatientList::clear() {
    while (head) {
        Patient* tmp = head;
        head = head->next;
        delete tmp;
    }
    count = 0;
}

// ---------- Module 1: Registration ----------
bool PatientList::addPatient(int id, const string& name, int age, const string& disease,
                             const string& doctor, const string& date) {
    if (searchById(id)) return false;  // IDs must be unique

    Patient* node = new Patient(id, name, age, disease, doctor, date);
    if (!head) {
        head = node;
    } else {
        Patient* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;  // insert at tail
    }
    ++count;
    return true;
}

// ---------- Module 2: Update and Discharge ----------
bool PatientList::updatePatient(int id, const string& name, int age,
                                const string& disease, const string& doctor) {
    Patient* p = searchById(id);
    if (!p) return false;
    if (!name.empty())    p->name = name;
    if (age > 0)          p->age = age;
    if (!disease.empty()) p->disease = disease;
    if (!doctor.empty())  p->doctor = doctor;
    return true;
}

bool PatientList::dischargePatient(int id) {
    Patient* prev = nullptr;
    Patient* cur = head;
    while (cur && cur->id != id) {
        prev = cur;
        cur = cur->next;
    }
    if (!cur) return false;

    if (prev) prev->next = cur->next;
    else      head = cur->next;
    delete cur;
    --count;
    return true;
}

// ---------- Module 3: Search ----------
Patient* PatientList::searchById(int id) const {
    for (Patient* cur = head; cur; cur = cur->next)
        if (cur->id == id) return cur;
    return nullptr;
}

int PatientList::searchByName(const string& name) const {
    int found = 0;
    for (Patient* cur = head; cur; cur = cur->next) {
        if (contains(cur->name, name)) {
            if (found == 0) printHeader();
            printRow(cur);
            ++found;
        }
    }
    return found;
}

int PatientList::searchByDisease(const string& disease) const {
    int found = 0;
    for (Patient* cur = head; cur; cur = cur->next) {
        if (contains(cur->disease, disease)) {
            if (found == 0) printHeader();
            printRow(cur);
            ++found;
        }
    }
    return found;
}

// ---------- Module 4: Sorting and Reports ----------
Patient* PatientList::merge(Patient* a, Patient* b, Less less) {
    Patient dummy(0, "", 0, "", "", "");
    Patient* tail = &dummy;
    while (a && b) {
        if (!less(b, a)) { tail->next = a; a = a->next; }  // <= keeps the sort stable
        else             { tail->next = b; b = b->next; }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}

Patient* PatientList::mergeSort(Patient* h, Less less) {
    if (!h || !h->next) return h;

    // Find the middle with slow/fast pointers and split the list in two.
    Patient* slow = h;
    Patient* fast = h->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    Patient* mid = slow->next;
    slow->next = nullptr;

    return merge(mergeSort(h, less), mergeSort(mid, less), less);
}

void PatientList::sortBy(SortKey key) {
    Less cmp = lessById;
    if (key == SortKey::NAME) cmp = lessByName;
    else if (key == SortKey::AGE) cmp = lessByAge;
    head = mergeSort(head, cmp);
}

void PatientList::doctorReport() const {
    if (!head) { cout << "No patient records available.\n"; return; }

    // Collect the distinct doctor names, then sort them alphabetically.
    vector<string> doctors;
    for (Patient* cur = head; cur; cur = cur->next) {
        bool seen = false;
        for (const string& d : doctors)
            if (toLower(d) == toLower(cur->doctor)) { seen = true; break; }
        if (!seen) doctors.push_back(cur->doctor);
    }
    sort(doctors.begin(), doctors.end(),
         [](const string& a, const string& b) { return toLower(a) < toLower(b); });

    for (const string& doc : doctors) {
        int n = 0;
        cout << "\n=== Doctor: " << doc << " ===\n";
        printHeader();
        for (Patient* cur = head; cur; cur = cur->next) {
            if (toLower(cur->doctor) == toLower(doc)) { printRow(cur); ++n; }
        }
        cout << "Total patients: " << n << "\n";
    }
}

void PatientList::displayAll() const {
    if (!head) { cout << "No patient records available.\n"; return; }
    printHeader();
    for (Patient* cur = head; cur; cur = cur->next) printRow(cur);
    cout << "Total patients: " << count << "\n";
}

// ---------- output formatting ----------
void PatientList::printHeader() {
    cout << left << setw(8) << "ID" << setw(22) << "Name" << setw(6) << "Age"
         << setw(20) << "Disease" << setw(20) << "Doctor" << setw(12) << "Admitted" << "\n"
         << string(88, '-') << "\n";
}

void PatientList::printRow(const Patient* p) {
    cout << left << setw(8) << p->id << setw(22) << p->name << setw(6) << p->age
         << setw(20) << p->disease << setw(20) << p->doctor << setw(12) << p->admissionDate << "\n";
}

// ---------- persistence (pipe-delimited text file) ----------
bool PatientList::saveToFile(const string& path) const {
    ofstream out(path);
    if (!out) return false;
    for (Patient* cur = head; cur; cur = cur->next)
        out << cur->id << '|' << cur->name << '|' << cur->age << '|' << cur->disease << '|'
            << cur->doctor << '|' << cur->admissionDate << '\n';
    return true;
}

bool PatientList::loadFromFile(const string& path) {
    ifstream in(path);
    if (!in) return false;
    clear();

    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string id, name, age, disease, doctor, date;
        if (getline(ss, id, '|') && getline(ss, name, '|') && getline(ss, age, '|') &&
            getline(ss, disease, '|') && getline(ss, doctor, '|') && getline(ss, date)) {
            try {
                addPatient(stoi(id), name, stoi(age), disease, doctor, date);
            } catch (...) { /* skip corrupted line */ }
        }
    }
    return true;
}
