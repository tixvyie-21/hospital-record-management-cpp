#ifndef PATIENT_H
#define PATIENT_H

#include <string>

// A single node of the singly linked list.
struct Patient {
    int id;
    std::string name;
    int age;
    std::string disease;
    std::string doctor;
    std::string admissionDate;  // DD-MM-YYYY
    Patient* next;

    Patient(int id, const std::string& name, int age, const std::string& disease,
            const std::string& doctor, const std::string& admissionDate)
        : id(id), name(name), age(age), disease(disease),
          doctor(doctor), admissionDate(admissionDate), next(nullptr) {}
};

#endif
