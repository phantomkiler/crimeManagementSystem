#include "crimeManagement.h"
//  FEATURE 7 — Evidence Cataloguing
int Evidence::evidenceCounter = 0;  // static member here

Evidence::Evidence() {
    evidenceID = ++evidenceCounter;
    description = "";
    timestamp = "";
    linkedFIR_ID = "";
}

Evidence::Evidence(string desc, string time, string firID) {
    evidenceID = ++evidenceCounter;
    description = desc;
    timestamp = time;
    linkedFIR_ID = firID;
}
Evidence& Evidence::setDescription(string desc) {// setters
    description = desc;
    return *this;
}
Evidence& Evidence::setTimestamp(string time) {
    timestamp = time;
    return *this;
}
Evidence& Evidence::setLinkedFIR(string firID) {
    linkedFIR_ID = firID;
    return *this;
}
int    Evidence::getEvidenceID()   const {// Getters
    return evidenceID;
}
string Evidence::getDescription()  const {
    return description;
}
string Evidence::getTimestamp()    const {
    return timestamp;
}
string Evidence::getLinkedFIR()    const { 
    return linkedFIR_ID;
}
Evidence Evidence::operator+(const Evidence& other) const {// Operator + 
    Evidence merged;
    merged.description = this->description + " | " + other.description;
    merged.timestamp = this->timestamp;          // keep first timestamp
    merged.linkedFIR_ID = this->linkedFIR_ID;
    return merged;
}

void Evidence::displayEvidence() const {
    cout << "\n Evidence Record " << endl;
    cout << "Evidence ID: " << evidenceID << endl;
    cout << "Linked FIR: " << linkedFIR_ID << endl;
    cout << "Description: " << description << endl;
    cout << "Timestamp: " << timestamp << endl;
}
//  FEATURE 13 MedicalProfileManagement
MedicalProfile::MedicalProfile() {
    bloodGroup = "Unknown";
    allergies = "None";
    mentalHealthStatus = "Unknown";
    physicalMarks = "None";
}
MedicalProfile::MedicalProfile(string blood, string allergy,string mental, string marks) {
    bloodGroup = blood;
    allergies = allergy;
    mentalHealthStatus = mental;
    physicalMarks = marks;
}
MedicalProfile& MedicalProfile::setBloodGroup(string bg) {//setters
    bloodGroup = bg;
    return *this;
}
MedicalProfile& MedicalProfile::setAllergies(string allergy) {
    allergies = allergy;
    return *this;
}
MedicalProfile& MedicalProfile::setMentalHealth(string status) {
    mentalHealthStatus = status;
    return *this;
}
MedicalProfile& MedicalProfile::setPhysicalMarks(string marks) {
    physicalMarks = marks;
    return *this;
}
string MedicalProfile::getBloodGroup()    const {// Getters
    return bloodGroup; 
}
string MedicalProfile::getAllergies()     const {
    return allergies; 
}
string MedicalProfile::getMentalHealth()  const {
    return mentalHealthStatus;
}
string MedicalProfile::getPhysicalMarks() const {
    return physicalMarks;
}

void MedicalProfile::displayMedicalProfile() const {
    cout << "\n Medical Profile " << endl;
    cout << "Blood Group: " << bloodGroup << endl;
    cout << "Allergies: " << allergies << endl;
    cout << "Mental Health: " << mentalHealthStatus << endl;
    cout << "Physical Marks: " << physicalMarks << endl;
}
//  FEATURE 14  KinshipFamily Mapping
FamilyRecord::FamilyRecord() {
    capacity = 5;
    count = 0;
    members = new FamilyMember[capacity];  // dynamic memory
}
FamilyRecord::~FamilyRecord() {
    delete[] members;   // destructor cleans up
}
void FamilyRecord::addFamilyMember(string name, string relation, string contact) {
    if (count >= capacity) {
        // Expand array if full
        capacity *= 2;//copying 
        FamilyMember* temp = new FamilyMember[capacity];
        for (int i = 0; i < count; i++) {
            temp[i] = members[i];
        }
        delete[] members;
        members = temp;
    }
    members[count].name = name;
    members[count].relation = relation;
    members[count].contact = contact;
    count++;
}
void FamilyRecord::displayFamily() const {
    cout << "\n Family / Kinship Record " << endl;
    if (count == 0) {
        cout << "No family members recorded " << endl;
        return;
    }
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". " << members[i].relation 
            << " : " << members[i].name << " (Contact: " << members[i].contact << ")" << endl;
    }
}
int FamilyRecord::getMemberCount() const { 
    return count;
}
