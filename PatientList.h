#ifndef PATIENT_LIST_H
#define PATIENT_LIST_H

#include <string>
#include "Patient.h"

enum class SortKey { ID, NAME, AGE };

// Singly linked list holding all patient records.
class PatientList {
public:
    PatientList();
    ~PatientList();
    PatientList(const PatientList&) = delete;
    PatientList& operator=(const PatientList&) = delete;

    // Module 1: Registration
    bool addPatient(int id, const std::string& name, int age, const std::string& disease,
                    const std::string& doctor, const std::string& date);

    // Module 2: Update and Discharge (empty string / age <= 0 keeps the old value)
    bool updatePatient(int id, const std::string& name, int age,
                       const std::string& disease, const std::string& doctor);
    bool dischargePatient(int id);

    // Module 3: Search (linear search)
    Patient* searchById(int id) const;
    int searchByName(const std::string& name) const;       // partial, case-insensitive
    int searchByDisease(const std::string& disease) const; // partial, case-insensitive

    // Module 4: Sorting and Reports
    void sortBy(SortKey key);   // merge sort on the linked list
    void doctorReport() const;
    void displayAll() const;

    // Persistence
    bool saveToFile(const std::string& path) const;
    bool loadFromFile(const std::string& path);

    int size() const { return count; }

private:
    Patient* head;
    int count;

    typedef bool (*Less)(const Patient*, const Patient*);
    static Patient* merge(Patient* a, Patient* b, Less less);
    static Patient* mergeSort(Patient* h, Less less);

    static void printHeader();
    static void printRow(const Patient* p);
    void clear();
};

#endif
