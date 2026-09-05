#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

enum InputType { TEXT_INPUT, INTEGER_INPUT, DECIMAL_INPUT, CHARACTER_INPUT };
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
const int MAX_RECORDS = 100;    // capacity per module
const string Patient_file_path = "patient.csv";
const string Doctor_file_path = "doctor.csv";
const string Medicine_file_path = "medicine.csv";
const string Appointment_file_path = "appointment.csv";

// GLOBAL DATA STORE
// The first dimension is the table row and the second dimension is the record slot.
// Each module uses one row because each slot already contains a typed record struct.
PatientRecord patient_record[1][MAX_RECORDS] = {};
DoctorRecord doctor_record[1][MAX_RECORDS] = {};
MedicineRecord medicine_record[1][MAX_RECORDS] = {};
AppointmentRecord appointment_record[1][MAX_RECORDS] = {};
int patient_count = 0;
int doctor_count = 0;
int medicine_count = 0;
int appointment_count = 0;

const int MAX_FIELDS = 6;

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
int  searchAppointmentByID(const string& id);   // Linear Search
void displayAllAppointments();
string generateAppointmentID();

// ---- Module 4: Reporting ----

void reportMenu();
void generateSummaryReport();
void generateDetailedReport();
void calculateStatistics();
void sortPatientsByName();      // Bubble Sort
void displayDoctorAppointment();

void displayMainMenu();

//Utilities tool
string readInput(const string& prompt, InputType inputType = TEXT_INPUT, bool allowEmpty = false);
void SimpleCsvParser(string& line, File_Type file_type,string fields[1][MAX_FIELDS], int& field_count);
void displayHeader(const string& title);
bool dateValidation(const string& date);
bool timeValidation(const string& time);

// BONUS
File_Status SaveData();
File_Status LoadData();
File_Status UpdateData(File_Type file_type);

// MAIN PROGRAM

int main() {
    int choice = 0;

    LoadData();

    do {
         cout<<R"(
 ███  █     ███ █   █ ███  ███      ███  ████  ████   ███  ███ █   █ █████ █   █ █████ █   █ █████     ████ █   █  ████ █████ █████ █   █
█     █      █  ██  █  █  █        █   █ █   █ █   █ █   █  █  ██  █   █   ██ ██ █     ██  █   █      █      █ █  █       █   █     ██ ██
█     █      █  █ █ █  █  █        █████ ████  ████  █   █  █  █ █ █   █   █ █ █ ████  █ █ █   █       ███    █    ███    █   ████  █ █ █
█     █      █  █  ██  █  █        █   █ █     █     █   █  █  █  ██   █   █   █ █     █  ██   █          █   █       █   █   █     █   █
 ███  █████ ███ █   █ ███  ███     █   █ █     █      ███  ███ █   █   █   █   █ █████ █   █   █      ████    █   ████    █   █████ █   █ )";

        cout << endl;
        displayMainMenu();
        choice = stoi(readInput("Enter your choice: ", INTEGER_INPUT));

        switch (choice) {
            case 1: patientMenu(); break;
            case 2: doctorMenu(); break;
            case 3: medicineMenu(); break;
            case 4: appointmentMenu(); break;
            case 5: reportMenu(); break;
            case 6:
                if (SaveData() == FAILURE) {
                    cout << "Failed to update data files." << endl;
                }
                cout << "Thank you for using the Clinic Management System." << endl;
                return 0;
            default: cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 6);
    return 0;
}

void displayMainMenu() {
    displayHeader("Clinic Management System");

    cout << "1. Patient Management" << endl;
    cout << "2. Doctor Management" << endl;
    cout << "3. Medicine Management" << endl;
    cout << "4. Appointment Management" << endl;
    cout << "5. Reporting" << endl;
    cout << "6. Exit" << endl;

}

// Utilities tool

// Reads one complete line, rejects empty input, and validates numeric values.
string readInput(const string& prompt, InputType inputType, bool allowEmpty) {
    while (true) {
        cout << prompt;

        string input;
        getline(cin, input);

        if (input.empty() && !allowEmpty) {
            cout << "Input cannot be empty. Please try again." << endl;
            continue;
        }

        if (inputType == TEXT_INPUT) {
            return input;
        }

        stringstream inputStream(input);
        char extraCharacter;

        if (inputType == INTEGER_INPUT) {
            int value;
            if (!(inputStream >> value) || inputStream >> extraCharacter) {
                cout << "Invalid whole number. Please try again." << endl;
                continue;
            }
            if (value < 0) {
                cout << "Negative values are not allowed. Please try again." << endl;
                continue;
            }
        }
        else if (inputType == DECIMAL_INPUT) {
            double value;
            if (!(inputStream >> value) || inputStream >> extraCharacter) {
                cout << "Invalid number. Please try again." << endl;
                continue;
            }
            if (value < 0) {
                cout << "Negative values are not allowed. Please try again." << endl;
                continue;
            }
        }
        else if (inputType == CHARACTER_INPUT && input.length() != 1) {
            cout << "Please enter one character." << endl;
            continue;
        }

        return input;
    }
}

void SimpleCsvParser(string& line, File_Type file_type,
                     string fields[1][MAX_FIELDS], int& field_count) {
    field_count = 0;
    stringstream ss(line);

    switch (file_type){
        case PATIENT:{
            // gender needs to be parsed to Gender after parsing the fields.
            string id, name, phone, gender, age;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, age, ',');
            getline(ss, gender, ',');
            getline(ss, phone, ',');

            fields[0][field_count++] = id;
            fields[0][field_count++] = name;
            fields[0][field_count++] = age;
            fields[0][field_count++] = gender;
            fields[0][field_count++] = phone;
            break;
        }

        case DOCTOR: {
            string id, name, speciality, fee;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, speciality, ',');
            getline(ss, fee, ',');

            fields[0][field_count++] = id;
            fields[0][field_count++] = name;
            fields[0][field_count++] = speciality;
            fields[0][field_count++] = fee;
            break;
        }

        case MEDICINE: {
            string id, name, description, price;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, description, ',');
            getline(ss, price, ',');

            fields[0][field_count++] = id;
            fields[0][field_count++] = name;
            fields[0][field_count++] = description;
            fields[0][field_count++] = price;
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

            fields[0][field_count++] = id;
            fields[0][field_count++] = patient_id;
            fields[0][field_count++] = doctor_id;
            fields[0][field_count++] = date;
            fields[0][field_count++] = time;
            fields[0][field_count++] = status;
            break;
        }

        default:
            break;
    }
}

void displayHeader(const string& title) {
    cout << "======================================" << endl;
    cout << setw(19 + static_cast<int>(title.length() / 2)) << title << endl;
    cout << "======================================" << endl << endl;
}

bool dateValidation(const string& date) {
    int date_parts[1][3] = {};
    int date_part_count = 0;
    string temp;
    stringstream ss(date);

    while (getline(ss, temp, '/')) {
        if (date_part_count >= 3)
            return false;
        try {
            date_parts[0][date_part_count++] = stoi(temp);
        } catch (...) {
            return false;
        }
    }

    // 验证日期格式是否为 DD/MM/YYYY
    if (date_part_count != 3)
        return false;

    // 验证日期各部分是否大于0
    for (int i = 0; i < date_part_count; ++i) {
        if (date_parts[0][i] <= 0)
            return false;
    }

    int day = date_parts[0][0];
    int month = date_parts[0][1];
    int year = date_parts[0][2];

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

bool timeValidation(const string& time) {
    int time_parts[1][2] = {};
    int time_part_count = 0;
    string temp;
    stringstream ss(time);

    while (getline(ss, temp, ':')) {
        if (time_part_count >= 2)
            return false;
        try {
            time_parts[0][time_part_count++] = stoi(temp);
        } catch (...) {
            return false;
        }
    }

    if (time_part_count != 2)
        return false;

    int hour = time_parts[0][0];
    int minute = time_parts[0][1];

    // 检查 00:00 到 24:00
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
        return false;

    return true;
}

string generateID(File_Type file_type) {
    int biggest = 0;

    for (int i = 0; i < appointment_count; ++i) {
        int id = stoi(appointment_record[0][i].id.substr(1));

        if (id > biggest) {
            biggest = id;
        }
    }

    string num = to_string(biggest + 1);

    while (num.length() < 3) {
        num.insert(0, 1, '0');
    }

    return num;
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

    PatientRecord loaded_patients[1][MAX_RECORDS] = {};
    DoctorRecord loaded_doctors[1][MAX_RECORDS] = {};
    MedicineRecord loaded_medicines[1][MAX_RECORDS] = {};
    AppointmentRecord loaded_appointments[1][MAX_RECORDS] = {};
    int loaded_patient_count = 0;
    int loaded_doctor_count = 0;
    int loaded_medicine_count = 0;
    int loaded_appointment_count = 0;

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

        string fields[1][MAX_FIELDS];
        int field_count = 0;
        SimpleCsvParser(line, PATIENT, fields, field_count);
        int age;
        Gender gender;
        if (field_count != 5 || fields[0][0].empty() || fields[0][1].empty()
            || fields[0][4].empty() || !parse_int(fields[0][2], age) || age < 0) {
            return FAILURE;
        }

        if (fields[0][3] == "Male") {
            gender = MALE;
        } else if (fields[0][3] == "Female") {
            gender = FEMALE;
        } else {
            return FAILURE;
        }

        if (loaded_patient_count >= MAX_RECORDS) {
            return FAILURE;
        }
        loaded_patients[0][loaded_patient_count++] =
            {fields[0][0], fields[0][1], age, gender, fields[0][4]};
    }

    while (getline(doctor_file, line)) {
        if (line.empty()) {
            continue;
        }

        string fields[1][MAX_FIELDS];
        int field_count = 0;
        SimpleCsvParser(line, DOCTOR, fields, field_count);
        double fee;
        if (field_count != 4 || fields[0][0].empty() || fields[0][1].empty()
            || fields[0][2].empty() || !parse_double(fields[0][3], fee) || fee < 0) {
            return FAILURE;
        }

        if (loaded_doctor_count >= MAX_RECORDS) {
            return FAILURE;
        }
        loaded_doctors[0][loaded_doctor_count++] =
            {fields[0][0], fields[0][1], fields[0][2], fee};
    }

    while (getline(medicine_file, line)) {
        if (line.empty()) {
            continue;
        }

        string fields[1][MAX_FIELDS];
        int field_count = 0;
        SimpleCsvParser(line, MEDICINE, fields, field_count);
        double price;
        if (field_count != 4 || fields[0][0].empty() || fields[0][1].empty()
            || fields[0][2].empty() || !parse_double(fields[0][3], price) || price < 0) {
            return FAILURE;
        }

        if (loaded_medicine_count >= MAX_RECORDS) {
            return FAILURE;
        }
        loaded_medicines[0][loaded_medicine_count++] =
            {fields[0][0], fields[0][1], fields[0][2], price};
    }

    while (getline(appointment_file, line)) {
        if (line.empty()) {
            continue;
        }

        string fields[1][MAX_FIELDS];
        int field_count = 0;
        SimpleCsvParser(line, APPOINTMENT, fields, field_count);
        Status status;
        if (field_count != 6 || fields[0][0].empty() || fields[0][1].empty()
            || fields[0][2].empty() || fields[0][3].empty() || fields[0][4].empty()) {
            return FAILURE;
        }

        if (fields[0][5] == "Assigned") {
            status = ASSIGNED;
        } else if (fields[0][5] == "Cancelled") {
            status = CANCELLED;
        } else {
            return FAILURE;
        }

        if (loaded_appointment_count >= MAX_RECORDS) {
            return FAILURE;
        }
        loaded_appointments[0][loaded_appointment_count++] =
            {fields[0][0], fields[0][1], fields[0][2], fields[0][3], fields[0][4], status};
    }

    for (int i = 0; i < loaded_patient_count; ++i) {
        patient_record[0][i] = loaded_patients[0][i];
    }
    for (int i = 0; i < loaded_doctor_count; ++i) {
        doctor_record[0][i] = loaded_doctors[0][i];
    }
    for (int i = 0; i < loaded_medicine_count; ++i) {
        medicine_record[0][i] = loaded_medicines[0][i];
    }
    for (int i = 0; i < loaded_appointment_count; ++i) {
        appointment_record[0][i] = loaded_appointments[0][i];
    }

    patient_count = loaded_patient_count;
    doctor_count = loaded_doctor_count;
    medicine_count = loaded_medicine_count;
    appointment_count = loaded_appointment_count;

    return SUCCESS;
}

File_Status UpdateData(File_Type file_type) {
    switch (file_type) {
        case PATIENT: {
            ofstream file(Patient_file_path);
            if (!file) return FAILURE;

            for (int i = 0; i < patient_count; ++i) {
                const PatientRecord& record = patient_record[0][i];
                string gender = record.gender == MALE ? "Male" : "Female";
                file << record.id << "," << record.name << "," << record.age << ","
                     << gender << "," << record.phone << endl;
            }
            return file ? SUCCESS : FAILURE;
        }
        case DOCTOR: {
            ofstream file(Doctor_file_path);
            if (!file) return FAILURE;

            for (int i = 0; i < doctor_count; ++i) {
                const DoctorRecord& record = doctor_record[0][i];
                file << record.id << "," << record.name << "," << record.specialty << ","
                     << record.fee << endl;
            }
            return file ? SUCCESS : FAILURE;
        }
        case MEDICINE: {
            ofstream file(Medicine_file_path);
            if (!file) return FAILURE;

            for (int i = 0; i < medicine_count; ++i) {
                const MedicineRecord& record = medicine_record[0][i];
                file << record.id << "," << record.name << "," << record.description << ","
                     << record.price << endl;
            }
            return file ? SUCCESS : FAILURE;
        }
        case APPOINTMENT: {
            ofstream file(Appointment_file_path);
            if (!file) return FAILURE;

            for (int i = 0; i < appointment_count; ++i) {
                const AppointmentRecord& record = appointment_record[0][i];
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

    int choice = 0;

    do {
        cout<<R"(
████   ███  █████ ███ █████ █   █ █████    █   █  ███  █   █  ███   ███  █████ █   █ █████ █   █ █████
█   █ █   █   █    █  █     ██  █   █      ██ ██ █   █ ██  █ █   █ █     █     ██ ██ █     ██  █   █
████  █████   █    █  ████  █ █ █   █      █ █ █ █████ █ █ █ █████ █  ██ ████  █ █ █ ████  █ █ █   █
█     █   █   █    █  █     █  ██   █      █   █ █   █ █  ██ █   █ █   █ █     █   █ █     █  ██   █
█     █   █   █   ███ █████ █   █   █      █   █ █   █ █   █ █   █  ███  █████ █   █ █████ █   █   █   )";
        cout << endl;
        displayHeader("Patient Management");

        cout << "1. Add Patient" << endl;
        cout << "2. Update Patient" << endl;
        cout << "3. Delete Patient" << endl;
        cout << "4. Search Patient By ID" << endl;
        cout << "5. Display All Patients" << endl;
        cout << "6. Back To Main Page" << endl;
        choice = stoi(readInput("Enter your choice: ", INTEGER_INPUT));

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
                string idx = readInput("Enter Patient ID to search: ", TEXT_INPUT, false);

                int id = searchPatientByID(idx);
                if (id != -1){
                    cout << "Found -> " << patient_record[0][id].name << endl;
                    cout << "Age: " << patient_record[0][id].age << endl;
                    cout << "Gender: "
                         << (patient_record[0][id].gender == MALE ? "Male" : "Female") << endl;
                    cout << "Phone: " << patient_record[0][id].phone << "\n";
                }
                else{
                    cout << "Patient not found.\n";
                }
                break;
            }
            case 5: {
                displayAllPatients();
                break;
            }
            case 6:{
                cout << "Returning to main menu ... " << endl;
                break;
            }
            default:{
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }

    } while (choice != 6);
}

void addPatient() {

    string id, name, phone;
    int age;
    char gender;
    Gender gender_type;

    if (patient_count >= MAX_RECORDS){
        cout << "Patient list is full" << endl;
        return;
    }

    name = readInput("Enter Name: ");

    age = stoi(readInput("Enter age: ", INTEGER_INPUT));

    do {
        gender = readInput("Enter Gender (M/F): ", CHARACTER_INPUT)[0];
        if (toupper(static_cast<unsigned char>(gender)) != 'M'
            && toupper(static_cast<unsigned char>(gender)) != 'F') {
            cout << "Invalid Gender. Please enter M or F." << endl;
        }
    } while (toupper(static_cast<unsigned char>(gender)) != 'M'
             && toupper(static_cast<unsigned char>(gender)) != 'F');

    phone = readInput("Enter Phone Number: ");

    if (toupper(gender) == 'M') {
        gender_type = MALE;
    } else {
        gender_type = FEMALE;
    }

    int biggest = 0;
    for (int i = 0; i < patient_count; ++i) {
        int idx = stoi(patient_record[0][i].id.substr(1));

        biggest = max(biggest, idx);
    }

    string num = to_string(biggest + 1);

    while (num.length() < 3) {
        num.insert(0, 1, '0');
    }

    id = "P" + num;

    patient_record[0][patient_count++] =
        PatientRecord{id, name, age, gender_type, phone};

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

    id = readInput("Enter Patient ID to update: ");
    while (searchPatientByID(id) == -1){
        cout << "Patient not found." << endl;
        id = readInput("Please enter again (type 0 to cancel): ");

        if (id == "0") {
            cout << "Operation cancelled." << endl;
            return;
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
        choice = stoi(readInput("Enter your choice (1-5): ", INTEGER_INPUT));

        switch (choice) {
            case 1:{
                name = readInput("Enter new Name: ");
                patient_record[0][index].name = name;
                break;
            }
            case 2:{
                age = stoi(readInput("Enter new Age: ", INTEGER_INPUT));
                patient_record[0][index].age = age;
                break;
            }
            case 3:{
                do {
                    gender = readInput("Enter new Gender (M/F): ", CHARACTER_INPUT)[0];
                    if (toupper(static_cast<unsigned char>(gender)) != 'M'
                        && toupper(static_cast<unsigned char>(gender)) != 'F') {
                        cout << "Invalid Gender. Please enter M or F." << endl;
                    }
                } while (toupper(static_cast<unsigned char>(gender)) != 'M'
                         && toupper(static_cast<unsigned char>(gender)) != 'F');
                if (toupper(static_cast<unsigned char>(gender)) == 'M'){
                    gender_type = MALE;
                }
                else{
                    gender_type = FEMALE;
                }
                patient_record[0][index].gender = gender_type;
                break;
            }
            case 4: {
                phone = readInput("Enter new Phone Number: ");
                patient_record[0][index].phone = phone;
                break;
            }

            case 5: {
                cout << "Returning to patient menu ... "<< endl;
                break;
            }
            default: cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice !=5);


    if (UpdateData(PATIENT) == FAILURE) {
        cout << "Failed to update patient data file." << endl;
    }

    cout << "Patient updated successfully. Patient ID: " << patient_record[0][index].id << endl;
}

void deletePatient(){

    string id;
    int index;

    id = readInput("Enter Patient ID to delete: ");
    while (searchPatientByID(id) == -1){
        cout << "Patient not found." << endl;
        id = readInput("Please enter again (type 0 to cancel): ");

        if (id == "0") {
            cout << "Operation cancelled." << endl;
            return;
        }
    }
    index = searchPatientByID(id);

    for (int i = index; i < patient_count - 1; ++i) {
        patient_record[0][i] = patient_record[0][i + 1];
    }
    --patient_count;

    if (UpdateData(PATIENT) == FAILURE) {
        cout << "Failed to update patient data file." << endl;
    }

    cout << "patient deleted successfully. Patient ID: " << id << endl;
}

int  searchPatientByID(string& id){
    for (int i = 0; i < patient_count; i++){
        if (patient_record[0][i].id == id){
            return i;
        }
    }
    return -1;
}

void displayAllPatients(){

    if (patient_count == 0){
        cout << "No patient records found. " << endl;
        return;
    }

    displayHeader("All Patients (" + to_string(patient_count) + ")");
    for (int i = 0; i < patient_count; ++i){
        const PatientRecord& record = patient_record[0][i];
        cout << "Patient ID: " << record.id << endl;
        cout << "Name: " << record.name << endl;
        cout << "Age: " << record.age << endl;
        cout << "Gender: " << (record.gender == MALE ? "Male" : "Female") << endl;
        cout << "Phone: " << record.phone << endl;
        cout << endl;
    }
}

// Doctor Management Menu

void doctorMenu() {

    displayHeader("Doctor Management");

    int choice = 0;

    do {
        cout<<R"(
████   ███   ███  █████  ███  ████     █   █  ███  █   █  ███   ███  █████ █   █ █████ █   █ █████
█   █ █   █ █       █   █   █ █   █    ██ ██ █   █ ██  █ █   █ █     █     ██ ██ █     ██  █   █
█   █ █   █ █       █   █   █ ████     █ █ █ █████ █ █ █ █████ █  ██ ████  █ █ █ ████  █ █ █   █
█   █ █   █ █       █   █   █ █  █     █   █ █   █ █  ██ █   █ █   █ █     █   █ █     █  ██   █
████   ███   ███    █    ███  █   █    █   █ █   █ █   █ █   █  ███  █████ █   █ █████ █   █   █    )";
        cout << endl;
        displayHeader("Doctor Management");
        cout << "1. Add Doctor" << endl;
        cout << "2. Update Doctor" << endl;
        cout << "3. Delete Doctor" << endl;
        cout << "4. Search Doctor By ID" << endl;
        cout << "5. Display All Doctors" << endl;
        cout << "6. Back To Main Page" << endl;
        choice = stoi(readInput("Enter your choice: ", INTEGER_INPUT));

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
                string idx = readInput("Enter Doctor ID to search: ");
                int id = searchDoctorByID(idx);
                if (id != -1)
                    cout << "Found -> " << doctor_record[0][id].name << ", Specialty: " << doctor_record[0][id].specialty
                         << ", Fee: " << doctor_record[0][id].fee << "\n";
                else
                    cout << "Doctor not found.\n";
                break;
            }
            case 5: {
                displayAllDoctors();
                break;
            }
            case 6:{
                cout << "Returning to main menu ... " << endl;
                break;
            }
            default:{
                cout << "Invalid choice. Please try again." << endl;
                break;
            }


        }
    } while (choice != 6);
}

void addDoctor(){

    string id, name, specialty;
    double fee;

    if (doctor_count >= MAX_RECORDS){
        cout << "Doctor list is full" << endl;
        return;
    }

    name = readInput("Enter Name: ");
    specialty = readInput("Enter Specialization: ");
    fee = stod(readInput("Enter consultation fee: ", DECIMAL_INPUT));

    int biggest = 0;
    for (int i = 0; i < doctor_count; ++i) {
        int idx = stoi(doctor_record[0][i].id.substr(1));

        biggest = max(biggest, idx);
    }

    string num = to_string(biggest + 1);

    while (num.length() < 3) {
        num.insert(0, 1, '0');
    }

    id = "D" + num;

    doctor_record[0][doctor_count++] = {id, name, specialty, fee};

    if (UpdateData(DOCTOR) == FAILURE) {
        cout << "Failed to update doctor data file." << endl;
    }

    cout << "Doctor added successfully. Doctor ID: " << id << endl;
}

void updateDoctor(){

    string id, name, specialty;
    int choice, index;
    double fee;

    id = readInput("Enter Doctor ID to update: ");
    while (searchDoctorByID(id) == -1){
        cout << "Doctor not found." << endl;
        id = readInput("Please enter again (type 0 to cancel): ");

        if (id == "0") {
            cout << "Operation cancelled." << endl;
            return;
        }
    }

    index = searchDoctorByID(id);

    do {
        cout << "What do you want to update?" << endl;
        cout << "1. Name" << endl;
        cout << "2. Specialty" << endl;
        cout << "3. Consultation Fee" << endl;
        cout << "4. Back to Doctor Menu" << endl;
        choice = stoi(readInput("Enter your choice (1-4): ", INTEGER_INPUT));

        switch (choice) {
        case 1:{
            name = readInput("Enter new Name: ");
            doctor_record[0][index].name = name;
            break;
        }
        case 2:{
            specialty = readInput("Enter new Specialty: ");
            doctor_record[0][index].specialty = specialty;
            break;
        }
        case 3:{
            fee = stod(readInput("Enter new Consultation Fee: ", DECIMAL_INPUT));
            doctor_record[0][index].fee = fee;
            break;
        }
        case 4: {
            cout << "Returning to doctor menu ... "<< endl;
            break;
        }
        default:{
            cout << "Invalid choice. Please try again." << endl;
        }

        }

    } while (choice !=4);

    if (UpdateData(DOCTOR) == FAILURE) {
        cout << "Failed to update doctor data file." << endl;
    }

    cout << "Doctor updated successfully. Doctor ID: " << doctor_record[0][index].id << endl;
}

void deleteDoctor(){

    string id;
    int index;

    id = readInput("Enter Doctor ID to delete: ");
    while (searchDoctorByID(id) == -1){
        cout << "Doctor not found." << endl;
        id = readInput("Please enter again (type 0 to cancel): ");
        if (id == "0") {
            cout << "Operation cancelled." << endl;
            return;
        }
    }
    index = searchDoctorByID(id);

    for (int i = index; i < doctor_count - 1; ++i) {
        doctor_record[0][i] = doctor_record[0][i + 1];
    }
    --doctor_count;

    if (UpdateData(DOCTOR) == FAILURE) {
        cout << "Failed to update doctor data file." << endl;
    }

    cout << "Doctor deleted successfully. Doctor ID: " << id << endl;
}

int  searchDoctorByID(string& id){

    for (int i = 0; i < doctor_count; i++) {
        if (doctor_record[0][i].id == id) {
            return i;
        }
    }
    return -1;
}

void displayAllDoctors(){

    if (doctor_count == 0){
    cout << "No doctor records found. " << endl;
    return;
    }

    displayHeader("All Doctors (" + to_string(doctor_count) + ")");
    for (int i = 0; i < doctor_count; ++i){
        const DoctorRecord& record = doctor_record[0][i];
        cout << "Doctor ID: " << record.id << endl;
        cout << "Name: " << record.name << endl;
        cout << "Specialty: " << record.specialty << endl;
        cout << "Consultation Fee: " << record.fee << endl;
        cout << endl;
    }
}

// Module 2: Medicine Management Menu

void medicineMenu() {

    displayHeader("Medicine Management");

    int choice = 0;

    do {
        cout<<R"(
█   █ █████ ████  ███  ███  ███ █   █ █████    █   █  ███  █   █  ███   ███  █████ █   █ █████ █   █ █████
██ ██ █     █   █  █  █      █  ██  █ █        ██ ██ █   █ ██  █ █   █ █     █     ██ ██ █     ██  █   █
█ █ █ ████  █   █  █  █      █  █ █ █ ████     █ █ █ █████ █ █ █ █████ █  ██ ████  █ █ █ ████  █ █ █   █
█   █ █     █   █  █  █      █  █  ██ █        █   █ █   █ █  ██ █   █ █   █ █     █   █ █     █  ██   █
█   █ █████ ████  ███  ███  ███ █   █ █████    █   █ █   █ █   █ █   █  ███  █████ █   █ █████ █   █   █     )";
        cout << endl;
        displayHeader("Medicine Management");
        cout << endl;
        cout << "1. Add Medicine" << endl;
        cout << "2. Update Medicine" << endl;
        cout << "3. Delete Medicine" << endl;
        cout << "4. Search Medicine By ID" << endl;
        cout << "5. Search Medicine By Name" << endl;
        cout << "6. Display All Medicines" << endl;
        cout << "7. Back To Main Page" << endl;
        choice = stoi(readInput("Enter your choice: ", INTEGER_INPUT));

        switch (choice) {

        case 1: {
            addMedicine();
            break;
        }

        case 2: {
            updateMedicine();
            break;
        }

        case 3: {
            deleteMedicine();
            break;
        }

        case 4: {
            string id = readInput("Enter Medicine ID to search: ");

            int index = searchMedicineByID(id);

            if (index != -1) {
                cout << "Found -> " << medicine_record[0][index].name << endl;
                cout << "Description: " << medicine_record[0][index].description << endl;
                cout << "Price: RM " << fixed << setprecision(2)
                    << medicine_record[0][index].price << endl;
            }
            else {
                cout << "Medicine not found." << endl;
            }

            break;
        }

        case 5: {
            string name = readInput("Enter Medicine Name to search: ");

            int index = searchMedicineByName(name);

            if (index != -1) {
                cout << "Found -> " << medicine_record[0][index].id << endl;
                cout << "Name: " << medicine_record[0][index].name << endl;
                cout << "Description: " << medicine_record[0][index].description << endl;
                cout << "Price: RM " << fixed << setprecision(2)
                    << medicine_record[0][index].price << endl;
            }
            else {
                cout << "Medicine not found." << endl;
            }

            break;
        }

        case 6: {
            displayAllMedicines();
            break;
        }

        case 7: {
            cout << "Returning to main menu ..." << endl;
            break;
        }

        default: {
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
        }

    } while (choice != 7);
}


void addMedicine() {

    string id, name, description;
    double price;

    if (medicine_count >= MAX_RECORDS) {
        cout << "Medicine list is full." << endl;
        return;
    }

    name = readInput("Enter Medicine Name: ");
    description = readInput("Enter Description: ");
    price = stod(readInput("Enter Price: ", DECIMAL_INPUT));

    id = "M" + to_string(medicine_count + 1);

    medicine_record[0][medicine_count++] = {
        id,
        name,
        description,
        price
    };

    if (UpdateData(MEDICINE) == FAILURE) {
        cout << "Failed to update medicine data file." << endl;
    }

    cout << "Medicine added successfully. Medicine ID: "
        << id << endl;
}


void updateMedicine() {

    string id;
    int choice;
    int index;

    id = readInput("Enter Medicine ID to update: ");

    while (searchMedicineByID(id) == -1) {
        cout << "Medicine not found." << endl;
        id = readInput("Please enter again (type 0 to cancel): ");
        if (id == "0") {
            cout << "Operation cancelled." << endl;
            return;
        }
    }

    index = searchMedicineByID(id);

    do {

        cout << endl;
        cout << "What do you want to update?" << endl;
        cout << "1. Name" << endl;
        cout << "2. Description" << endl;
        cout << "3. Price" << endl;
        cout << "4. Back to Medicine Menu" << endl;
        choice = stoi(readInput("Enter your choice: ", INTEGER_INPUT));

        switch (choice) {

        case 1: {

            string name = readInput("Enter new Medicine Name: ");

            medicine_record[0][index].name = name;

            cout << "Medicine name updated successfully." << endl;

            break;
        }

        case 2: {

            string description = readInput("Enter new Description: ");

            medicine_record[0][index].description = description;

            cout << "Medicine description updated successfully." << endl;

            break;
        }

        case 3: {

            double price = stod(readInput("Enter new Price: ", DECIMAL_INPUT));

            medicine_record[0][index].price = price;

            cout << "Medicine price updated successfully." << endl;

            break;
        }

        case 4: {

            cout << "Returning to medicine menu ..." << endl;

            break;
        }

        default: {

            cout << "Invalid choice. Please try again." << endl;

            break;
        }
        }

        if (choice >= 1 && choice <= 3) {

            if (UpdateData(MEDICINE) == FAILURE) {
                cout << "Failed to update medicine data file." << endl;
            }
        }

    } while (choice != 4);

    cout << "Medicine ID: " << medicine_record[0][index].id << endl;
}


void deleteMedicine() {

    string id;
    int index;

    id = readInput("Enter Medicine ID to delete: ");

    while (searchMedicineByID(id) == -1) {
        cout << "Medicine not found." << endl;
        id = readInput("Please enter again (type 0 to cancel): ");
        if (id == "0") {
            cout << "Operation cancelled." << endl;
            return;
        }
    }

    index = searchMedicineByID(id);

    for (int i = index; i < medicine_count - 1; ++i) {
        medicine_record[0][i] = medicine_record[0][i + 1];
    }
    --medicine_count;

    if (UpdateData(MEDICINE) == FAILURE) {
        cout << "Failed to update medicine data file." << endl;
    }

    cout << "Medicine deleted successfully. Medicine ID: "
        << id << endl;
}


int searchMedicineByID(string& id) {

    for (int i = 0; i < medicine_count; i++) {

        if (medicine_record[0][i].id == id) {
            return i;
        }
    }

    return -1;
}


int searchMedicineByName(const string& name) {

    for (int i = 0; i < medicine_count; i++) {

        if (medicine_record[0][i].name == name) {
            return i;
        }
    }

    return -1;
}


void displayAllMedicines() {

    if (medicine_count == 0) {
        cout << "No medicine records found." << endl;
        return;
    }

    displayHeader(
        "All Medicines (" +
        to_string(medicine_count) +
        ")"
    );

    for (int i = 0; i < medicine_count; ++i) {
        const MedicineRecord& record = medicine_record[0][i];
        cout << "Medicine ID: " << record.id << endl;
        cout << "Name: " << record.name << endl;
        cout << "Description: " << record.description << endl;
        cout << "Price: RM "
            << fixed << setprecision(2)
            << record.price << endl;

        cout << endl;
    }
}

// Module 3: Appointment Booking

void appointmentMenu() {
    int choice = 0;

    do {
        cout<<R"(
 ███  ████  ████   ███  ███ █   █ █████ █   █ █████ █   █ █████    █   █  ███  █   █  ███   ███  █████ █   █ █████ █   █ █████
█   █ █   █ █   █ █   █  █  ██  █   █   ██ ██ █     ██  █   █      ██ ██ █   █ ██  █ █   █ █     █     ██ ██ █     ██  █   █
█████ ████  ████  █   █  █  █ █ █   █   █ █ █ ████  █ █ █   █      █ █ █ █████ █ █ █ █████ █  ██ ████  █ █ █ ████  █ █ █   █
█   █ █     █     █   █  █  █  ██   █   █   █ █     █  ██   █      █   █ █   █ █  ██ █   █ █   █ █     █   █ █     █  ██   █
█   █ █     █      ███  ███ █   █   █   █   █ █████ █   █   █      █   █ █   █ █   █ █   █  ███  █████ █   █ █████ █   █   █       )";
        cout << endl;
        displayHeader("Appointment Management");

        cout << "1. Create Appointment" << endl;
        cout << "2. Cancel Appointment" << endl;
        cout << "3. Modify Appointment" << endl;
        cout << "4. Display All Appointments" << endl;
        cout << "5. Back to Main Menu" << endl;
        choice = stoi(readInput("Enter your choice: ", INTEGER_INPUT));

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
    if (appointment_count >= MAX_RECORDS) {
        cout << "Maximum number of appointments reached." << endl;
        return;
    }

    string patient_ID, doctor_ID, date, time;

    patient_ID = readInput("Enter patient ID: ");
    while (searchPatientByID(patient_ID) == -1) {
        cout << "Patient not found. Please try again." << endl;
        patient_ID = readInput("Enter patient ID: ");
    }

    doctor_ID = readInput("Enter doctor ID: ");
    while (searchDoctorByID(doctor_ID) == -1) {
        cout << "Doctor not found. Please try again." << endl;
        doctor_ID = readInput("Enter doctor ID: ");
    }

    date = readInput("Enter date (DD/MM/YYYY): ");
    while (dateValidation(date) == false) {
        cout << "Invalid date. Please try again." << endl;
        date = readInput("Enter date (DD/MM/YYYY): ");
    }

    time = readInput("Enter time (HH:MM): ");
    while (timeValidation(time) == false) {
        cout << "Invalid time. Please try again." << endl;
        time = readInput("Enter time (HH:MM): ");
    }

    appointment_record[0][appointment_count] =
        AppointmentRecord{generateAppointmentID(), patient_ID, doctor_ID, date, time, ASSIGNED};
    ++appointment_count;

    if (UpdateData(APPOINTMENT) == FAILURE) {
        cout << "Failed to update appointment data file." << endl;
    }

    cout << endl;
    cout << "Your appointment ID is: " << appointment_record[0][appointment_count - 1].id << endl;
    cout << "Patient ID and name: " << patient_ID << " "
         << patient_record[0][searchPatientByID(patient_ID)].name << endl;
    cout << "Doctor ID and name: " << doctor_ID << " "
         << doctor_record[0][searchDoctorByID(doctor_ID)].name << endl;
    cout << "Date and time: " << date << " " << time << endl;

    cout << "Appointment created successfully." << endl;
}

void cancelAppointment() {
    string appointment_ID;

    appointment_ID = readInput("Enter appointment ID: ");
    while (searchAppointmentByID(appointment_ID) == -1) {
        cout << "Appointment not found. Please try again." << endl;
        appointment_ID = readInput("Enter appointment ID (type 0 to cancel): ");
        if (appointment_ID == "0") {
            cout << "Operation cancelled." << endl;
            return;
        }
    }

    if (appointment_record[0][searchAppointmentByID(appointment_ID)].status == CANCELLED) {
        cout << "Appointment already canceled." << endl;
        return;
    }

    appointment_record[0][searchAppointmentByID(appointment_ID)].status = CANCELLED;

    if (UpdateData(APPOINTMENT) == FAILURE) {
        cout << "Failed to update appointment data file." << endl;
    }

    cout << "Appointment canceled successfully." << endl;
}

void modifyAppointment() {
    string appointment_ID;
    int choice;

    appointment_ID = readInput("Enter appointment ID: ");
    while (searchAppointmentByID(appointment_ID) == -1) {
        cout << "Appointment not found. Please try again." << endl;
        appointment_ID = readInput("Enter appointment ID (type 0 to cancel): ");
        if (appointment_ID == "0") {
            cout << "Operation cancelled." << endl;
            return;
        }
    }

    do {
        cout << "What do you want to modify?" << endl;
        cout << "1. Patient" << endl;
        cout << "2. Doctor" << endl;
        cout << "3. Date" << endl;
        cout << "4. Time" << endl;
        cout << "5. Back to Appointment Menu" << endl;
        choice = stoi(readInput("Enter your choice: ", INTEGER_INPUT));

        switch (choice) {
            case 1: {
                string patient_ID = readInput("Enter patient ID (type 0 to cancel): ");
                if (patient_ID == "0") {
                    cout << "Operation cancelled." << endl;
                    return;
                }
                while (searchPatientByID(patient_ID) == -1) {
                    cout << "Patient not found. Please try again." << endl;
                    patient_ID = readInput("Enter patient ID (type 0 to cancel): ");
                    if (patient_ID == "0") {
                        cout << "Operation cancelled." << endl;
                        return;
                    }
                }

                appointment_record[0][searchAppointmentByID(appointment_ID)].patient_id = patient_ID;

                if (UpdateData(APPOINTMENT) == FAILURE) {
                    cout << "Failed to update appointment data file." << endl;
                }

                cout << "Appointment updated successfully." << endl;
                break;
            }
            case 2: {
                string doctor_ID = readInput("Enter doctor ID (type 0 to cancel): ");
                if (doctor_ID == "0") {
                    cout << "Operation cancelled." << endl;
                    return;
                }
                while (searchDoctorByID(doctor_ID) == -1) {
                    cout << "Doctor not found. Please try again." << endl;
                    doctor_ID = readInput("Enter doctor ID (type 0 to cancel): ");
                    if (doctor_ID == "0") {
                        cout << "Operation cancelled." << endl;
                        return;
                    }
                }

                appointment_record[0][searchAppointmentByID(appointment_ID)].doctor_id = doctor_ID;

                if (UpdateData(APPOINTMENT) == FAILURE) {
                    cout << "Failed to update appointment data file." << endl;
                }

                cout << "Appointment updated successfully." << endl;
                break;
            }

            case 3: {
                string date = readInput("Enter date (DD/MM/YYYY) (type 0 to cancel): ");
                if (date == "0") {
                    cout << "Operation cancelled." << endl;
                    return;
                }
                while (dateValidation(date) == false) {
                    cout << "Invalid date. Please try again." << endl;
                    date = readInput("Enter date (DD/MM/YYYY) (type 0 to cancel): ");
                    if (date == "0") {
                        cout << "Operation cancelled." << endl;
                        return;
                    }
                }

                appointment_record[0][searchAppointmentByID(appointment_ID)].date = date;

                if (UpdateData(APPOINTMENT) == FAILURE) {
                    cout << "Failed to update appointment data file." << endl;
                }

                cout << "Appointment updated successfully." << endl;
                break;
            }

            case 4: {
                string time = readInput("Enter time (HH:MM) (type 0 to cancel): ");
                if (time == "0") {
                    cout << "Operation cancelled." << endl;
                    return;
                }
                while (timeValidation(time) == false) {
                    cout << "Invalid time. Please try again." << endl;
                    time = readInput("Enter time (HH:MM) (type 0 to cancel): ");
                    if (time == "0") {
                        cout << "Operation cancelled." << endl;
                        return;
                    }
                }
                appointment_record[0][searchAppointmentByID(appointment_ID)].time = time;

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

int searchAppointmentByID(const string& appointment_ID) {
    // Linear search
    for (int i = 0; i < appointment_count; i++) {
        if (appointment_record[0][i].id == appointment_ID) {
            return i;
        }
    }
    return -1;
}

void displayAllAppointments() {
    if (appointment_count == 0) {
        cout << "No appointment records found." << endl;
        return;
    }

    displayHeader("All Appointments (" + to_string(appointment_count) + ")");
    for (int i = 0; i < appointment_count; ++i) {
        const AppointmentRecord& record = appointment_record[0][i];
        cout << "Appointment ID: " << record.id << endl;
        cout << "Patient ID: " << record.patient_id << endl;
        cout << "Doctor ID: " << record.doctor_id << endl;
        cout << "Date: " << record.date << endl;
        cout << "Time: " << record.time << endl;
        cout << "Status: " << (record.status == ASSIGNED ? "Assigned" : "Cancelled") << endl;
        cout << endl;
    }
}

string generateAppointmentID() {
    int biggest = 0;

    for (int i = 0; i < appointment_count; ++i) {
        int id = stoi(appointment_record[0][i].id.substr(1));

        biggest = max(biggest, id);
    }

    string num = to_string(biggest + 1);

    while (num.length() < 3) {
        num.insert(0, 1, '0');
    }

    return "A" + num;
}

// Module 4: Reporting Module

void reportMenu() {
    int choice = 0;
    do {
        cout<<R"(
████  █████ ████   ███  ████  █████ ███ █   █  ███      ████ █   █  ████ █████ █████ █   █
█   █ █     █   █ █   █ █   █   █    █  ██  █ █        █      █ █  █       █   █     ██ ██
████  ████  ████  █   █ ████    █    █  █ █ █ █  ██     ███    █    ███    █   ████  █ █ █
█  █  █     █     █   █ █  █    █    █  █  ██ █   █        █   █       █   █   █     █   █
█   █ █████ █      ███  █   █   █   ███ █   █  ███     ████    █   ████    █   █████ █   █       )";
        cout << endl;

        displayHeader("REPORTING SYSTEM");
        cout << "1. Summary Report" << endl;
        cout << "2. Detailed Report" << endl;
        cout << "3. Statistics Report" << endl;
        cout << "4. Sort Patients By Name" << endl;
        cout << "5. Display Doctor Appointment" << endl;
        cout << "6. Back To Main Menu" << endl;
        choice = stoi(readInput("Enter your choice: ", INTEGER_INPUT));
        cout << endl;

        switch (choice) {
        case 1:
            generateSummaryReport();
            readInput("\nPress <ENTER> to back to the Report Menu.\n", TEXT_INPUT, true);
            break;
        case 2:
            generateDetailedReport();
            readInput("\nPress <ENTER> to back to the Report Menu.\n", TEXT_INPUT, true);
            break;
        case 3:
            calculateStatistics();
            readInput("\nPress <ENTER> to back to the Report Menu.\n", TEXT_INPUT, true);
            break;
        case 4:
            sortPatientsByName();
            readInput("\nPress <ENTER> to back to the Report Menu.\n", TEXT_INPUT, true);
            break;
        case 5:
            displayDoctorAppointment();
            readInput("\nPress <ENTER> to back to the Report Menu.\n", TEXT_INPUT, true);
            break;
        case 6:
            break;
        default:
            cout << "Invalid Input. Enter Again." << endl << endl;
        }
    } while (choice != 6);
}

void generateSummaryReport() {
    int scheduled = 0, canceled = 0;
    for (int i = 0; i < appointment_count; i++) {
        if (appointment_record[0][i].status == ASSIGNED) {
            scheduled++;
        }
        else if (appointment_record[0][i].status == CANCELLED) {
            canceled++;
        }
    }
    displayHeader("SUMMARY REPORT");
    cout << "Total Patients          : " << patient_count << endl
        << "Total Doctors           : " << doctor_count << endl
        << "Total Appointments      : " << appointment_count << endl
        << "Scheduled Appointments  : " << scheduled << endl
        << "Cancelled Appointments  : " << canceled << endl << endl;

}

void generateDetailedReport() {


    displayHeader("DETAILED REPORT");
    cout << left
        << setw(19) << "Appointment ID"
        << setw(13) << "Patient ID"
        << setw(19) << "Patient Name"
        << setw(19) << "Doctor Name"
        << setw(11) << "Date"
        << setw(9) << "Time"
        << "Status" << endl;
    cout << "-------------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < appointment_count; i++) {
        int patientIndex = searchPatientByID(appointment_record[0][i].patient_id);
        int doctorIndex = searchDoctorByID(appointment_record[0][i].doctor_id);
        string tempname;

        if (patientIndex == -1 || doctorIndex == -1) {
            continue;
        }

        cout << left
            << setw(19) << appointment_record[0][i].id
            << setw(13) << appointment_record[0][i].patient_id;
        if (patient_record[0][patientIndex].name.length() > 18) {
            tempname = patient_record[0][patientIndex].name;
            cout << setw(19) << tempname.replace(15, tempname.length() - 15, "...");
        }
        else {
            cout << setw(19) << patient_record[0][patientIndex].name;
        }
        if (doctor_record[0][doctorIndex].name.length() > 18) {
            tempname = doctor_record[0][doctorIndex].name;
            cout << setw(19) << tempname.replace(15, tempname.length() - 15, "...");
        }
        else {
            cout << setw(19) << doctor_record[0][doctorIndex].name;
        }
        cout << setw(11) << appointment_record[0][i].date
            << setw(9) << appointment_record[0][i].time;

        if (appointment_record[0][i].status == ASSIGNED) {
            cout << "Scheduled";
        }
        else if (appointment_record[0][i].status == CANCELLED) {
            cout << "Cancelled";
        }

        cout << endl;
    }
}

void calculateStatistics() {
    int validAppointments = 0;
    int cancelledAppointments = 0;
    double cancellationRate = 0.0;
    double averageAppointmentsPerDoctor = 0.0;

    for (int i = 0; i < appointment_count; i++) {
        if (appointment_record[0][i].status == ASSIGNED) {
            validAppointments++;
        }
        else if (appointment_record[0][i].status == CANCELLED) {
            cancelledAppointments++;
        }
    }

    if (appointment_count > 0) {
        cancellationRate = static_cast<double>(cancelledAppointments) / (cancelledAppointments + validAppointments) * 100.0;
    }

    if (doctor_count > 0) {
        averageAppointmentsPerDoctor = static_cast<double>(cancelledAppointments + validAppointments) / doctor_count;
    }

    displayHeader("STATISTICS");

    cout << "Valid Appointment               : " << validAppointments << endl
        << "Cancelled Appointment            : " << cancelledAppointments << endl
        << "Cancellation Rate               : " << fixed << setprecision(2) << cancellationRate << "%" << endl
        << "Average appointments Per Doctor : " << averageAppointmentsPerDoctor << endl << endl;
}

void sortPatientsByName() {
    int order[MAX_RECORDS] = { 0 };
    int temp, index;

    int patientCount = patient_count;

    if (patientCount == 0) {
        cout << "No patient records found." << endl;
        return;
    }

    for (int i = 0;i < patientCount;i++) {
        order[i] = i;
    }

    for (int i = 0;i < patientCount - 1;i++) {
        for (int j = 0;j < patientCount - 1 - i;j++) {
            if (patient_record[0][order[j]].name > patient_record[0][order[j + 1]].name) {
                temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }
    displayHeader("SORT PATIENTS BY NAME");
    cout << left
        << setw(19) << "Name"
        << setw(13) << "ID"
        << setw(12) << "Gender"
        << setw(8) << "Age"
        << "Phone" << endl;
    cout << "-----------------------------------------------------------------------" << endl;

    for (int i = 0;i < patientCount;i++) {
        index = order[i];

        cout << left;
        if (patient_record[0][index].name.length() > 18) {
            string tempname;
            tempname = patient_record[0][index].name;
            cout << setw(19) << tempname.replace(15, tempname.length() - 15, "...");
        }
        else {
            cout << setw(19) << patient_record[0][index].name;
        }
        cout << setw(13) << patient_record[0][index].id;

        if (patient_record[0][index].gender == MALE) {
            cout << setw(12) << 'M';
        }
        else {
            cout << setw(12) << 'F';
        }

        cout << setw(8) << patient_record[0][index].age
            << patient_record[0][index].phone << endl;
    }
}

void displayDoctorAppointment() {
    int validAppointments[MAX_RECORDS] = { 0 };
    int canceledAppointments[MAX_RECORDS] = { 0 };
    int doctorIndex, totalValid = 0, totalCanceled = 0;
    string tempname;

    for (int i = 0; i < appointment_count; i++) {

        doctorIndex = searchDoctorByID(appointment_record[0][i].doctor_id);

        if (doctorIndex == -1) {
            continue;
        }
        if (appointment_record[0][i].status == ASSIGNED) {
            validAppointments[doctorIndex]++;
        }
        else if (appointment_record[0][i].status == CANCELLED) {
            canceledAppointments[doctorIndex]++;
        }
    }

    for (int i = 0; i < doctor_count; i++) {
        totalValid += validAppointments[i];
        totalCanceled += canceledAppointments[i];
    }

    displayHeader("DOCTOR APPOINTMENT ANALYSIS");
    cout << left
        << setw(13) << "ID"
        << setw(19) << "Name"
        << setw(13) << "Valid"
        << "Cancelled" << endl;
    cout << "-----------------------------------------------------------" << endl;

    for (int i = 0; i < doctor_count; i++) {
        cout << left
            << setw(13) << doctor_record[0][i].id;
        if (doctor_record[0][i].name.length() > 18) {
            tempname = doctor_record[0][i].name;
            cout << setw(19) << tempname.replace(15, tempname.length() - 15, "...");
        }
        else {
            cout << setw(19) << doctor_record[0][i].name;
        }

        cout << setw(13) << validAppointments[i]
            << canceledAppointments[i] << endl;
    }

    cout << left
        << setw(32) << "TOTAL"
        << setw(13) << totalValid
        << totalCanceled << endl;
}
