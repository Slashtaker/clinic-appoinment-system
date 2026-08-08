# include <iostream>
# include <string>
# include <iomanip>
# include <algorithm>
#include <vector>

using namespace std;

enum Gender {
    MALE,
    FEMALE,
};

struct Patient {
    string id;
    string name;
    int age;
    Gender gender;
    string phone;
};

struct Doctor {
    string id;
    string name;
    string specialty;
    double fee;
};

struct Appointment {
    string id; // generated automatically
    Patient patient;
    Doctor doctor;
    string date;
    string time;
};


// =============================================================
// GLOBAL DATA STORE  (shared by all 4 modules)
// =============================================================

vector<Patient> LIST_patients;
vector<Doctor> LIST_doctors;
vector<Appointment> LIST_appointments;

// =============================================================
// FUNCTION PROTOTYPES
// =============================================================

void displayHeader(string title);
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
int  searchAppointmentByID(string id);   // Linear Search -> returns index or -1'
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

int main(){
    srand(time(0)); // Seed the random number generator
}

void displayHeader(string title) {
    cout << "=========================================================" << endl;
    cout << setw(29 + (title.length() / 2)) << title << endl;
    cout << "=========================================================" << endl;
}

bool InputValidation(string input, string type) {
    std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    // Validate string input
    if (type == "string"){
        if (input.empty()) {
            return false;
        } 
    // Validate char input
    } else if (type == "char") {
        if (input.length() != 1 || !isalpha(input[0])) {
            return false;
        }
    // Validate int input
    } else if (type == "int") {
        try {
            std::stoi(input);
        } catch (...) {
            return false;
        }
    // Validate double input
    } else if (type == "double") {
        try {
            std::stod(input);
        } catch (...) {
            return false;
        }
    }
    return true;
}

// Module 3 - Appointment Booking 

string generateAppointmentID() {
    return "APT_" + to_string(rand()); // Generates a random appointment ID
}

void createAppointment(Doctor doctor, Patient patient) {
    string date, time;

    Appointment newAppointment;
    newAppointment.id = generateAppointmentID();
    newAppointment.doctor = doctor;
    newAppointment.patient = patient;
    newAppointment.date = date;
    newAppointment.time = time;

    LIST_appointments.push_back(newAppointment);
    
}