# include <iostream>
# include <string>
# include <iomanip>
# include <limits>
# include <vector>
# include <sstream>
using namespace std;


// GLOBAL CONSTANTS
const int MAX_RECORDS = 100;    // capacity per module (assignment requires at least 50)
const int APPT_FIELDS  = 6;     // number of columns in the appointment 2D array
const int COL_ID      = 0;
const int COL_PATIENT = 1;
const int COL_DOCTOR  = 2;
const int COL_DATE    = 3;
const int COL_TIME    = 4;
const int COL_STATUS  = 5;

// GLOBAL DATA STORE
string patientID[MAX_RECORDS];
string patientName[MAX_RECORDS];
int patientAge[MAX_RECORDS];
char patientGender[MAX_RECORDS];
string patientPhone[MAX_RECORDS];
int patientCount = 0;

string doctorID[MAX_RECORDS];
string doctorName[MAX_RECORDS];
string doctorSpecialty[MAX_RECORDS];
double doctorFee[MAX_RECORDS];
int doctorCount = 0;

string appointment[MAX_RECORDS][APPT_FIELDS];
int appointmentCount = 0;

// FUNCTIONS
// ---- Module 1: Patient & Doctor Management ----

void patientMenu();
void addPatient();
void updatePatient();
void deletePatient();
int searchPatientByID(string id);
void displayAllPatients();

void doctorMenu();
void addDoctor();
void updateDoctor();
void deleteDoctor();
int  searchDoctorByID(string id);  
void displayAllDoctors();

// ---- Module 2: Medicine Management ----

// ---- Module 3: Appointment Booking ----

void appointmentMenu();
void createAppointment();
void cancelAppointment();
void modifyAppointment();
int  searchAppointmentByID(string id);   // Linear Search
void displayAllAppointments();
string generateAppointmentID();

// ---- Module 4: Reporting ----

void reportMenu();
void generateSummaryReport();
void generateDetailedReport();
void calculateStatistics();
void sortPatientsByName();      // Bubble Sort
double calculateTotalRevenue();

void displayMainMenu();

//Utilities tool
void displayHeader(string title);
bool dateValidation(string date);
bool timeValidation(string time);

// MAIN PROGRAM

int main() {
    int choice;

    do {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> choice;

        displayMainMenu();

        switch (choice) {
            case 1: patientMenu(); break;
            case 2: doctorMenu(); break;
            case 3: appointmentMenu(); break;
            case 4: reportMenu(); break;
            case 5: break;
            default: cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 5);
}

void displayMainMenu() {
    displayHeader("Clinic Management System");

    cout << "1. Patient Management" << endl;
    cout << "2. Doctor Management" << endl;
    cout << "3. Appointment Management" << endl;
    cout << "4. Reporting" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";

}

void displayHeader(string title) {
    cout << "======================================" << endl;
    cout << setw(19 + title.length()/2) << title << endl;
    cout << "======================================" << endl << endl;
}

bool dateValidation(string date) {
    vector<int> date_parts;
    string temp;
    stringstream ss(date);

    while (getline(ss, temp, '/')) {
        date_parts.push_back(stoi(temp));
    }

    // 验证日期各部分是否大于0
    for (int date_part: date_parts) {
        if (date_part <= 0)
            return false;
    }

    // 验证日期格式是否为 DD/MM/YYYY
    if (date_parts.size() != 3)
        return false;

    int day = date_parts[0];
    int month = date_parts[1];
    int year = date_parts[2];

    // 验证月份范围
    if (month < 1 || month > 12)
        return false;

    // 验证闰年二月
    bool isLeapYear = year % 4 == 0;
    if (month == 2) {
        if (isLeapYear && day > 29)
            return false;
        if (!isLeapYear && day > 28)
            return false;
    }

    // 验证小月(4, 6, 9, 11月)
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return false;

    // 验证大月(1, 3, 5, 7, 8, 10, 12月)
    if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 31)
        return false;

    return true;
}

bool timeValidation(string time) {
    vector<int> time_parts;
    string temp;
    stringstream ss(time);

    while (getline(ss, temp, ':')) {
        time_parts.push_back(stoi(temp));
    }

    int hour = time_parts[0];
    int minute = time_parts[1];

    // 检查 00:00 到 24:00
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
        return false;

    return true;
}

// Module 1:User/Record Management Module

//Patient Management Menu

void patientMenu() {
    displayHeader("Patient Management");

    int choice;

    do {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> choice;

        cout << "1. Add Patient" << endl;
        cout << "2. Update Patient" << endl;
        cout << "3. Delete Patient" << endl;
        cout << "4. Search Patient By ID" << endl;
        cout << "5. Display All Patient" << endl;
        cout << "6. Back To Main Page" << endl;
        cout << "Enter your choice: ";

        switch (choice) {
            case 1: {
                addPatient(); 
                break;
            }
            case 2:{ 
                updatePatient(); 
                break;
            }
            case 3:{ 
                deletePatient(); 
                break;
            }
            case 4: {
                string idx;

                cin.ignore();
                cout << "Enter Patient ID to search: ";
                getline(cin, idx);

                int id = searchPatientByID(idx);
                if (id != -1){
                    cout << "Found -> " << patientName[id] << endl;
                    cout << "Age: " << patientAge[id] << endl;
                    cout << "Gender: " << patientGender[id] << endl;
                    cout << "Phone: " << patientPhone[id] << "\n";
                }
                else{
                    cout << "Patient not found.\n";
                }
                break;
            }
            case 5:{
                cout << "Returning to main menu ... " << endl;
                break;
            }
            default:{ 
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }

    } while (choice != 5);
}

void addPatient() {

    string id, name, phone;
    int age;
    char gender;

    if (patientCount >= MAX_RECORDS){
        cout << "Patient list is full" << endl;
        return;
    }

    cout << "Enter Name: ";
    getline(cin, name);
    while (name.empty()){
        cout << "Name cannot be empty. Please enter again: ";
        getline(cin, name);
    }

    cout << "Enter age: ";
    cin >> age;
    while (!(cin >> age) || age < 0) {
        cout << "Invalid Age. Please enter a postive answer.";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

   
    cout << "Enter Gender (M/F): ";
    cin >> gender;
    while (toupper(gender) != 'M' && toupper(gender) != 'F'){
        cout << "Invalid Gender. Please enter M or F: ";
        cin >> gender;
    }

    cout << "Enter Phone Number: ";
    cin >> phone;
    while (phone.empty()){
        cout << "Phone number cannot be empty. Please enter again: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> phone;
    }

    patientID[patientCount]   = "P" + to_string(patientCount + 1);
    patientName[patientCount] = name;
    patientAge[patientCount]  = age;   
    patientGender[patientCount] = toupper(gender);
    patientPhone[patientCount] = phone;

    cout << "Patient added successfully. Patient ID: " << patientID[patientCount] << endl;
    patientCount++;
}

void updatePatient(){
    string id, name, phone;
    int choice, age;
    char gender;

    cout << "Enter Patient ID to update: ";
    getline(cin, id);
    while (id.empty()){
        cout << "ID cannot be empty. Please enter again: ";
        getline(cin, id);
    }

    while (searchPatientByID(id) == -1){
        cout << "Patient not found. Please enter again: ";
        getline(cin, id);
    }

    do {
        cout << "What do you want to update?" << endl;
        cout << "1. Name" << endl;
        cout << "2. Age" << endl;
        cout << "3. Gender (M/F)" << endl;
        cout << "4. Phone Number" << endl;
        cout << "5. Back to Patient Menu" << endl;
        cout << "Enter your choice: (1-5)";
        cin >> choice;
       
        switch (choice) {
        case 1:{
            cout << "Enter new Name: ";
            getline(cin, name);
            while (name.empty()){
                cout << "Name cannot be empty. Please enter again: ";
            }
        }
        case 2:{
            cout << "Enter new Age: ";
            cin >> age;
            while (age <0){
                cout << "Invalid Age. Please enter a postive answer.";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        case 3:{
            cout << "Enter new Gender (M/F): ";
            cin >> gender;
            while (toupper(gender) != 'M' && toupper(gender) != 'F'){
                cout << "Invalid Gender. Please enter M or F: ";
                cin >> gender;
            }
        }
        case 4: {
            cout << "Enter new Phone Number: ";
            cin >> phone;
            while (phone.empty()){
                 cout << "Phone number cannot be empty. Please enter again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, phone);
            }
        }

        case 5: {
            cout << "Returning to patient menu ... "<< endl;
            break;
        }
        default:{
            cout << "Invalid choice. Please try again." << endl;
        }

        }


    }while (choice !=5);
 
    patientID[patientCount]   = "P" + to_string(patientCount + 1);
    patientName[patientCount] = name;
    patientAge[patientCount]  = age;   
    patientGender[patientCount] = toupper(gender);
    patientPhone[patientCount] = phone;

    cout << "Patient updated successfully. Patient ID: " << patientID[patientCount] << endl;
}

void deletePatient(){

    string id;

    cout << "Enter Patient ID to delete: ";
    getline(cin, id);
    while (id.empty()){
        cout << "ID cannot be empty. Please enter again: ";
        getline(cin, id);
    }

    if (searchPatientByID(id) == -1){
        cout << "Patient not found. Please enter again: ";
        getline(cin, id);
    }

    for (int i = searchPatientByID(id); i < patientCount - 1; i++){
        patientID[i] = patientID[i + 1];
        patientName[i] = patientName[i + 1];
        patientAge[i] = patientAge[i + 1];
        patientGender[i] = patientGender[i + 1];
        patientPhone[i] = patientPhone[i + 1];
    }
    patientCount--;
    cout << "patient deleted successfully. Patient ID: " << id << endl;
}

int  searchPatientByID(string id){
    
    for (int i = 0; i < patientCount; i++){
        if (patientID[i] == id){
            return i;
        }
    }
    return -1;
}

void displayAllPatients(){

    if (patientCount == 0){
        cout << "No patient records found. " << endl;
        return;
    }

    displayHeader("All Patients (" + to_string(patientCount) + ")");
    for (int i = 0; i < patientCount; i++){
        cout << "Patient ID: " << patientID[i] << endl;
        cout << "Name: " << patientName[i] << endl;
        cout << "Age: " << patientAge[i] << endl;
        cout << "Gender: " << patientGender[i] << endl;
        cout << "Phone: " << patientPhone[i] << endl;
        cout << endl;
    }
}

// Doctor Management Menu

void doctorMenu() {

    displayHeader("Doctor Management");

    int choice;

    do {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> choice;

        cout << "1. Add Doctor" << endl;
        cout << "2. Update Doctor" << endl;
        cout << "3. Delete Doctor" << endl;
        cout << "4. Search Doctor By ID" << endl;
        cout << "5. Display All Doctors" << endl;
        cout << "6. Back To Main Page" << endl;
        cout << "Enter your choice: ";

        switch (choice) {
            case 1: {
                addDoctor(); 
                break;
            }
            case 2:{ 
                updateDoctor(); 
                break;
            }
            case 3:{ 
                deleteDoctor(); 
                break;
            }
            case 4:{ 
                string idx;

                cin.ignore();
                cout << "Enter Doctor ID to search: ";

                getline(cin, idx);
                int id = searchDoctorByID(idx);
                if (id != -1)
                    cout << "Found -> " << doctorName[id] << ", Specialty: " << doctorSpecialty[id]
                         << ", Fee: " << doctorFee[id] << "\n";
                else
                    cout << "Doctor not found.\n";
                break;
            }
            case 5:{
                cout << "Returning to main menu ... " << endl;
                break;
            }
            default:{ 
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        

        } 
    } while (choice != 5);
}

void addDoctor(){

    string id, name, specialty;
    double fee;

    if (doctorCount >= MAX_RECORDS){
        cout << "Doctor list is full" << endl;
        return;
    }

    cout << "Enter Name: ";
    getline(cin, name);
    while (name.empty()){
        cout << "Name cannot be empty. Please enter again: ";
        getline(cin, name);
    }

    cout << "Enter Specialization: ";
    getline(cin, specialty);
    while (specialty.empty()){
        cout << "Specialization cannot be empty. Please enter again: ";
        getline(cin, specialty);
    }

    cout << "Enter consultation fee: ";
    cin >> fee;
    while (!(cin >> fee) || fee < 0){
        cout << "Invalid fee. Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }


    doctorID[doctorCount]   = "D" + to_string(doctorCount + 1);
    doctorName[doctorCount] = name;
    doctorSpecialty[doctorCount]  = specialty;   
    doctorFee[doctorCount] = fee;


    cout << "Doctor added successfully. Doctor ID: " << doctorID[doctorCount] << endl;
    doctorCount++;
}

void updateDoctor(){

    string id, name, specialty;
    int choice;
    double fee;
 

    cout << "Enter Doctor ID to update: ";
    getline(cin, id);
    while (id.empty()){
        cout << "ID cannot be empty. Please enter again: ";
        getline(cin, id);
    }

    while (searchDoctorByID(id) == -1){
        cout << "Doctor not found. Please enter again: ";
        getline(cin, id);
    }

    do {
        cout << "What do you want to update?" << endl;
        cout << "1. Name" << endl;
        cout << "2. Specialty" << endl;
        cout << "3. Consultation Fee" << endl;
        cout << "4. Back to Doctor Menu" << endl;
        cout << "Enter your choice: (1-5)";
        cin >> choice;
       
        switch (choice) {
        case 1:{
            cout << "Enter new Name: ";
            getline(cin, name);
            while (name.empty()){
                cout << "Name cannot be empty. Please enter again: ";
            }
        }
        case 2:{
            cout << "Enter new Specialty: ";
            getline(cin, specialty);
            while (specialty.empty()){
                cout << "Specialty cannot be empty. Please enter again: ";
                getline(cin, specialty);
            }
        }
        case 3:{
            cout << "Enter new Consultation Fee: ";
            cin >> fee;
            while (!(cin >> fee) || fee < 0){
                cout << "Invalid fee. Please enter a positive number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        case 4: {
            cout << "Returning to doctor menu ... "<< endl;
            break;
        }
        default:{
            cout << "Invalid choice. Please try again." << endl;
        }

        }


    }while (choice !=5);
 
    doctorID[doctorCount]   = "D" + to_string(doctorCount + 1);
    doctorName[doctorCount] = name;
    doctorSpecialty[doctorCount]  = specialty;
    doctorFee[doctorCount] = fee;

    cout << "Doctor updated successfully. Doctor ID: " << doctorID[doctorCount] << endl;
}

void deleteDoctor(){

    string id;

    cout << "Enter Doctor ID to delete: ";
    getline(cin, id);
    while (id.empty()){
        cout << "ID cannot be empty. Please enter again: ";
        getline(cin, id);
    }

    if (searchDoctorByID(id) == -1){
        cout << "Doctor not found. Please enter again: ";
        getline(cin, id);
    }

    for (int i = searchDoctorByID(id); i < doctorCount - 1; i++){
        doctorID[i] = doctorID[i + 1];
        doctorName[i] = doctorName[i + 1];
        doctorSpecialty[i] = doctorSpecialty[i + 1];
        doctorFee[i] = doctorFee[i + 1];
    }
    doctorCount--;
    cout << "Doctor deleted successfully. Doctor ID: " << id << endl;
}

int  searchDoctorByID(string id){

    for (int i = 0; i < MAX_RECORDS; i++) {
        if (doctorID[i] == id) {
            return i;
        }
    }
    return -1;
}

void displayAllDoctors(){

    if (doctorCount == 0){
    cout << "No doctor records found. " << endl;
    return;
    }

    displayHeader("All Doctors (" + to_string(doctorCount) + ")");
    for (int i = 0; i < doctorCount; i++){
        cout << "Doctor ID: " << doctorID[i] << endl;
        cout << "Name: " << doctorName[i] << endl;
        cout << "Specialty: " << doctorSpecialty[i] << endl;
        cout << "Consultation Fee: " << doctorFee[i] << endl;
        cout << endl;
    }
}

// ---- Module 3: Appointment Booking (Student C) ----

void appointmentMenu() {
    displayHeader("Appointment Management");

    int choice;

    do {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> choice;

        cout << "1. Create Appoinment" << endl;
        cout << "2. Cancel Appoinment" << endl;
        cout << "3. Modify Appoinment" << endl;
        cout << "4. Display All Appointments" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << "Enter your choice: ";

        switch (choice) {
            case 1: createAppointment(); break;
            case 2: cancelAppointment(); break;
            case 3: modifyAppointment(); break;
            case 4: displayAllAppointments(); break;
            case 5: cout << "Returning to main menu ... " << endl; break;
            default: cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 5);
}

void createAppointment() {
    string patient_ID, doctor_ID, date, time;

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter patient ID: ";
    getline(cin, patient_ID);
    while (searchPatientByID(patient_ID) == -1) {
        cout << "Patient not found. Please try again." << endl;
        cout << "Enter patient ID: ";
        getline(cin, patient_ID);
    }

    cout << "Enter doctor ID: ";
    getline(cin, doctor_ID);
    while (searchDoctorByID(doctor_ID) == -1) {
        cout << "Doctor not found. Please try again." << endl;
        cout << "Enter doctor ID: ";
        getline(cin, doctor_ID);
    }

    cout << "Enter date (DD/MM/YYYY): ";
    getline(cin, date);
    while (dateValidation(date) == false) {
        cout << "Invalid date. Please try again." << endl;
        cout << "Enter date (DD/MM/YYYY): ";
        getline(cin, date);
    }

    cout << "Enter time (HH:MM): ";
    getline(cin, time);
    while (timeValidation(time) == false) {
        cout << "Invalid time. Please try again." << endl;
        cout << "Enter time (HH:MM): ";
        getline(cin, time);
    }

    appointment[appointmentCount][COL_ID] = generateAppointmentID();
    appointment[appointmentCount][COL_PATIENT] = patient_ID;
    appointment[appointmentCount][COL_DOCTOR] = doctor_ID;
    appointment[appointmentCount][COL_DATE] = date;
    appointment[appointmentCount][COL_TIME] = time;
    appointment[appointmentCount][COL_STATUS] = "Scheduled";

    cout << endl;
    cout << "Your appointment ID is: " << appointment[appointmentCount][COL_ID] << endl;
    cout << "Patient ID and name: " << patient_ID << " " << patientName[searchPatientByID(patient_ID)] << endl;
    cout << "Doctor ID and name: " << doctor_ID << " " << doctorName[searchDoctorByID(doctor_ID)] << endl;
    cout << "Date and time: " << date << " " << time << endl;

    cout << "Appointment created successfully." << endl;
    appointmentCount++;
}

void cancelAppointment() {
    string appointment_ID;

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter appointment ID: ";
    getline(cin, appointment_ID);
    while (searchAppointmentByID(appointment_ID) == -1) {
        cout << "Appointment not found. Please try again." << endl;
        cout << "Enter appointment ID: ";
        getline(cin, appointment_ID);
    }

    if (appointment[searchAppointmentByID(appointment_ID)][COL_STATUS] == "Canceled") {
        cout << "Appointment already canceled." << endl;
        return;
    }

    appointment[searchAppointmentByID(appointment_ID)][COL_STATUS] = "Canceled";
    cout << "Appointment canceled successfully." << endl;
}

void modifyAppointment() {
    string appointment_ID;
    int choice;

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter appointment ID: ";
    getline(cin, appointment_ID);
    while (searchAppointmentByID(appointment_ID) == -1) {
        cout << "Appointment not found. Please try again." << endl;
        cout << "Enter appointment ID: ";
        getline(cin, appointment_ID);
    }

    do {
        cout << "What do you want to modify?" << endl;
        cout << "1. Patient" << endl;
        cout << "2. Doctor" << endl;
        cout << "3. Date" << endl;
        cout << "4. Time" << endl;
        cout << "5. Back to Appointment Menu" << endl;
        cout << "Enter your choice: ";

        cin >> choice;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                string patient_ID;
                cout << "Enter patient ID: ";
                getline(cin, patient_ID);
                while (searchPatientByID(patient_ID) == -1) {
                    cout << "Patient not found. Please try again." << endl;
                    cout << "Enter patient ID: ";
                    getline(cin, patient_ID);
                }

                appointment[searchAppointmentByID(appointment_ID)][COL_PATIENT] = patient_ID;

                cout << "Appointment updated successfully." << endl;
                break;
            }
            case 2: {
                string doctor_ID;
                cout << "Enter doctor ID: ";
                getline(cin, doctor_ID);
                while (searchDoctorByID(doctor_ID) == -1) {
                    cout << "Doctor not found. Please try again." << endl;
                    cout << "Enter doctor ID: ";
                    getline(cin, doctor_ID);
                }

                appointment[searchAppointmentByID(appointment_ID)][COL_DOCTOR] = doctor_ID;
                cout << "Appointment updated successfully." << endl;
                break;
            }

            case 3: {
                string date;
                cout << "Enter date (DD/MM/YYYY): ";
                getline(cin, date);
                while (dateValidation(date) == false) {
                    cout << "Invalid date. Please try again." << endl;
                    cout << "Enter date (DD/MM/YYYY): ";
                    getline(cin, date);
                }

                appointment[searchAppointmentByID(appointment_ID)][COL_DATE] = date;

                cout << "Appointment updated successfully." << endl;
                break;
            }

            case 4: {
                string time;
                cout << "Enter time (HH:MM): ";
                getline(cin, time);
                while (timeValidation(time) == false) {
                    cout << "Invalid time. Please try again." << endl;
                    cout << "Enter time (HH:MM): ";
                    getline(cin, time);
                }
                appointment[searchAppointmentByID(appointment_ID)][COL_TIME] = time;

                cout << "Appointment updated successfully." << endl;
                break;
            }

            case 5:
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

    } while (choice != 5);
}

int searchAppointmentByID(string appointment_ID) {
    // Linear search
    for (int i = 0; i < MAX_RECORDS; i++) {
        if (appointment[i][COL_ID] == appointment_ID) {
            return i;
        }
    }
    return -1;
}

void displayAllAppointments() {
    cout << endl;
    for (int i = 0; i < appointmentCount; i++) {
        cout << "Appointment ID: " << appointment[i][COL_ID] << endl;
        cout << "Doctor ID: " << appointment[i][COL_DOCTOR] << endl;
        cout << "Patient ID: " << appointment[i][COL_PATIENT] << endl;
        cout << "Date: " << appointment[i][COL_DATE] << endl;
        cout << "Time: " << appointment[i][COL_TIME] << endl;
        cout << "Status: " << appointment[i][COL_STATUS] << endl;
        cout << endl;
    }
}
