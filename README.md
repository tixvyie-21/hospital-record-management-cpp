# Hospital Patient Record System (C++)

A console-based hospital record manager built to practise core data structures and algorithms:
**Linked List**, **Searching** and **Sorting (Merge Sort)**.

## Modules

| # | Module | What it does |
|---|--------|--------------|
| 1 | Patient Registration | Adds a patient node (ID, name, age, disease, doctor, admission date). Duplicate IDs are rejected. |
| 2 | Update and Discharge | Edits patient details; deletes the node on discharge. |
| 3 | Search | Linear search by ID, name (partial, case-insensitive) or disease. |
| 4 | Sorting and Reports | Merge sort by ID, name or age; doctor-wise patient report. |

## DSA concepts used

- **Singly linked list**: tail insertion O(n), deletion with a `prev` pointer O(n), full memory cleanup in the destructor.
- **Linear search**: O(n) over the list.
- **Merge sort on a linked list**: O(n log n) time, stable, no extra array; the middle is found with slow/fast pointers.
- **Doctor-wise report**: groups records by doctor, with doctors listed alphabetically.

## Project structure

```
Patient.h        # Node definition
PatientList.h    # Linked list class interface
PatientList.cpp  # All four modules + file persistence
main.cpp         # Menu-driven user interface
Makefile
```

## Build and run

```bash
make
./hospital
```

Or without `make`:

```bash
g++ -std=c++11 -O2 -o hospital main.cpp PatientList.cpp
```

Records are saved to `patients.txt` on exit and loaded automatically on the next start.

## Menu

```
1. Register patient
2. Update patient details
3. Discharge patient
4. Search patient
5. Sort patients
6. Doctor-wise report
7. Display all patients
0. Save and exit
```

## Possible improvements

- Binary search after sorting by ID
- Doubly linked list for O(1) deletion given a node
- Hash map index for O(1) ID lookup
- Billing and appointment modules
