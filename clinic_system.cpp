/*
 * ================================================================
 *  CLINIC APPOINTMENT SYSTEM  -  Single-file version
 *  AMCS1013 Problem Solving and Programming
 *
 *  Module 1: Patient Management      (Student A)
 *  Module 2: Doctor Management       (Student B)
 *  Module 3: Appointment Booking     (Student C)
 *  Module 4: Reporting               (Student D)
 *
 *  Compile:  g++ clinic_system.cpp -o clinic_system
 *  Run:      ./clinic_system
 * ================================================================
 */

#include <iostream>
#include <iomanip>
#include <limits>
#include <cctype>
#include <utility>
using namespace std;

// =============================================================
// GLOBAL CONSTANTS
// =============================================================
const int MAX_RECORDS = 100;    // capacity per module (assignment requires at least 50)
const int APPT_FIELDS  = 6;     // number of columns in the appointment 2D array
const int COL_ID      = 0;
const int COL_PATIENT = 1;
const int COL_DOCTOR  = 2;
const int COL_DATE    = 3;
const int COL_TIME    = 4;
const int COL_STATUS  = 5;

// =============================================================
// GLOBAL DATA STORE  (shared by all 4 modules)
// =============================================================

string patientID[MAX_RECORDS];
string patientName[MAX_RECORDS];
int    patientAge[MAX_RECORDS];
char   patientGender[MAX_RECORDS];
string patientPhone[MAX_RECORDS];
int    patientCount = 0;

string doctorID[MAX_RECORDS];
string doctorName[MAX_RECORDS];
string doctorSpecialty[MAX_RECORDS];
double doctorFee[MAX_RECORDS];
int    doctorCount = 0;

string appointment[MAX_RECORDS][APPT_FIELDS];
int appointmentCount = 0;

// =============================================================
// FUNCTION PROTOTYPES
// =============================================================
// ---- Module 1: Patient Management (Student A) ----

void patientMenu();
void addPatient();
void updatePatient();
void deletePatient();
int  searchPatientByID(string id);   // Linear Search -> returns index or -1
void displayAllPatients();
bool isDuplicatePatientID(string id);

// ---- Module 2: Doctor Management (Student B) ----

void doctorMenu();
void addDoctor();
void updateDoctor();
void deleteDoctor();
int  searchDoctorByID(string id);   // Linear Search -> returns index or -1
void displayAllDoctors();
bool isDuplicateDoctorID(string id);
double getDoctorFee(string id);

// ---- Module 3: Appointment Booking (Student C) ----

void appointmentMenu();
void createAppointment();
void cancelAppointment();
void modifyAppointment();
int  searchAppointmentByID(string id);   // Linear Search -> returns index or -1
void displayAllAppointments();
string generateAppointmentID();

// ---- Module 4: Reporting (Student D) ----

void reportMenu();
void generateSummaryReport();
void generateDetailedReport();
void calculateStatistics();
void sortPatientsByName();      // Bubble Sort
double calculateTotalRevenue();

void displayMainMenu();

// =============================================================
// MAIN PROGRAM
// =============================================================

int main() {
    int choice;

    cout << "=================================================\n";
    cout << "     CLINIC APPOINTMENT SYSTEM - MAIN MENU        \n";
    cout << "=================================================\n";

    do {
        displayMainMenu();

        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: patientMenu(); break;
            case 2: doctorMenu(); break;
            case 3: appointmentMenu(); break;
            case 4: reportMenu(); break;
            case 5: cout << "\nThank you for using the Clinic Appointment System. Goodbye!\n"; break;
            default: cout << "Invalid menu selection. Please choose 1-5.\n";
        }
    } while (choice != 5);

    return 0;
}

void displayMainMenu() {
    cout << "\n===== MAIN MENU =====\n";
    cout << "1. Patient Management\n";
    cout << "2. Doctor Management\n";
    cout << "3. Appointment Booking\n";
    cout << "4. Reports\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

// =====================================================================
// MODULE 1: PATIENT / RECORD MANAGEMENT   (Owner: Student A)
// Functions: Add, Update, Delete, Search, Display
// =====================================================================

void patientMenu() {
    int choice;
    do {
        cout << "\n===== PATIENT MANAGEMENT MODULE =====\n";
        cout << "1. Add Patient\n";
        cout << "2. Update Patient\n";
        cout << "3. Delete Patient\n";
        cout << "4. Search Patient\n";
        cout << "5. Display All Patients\n";
        cout << "6. Back to Main Menu\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice)) {                       // reject non-numeric menu input
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: addPatient(); break;
            case 2: updatePatient(); break;
            case 3: deletePatient(); break;
            case 4: {
                cin.ignore();
                cout << "Enter Patient ID to search: ";
                string id;
                getline(cin, id);
                int idx = searchPatientByID(id);
                if (idx != -1)
                    cout << "Found -> " << patientName[idx] << ", Age: " << patientAge[idx]
                         << ", Gender: " << patientGender[idx] << ", Phone: " << patientPhone[idx] << "\n";
                else
                    cout << "Patient not found.\n";
                break;
            }
            case 5: displayAllPatients(); break;
            case 6: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid menu selection. Please choose 1-6.\n";   // invalid menu selection check
        }
    } while (choice != 6);
}

bool isDuplicatePatientID(string id) {
    for (int i = 0; i < patientCount; i++) {
        if (patientID[i] == id) return true;
    }
    return false;
}

// Linear Search: scans the array from the start until a match is found
int searchPatientByID(string id) {
    for (int i = 0; i < patientCount; i++) {
        if (patientID[i] == id) return i;
    }
    return -1;
}

void addPatient() {
    if (patientCount >= MAX_RECORDS) {
        cout << "Patient list is full!\n";
        return;
    }

    cin.ignore();
    string id, name, phone;
    int age;
    char gender;

    cout << "Enter Patient ID (e.g., P001): ";
    getline(cin, id);
    while (id.empty() || isDuplicatePatientID(id)) {          // empty-input + duplicate-ID validation
        if (id.empty()) cout << "ID cannot be empty. Enter again: ";
        else             cout << "Duplicate ID! Enter a different ID: ";
        getline(cin, id);
    }

    cout << "Enter Name: ";
    getline(cin, name);
    while (name.empty()) {
        cout << "Name cannot be empty. Enter again: ";
        getline(cin, name);
    }

    cout << "Enter Age: ";
    while (!(cin >> age) || age < 0) {                        // negative-value validation
        cout << "Invalid age. Enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter Gender (M/F): ";
    cin >> gender;
    while (toupper(gender) != 'M' && toupper(gender) != 'F') {
        cout << "Invalid gender. Enter M or F: ";
        cin >> gender;
    }

    cin.ignore();
    cout << "Enter Phone Number: ";
    getline(cin, phone);
    while (phone.empty()) {
        cout << "Phone cannot be empty. Enter again: ";
        getline(cin, phone);
    }

    patientID[patientCount]     = id;
    patientName[patientCount]   = name;
    patientAge[patientCount]    = age;
    patientGender[patientCount] = toupper(gender);
    patientPhone[patientCount]  = phone;
    patientCount++;

    cout << "Patient record added successfully!\n";
}

void updatePatient() {
    cin.ignore();
    cout << "Enter Patient ID to update: ";
    string id;
    getline(cin, id);
    int idx = searchPatientByID(id);

    if (idx == -1) {
        cout << "Patient not found.\n";
        return;
    }

    cout << "Current record -> Name: " << patientName[idx] << ", Age: " << patientAge[idx]
         << ", Gender: " << patientGender[idx] << ", Phone: " << patientPhone[idx] << "\n";

    string name, phone;
    int age;
    char gender;

    cout << "Enter new Name: ";
    getline(cin, name);
    while (name.empty()) {
        cout << "Name cannot be empty. Enter again: ";
        getline(cin, name);
    }

    cout << "Enter new Age: ";
    while (!(cin >> age) || age < 0) {
        cout << "Invalid age. Enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter new Gender (M/F): ";
    cin >> gender;
    while (toupper(gender) != 'M' && toupper(gender) != 'F') {
        cout << "Invalid gender. Enter M or F: ";
        cin >> gender;
    }

    cin.ignore();
    cout << "Enter new Phone Number: ";
    getline(cin, phone);
    while (phone.empty()) {
        cout << "Phone cannot be empty. Enter again: ";
        getline(cin, phone);
    }

    patientName[idx]   = name;
    patientAge[idx]    = age;
    patientGender[idx] = toupper(gender);
    patientPhone[idx]  = phone;

    cout << "Patient record updated successfully!\n";
}

void deletePatient() {
    cin.ignore();
    cout << "Enter Patient ID to delete: ";
    string id;
    getline(cin, id);
    int idx = searchPatientByID(id);

    if (idx == -1) {
        cout << "Patient not found.\n";
        return;
    }

    // shift every later record one position left to "remove" this one
    for (int i = idx; i < patientCount - 1; i++) {
        patientID[i]     = patientID[i + 1];
        patientName[i]   = patientName[i + 1];
        patientAge[i]    = patientAge[i + 1];
        patientGender[i] = patientGender[i + 1];
        patientPhone[i]  = patientPhone[i + 1];
    }
    patientCount--;
    cout << "Patient record deleted successfully!\n";
}

void displayAllPatients() {
    if (patientCount == 0) {
        cout << "No patient records found.\n";
        return;
    }
    cout << "\n----- ALL PATIENTS (" << patientCount << ") -----\n";
    cout << left << setw(8) << "ID" << setw(20) << "Name" << setw(6) << "Age"
         << setw(8) << "Gender" << "Phone\n";
    for (int i = 0; i < patientCount; i++) {
        cout << left << setw(8) << patientID[i] << setw(20) << patientName[i]
             << setw(6) << patientAge[i] << setw(8) << patientGender[i]
             << patientPhone[i] << "\n";
    }
}

// =====================================================================
// MODULE 2: DOCTOR / SERVICE MANAGEMENT   (Owner: Student B)
// Functions: Add, Update, Delete, Search, Display
// =====================================================================

void doctorMenu() {
    int choice;
    do {
        cout << "\n===== DOCTOR MANAGEMENT MODULE =====\n";
        cout << "1. Add Doctor\n";
        cout << "2. Update Doctor\n";
        cout << "3. Delete Doctor\n";
        cout << "4. Search Doctor\n";
        cout << "5. Display All Doctors\n";
        cout << "6. Back to Main Menu\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: addDoctor(); break;
            case 2: updateDoctor(); break;
            case 3: deleteDoctor(); break;
            case 4: {
                cin.ignore();
                cout << "Enter Doctor ID to search: ";
                string id;
                getline(cin, id);
                int idx = searchDoctorByID(id);
                if (idx != -1)
                    cout << "Found -> " << doctorName[idx] << ", " << doctorSpecialty[idx]
                         << ", Fee: RM" << fixed << setprecision(2) << doctorFee[idx] << "\n";
                else
                    cout << "Doctor not found.\n";
                break;
            }
            case 5: displayAllDoctors(); break;
            case 6: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid menu selection. Please choose 1-6.\n";
        }
    } while (choice != 6);
}

bool isDuplicateDoctorID(string id) {
    for (int i = 0; i < doctorCount; i++) {
        if (doctorID[i] == id) return true;
    }
    return false;
}

int searchDoctorByID(string id) {
    for (int i = 0; i < doctorCount; i++) {         // Linear Search
        if (doctorID[i] == id) return i;
    }
    return -1;
}

double getDoctorFee(string id) {
    int idx = searchDoctorByID(id);
    return (idx != -1) ? doctorFee[idx] : -1;
}

void addDoctor() {
    if (doctorCount >= MAX_RECORDS) {
        cout << "Doctor list is full!\n";
        return;
    }

    cin.ignore();
    string id, name, specialty;
    double fee;

    cout << "Enter Doctor ID (e.g., D001): ";
    getline(cin, id);
    while (id.empty() || isDuplicateDoctorID(id)) {
        if (id.empty()) cout << "ID cannot be empty. Enter again: ";
        else             cout << "Duplicate ID! Enter a different ID: ";
        getline(cin, id);
    }

    cout << "Enter Doctor Name: ";
    getline(cin, name);
    while (name.empty()) {
        cout << "Name cannot be empty. Enter again: ";
        getline(cin, name);
    }

    cout << "Enter Specialization: ";
    getline(cin, specialty);
    while (specialty.empty()) {
        cout << "Specialization cannot be empty. Enter again: ";
        getline(cin, specialty);
    }

    cout << "Enter Consultation Fee (RM): ";
    while (!(cin >> fee) || fee < 0) {                // negative-value validation
        cout << "Invalid fee. Enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    doctorID[doctorCount]        = id;
    doctorName[doctorCount]      = name;
    doctorSpecialty[doctorCount] = specialty;
    doctorFee[doctorCount]       = fee;
    doctorCount++;

    cout << "Doctor record added successfully!\n";
}

void updateDoctor() {
    cin.ignore();
    cout << "Enter Doctor ID to update: ";
    string id;
    getline(cin, id);
    int idx = searchDoctorByID(id);

    if (idx == -1) {
        cout << "Doctor not found.\n";
        return;
    }

    cout << "Current record -> Name: " << doctorName[idx] << ", Specialty: " << doctorSpecialty[idx]
         << ", Fee: RM" << fixed << setprecision(2) << doctorFee[idx] << "\n";

    string name, specialty;
    double fee;

    cout << "Enter new Name: ";
    getline(cin, name);
    while (name.empty()) {
        cout << "Name cannot be empty. Enter again: ";
        getline(cin, name);
    }

    cout << "Enter new Specialization: ";
    getline(cin, specialty);
    while (specialty.empty()) {
        cout << "Specialization cannot be empty. Enter again: ";
        getline(cin, specialty);
    }

    cout << "Enter new Consultation Fee (RM): ";
    while (!(cin >> fee) || fee < 0) {
        cout << "Invalid fee. Enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    doctorName[idx]      = name;
    doctorSpecialty[idx] = specialty;
    doctorFee[idx]       = fee;

    cout << "Doctor record updated successfully!\n";
}

void deleteDoctor() {
    cin.ignore();
    cout << "Enter Doctor ID to delete: ";
    string id;
    getline(cin, id);
    int idx = searchDoctorByID(id);

    if (idx == -1) {
        cout << "Doctor not found.\n";
        return;
    }

    for (int i = idx; i < doctorCount - 1; i++) {
        doctorID[i]        = doctorID[i + 1];
        doctorName[i]      = doctorName[i + 1];
        doctorSpecialty[i] = doctorSpecialty[i + 1];
        doctorFee[i]       = doctorFee[i + 1];
    }
    doctorCount--;
    cout << "Doctor record deleted successfully!\n";
}

void displayAllDoctors() {
    if (doctorCount == 0) {
        cout << "No doctor records found.\n";
        return;
    }
    cout << "\n----- ALL DOCTORS (" << doctorCount << ") -----\n";
    cout << left << setw(8) << "ID" << setw(20) << "Name" << setw(18) << "Specialty" << "Fee (RM)\n";
    for (int i = 0; i < doctorCount; i++) {
        cout << left << setw(8) << doctorID[i] << setw(20) << doctorName[i] << setw(18) << doctorSpecialty[i]
             << fixed << setprecision(2) << doctorFee[i] << "\n";
    }
}

// =====================================================================
// MODULE 3: TRANSACTION / BOOKING MANAGEMENT   (Owner: Student C)
// Functions: Create, Cancel, Modify, Search, Display Summary
// Uses the two-dimensional array: appointment[MAX_RECORDS][APPT_FIELDS]
// =====================================================================

void appointmentMenu() {
    int choice;
    do {
        cout << "\n===== APPOINTMENT BOOKING MODULE =====\n";
        cout << "1. Create Appointment\n";
        cout << "2. Cancel Appointment\n";
        cout << "3. Modify Appointment\n";
        cout << "4. Search Appointment\n";
        cout << "5. Display Booking Summary\n";
        cout << "6. Back to Main Menu\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: createAppointment(); break;
            case 2: cancelAppointment(); break;
            case 3: modifyAppointment(); break;
            case 4: {
                cin.ignore();
                cout << "Enter Appointment ID to search: ";
                string id;
                getline(cin, id);
                int idx = searchAppointmentByID(id);
                if (idx != -1) {
                    cout << "Found -> Patient: " << appointment[idx][COL_PATIENT]
                         << ", Doctor: "  << appointment[idx][COL_DOCTOR]
                         << ", Date: "    << appointment[idx][COL_DATE]
                         << ", Status: "  << appointment[idx][COL_STATUS] << "\n";
                } else {
                    cout << "Appointment not found.\n";
                }
                break;
            }
            case 5: displayAllAppointments(); break;
            case 6: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid menu selection. Please choose 1-6.\n";
        }
    } while (choice != 6);
}

// Auto-generates the next Appointment ID, e.g. A001, A002, A003...
string generateAppointmentID() {
    int n = appointmentCount + 1;
    string num = to_string(n);
    while (num.length() < 3) num = "0" + num;
    return "A" + num;
}

int searchAppointmentByID(string id) {
    for (int i = 0; i < appointmentCount; i++) {   // Linear Search
        if (appointment[i][COL_ID] == id) return i;
    }
    return -1;
}

void createAppointment() {
    if (appointmentCount >= MAX_RECORDS) {
        cout << "Appointment list is full!\n";
        return;
    }

    cin.ignore();
    string patientId, doctorId, date, time;

    cout << "Enter Patient ID: ";
    getline(cin, patientId);
    while (searchPatientByID(patientId) == -1) {       // cross-module check (Module 1)
        cout << "Patient not found. Please register the patient first, or re-enter ID: ";
        getline(cin, patientId);
    }

    cout << "Enter Doctor ID: ";
    getline(cin, doctorId);
    while (searchDoctorByID(doctorId) == -1) {          // cross-module check (Module 2)
        cout << "Doctor not found. Re-enter Doctor ID: ";
        getline(cin, doctorId);
    }

    cout << "Enter Appointment Date (DD/MM/YYYY): ";
    getline(cin, date);
    while (date.empty()) {
        cout << "Date cannot be empty. Enter again: ";
        getline(cin, date);
    }

    cout << "Enter Appointment Time (e.g., 10:30AM): ";
    getline(cin, time);
    while (time.empty()) {
        cout << "Time cannot be empty. Enter again: ";
        getline(cin, time);
    }

    string newId = generateAppointmentID();
    appointment[appointmentCount][COL_ID]      = newId;
    appointment[appointmentCount][COL_PATIENT] = patientId;
    appointment[appointmentCount][COL_DOCTOR]  = doctorId;
    appointment[appointmentCount][COL_DATE]    = date;
    appointment[appointmentCount][COL_TIME]    = time;
    appointment[appointmentCount][COL_STATUS]  = "Confirmed";
    appointmentCount++;

    cout << "Appointment booked successfully! Your Appointment ID is " << newId << "\n";
}

void cancelAppointment() {
    cin.ignore();
    cout << "Enter Appointment ID to cancel: ";
    string id;
    getline(cin, id);
    int idx = searchAppointmentByID(id);

    if (idx == -1) {
        cout << "Appointment not found.\n";
        return;
    }
    if (appointment[idx][COL_STATUS] == "Cancelled") {
        cout << "Appointment already cancelled.\n";
        return;
    }

    appointment[idx][COL_STATUS] = "Cancelled";
    cout << "Appointment cancelled successfully.\n";
}

void modifyAppointment() {
    cin.ignore();
    cout << "Enter Appointment ID to modify: ";
    string id;
    getline(cin, id);
    int idx = searchAppointmentByID(id);

    if (idx == -1) {
        cout << "Appointment not found.\n";
        return;
    }

    string date, time;
    cout << "Enter new Date (DD/MM/YYYY): ";
    getline(cin, date);
    while (date.empty()) {
        cout << "Date cannot be empty. Enter again: ";
        getline(cin, date);
    }

    cout << "Enter new Time: ";
    getline(cin, time);
    while (time.empty()) {
        cout << "Time cannot be empty. Enter again: ";
        getline(cin, time);
    }

    appointment[idx][COL_DATE] = date;
    appointment[idx][COL_TIME] = time;
    cout << "Appointment updated successfully.\n";
}

void displayAllAppointments() {
    if (appointmentCount == 0) {
        cout << "No appointments found.\n";
        return;
    }
    cout << "\n----- BOOKING SUMMARY (" << appointmentCount << ") -----\n";
    cout << left << setw(8) << "ApptID" << setw(10) << "PatientID" << setw(10) << "DoctorID"
         << setw(14) << "Date" << setw(10) << "Time" << "Status\n";
    for (int i = 0; i < appointmentCount; i++) {
        cout << left << setw(8)  << appointment[i][COL_ID]     << setw(10) << appointment[i][COL_PATIENT]
             << setw(10) << appointment[i][COL_DOCTOR] << setw(14) << appointment[i][COL_DATE]
             << setw(10) << appointment[i][COL_TIME]   << appointment[i][COL_STATUS] << "\n";
    }
}

// =====================================================================
// MODULE 4: REPORTING   (Owner: Student D)
// Functions: Summary Report, Detailed Report, Statistics, Sort, Analysis
// =====================================================================

void reportMenu() {
    int choice;
    do {
        cout << "\n===== REPORTING MODULE =====\n";
        cout << "1. Generate Summary Report\n";
        cout << "2. Generate Detailed Report\n";
        cout << "3. Calculate Statistics (Most Popular Doctor)\n";
        cout << "4. Sort Patients by Name (Bubble Sort)\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: generateSummaryReport(); break;
            case 2: generateDetailedReport(); break;
            case 3: calculateStatistics(); break;
            case 4: sortPatientsByName(); break;
            case 5: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid menu selection. Please choose 1-5.\n";
        }
    } while (choice != 5);
}

double calculateTotalRevenue() {
    double total = 0;
    for (int i = 0; i < appointmentCount; i++) {
        if (appointment[i][COL_STATUS] == "Confirmed") {
            double fee = getDoctorFee(appointment[i][COL_DOCTOR]);
            if (fee > 0) total += fee;
        }
    }
    return total;
}

void generateSummaryReport() {
    int confirmed = 0, cancelled = 0;
    for (int i = 0; i < appointmentCount; i++) {
        if (appointment[i][COL_STATUS] == "Confirmed")      confirmed++;
        else if (appointment[i][COL_STATUS] == "Cancelled") cancelled++;
    }

    cout << "\n----- SUMMARY REPORT -----\n";
    cout << "Total Patients registered : " << patientCount << "\n";
    cout << "Total Doctors available   : " << doctorCount << "\n";
    cout << "Total Appointments made   : " << appointmentCount << "\n";
    cout << "  - Confirmed             : " << confirmed << "\n";
    cout << "  - Cancelled             : " << cancelled << "\n";
    cout << "Estimated Revenue (RM)    : " << fixed << setprecision(2) << calculateTotalRevenue() << "\n";
}

void generateDetailedReport() {
    if (appointmentCount == 0) {
        cout << "No appointments to report.\n";
        return;
    }

    cout << "\n----- DETAILED APPOINTMENT REPORT -----\n";
    for (int i = 0; i < appointmentCount; i++) {
        int pIdx = searchPatientByID(appointment[i][COL_PATIENT]);
        int dIdx = searchDoctorByID(appointment[i][COL_DOCTOR]);

        cout << "Appointment " << appointment[i][COL_ID] << ":\n";
        cout << "   Patient  : " << (pIdx != -1 ? patientName[pIdx] : "Unknown") << "\n";
        cout << "   Doctor   : " << (dIdx != -1 ? doctorName[dIdx] : "Unknown") << "\n";
        cout << "   Date/Time: " << appointment[i][COL_DATE] << " " << appointment[i][COL_TIME] << "\n";
        cout << "   Status   : " << appointment[i][COL_STATUS] << "\n\n";
    }
}

void calculateStatistics() {
    if (appointmentCount == 0) {
        cout << "No data available for statistics.\n";
        return;
    }

    // tally how many appointments each doctor has
    string tallyID[MAX_RECORDS];
    int tallyCount[MAX_RECORDS];
    int tallySize = 0;

    for (int i = 0; i < appointmentCount; i++) {
        string docId = appointment[i][COL_DOCTOR];
        bool found = false;
        for (int j = 0; j < tallySize; j++) {
            if (tallyID[j] == docId) {
                tallyCount[j]++;
                found = true;
                break;
            }
        }
        if (!found) {
            tallyID[tallySize] = docId;
            tallyCount[tallySize] = 1;
            tallySize++;
        }
    }

    int maxIdx = 0;
    for (int i = 1; i < tallySize; i++) {
        if (tallyCount[i] > tallyCount[maxIdx]) maxIdx = i;
    }

    int dIdx = searchDoctorByID(tallyID[maxIdx]);
    cout << "\n----- STATISTICS -----\n";
    cout << "Most popular doctor: " << (dIdx != -1 ? doctorName[dIdx] : tallyID[maxIdx])
         << " with " << tallyCount[maxIdx] << " appointment(s).\n";
}

// Bubble Sort: repeatedly swaps adjacent out-of-order elements.
// All parallel patient arrays are swapped together so records stay aligned.
void sortPatientsByName() {
    if (patientCount < 2) {
        cout << "Not enough records to sort.\n";
        return;
    }

    for (int i = 0; i < patientCount - 1; i++) {
        for (int j = 0; j < patientCount - 1 - i; j++) {
            if (patientName[j] > patientName[j + 1]) {
                swap(patientName[j],   patientName[j + 1]);
                swap(patientID[j],     patientID[j + 1]);
                swap(patientAge[j],    patientAge[j + 1]);
                swap(patientGender[j], patientGender[j + 1]);
                swap(patientPhone[j],  patientPhone[j + 1]);
            }
        }
    }

    cout << "Patients sorted by name (A-Z) successfully!\n";
    displayAllPatients();
}
