# include <iostream>
# include <string>

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

int main(){}