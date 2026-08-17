#include <fstream>
# include <iostream>
# include <string>
# include <iomanip>
# include <limits>
# include <vector>
# include <sstream>

using namespace std;

enum Gender { MALE, FEMALE };
enum Status { ASSIGNED, CANCELLED };
enum File_Type { PATIENT, DOCTOR, MEDICINE, APPOINTMENT};
enum File_Status { SUCCESS, FAILURE };

struct PatientRecord
{
    string id;
    string name;
    int age;
    Gender gender;
    string phone;
};

struct DoctorRecord
{
    string id;
    string name;
    string specialty;
    double fee;
};

struct MedicineRecord
{
    string id;
    string name;
    string description;
    double price;
};

struct AppointmentRecord
{
    string id;
    string patient_id;
    string doctor_id;
    string date;
    string time;
    Status status;
};

// GLOBAL CONSTANTS
const int MAX_RECORDS = 100;    // capacity per module (assignment requires at least 50)
const string Patient_file_path = "patient.csv";
const string Doctor_file_path = "doctor.csv";
const string Medicine_file_path = "medicine.csv";
const string Appointment_file_path = "appointment.csv";

// GLOBAL DATA STORE
vector<PatientRecord> patient_record;
vector<DoctorRecord> doctor_record;
vector<MedicineRecord> medicine_record;
vector<AppointmentRecord> appointment_record;

// FUNCTIONS
// ---- Module 1: Patient & Doctor Management ----

void patientMenu();
void addPatient();
void updatePatient();
void deletePatient();
int searchPatientByID(string& id);
void displayAllPatients();

void doctorMenu();
void addDoctor();
void updateDoctor();
void deleteDoctor();
int  searchDoctorByID(string& id);
void displayAllDoctors();

// ---- Module 2: Medicine Management ----

void medicineMenu();
void addMedicine();
void updateMedicine();
void deleteMedicine();
int  searchMedicineByID(string& id);
int  searchMedicineByName(const string& name);
void displayAllMedicines();

// ---- Module 3: Appointment Booking ----

void appointmentMenu();
void createAppointment();
void cancelAppointment();
void modifyAppointment();
int  searchAppointmentByID(string& id);   // Linear Search
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
vector<string> SimpleCsvParser(string& line, File_Type file_type);
void displayHeader(string title);
bool dateValidation(string date);
bool timeValidation(string time);

// BONUS
File_Status SaveData();
File_Status LoadData();
File_Status UpdateData(File_Type file_type);

// MAIN PROGRAM

int main() {
    int choice;

    LoadData();

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
            case 5:
                if (SaveData() == FAILURE) {
                    cout << "Failed to update data files." << endl;
                }
                cout << "Thank you for using the Clinic Management System.";
                return 0;
            default: cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 5);
    return 0;
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

// Utilities tool
vector<string> SimpleCsvParser(string& line, File_Type file_type) {
    vector<string> result;
    stringstream ss(line);
    switch (file_type){
        case PATIENT:{
            // gender need to parse to Gender type after parse
            string id, name, phone, gender, age;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, age, ',');
            getline(ss, gender, ',');
            getline(ss, phone, ',');
    
            result.push_back(id);
            result.push_back(name);
            result.push_back(age);
            result.push_back(gender);
            result.push_back(phone);
                
            break;
            }

        case DOCTOR: {
            string id, name, speciality, fee;
            
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, speciality, ',');
            getline(ss, fee, ',');

            result.push_back(id);
            result.push_back(name);
            result.push_back(speciality);
            result.push_back(fee);

            break;
        }

        case MEDICINE: {
            string id, name, description, price;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, description, ',');
            getline(ss, price, ',');

            result.push_back(id);
            result.push_back(name);
            result.push_back(description);
            result.push_back(price);

            break;
        }
        case APPOINTMENT: {
            string id, patient_id, doctor_id, date, time, status;

            getline(ss, id, ',');
            getline(ss, patient_id, ',');
            getline(ss, doctor_id, ',');
            getline(ss, date, ',');
            getline(ss, time, ',');
            getline(ss, status, ',');

            result.push_back(id);
            result.push_back(patient_id);
            result.push_back(doctor_id);
            result.push_back(date);
            result.push_back(time);
            result.push_back(status);

            break;
        }
        default: return result;
            break;
    }

    return result;
}

void displayHeader(string& title) {
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

// BONUS
File_Status SaveData() {
    File_Status status = SUCCESS;
    if (UpdateData(PATIENT) == FAILURE) status = FAILURE;
    if (UpdateData(DOCTOR) == FAILURE) status = FAILURE;
    if (UpdateData(MEDICINE) == FAILURE) status = FAILURE;
    if (UpdateData(APPOINTMENT) == FAILURE) status = FAILURE;
    return status;
}

File_Status LoadData() {
    ifstream patient_file(Patient_file_path);
    ifstream doctor_file(Doctor_file_path);
    ifstream medicine_file(Medicine_file_path);
    ifstream appointment_file(Appointment_file_path);

    if (!patient_file || !doctor_file || !medicine_file || !appointment_file) {
        return FAILURE;
    }

    vector<PatientRecord> loaded_patients;
    vector<DoctorRecord> loaded_doctors;
    vector<MedicineRecord> loaded_medicines;
    vector<AppointmentRecord> loaded_appointments;

    auto parse_int = [](const string& value, int& result) {
        try {
            size_t position = 0;
            result = stoi(value, &position);
            return position == value.length();
        } catch (...) {
            return false;
        }
    };

    auto parse_double = [](const string& value, double& result) {
        try {
            size_t position = 0;
            result = stod(value, &position);
            return position == value.length();
        } catch (...) {
            return false;
        }
    };

    string line;
    while (getline(patient_file, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> fields = SimpleCsvParser(line, PATIENT);
        int age;
        Gender gender;
        if (fields.size() != 5 || fields[0].empty() || fields[1].empty() || fields[4].empty()
            || !parse_int(fields[2], age) || age < 0) {
            return FAILURE;
        }

        if (fields[3] == "Male") {
            gender = MALE;
        } else if (fields[3] == "Female") {
            gender = FEMALE;
        } else {
            return FAILURE;
        }

        loaded_patients.push_back({fields[0], fields[1], age, gender, fields[4]});
        if (loaded_patients.size() > MAX_RECORDS) {
            return FAILURE;
        }
    }

    while (getline(doctor_file, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> fields = SimpleCsvParser(line, DOCTOR);
        double fee;
        if (fields.size() != 4 || fields[0].empty() || fields[1].empty() || fields[2].empty()
            || !parse_double(fields[3], fee) || fee < 0) {
            return FAILURE;
        }

        loaded_doctors.push_back({fields[0], fields[1], fields[2], fee});
        if (loaded_doctors.size() > MAX_RECORDS) {
            return FAILURE;
        }
    }

    while (getline(medicine_file, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> fields = SimpleCsvParser(line, MEDICINE);
        double price;
        if (fields.size() != 4 || fields[0].empty() || fields[1].empty() || fields[2].empty()
            || !parse_double(fields[3], price) || price < 0) {
            return FAILURE;
        }

        loaded_medicines.push_back({fields[0], fields[1], fields[2], price});
        if (loaded_medicines.size() > MAX_RECORDS) {
            return FAILURE;
        }
    }

    while (getline(appointment_file, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> fields = SimpleCsvParser(line, APPOINTMENT);
        Status status;
        if (fields.size() != 6 || fields[0].empty() || fields[1].empty() || fields[2].empty()
            || fields[3].empty() || fields[4].empty()) {
            return FAILURE;
        }

        if (fields[5] == "Assigned") {
            status = ASSIGNED;
        } else if (fields[5] == "Cancelled") {
            status = CANCELLED;
        } else {
            return FAILURE;
        }

        loaded_appointments.push_back({fields[0], fields[1], fields[2], fields[3], fields[4], status});
        if (loaded_appointments.size() > MAX_RECORDS) {
            return FAILURE;
        }
    }

    patient_record.swap(loaded_patients);
    doctor_record.swap(loaded_doctors);
    medicine_record.swap(loaded_medicines);
    appointment_record.swap(loaded_appointments);

    return SUCCESS;
}

File_Status UpdateData(File_Type file_type) {
    switch (file_type) {
        case PATIENT: {
            ofstream file(Patient_file_path);
            if (!file) return FAILURE;

            for (const PatientRecord& record : patient_record) {
                string gender = record.gender == MALE ? "Male" : "Female";
                file << record.id << "," << record.name << "," << record.age << ","
                     << gender << "," << record.phone << endl;
            }
            return file ? SUCCESS : FAILURE;
        }
        case DOCTOR: {
            ofstream file(Doctor_file_path);
            if (!file) return FAILURE;

            for (const DoctorRecord& record : doctor_record) {
                file << record.id << "," << record.name << "," << record.specialty << ","
                     << record.fee << endl;
            }
            return file ? SUCCESS : FAILURE;
        }
        case MEDICINE: {
            ofstream file(Medicine_file_path);
            if (!file) return FAILURE;

            for (const MedicineRecord& record : medicine_record) {
                file << record.id << "," << record.name << "," << record.description << ","
                     << record.price << endl;
            }
            return file ? SUCCESS : FAILURE;
        }
        case APPOINTMENT: {
            ofstream file(Appointment_file_path);
            if (!file) return FAILURE;

            for (const AppointmentRecord& record : appointment_record) {
                string status = record.status == ASSIGNED ? "Assigned" : "Cancelled";
                file << record.id << "," << record.patient_id << "," << record.doctor_id << ","
                     << record.date << "," << record.time << "," << status << endl;
            }
            return file ? SUCCESS : FAILURE;
        }
        default:
            return FAILURE;
    }
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
                    cout << "Found -> " << patient_record[id].name << endl;
                    cout << "Age: " << patient_record[id].age << endl;
                    cout << "Gender: " << patient_record[id].gender << endl;
                    cout << "Phone: " << patient_record[id].phone << "\n";
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
    Gender gender_type;

    if (patient_record.size() >= MAX_RECORDS){
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
    
    id = "P" + to_string(patient_record.size() + 1);
    if (toupper(gender) == 'M') {
        gender_type = MALE;
    } else {
        gender_type = FEMALE;
    }

    patient_record.push_back(PatientRecord{id, name, age, gender_type, phone});

    if (UpdateData(PATIENT) == FAILURE) {
        cout << "Failed to update patient data file." << endl;
    }

    cout << "Patient added successfully. Patient ID: " << id << endl;
}

void updatePatient(){
    string id, name, phone;
    int choice, age, index;
    char gender;
    Gender gender_type;

    cout << "Enter Patient ID to update: ";
    getline(cin, id);
    while (searchPatientByID(id) == -1){
        if (id.empty()){
            cout << "ID cannot be empty. Please enter again: ";
            getline(cin, id);
        }
        else{
            cout << "Patient not found. Please enter again: ";
            getline(cin, id);
        }
    }

    index = searchPatientByID(id);


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
                while (age < 0){
                    cout << "Invalid Age. Please enter a positive answer.";
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
                if (toupper(gender) == 'M'){
                    gender_type = MALE;
                }
                else{
                    gender_type = FEMALE;
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
            default: cout << "Invalid choice. Please try again." << endl;
        }


    } while (choice !=5);

    patient_record[index].name = name;
    patient_record[index].age = age;
    patient_record[index].gender = gender_type;
    patient_record[index].phone = phone;

    if (UpdateData(PATIENT) == FAILURE) {
        cout << "Failed to update patient data file." << endl;
    }

    cout << "Patient updated successfully. Patient ID: " << patient_record[index].id << endl;
}

void deletePatient(){

    string id;
    int index;

    cout << "Enter Patient ID to delete: ";
    getline(cin, id);
    while (searchPatientByID(id) == -1){
        if (id.empty()){
            cout << "ID cannot be empty. Please enter again: ";
            getline(cin, id);
        }
        else{
            cout << "Patient not found. Please enter again: ";
            getline(cin, id);
        }
    }
    index = searchPatientByID(id);

    for (int i = searchPatientByID(id); i < patient_record.size(); i++){
        patient_record[i] = patient_record[i + 1];
    }
    patient_record.erase(patient_record.begin() + index);

    if (UpdateData(PATIENT) == FAILURE) {
        cout << "Failed to update patient data file." << endl;
    }

    cout << "patient deleted successfully. Patient ID: " << id << endl;
}

int  searchPatientByID(string& id){
    for (int i = 0; i < patient_record.size(); i++){
        if (patient_record[i].id == id){
            return i;
        }
    }
    return -1;
}

void displayAllPatients(){

    if (patient_record.empty()){
        cout << "No patient records found. " << endl;
        return;
    }

    displayHeader("All Patients (" + to_string(patient_record.size()) + ")");
    for (int i = 0; i < patient_record.size(); i++){
        cout << "Patient ID: " << patient_record[i].id << endl;
        cout << "Name: " << patient_record[i].name << endl;
        cout << "Age: " << patient_record[i].age << endl;
        cout << "Gender: " << patient_record[i].gender << endl;
        cout << "Phone: " << patient_record[i].phone << endl;
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
                    cout << "Found -> " << doctor_record[id].name << ", Specialty: " << doctor_record[id].specialty
                         << ", Fee: " << doctor_record[id].fee << "\n";
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

    if (doctor_record.size() >= MAX_RECORDS){
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

    id = "D" + to_string(doctor_record.size() + 1);

    doctor_record.push_back({id, name, specialty, fee});

    if (UpdateData(DOCTOR) == FAILURE) {
        cout << "Failed to update doctor data file." << endl;
    }

    cout << "Doctor added successfully. Doctor ID: " << id << endl;
}

void updateDoctor(){

    string id, name, specialty;
    int choice, index;
    double fee;


    cout << "Enter Doctor ID to update: ";
    getline(cin, id);
    while (searchDoctorByID(id) == -1){
        if (id.empty()){
            cout << "ID cannot be empty. Please enter again: ";
            getline(cin, id);
        }
        else{
            cout << "Doctor not found. Please enter again: ";
            getline(cin, id);
        }
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


    } while (choice !=5);

    index = searchDoctorByID(id);

    doctor_record[index].name = name;
    doctor_record[index].specialty = specialty;
    doctor_record[index].fee = fee;

    if (UpdateData(DOCTOR) == FAILURE) {
        cout << "Failed to update doctor data file." << endl;
    }

    cout << "Doctor updated successfully. Doctor ID: " << doctor_record[index].id << endl;
}

void deleteDoctor(){

    string id;
    int index;

    cout << "Enter Doctor ID to delete: ";
    getline(cin, id);
    while (id.empty()){
        if (searchDoctorByID(id) == -1){
            cout << "Doctor not found. Please enter again: ";
            getline(cin, id);
        } else {
            cout << "ID cannot be empty. Please enter again: ";
            getline(cin, id);
        }
    }
    index = searchDoctorByID(id);



    for (int i = index; i < doctor_record.size(); i++){
        doctor_record[i] = doctor_record[i + 1];
    }
    doctor_record.erase(doctor_record.begin() + index);

    if (UpdateData(DOCTOR) == FAILURE) {
        cout << "Failed to update doctor data file." << endl;
    }

    cout << "Doctor deleted successfully. Doctor ID: " << id << endl;
}

int  searchDoctorByID(string& id){

    for (int i = 0; i < MAX_RECORDS; i++) {
        if (doctor_record[i].id == id) {
            return i;
        }
    }
    return -1;
}

void displayAllDoctors(){

    if (doctor_record.empty()){
    cout << "No doctor records found. " << endl;
    return;
    }

    displayHeader("All Doctors (" + to_string(doctor_record.size()) + ")");
    for (auto & i : doctor_record){
        cout << "Doctor ID: " << i.id << endl;
        cout << "Name: " << i.name << endl;
        cout << "Specialty: " << i.specialty << endl;
        cout << "Consultation Fee: " << i.fee << endl;
        cout << endl;
    }
}

// Module 3: Appointment Booking

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
    if (appointment_record.size() == MAX_RECORDS) {
        cout << "Maximum number of appointments reached." << endl;
        return;
    }

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

    appointment_record.push_back(AppointmentRecord{generateAppointmentID(), patient_ID, doctor_ID, date, time, ASSIGNED});

    if (UpdateData(APPOINTMENT) == FAILURE) {
        cout << "Failed to update appointment data file." << endl;
    }

    cout << endl;
    cout << "Your appointment ID is: " << appointment_record.back().id << endl;
    cout << "Patient ID and name: " << patient_ID << " " << patient_record[searchPatientByID(patient_ID)].name << endl;
    cout << "Doctor ID and name: " << doctor_ID << " " << doctor_record[searchDoctorByID(doctor_ID)].name << endl;
    cout << "Date and time: " << date << " " << time << endl;

    cout << "Appointment created successfully." << endl;
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

    if (appointment_record[searchAppointmentByID(appointment_ID)].status == CANCELLED) {
        cout << "Appointment already canceled." << endl;
        return;
    }

    appointment_record[searchAppointmentByID(appointment_ID)].status = CANCELLED;

    if (UpdateData(APPOINTMENT) == FAILURE) {
        cout << "Failed to update appointment data file." << endl;
    }

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

                appointment_record[searchAppointmentByID(appointment_ID)].patient_id = patient_ID;

                if (UpdateData(APPOINTMENT) == FAILURE) {
                    cout << "Failed to update appointment data file." << endl;
                }

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

                appointment_record[searchAppointmentByID(appointment_ID)].doctor_id = doctor_ID;

                if (UpdateData(APPOINTMENT) == FAILURE) {
                    cout << "Failed to update appointment data file." << endl;
                }

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

                appointment_record[searchAppointmentByID(appointment_ID)].date = date;

                if (UpdateData(APPOINTMENT) == FAILURE) {
                    cout << "Failed to update appointment data file." << endl;
                }

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
                appointment_record[searchAppointmentByID(appointment_ID)].time = time;

                if (UpdateData(APPOINTMENT) == FAILURE) {
                    cout << "Failed to update appointment data file." << endl;
                }

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

int searchAppointmentByID(string& appointment_ID) {
    // Linear search
    for (int i = 0; i < appointment_record.size(); i++) {
        if (appointment_record[i].id == appointment_ID) {
            return i;
        }
    }
    return -1;
}

void displayAllAppointments() {
    cout << endl;
    for (int i = 0; i < appointment_record.size(); i++) {
        cout << "Appointment ID: " << appointment_record[i].id << endl;
        cout << "Doctor ID: " << appointment_record[i].doctor_id << endl;
        cout << "Patient ID: " << appointment_record[i].patient_id << endl;
        cout << "Date: " << appointment_record[i].date << endl;
        cout << "Time: " << appointment_record[i].time << endl;
        cout << "Status: " << appointment_record[i].status << endl;
        cout << endl;
    }
}

string generateAppointmentID() {
    int n = appointment_record.size() + 1;
    string num = to_string(n);
    while (num.length() < 3) {
        num = "0" + num;
    }
    return "A" + num;
} 

