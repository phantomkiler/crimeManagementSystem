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




  // PERSON CLASS IMPLEMENTATION
  

Person::Person() : name("Unknown"), age(0) {}
Person::Person(const string& n, int a) : name(n), age(a) {}
Person::~Person() {}

string Person::getName() const { return name; }
int Person::getAge() const { return age; }

Person& Person::setName(const string& n) {
    name = n;
    return *this;
}

Person& Person::setAge(int a) {
    age = a;
    return *this;
}


   //OFFICER CLASS IMPLEMENTATION
   

int Officer::badgeCounter = 1000;

Officer::Officer() : Person(), rank("Constable") {
    badgeNumber = ++badgeCounter;
}

Officer::Officer(const string& n, int a, const string& r)
    : Person(n, a), rank(r) {
    badgeNumber = ++badgeCounter;
}

Officer::~Officer() {}

int Officer::getBadgeNumber() const { return badgeNumber; }
string Officer::getRank() const { return rank; }

Officer& Officer::setRank(const string& r) {
    rank = r;
    return *this;
}

void Officer::display() const {
    cout << "Officer: " << rank << " " << name
        << " | Age: " << age
        << " | Badge: " << badgeNumber << endl;
}

void Officer::setBadgeNumber(int b) {
    badgeNumber = b;
}

void Officer::setCounter(int value) {
    badgeCounter = value;
}


   //CASE CLASS IMPLEMENTATION
 
int Case::caseCounter = 0;

Case::Case(const string& crimName, const string& crime)
    : criminalName(crimName), crimeType(crime),
    status("Open"), isMostWanted(false),
    assignedOfficer(nullptr) {
    caseID = ++caseCounter;
}

Case::~Case() {
    assignedOfficer = nullptr;
}

/* ── getters, setters, status ── */
int Case::getTotalCases() { return caseCounter; }

int Case::getCaseID() const { return caseID; }
string Case::getCriminalName() const { return criminalName; }
string Case::getCrimeType() const { return crimeType; }
string Case::getStatus() const { return status; }
bool Case::getIsMostWanted() const { return isMostWanted; }
Officer* Case::getOfficer() const { return assignedOfficer; }

/* ── Feature: status tracker ── */
Case& Case::updateStatus(const string& newStatus) {
    if (newStatus == "Open" ||
        newStatus == "Under Investigation" ||
        newStatus == "Closed") {

        status = newStatus;
    }
    return *this;
}

/* ── Feature: officer assignment ── */
void Case::assignOfficer(Officer* officer) {
    assignedOfficer = officer;
}

void Case::unassignOfficer() {
    assignedOfficer = nullptr;
}

/* ── operator overloading ── */
bool Case::operator==(int id) const {
    return caseID == id;
}

bool Case::operator==(const string& name) const {
    return criminalName == name;
}

/* ── display ── */
void Case::display() const {
    cout << "\nCase ID: " << caseID
        << "\nCriminal: " << criminalName
        << "\nCrime: " << crimeType
        << "\nStatus: " << status
        << "\nMost Wanted: " << (isMostWanted ? "Yes" : "No") << endl;
}


  // CASE DATABASE IMPLEMENTATION
  
CaseDatabase::CaseDatabase() {
    cases = nullptr;
    count = 0;
    capacity = 0;
}

CaseDatabase::~CaseDatabase() {
    for (int i = 0; i < count; i++)
        delete cases[i];
    delete[] cases;
}

void CaseDatabase::resize() {
    int newCap = (capacity == 0) ? 4 : capacity * 2;

    Case** temp = new Case * [newCap];

    for (int i = 0; i < count; i++)
        temp[i] = cases[i];

    delete[] cases;
    cases = temp;
    capacity = newCap;
}

void CaseDatabase::addCase(const string& crimName, const string& crime) {
    if (count == capacity) resize();
    cases[count++] = new Case(crimName, crime);
}

Case* CaseDatabase::getCase(int index) {
    return (index >= 0 && index < count) ? cases[index] : nullptr;
}

Case* CaseDatabase::searchByID(int id) {
    for (int i = 0; i < count; i++)
        if (*cases[i] == id)
            return cases[i];
    return nullptr;
}

Case* CaseDatabase::searchByName(const string& name) {
    for (int i = 0; i < count; i++)
        if (*cases[i] == name)
            return cases[i];
    return nullptr;
}

void CaseDatabase::displayMostWanted() const {
    for (int i = 0; i < count; i++)
        if (cases[i]->getIsMostWanted())
            cases[i]->display();
}

void CaseDatabase::displayAll() const {
    for (int i = 0; i < count; i++)
        cases[i]->display();
}