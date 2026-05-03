#define _CRT_SECURE_NO_WARNINGS
#pragma managed(push, off)   // ← ADD THIS as first line
#include "CrimeManagement.h"
using namespace std;

using namespace std;
//  FEATURE 13 — Medical Profile Management
MedicalProfile::MedicalProfile() {
    bloodGroup = "Unknown"; allergies = "None";
    mentalHealthStatus = "Unknown"; physicalMarks = "None";
}
MedicalProfile::MedicalProfile(string blood, string allergy, string mental, string marks) {
    bloodGroup = blood; allergies = allergy;
    mentalHealthStatus = mental; physicalMarks = marks;
}
MedicalProfile& MedicalProfile::setBloodGroup(string bg) { bloodGroup = bg; return *this; }
MedicalProfile& MedicalProfile::setAllergies(string allergy) { allergies = allergy; return *this; }
MedicalProfile& MedicalProfile::setMentalHealth(string status) { mentalHealthStatus = status; return *this; }
MedicalProfile& MedicalProfile::setPhysicalMarks(string marks) { physicalMarks = marks; return *this; }
string MedicalProfile::getBloodGroup()    const { return bloodGroup; }
string MedicalProfile::getAllergies()     const { return allergies; }
string MedicalProfile::getMentalHealth()  const { return mentalHealthStatus; }
string MedicalProfile::getPhysicalMarks() const { return physicalMarks; }
void MedicalProfile::displayMedicalProfile() const {
    cout << "\n--- Medical Profile ---\n";
    cout << "Blood Group    : " << bloodGroup << "\n";
    cout << "Allergies      : " << allergies << "\n";
    cout << "Mental Health  : " << mentalHealthStatus << "\n";
    cout << "Physical Marks : " << physicalMarks << "\n";
}

//  FEATURE 14 — Kinship / Family Mapping
FamilyRecord::FamilyRecord() { capacity = 5; count = 0; members = new FamilyMember[capacity]; }
FamilyRecord::~FamilyRecord() { delete[] members; }
void FamilyRecord::addFamilyMember(string name, string relation, string contact) {
    if (count >= capacity) {
        capacity *= 2;
        FamilyMember* temp = new FamilyMember[capacity];
        for (int i = 0; i < count; i++) temp[i] = members[i];
        delete[] members; members = temp;
    }
    members[count].name = name; members[count].relation = relation;
    members[count].contact = contact; count++;
}
void FamilyRecord::displayFamily() const {
    cout << "\n--- Family / Kinship Record ---\n";
    if (count == 0) { cout << "No family members recorded.\n"; return; }
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << members[i].relation << " : " << members[i].name
        << " (Contact: " << members[i].contact << ")\n";
}
int FamilyRecord::getMemberCount() const { return count; }

//  PERSON

Person::Person() : name("Unknown"), age(0) {}
Person::Person(const string& n, int a) : name(n), age(a) {}
Person::~Person() {}
string  Person::getName() const { return name; }
int     Person::getAge()  const { return age; }
Person& Person::setName(const string& n) { name = n; return *this; }
Person& Person::setAge(int a) { age = a; return *this; }

//  OFFICER
Suspect::Suspect() : Person(), crimeCommitted("Unknown") {}
Suspect::Suspect(const string& n, int a, const string& c) : Person(n, a), crimeCommitted(c) {}
string Suspect::getCrime() const { return crimeCommitted; }
void Suspect::setCrime(const string& c) { crimeCommitted = c; }
void Suspect::display() const {
    cout << "Suspect Name: " << name << ", Age: " << age << ", Crime: " << crimeCommitted << endl;
}

int Officer::badgeCounter = 1000;
Officer::Officer() : Person(), rank("Constable"), isAvailable(true) { badgeNumber = ++badgeCounter; }
Officer::Officer(const string& n, int a, const string& r) : Person(n, a), rank(r), isAvailable(true) { badgeNumber = ++badgeCounter; }
Officer::~Officer() {}
int    Officer::getBadgeNumber() const { return badgeNumber; }
string Officer::getRank()        const { return rank; }
bool   Officer::getIsAvailable() const { return isAvailable; }
Officer& Officer::setRank(const string& r) { rank = r; return *this; }
Officer& Officer::setAvailable(bool avail) { isAvailable = avail; return *this; }
void Officer::display() const {
    cout << "Officer: " << rank << " " << name
        << " | Age: " << age << " | Badge: " << badgeNumber << " | Available: " << (isAvailable ? "Yes" : "No") << "\n";
}
void Officer::setBadgeNumber(int b) { badgeNumber = b; }
void Officer::setCounter(int value) { badgeCounter = value; }

//  CASE
int Case::caseCounter = 0;
Case::Case(const string& crimName, const string& crime)
    : criminalName(crimName), crimeType(crime), status("Open"),
    isMostWanted(false), assignedOfficer(nullptr) {
    caseID = ++caseCounter;
}
Case::~Case() { assignedOfficer = nullptr; }
int    Case::getTotalCases() { return caseCounter; }
int    Case::getCaseID()       const { return caseID; }
string Case::getCriminalName() const { return criminalName; }
string Case::getCrimeType()    const { return crimeType; }
string Case::getStatus()       const { return status; }
bool   Case::getIsMostWanted() const { return isMostWanted; }
Officer* Case::getOfficer()    const { return assignedOfficer; }
Case& Case::updateStatus(const string& newStatus) {
    if (newStatus == "Open" || newStatus == "Under Investigation" || newStatus == "Closed")
        status = newStatus;
    return *this;
}
void Case::assignOfficer(Officer* officer) { assignedOfficer = officer; }
void Case::unassignOfficer() { assignedOfficer = nullptr; }
bool Case::operator==(int id)             const { return caseID == id; }
bool Case::operator==(const string& name) const { return criminalName == name; }
void Case::setCaseID(int id) { caseID = id; }
void Case::setCounter(int value) { caseCounter = value; }
void Case::display() const {
    cout << "\nCase ID  : " << caseID
        << "\nCriminal : " << criminalName
        << "\nCrime    : " << crimeType
        << "\nStatus   : " << status
        << "\nMost Wanted: " << (isMostWanted ? "Yes" : "No") << "\n";
}
//  CASE DATABASE
CaseDatabase::CaseDatabase() { cases = nullptr; count = 0; capacity = 0; }
CaseDatabase::~CaseDatabase() {
    for (int i = 0; i < count; i++) delete cases[i];
    delete[] cases;
}
void CaseDatabase::resize() {
    int newCap = (capacity == 0) ? 4 : capacity * 2;
    Case** temp = new Case * [newCap];
    for (int i = 0; i < count; i++) temp[i] = cases[i];
    delete[] cases; cases = temp; capacity = newCap;
}
void CaseDatabase::addCase(const string& crimName, const string& crime) {
    if (count == capacity) resize();
    cases[count++] = new Case(crimName, crime);
}
Case* CaseDatabase::getCase(int index) { return (index >= 0 && index < count) ? cases[index] : nullptr; }
Case* CaseDatabase::searchByID(int id) {
    for (int i = 0; i < count; i++) if (*cases[i] == id) return cases[i];
    return nullptr;
}
Case* CaseDatabase::searchByName(const string& name) {
    for (int i = 0; i < count; i++) if (*cases[i] == name) return cases[i];
    return nullptr;
}
void CaseDatabase::displayMostWanted() const {
    for (int i = 0; i < count; i++) if (cases[i]->getIsMostWanted()) cases[i]->display();
}
void CaseDatabase::displayAll() const {
    for (int i = 0; i < count; i++) cases[i]->display();
}
int CaseDatabase::getCount() const { return count; }
void CaseDatabase::saveToFile(const string& recordFile) const {
    ofstream file(recordFile);
    if (!file) { cout << "Cannot open file for saving.\n"; return; }
    file << count << "\n";
    for (int i = 0; i < count; i++) {
        file << cases[i]->getCaseID() << "\n";
        file << cases[i]->getCriminalName() << "\n";
        file << cases[i]->getCrimeType() << "\n";
        file << cases[i]->getStatus() << "\n";
    }
    file.close();
    cout << "Cases saved to " << recordFile << "\n";
}
void CaseDatabase::loadFromFile(const string& recordFile, Officer** officers, int officerCount) {
    ifstream file(recordFile);
    if (!file) { cout << "No saved cases found.\n"; return; }
    int n; file >> n; file.ignore();
    for (int i = 0; i < n; i++) {
        char crimName[MAX_STRING], crime[MAX_STRING], status[MAX_STRING];
        int id;
        file >> id; file.ignore();
        file.getline(crimName, MAX_STRING);
        file.getline(crime, MAX_STRING);
        file.getline(status, MAX_STRING);
        addCase(crimName, crime);
        cases[count - 1]->setCaseID(id);
        cases[count - 1]->updateStatus(status);
    }
    file.close();
}
void saveOfficersToFile(const string& officerFile, Officer** officers, int officerCount) {
    ofstream file(officerFile);
    file << officerCount << "\n";
    for (int i = 0; i < officerCount; i++) {
        file << officers[i]->getBadgeNumber() << "\n";
        file << officers[i]->getName() << "\n";
        file << officers[i]->getAge() << "\n";
        file << officers[i]->getRank() << "\n";
    }
    file.close();
}
int loadOfficersFromFile(const string& officerFile, Officer** officers) {
    ifstream file(officerFile);
    if (!file) return 0;
    int n; file >> n; file.ignore();
    for (int i = 0; i < n; i++) {
        int badge, age; char name[MAX_STRING], rank[MAX_STRING];
        file >> badge; file.ignore();
        file.getline(name, MAX_STRING);
        file >> age; file.ignore();
        file.getline(rank, MAX_STRING);
        officers[i] = new Officer(name, age, rank);
        officers[i]->setBadgeNumber(badge);
    }
    Officer::setCounter(n > 0 ? officers[n - 1]->getBadgeNumber() : 1000);
    file.close();
    return n;
}

//  FEATURE 10 — Anonymous Crime Reporting  
int AnonymousReport::counter = 0;
AnonymousReport::AnonymousReport() {
    counter++; reportid = counter;
    description = ""; location = ""; status = "Pending";
}
void AnonymousReport::setdata() {
    cout << "Enter Crime Description: "; cin >> description;
    cout << "Enter Location: ";          cin >> location;
    status = "Pending";
}
void AnonymousReport::display() {
    cout << "\n--- Anonymous Report ---\n";
    cout << "Report ID   : " << reportid << "\n";
    cout << "Description : " << description << "\n";
    cout << "Location    : " << location << "\n";
    cout << "Status      : " << status << "\n";
}
void AnonymousReport::updatestatus() {
    cout << "1| Pending  2| Reviewed  3| Escalated\nChoose: ";
    int choice; cin >> choice;
    if (choice == 1) status = "Pending";
    else if (choice == 2) status = "Reviewed";
    else if (choice == 3) status = "Escalated";
}
int AnonymousReport::getreportid() { return reportid; }
void AnonymousReport::savetofile(ofstream& file) {
    file << reportid << " " << description << " " << location << " " << status << "\n";
}
void AnonymousReport::loadfromfile(ifstream& file) {
    file >> reportid >> description >> location >> status;
    if (reportid > counter) counter = reportid;
}

//  FEATURE 11 — Crime Statistics Dashboard  
int StatsDashboard::totalcases = 0;
void StatsDashboard::addcase() { totalcases++; }
int  StatsDashboard::gettotal() { return totalcases; }
void StatsDashboard::display(int open, int underinvestigation, int closed, int tips) {
    cout << "\nCRIME STATISTICS DASHBOARD \n";
    cout << "Total Cases: " << totalcases << "\n";
    cout << "Open Cases: " << open << "\n";
    cout << "Under Investigation  : " << underinvestigation << "\n";
    cout << "Closed Cases: " << closed << "\n";
    cout << "Anonymous Tips: " << tips << "\n";
}

//  FEATURE 12 — File Persistence / MedicalProfileSimple
void MedicalProfileSimple::setdata() {
    cout << "Enter Criminal ID: ";    cin >> criminalid;
    cout << "Enter Blood Group: ";    cin >> bloodgroup;
    cout << "Enter Allergies: ";      cin >> allergies;
    cout << "Enter Mental Health Status: "; cin >> mentalhealth;
    cout << "Enter Physical Marks: "; cin >> physicalmarks;
}
void MedicalProfileSimple::display() {
    cout << "\n--- Medical Profile ---\n";
    cout << "Criminal ID    : " << criminalid << "\n";
    cout << "Blood Group    : " << bloodgroup << "\n";
    cout << "Allergies      : " << allergies << "\n";
    cout << "Mental Health  : " << mentalhealth << "\n";
    cout << "Physical Marks : " << physicalmarks << "\n";
}
string MedicalProfileSimple::getcriminalid() { return criminalid; }
void MedicalProfileSimple::savetofile(ofstream& file) {
    file << criminalid << " " << bloodgroup << " " << allergies << " "
        << mentalhealth << " " << physicalmarks << "\n";
}
void MedicalProfileSimple::loadfromfile(ifstream& file) {
    file >> criminalid >> bloodgroup >> allergies >> mentalhealth >> physicalmarks;
}
void FilePersistence::savemedical(MedicalProfileSimple profiles[], int count) {
    ofstream file("medical.txt");
    for (int i = 0; i < count; i++) profiles[i].savetofile(file);
    file.close(); cout << "Medical profiles saved.\n";
}
void FilePersistence::loadmedical(MedicalProfileSimple profiles[], int& count) {
    ifstream file("medical.txt");
    if (!file) { cout << "No medical file found.\n"; return; }
    count = 0;
    while (!file.eof() && count < 10) { profiles[count].loadfromfile(file); if (file) count++; }
    file.close(); cout << count << " medical profile(s) loaded.\n";
}
void FilePersistence::savereports(AnonymousReport reports[], int count) {
    ofstream file("anon_reports.txt");
    for (int i = 0; i < count; i++) reports[i].savetofile(file);
    file.close(); cout << "Anonymous reports saved.\n";
}
void FilePersistence::loadreports(AnonymousReport reports[], int& count) {
    ifstream file("anon_reports.txt");
    if (!file) { cout << "No anonymous reports file found.\n"; return; }
    count = 0;
    while (!file.eof() && count < 10) { reports[count].loadfromfile(file); if (file) count++; }
    file.close(); cout << count << " anonymous report(s) loaded.\n";
}

//  CRIMINAL RECORD 
void CriminalRecord::allocateAndCopy(char*& destination, const char* source) {
    delete[] destination;
    if (source && strlen(source) > 0) {
        destination = new char[strlen(source) + 1];
        strcpy(destination, source);
    }
    else {
        destination = new char[1]; destination[0] = '\0';
    }
}
void CriminalRecord::deallocate(char*& ptr) { delete[] ptr; ptr = nullptr; }
bool CriminalRecord::isValidDate(const char* date) {
    if (strlen(date) != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    for (int i = 0; i < 10; i++) { if (i == 4 || i == 7) continue; if (!isdigit(date[i])) return false; }
    return true;
}
bool CriminalRecord::isValidPhone(const char* phone) {
    int len = strlen(phone);
    if (len < 10 || len > 13) return false;
    for (int i = 0; i < len; i++) if (!isdigit(phone[i]) && phone[i] != '+' && phone[i] != '-') return false;
    return true;
}
CriminalRecord::CriminalRecord() {
    criminalID = nullptr; fullName = nullptr; alias = nullptr;
    fatherName = nullptr; motherName = nullptr; dateOfBirth = nullptr;
    gender = nullptr; nationality = nullptr; cnic = nullptr; passportNumber = nullptr;
    height = nullptr; weight = nullptr; eyeColor = nullptr; hairColor = nullptr;
    bloodGroup = nullptr; distinguishingMarks = nullptr;
    permanentAddress = nullptr; currentAddress = nullptr; phoneNumber = nullptr; email = nullptr;
    criminalRecordNumber = nullptr; dateOfFirstArrest = nullptr; criminalHistorySummary = nullptr;
    knownAssociates = nullptr; gangAffiliation = nullptr;
    firNumber = nullptr; policeStation = nullptr; crimeType = nullptr;
    crimeDate = nullptr; crimeLocation = nullptr; crimeDescription = nullptr;
    arrestingOfficer = nullptr; arrestingOfficerBadge = nullptr;
    currentStatus = nullptr; courtName = nullptr; caseNumber = nullptr;
    verdict = nullptr; sentence = nullptr; bailStatus = nullptr;
    fingerprints = nullptr; dnaProfile = nullptr; photographPath = nullptr;
    additionalNotes = nullptr; createdBy = nullptr; creationDate = nullptr;
    lastModifiedBy = nullptr; lastModifiedDate = nullptr;
    totalArrests = 0; totalConvictions = 0;
    allocateAndCopy(criminalID, ""); allocateAndCopy(fullName, ""); allocateAndCopy(alias, "None");
    allocateAndCopy(fatherName, ""); allocateAndCopy(motherName, ""); allocateAndCopy(dateOfBirth, "");
    allocateAndCopy(gender, ""); allocateAndCopy(nationality, "Pakistani"); allocateAndCopy(cnic, "");
    allocateAndCopy(passportNumber, "None"); allocateAndCopy(height, ""); allocateAndCopy(weight, "");
    allocateAndCopy(eyeColor, ""); allocateAndCopy(hairColor, ""); allocateAndCopy(bloodGroup, "Unknown");
    allocateAndCopy(distinguishingMarks, "None"); allocateAndCopy(permanentAddress, "");
    allocateAndCopy(currentAddress, ""); allocateAndCopy(phoneNumber, ""); allocateAndCopy(email, "");
    allocateAndCopy(criminalRecordNumber, ""); allocateAndCopy(dateOfFirstArrest, "");
    allocateAndCopy(criminalHistorySummary, "None"); allocateAndCopy(knownAssociates, "None");
    allocateAndCopy(gangAffiliation, "None"); allocateAndCopy(firNumber, ""); allocateAndCopy(policeStation, "");
    allocateAndCopy(crimeType, ""); allocateAndCopy(crimeDate, ""); allocateAndCopy(crimeLocation, "");
    allocateAndCopy(crimeDescription, ""); allocateAndCopy(arrestingOfficer, "");
    allocateAndCopy(arrestingOfficerBadge, ""); allocateAndCopy(currentStatus, "Active");
    allocateAndCopy(courtName, ""); allocateAndCopy(caseNumber, ""); allocateAndCopy(verdict, "Pending");
    allocateAndCopy(sentence, "None"); allocateAndCopy(bailStatus, "None");
    allocateAndCopy(fingerprints, "Not Available"); allocateAndCopy(dnaProfile, "Not Available");
    allocateAndCopy(photographPath, "None"); allocateAndCopy(additionalNotes, "None");
    allocateAndCopy(createdBy, ""); allocateAndCopy(creationDate, "");
    allocateAndCopy(lastModifiedBy, ""); allocateAndCopy(lastModifiedDate, "");
}
CriminalRecord::CriminalRecord(const CriminalRecord& other) {
    criminalID = nullptr; fullName = nullptr; alias = nullptr;
    fatherName = nullptr; motherName = nullptr; dateOfBirth = nullptr;
    gender = nullptr; nationality = nullptr; cnic = nullptr; passportNumber = nullptr;
    height = nullptr; weight = nullptr; eyeColor = nullptr; hairColor = nullptr;
    bloodGroup = nullptr; distinguishingMarks = nullptr;
    permanentAddress = nullptr; currentAddress = nullptr; phoneNumber = nullptr; email = nullptr;
    criminalRecordNumber = nullptr; dateOfFirstArrest = nullptr; criminalHistorySummary = nullptr;
    knownAssociates = nullptr; gangAffiliation = nullptr;
    firNumber = nullptr; policeStation = nullptr; crimeType = nullptr;
    crimeDate = nullptr; crimeLocation = nullptr; crimeDescription = nullptr;
    arrestingOfficer = nullptr; arrestingOfficerBadge = nullptr;
    currentStatus = nullptr; courtName = nullptr; caseNumber = nullptr;
    verdict = nullptr; sentence = nullptr; bailStatus = nullptr;
    fingerprints = nullptr; dnaProfile = nullptr; photographPath = nullptr;
    additionalNotes = nullptr; createdBy = nullptr; creationDate = nullptr;
    lastModifiedBy = nullptr; lastModifiedDate = nullptr;
    totalArrests = 0; totalConvictions = 0;
    *this = other;
}
CriminalRecord::~CriminalRecord() {
    deallocate(criminalID); deallocate(fullName); deallocate(alias);
    deallocate(fatherName); deallocate(motherName); deallocate(dateOfBirth);
    deallocate(gender); deallocate(nationality); deallocate(cnic); deallocate(passportNumber);
    deallocate(height); deallocate(weight); deallocate(eyeColor); deallocate(hairColor);
    deallocate(bloodGroup); deallocate(distinguishingMarks);
    deallocate(permanentAddress); deallocate(currentAddress); deallocate(phoneNumber); deallocate(email);
    deallocate(criminalRecordNumber); deallocate(dateOfFirstArrest); deallocate(criminalHistorySummary);
    deallocate(knownAssociates); deallocate(gangAffiliation);
    deallocate(firNumber); deallocate(policeStation); deallocate(crimeType);
    deallocate(crimeDate); deallocate(crimeLocation); deallocate(crimeDescription);
    deallocate(arrestingOfficer); deallocate(arrestingOfficerBadge);
    deallocate(currentStatus); deallocate(courtName); deallocate(caseNumber);
    deallocate(verdict); deallocate(sentence); deallocate(bailStatus);
    deallocate(fingerprints); deallocate(dnaProfile); deallocate(photographPath);
    deallocate(additionalNotes); deallocate(createdBy); deallocate(creationDate);
    deallocate(lastModifiedBy); deallocate(lastModifiedDate);
}
CriminalRecord& CriminalRecord::operator=(const CriminalRecord& other) {
    if (this == &other) return *this;
    allocateAndCopy(criminalID, other.criminalID); allocateAndCopy(fullName, other.fullName);
    allocateAndCopy(alias, other.alias); allocateAndCopy(fatherName, other.fatherName);
    allocateAndCopy(motherName, other.motherName); allocateAndCopy(dateOfBirth, other.dateOfBirth);
    allocateAndCopy(gender, other.gender); allocateAndCopy(nationality, other.nationality);
    allocateAndCopy(cnic, other.cnic); allocateAndCopy(passportNumber, other.passportNumber);
    allocateAndCopy(height, other.height); allocateAndCopy(weight, other.weight);
    allocateAndCopy(eyeColor, other.eyeColor); allocateAndCopy(hairColor, other.hairColor);
    allocateAndCopy(bloodGroup, other.bloodGroup); allocateAndCopy(distinguishingMarks, other.distinguishingMarks);
    allocateAndCopy(permanentAddress, other.permanentAddress); allocateAndCopy(currentAddress, other.currentAddress);
    allocateAndCopy(phoneNumber, other.phoneNumber); allocateAndCopy(email, other.email);
    allocateAndCopy(criminalRecordNumber, other.criminalRecordNumber);
    allocateAndCopy(dateOfFirstArrest, other.dateOfFirstArrest);
    allocateAndCopy(criminalHistorySummary, other.criminalHistorySummary);
    totalArrests = other.totalArrests; totalConvictions = other.totalConvictions;
    allocateAndCopy(knownAssociates, other.knownAssociates); allocateAndCopy(gangAffiliation, other.gangAffiliation);
    allocateAndCopy(firNumber, other.firNumber); allocateAndCopy(policeStation, other.policeStation);
    allocateAndCopy(crimeType, other.crimeType); allocateAndCopy(crimeDate, other.crimeDate);
    allocateAndCopy(crimeLocation, other.crimeLocation); allocateAndCopy(crimeDescription, other.crimeDescription);
    allocateAndCopy(arrestingOfficer, other.arrestingOfficer);
    allocateAndCopy(arrestingOfficerBadge, other.arrestingOfficerBadge);
    allocateAndCopy(currentStatus, other.currentStatus); allocateAndCopy(courtName, other.courtName);
    allocateAndCopy(caseNumber, other.caseNumber); allocateAndCopy(verdict, other.verdict);
    allocateAndCopy(sentence, other.sentence); allocateAndCopy(bailStatus, other.bailStatus);
    allocateAndCopy(fingerprints, other.fingerprints); allocateAndCopy(dnaProfile, other.dnaProfile);
    allocateAndCopy(photographPath, other.photographPath); allocateAndCopy(additionalNotes, other.additionalNotes);
    allocateAndCopy(createdBy, other.createdBy); allocateAndCopy(creationDate, other.creationDate);
    allocateAndCopy(lastModifiedBy, other.lastModifiedBy); allocateAndCopy(lastModifiedDate, other.lastModifiedDate);
    return *this;
}
// Setters
CriminalRecord& CriminalRecord::setCriminalID(const char* id) { allocateAndCopy(criminalID, id); return *this; }
CriminalRecord& CriminalRecord::setFullName(const char* name) { allocateAndCopy(fullName, name); return *this; }
CriminalRecord& CriminalRecord::setAlias(const char* a) { allocateAndCopy(alias, a); return *this; }
CriminalRecord& CriminalRecord::setFatherName(const char* name) { allocateAndCopy(fatherName, name); return *this; }
CriminalRecord& CriminalRecord::setMotherName(const char* name) { allocateAndCopy(motherName, name); return *this; }
CriminalRecord& CriminalRecord::setDateOfBirth(const char* dob) { allocateAndCopy(dateOfBirth, dob); return *this; }
CriminalRecord& CriminalRecord::setGender(const char* g) { allocateAndCopy(gender, g); return *this; }
CriminalRecord& CriminalRecord::setNationality(const char* nation) { allocateAndCopy(nationality, nation); return *this; }
CriminalRecord& CriminalRecord::setCNIC(const char* c) { allocateAndCopy(cnic, c); return *this; }
CriminalRecord& CriminalRecord::setPassportNumber(const char* passport) { allocateAndCopy(passportNumber, passport); return *this; }
CriminalRecord& CriminalRecord::setHeight(const char* h) { allocateAndCopy(height, h); return *this; }
CriminalRecord& CriminalRecord::setWeight(const char* w) { allocateAndCopy(weight, w); return *this; }
CriminalRecord& CriminalRecord::setEyeColor(const char* color) { allocateAndCopy(eyeColor, color); return *this; }
CriminalRecord& CriminalRecord::setHairColor(const char* color) { allocateAndCopy(hairColor, color); return *this; }
CriminalRecord& CriminalRecord::setBloodGroup(const char* bg) { allocateAndCopy(bloodGroup, bg); return *this; }
CriminalRecord& CriminalRecord::setDistinguishingMarks(const char* m) { allocateAndCopy(distinguishingMarks, m); return *this; }
CriminalRecord& CriminalRecord::setPermanentAddress(const char* addr) { allocateAndCopy(permanentAddress, addr); return *this; }
CriminalRecord& CriminalRecord::setCurrentAddress(const char* addr) { allocateAndCopy(currentAddress, addr); return *this; }
CriminalRecord& CriminalRecord::setPhoneNumber(const char* phone) { allocateAndCopy(phoneNumber, phone); return *this; }
CriminalRecord& CriminalRecord::setEmail(const char* e) { allocateAndCopy(email, e); return *this; }
CriminalRecord& CriminalRecord::setCriminalRecordNumber(const char* r) { allocateAndCopy(criminalRecordNumber, r); return *this; }
CriminalRecord& CriminalRecord::setDateOfFirstArrest(const char* date) { allocateAndCopy(dateOfFirstArrest, date); return *this; }
CriminalRecord& CriminalRecord::setCriminalHistorySummary(const char* s) { allocateAndCopy(criminalHistorySummary, s); return *this; }
CriminalRecord& CriminalRecord::setTotalArrests(int arrests) { totalArrests = arrests; return *this; }
CriminalRecord& CriminalRecord::setTotalConvictions(int convictions) { totalConvictions = convictions; return *this; }
CriminalRecord& CriminalRecord::setKnownAssociates(const char* a) { allocateAndCopy(knownAssociates, a); return *this; }
CriminalRecord& CriminalRecord::setGangAffiliation(const char* gang) { allocateAndCopy(gangAffiliation, gang); return *this; }
CriminalRecord& CriminalRecord::setFIRNumber(const char* fir) { allocateAndCopy(firNumber, fir); return *this; }
CriminalRecord& CriminalRecord::setPoliceStation(const char* s) { allocateAndCopy(policeStation, s); return *this; }
CriminalRecord& CriminalRecord::setCrimeType(const char* type) { allocateAndCopy(crimeType, type); return *this; }
CriminalRecord& CriminalRecord::setCrimeDate(const char* date) { allocateAndCopy(crimeDate, date); return *this; }
CriminalRecord& CriminalRecord::setCrimeLocation(const char* loc) { allocateAndCopy(crimeLocation, loc); return *this; }
CriminalRecord& CriminalRecord::setCrimeDescription(const char* desc) { allocateAndCopy(crimeDescription, desc); return *this; }
CriminalRecord& CriminalRecord::setArrestingOfficer(const char* o) { allocateAndCopy(arrestingOfficer, o); return *this; }
CriminalRecord& CriminalRecord::setArrestingOfficerBadge(const char* b) { allocateAndCopy(arrestingOfficerBadge, b); return *this; }
CriminalRecord& CriminalRecord::setCurrentStatus(const char* s) { allocateAndCopy(currentStatus, s); return *this; }
CriminalRecord& CriminalRecord::setCourtName(const char* court) { allocateAndCopy(courtName, court); return *this; }
CriminalRecord& CriminalRecord::setCaseNumber(const char* cn) { allocateAndCopy(caseNumber, cn); return *this; }
CriminalRecord& CriminalRecord::setVerdict(const char* v) { allocateAndCopy(verdict, v); return *this; }
CriminalRecord& CriminalRecord::setSentence(const char* s) { allocateAndCopy(sentence, s); return *this; }
CriminalRecord& CriminalRecord::setBailStatus(const char* bail) { allocateAndCopy(bailStatus, bail); return *this; }
CriminalRecord& CriminalRecord::setFingerprints(const char* fp) { allocateAndCopy(fingerprints, fp); return *this; }
CriminalRecord& CriminalRecord::setDNAProfile(const char* dna) { allocateAndCopy(dnaProfile, dna); return *this; }
CriminalRecord& CriminalRecord::setPhotographPath(const char* path) { allocateAndCopy(photographPath, path); return *this; }
CriminalRecord& CriminalRecord::setAdditionalNotes(const char* notes) { allocateAndCopy(additionalNotes, notes); return *this; }
CriminalRecord& CriminalRecord::setCreatedBy(const char* creator) { allocateAndCopy(createdBy, creator); return *this; }
CriminalRecord& CriminalRecord::setCreationDate(const char* date) { allocateAndCopy(creationDate, date); return *this; }
CriminalRecord& CriminalRecord::setLastModifiedBy(const char* m) { allocateAndCopy(lastModifiedBy, m); return *this; }
CriminalRecord& CriminalRecord::setLastModifiedDate(const char* date) { allocateAndCopy(lastModifiedDate, date); return *this; }
// Getters
const char* CriminalRecord::getCriminalID()           const { return criminalID; }
const char* CriminalRecord::getFullName()             const { return fullName; }
const char* CriminalRecord::getAlias()                const { return alias; }
const char* CriminalRecord::getDateOfBirth()          const { return dateOfBirth; }
const char* CriminalRecord::getGender()               const { return gender; }
const char* CriminalRecord::getCNIC()                 const { return cnic; }
const char* CriminalRecord::getHeight()               const { return height; }
const char* CriminalRecord::getWeight()               const { return weight; }
const char* CriminalRecord::getBloodGroup()           const { return bloodGroup; }
const char* CriminalRecord::getDistinguishingMarks()  const { return distinguishingMarks; }
const char* CriminalRecord::getPermanentAddress()     const { return permanentAddress; }
const char* CriminalRecord::getPhoneNumber()          const { return phoneNumber; }
const char* CriminalRecord::getEmail()                const { return email; }
const char* CriminalRecord::getCriminalRecordNumber() const { return criminalRecordNumber; }
int         CriminalRecord::getTotalArrests()         const { return totalArrests; }
int         CriminalRecord::getTotalConvictions()     const { return totalConvictions; }
const char* CriminalRecord::getKnownAssociates()      const { return knownAssociates; }
const char* CriminalRecord::getGangAffiliation()      const { return gangAffiliation; }
const char* CriminalRecord::getFIRNumber()            const { return firNumber; }
const char* CriminalRecord::getCrimeType()            const { return crimeType; }
const char* CriminalRecord::getCrimeDate()            const { return crimeDate; }
const char* CriminalRecord::getCrimeLocation()        const { return crimeLocation; }
const char* CriminalRecord::getArrestingOfficer()     const { return arrestingOfficer; }
const char* CriminalRecord::getCurrentStatus()        const { return currentStatus; }
const char* CriminalRecord::getVerdict()              const { return verdict; }
const char* CriminalRecord::getSentence()             const { return sentence; }
bool CriminalRecord::operator==(const char* id) const { return strcmp(criminalID, id) == 0; }
void CriminalRecord::displayBrief() const {
    cout << left << setw(12) << criminalID << " | " << setw(22) << fullName
        << " | " << setw(15) << crimeType << " | " << currentStatus << "\n";
}
void CriminalRecord::display() const {
    cout << "\n========== CRIMINAL RECORD ==========\n";
    cout << "Criminal ID   : " << criminalID << "\n";
    cout << "Full Name     : " << fullName << "\n";
    cout << "Alias         : " << alias << "\n";
    cout << "Date of Birth : " << dateOfBirth << "\n";
    cout << "Gender        : " << gender << "\n";
    cout << "CNIC          : " << cnic << "\n";
    cout << "Nationality   : " << nationality << "\n";
    cout << "Blood Group   : " << bloodGroup << "\n";
    cout << "Height / Wt   : " << height << " / " << weight << "\n";
    cout << "Eye / Hair    : " << eyeColor << " / " << hairColor << "\n";
    cout << "Marks         : " << distinguishingMarks << "\n";
    cout << "\n--- Criminal History ---\n";
    cout << "Record No     : " << criminalRecordNumber << "\n";
    cout << "FIR Number    : " << firNumber << "\n";
    cout << "Police Station: " << policeStation << "\n";
    cout << "Crime Type    : " << crimeType << "\n";
    cout << "Crime Date    : " << crimeDate << "\n";
    cout << "Crime Location: " << crimeLocation << "\n";
    cout << "Total Arrests : " << totalArrests << "\n";
    cout << "Convictions   : " << totalConvictions << "\n";
    cout << "Gang/Assoc    : " << gangAffiliation << " / " << knownAssociates << "\n";
    cout << "\n--- Legal Status ---\n";
    cout << "Status        : " << currentStatus << "\n";
    cout << "Verdict       : " << verdict << "\n";
    cout << "Sentence      : " << sentence << "\n";
    cout << "Bail Status   : " << bailStatus << "\n";
    cout << "======================================\n";
}

//  COMPLAINT  
void Complaint::setDynamic(char*& dest, const char* src) {
    delete[] dest;
    if (src && strlen(src) > 0) {
        dest = new char[strlen(src) + 1]; strcpy(dest, src);
    }
    else { dest = new char[1]; dest[0] = '\0'; }
}
Complaint::Complaint() {
    trackingNumber = 0;
    complainantName = nullptr; complainantCNIC = nullptr; complainantPhone = nullptr;
    complainantAddress = nullptr; incidentType = nullptr; incidentDate = nullptr;
    incidentLocation = nullptr; incidentDescription = nullptr; accusedName = nullptr;
    witnesses = nullptr; status = nullptr; assignedOfficer = nullptr; filingDate = nullptr;
    setDynamic(complainantName, "");   setDynamic(complainantCNIC, "");
    setDynamic(complainantPhone, ""); setDynamic(complainantAddress, "");
    setDynamic(incidentType, "");     setDynamic(incidentDate, "");
    setDynamic(incidentLocation, ""); setDynamic(incidentDescription, "");
    setDynamic(accusedName, "None");  setDynamic(witnesses, "None");
    setDynamic(status, "Pending");    setDynamic(assignedOfficer, "Not Assigned");
    setDynamic(filingDate, "");
}
Complaint::Complaint(const Complaint& other) {
    complainantName = nullptr; complainantCNIC = nullptr; complainantPhone = nullptr;
    complainantAddress = nullptr; incidentType = nullptr; incidentDate = nullptr;
    incidentLocation = nullptr; incidentDescription = nullptr; accusedName = nullptr;
    witnesses = nullptr; status = nullptr; assignedOfficer = nullptr; filingDate = nullptr;
    trackingNumber = 0;
    *this = other;
}
Complaint::~Complaint() {
    delete[] complainantName; delete[] complainantCNIC; delete[] complainantPhone;
    delete[] complainantAddress; delete[] incidentType; delete[] incidentDate;
    delete[] incidentLocation; delete[] incidentDescription; delete[] accusedName;
    delete[] witnesses; delete[] status; delete[] assignedOfficer; delete[] filingDate;
}
Complaint& Complaint::setTrackingNumber(long long num) { trackingNumber = num; return *this; }
Complaint& Complaint::setComplainantName(const char* n) { setDynamic(complainantName, n); return *this; }
Complaint& Complaint::setComplainantCNIC(const char* c) { setDynamic(complainantCNIC, c); return *this; }
Complaint& Complaint::setComplainantPhone(const char* p) { setDynamic(complainantPhone, p); return *this; }
Complaint& Complaint::setComplainantAddress(const char* a) { setDynamic(complainantAddress, a); return *this; }
Complaint& Complaint::setIncidentType(const char* t) { setDynamic(incidentType, t); return *this; }
Complaint& Complaint::setIncidentDate(const char* d) { setDynamic(incidentDate, d); return *this; }
Complaint& Complaint::setIncidentLocation(const char* l) { setDynamic(incidentLocation, l); return *this; }
Complaint& Complaint::setIncidentDescription(const char* d) { setDynamic(incidentDescription, d); return *this; }
Complaint& Complaint::setAccusedName(const char* n) { setDynamic(accusedName, n); return *this; }
Complaint& Complaint::setWitnesses(const char* w) { setDynamic(witnesses, w); return *this; }
Complaint& Complaint::setStatus(const char* s) { setDynamic(status, s); return *this; }
Complaint& Complaint::setAssignedOfficer(const char* o) { setDynamic(assignedOfficer, o); return *this; }
Complaint& Complaint::setFilingDate(const char* d) { setDynamic(filingDate, d); return *this; }
long long   Complaint::getTrackingNumber()  const { return trackingNumber; }
const char* Complaint::getComplainantName() const { return complainantName; }
const char* Complaint::getComplainantCNIC() const { return complainantCNIC; }
const char* Complaint::getStatus()          const { return status; }
const char* Complaint::getAssignedOfficer() const { return assignedOfficer; }
const char* Complaint::getIncidentType()    const { return incidentType; }
const char* Complaint::getFilingDate()      const { return filingDate; }
void Complaint::displayBrief() const {
    cout << "| " << left << setw(12) << trackingNumber << " | " << setw(20) << complainantName
        << " | " << setw(12) << incidentType << " | " << setw(14) << status << " |\n";
}
void Complaint::displayFull() const {
    cout << "\n+--------------------------------------------------+\n";
    cout << "|               COMPLAINT DETAILS                 |\n";
    cout << "+--------------------------------------------------+\n";
    cout << "| Tracking Number  : " << trackingNumber << "\n";
    cout << "| Filing Date      : " << filingDate << "\n";
    cout << "| Complainant Name : " << complainantName << "\n";
    cout << "| Complainant CNIC : " << complainantCNIC << "\n";
    cout << "| Phone            : " << complainantPhone << "\n";
    cout << "| Address          : " << complainantAddress << "\n";
    cout << "| Incident Type    : " << incidentType << "\n";
    cout << "| Incident Date    : " << incidentDate << "\n";
    cout << "| Incident Location: " << incidentLocation << "\n";
    cout << "| Description      : " << incidentDescription << "\n";
    cout << "| Accused          : " << accusedName << "\n";
    cout << "| Witnesses        : " << witnesses << "\n";
    cout << "| Status           : " << status << "\n";
    cout << "| Assigned Officer : " << assignedOfficer << "\n";
    cout << "+--------------------------------------------------+\n";
}
Complaint& Complaint::operator=(const Complaint& other) {
    if (this == &other) return *this;
    trackingNumber = other.trackingNumber;
    setDynamic(complainantName, other.complainantName);
    setDynamic(complainantCNIC, other.complainantCNIC);
    setDynamic(complainantPhone, other.complainantPhone);
    setDynamic(complainantAddress, other.complainantAddress);
    setDynamic(incidentType, other.incidentType);
    setDynamic(incidentDate, other.incidentDate);
    setDynamic(incidentLocation, other.incidentLocation);
    setDynamic(incidentDescription, other.incidentDescription);
    setDynamic(accusedName, other.accusedName);
    setDynamic(witnesses, other.witnesses);
    setDynamic(status, other.status);
    setDynamic(assignedOfficer, other.assignedOfficer);
    setDynamic(filingDate, other.filingDate);
    return *this;
}

//  CRIME REPORT  
CrimeReport::CrimeReport() {
    reportID = 0;
    crimeType = new char[1]; location = new char[1]; date = new char[1];
    description = new char[1]; reporterName = new char[1];
    status = new char[MAX_STRING]; assignedOfficer = new char[MAX_STRING];
    lastUpdated = new char[1];
    crimeType[0] = '\0'; location[0] = '\0'; date[0] = '\0';
    description[0] = '\0'; reporterName[0] = '\0'; lastUpdated[0] = '\0';
    strcpy(status, "Pending"); strcpy(assignedOfficer, "Not Assigned");
}
CrimeReport::CrimeReport(const CrimeReport& other) {
    reportID = other.reportID;
    crimeType = new char[strlen(other.crimeType) + 1];
    location = new char[strlen(other.location) + 1];
    date = new char[strlen(other.date) + 1];
    description = new char[strlen(other.description) + 1];
    reporterName = new char[strlen(other.reporterName) + 1];
    status = new char[strlen(other.status) + 1];
    assignedOfficer = new char[strlen(other.assignedOfficer) + 1];
    lastUpdated = new char[strlen(other.lastUpdated) + 1];
    strcpy(crimeType, other.crimeType); strcpy(location, other.location);
    strcpy(date, other.date); strcpy(description, other.description);
    strcpy(reporterName, other.reporterName); strcpy(status, other.status);
    strcpy(assignedOfficer, other.assignedOfficer); strcpy(lastUpdated, other.lastUpdated);
}
CrimeReport::~CrimeReport() {
    delete[] crimeType; delete[] location; delete[] date;
    delete[] description; delete[] reporterName; delete[] status; delete[] assignedOfficer; delete[] lastUpdated;
}
void CrimeReport::setReportID(int id) { reportID = id; }
void CrimeReport::setCrimeType(const char* type) { delete[] crimeType;       crimeType = new char[strlen(type) + 1];         strcpy(crimeType, type); }
void CrimeReport::setLocation(const char* loc) { delete[] location;        location = new char[strlen(loc) + 1];          strcpy(location, loc); }
void CrimeReport::setDate(const char* d) { delete[] date;            date = new char[strlen(d) + 1];            strcpy(date, d); }
void CrimeReport::setDescription(const char* desc) { delete[] description;     description = new char[strlen(desc) + 1];       strcpy(description, desc); }
void CrimeReport::setReporterName(const char* name) { delete[] reporterName;    reporterName = new char[strlen(name) + 1];      strcpy(reporterName, name); }
void CrimeReport::setStatus(const char* s) { delete[] status;          status = new char[strlen(s) + 1];            strcpy(status, s); }
void CrimeReport::setAssignedOfficer(const char* o) { delete[] assignedOfficer; assignedOfficer = new char[strlen(o) + 1];     strcpy(assignedOfficer, o); }
void CrimeReport::setLastUpdated(const char* updated) { delete[] lastUpdated; lastUpdated = new char[strlen(updated) + 1]; strcpy(lastUpdated, updated); }
int         CrimeReport::getReportID()        const { return reportID; }
const char* CrimeReport::getCrimeType()       const { return crimeType; }
const char* CrimeReport::getLocation()        const { return location; }
const char* CrimeReport::getDate()            const { return date; }
const char* CrimeReport::getDescription()     const { return description; }
const char* CrimeReport::getReporterName()    const { return reporterName; }
const char* CrimeReport::getStatus()          const { return status; }
const char* CrimeReport::getAssignedOfficer() const { return assignedOfficer; }
const char* CrimeReport::getLastUpdated()     const { return lastUpdated; }
void CrimeReport::display() const {
    cout << "\n----------------------------------------\n";
    cout << "Report ID        : " << reportID << "\n";
    cout << "Crime Type       : " << crimeType << "\n";
    cout << "Location         : " << location << "\n";
    cout << "Date             : " << date << "\n";
    cout << "Description      : " << description << "\n";
    cout << "Reported By      : " << reporterName << "\n";
    cout << "Status           : " << status << "\n";
    cout << "Assigned Officer : " << assignedOfficer << "\n";
}
CrimeReport& CrimeReport::operator=(const CrimeReport& other) {
    if (this == &other) return *this;
    reportID = other.reportID;
    delete[] crimeType; delete[] location; delete[] date;
    delete[] description; delete[] reporterName; delete[] status; delete[] assignedOfficer; delete[] lastUpdated;
    crimeType = new char[strlen(other.crimeType) + 1];
    location = new char[strlen(other.location) + 1];
    date = new char[strlen(other.date) + 1];
    description = new char[strlen(other.description) + 1];
    reporterName = new char[strlen(other.reporterName) + 1];
    status = new char[strlen(other.status) + 1];
    assignedOfficer = new char[strlen(other.assignedOfficer) + 1];
    lastUpdated = new char[strlen(other.lastUpdated) + 1];
    strcpy(crimeType, other.crimeType); strcpy(location, other.location);
    strcpy(date, other.date); strcpy(description, other.description);
    strcpy(reporterName, other.reporterName); strcpy(status, other.status);
    strcpy(assignedOfficer, other.assignedOfficer); strcpy(lastUpdated, other.lastUpdated);
    return *this;
}

//  USER  
User::User() {
    username = new char[1]; password = new char[1]; fullName = new char[1];
    phone = new char[1]; email = new char[1]; securityQuestion = new char[1]; securityAnswer = new char[1];
    username[0] = '\0'; password[0] = '\0'; fullName[0] = '\0'; phone[0] = '\0'; email[0] = '\0';
    securityQuestion[0] = '\0'; securityAnswer[0] = '\0';
}
User::User(const User& other) {
    username = new char[strlen(other.username) + 1]; password = new char[strlen(other.password) + 1];
    fullName = new char[strlen(other.fullName) + 1]; phone = new char[strlen(other.phone) + 1];
    email = new char[strlen(other.email) + 1]; securityQuestion = new char[strlen(other.securityQuestion) + 1];
    securityAnswer = new char[strlen(other.securityAnswer) + 1];
    strcpy(username, other.username); strcpy(password, other.password);
    strcpy(fullName, other.fullName); strcpy(phone, other.phone); strcpy(email, other.email);
    strcpy(securityQuestion, other.securityQuestion); strcpy(securityAnswer, other.securityAnswer);
}
User::~User() { delete[] username; delete[] password; delete[] fullName; delete[] phone; delete[] email; delete[] securityQuestion; delete[] securityAnswer; }
void User::setUsername(const char* uname) { delete[] username; username = new char[strlen(uname) + 1]; strcpy(username, uname); }
void User::setPassword(const char* pwd) { delete[] password; password = new char[strlen(pwd) + 1];   strcpy(password, pwd); }
void User::setFullName(const char* name) { delete[] fullName; fullName = new char[strlen(name) + 1];  strcpy(fullName, name); }
void User::setPhone(const char* ph) { delete[] phone;    phone = new char[strlen(ph) + 1];    strcpy(phone, ph); }
void User::setEmail(const char* mail) { delete[] email;    email = new char[strlen(mail) + 1];  strcpy(email, mail); }
void User::setSecurityQuestion(const char* sq) { delete[] securityQuestion; securityQuestion = new char[strlen(sq) + 1]; strcpy(securityQuestion, sq); }
void User::setSecurityAnswer(const char* sa) { delete[] securityAnswer; securityAnswer = new char[strlen(sa) + 1]; strcpy(securityAnswer, sa); }
const char* User::getUsername() const { return username; }
const char* User::getPassword() const { return password; }
const char* User::getFullName() const { return fullName; }
const char* User::getPhone()    const { return phone; }
const char* User::getEmail()    const { return email; }
const char* User::getSecurityQuestion() const { return securityQuestion; }
const char* User::getSecurityAnswer() const { return securityAnswer; }
void User::display() const {
    cout << "\n----------------------------------------\n";
    cout << "Username  : " << username << "\n";
    cout << "Full Name : " << fullName << "\n";
    cout << "Phone     : " << phone << "\n";
    cout << "Email     : " << email << "\n";
}
bool User::authenticate(const char* uname, const char* pwd) const {
    return (strcmp(username, uname) == 0 && strcmp(password, pwd) == 0);
}
User& User::operator=(const User& other) {
    if (this == &other) return *this;
    delete[] username; delete[] password; delete[] fullName; delete[] phone; delete[] email; delete[] securityQuestion; delete[] securityAnswer;
    username = new char[strlen(other.username) + 1]; password = new char[strlen(other.password) + 1];
    fullName = new char[strlen(other.fullName) + 1]; phone = new char[strlen(other.phone) + 1];
    email = new char[strlen(other.email) + 1]; securityQuestion = new char[strlen(other.securityQuestion) + 1];
    securityAnswer = new char[strlen(other.securityAnswer) + 1];
    strcpy(username, other.username); strcpy(password, other.password);
    strcpy(fullName, other.fullName); strcpy(phone, other.phone); strcpy(email, other.email);
    strcpy(securityQuestion, other.securityQuestion); strcpy(securityAnswer, other.securityAnswer);
    return *this;
}

//  ADMIN 
Admin::Admin() {
    username = new char[1]; password = new char[1]; fullName = new char[1];
    badgeNumber = new char[1]; policeStation = new char[1];
    securityQuestion = new char[1]; securityAnswer = new char[1];
    username[0] = '\0'; password[0] = '\0'; fullName[0] = '\0'; badgeNumber[0] = '\0'; policeStation[0] = '\0';
    securityQuestion[0] = '\0'; securityAnswer[0] = '\0';
}
Admin::Admin(const Admin& other) {
    username = new char[strlen(other.username) + 1];
    password = new char[strlen(other.password) + 1];
    fullName = new char[strlen(other.fullName) + 1];
    badgeNumber = new char[strlen(other.badgeNumber) + 1];
    policeStation = new char[strlen(other.policeStation) + 1];
    securityQuestion = new char[strlen(other.securityQuestion) + 1];
    securityAnswer = new char[strlen(other.securityAnswer) + 1];
    strcpy(username, other.username); strcpy(password, other.password);
    strcpy(fullName, other.fullName); strcpy(badgeNumber, other.badgeNumber);
    strcpy(policeStation, other.policeStation); strcpy(securityQuestion, other.securityQuestion);
    strcpy(securityAnswer, other.securityAnswer);
}
Admin::~Admin() { delete[] username; delete[] password; delete[] fullName; delete[] badgeNumber; delete[] policeStation; delete[] securityQuestion; delete[] securityAnswer; }
void Admin::setUsername(const char* uname) { delete[] username;      username = new char[strlen(uname) + 1];    strcpy(username, uname); }
void Admin::setPassword(const char* pwd) { delete[] password;      password = new char[strlen(pwd) + 1];      strcpy(password, pwd); }
void Admin::setFullName(const char* name) { delete[] fullName;      fullName = new char[strlen(name) + 1];     strcpy(fullName, name); }
void Admin::setBadgeNumber(const char* badge) { delete[] badgeNumber;   badgeNumber = new char[strlen(badge) + 1];    strcpy(badgeNumber, badge); }
void Admin::setPoliceStation(const char* s) { delete[] policeStation; policeStation = new char[strlen(s) + 1];        strcpy(policeStation, s); }
void Admin::setSecurityQuestion(const char* sq) { delete[] securityQuestion; securityQuestion = new char[strlen(sq) + 1]; strcpy(securityQuestion, sq); }
void Admin::setSecurityAnswer(const char* sa) { delete[] securityAnswer; securityAnswer = new char[strlen(sa) + 1]; strcpy(securityAnswer, sa); }
const char* Admin::getUsername()      const { return username; }
const char* Admin::getPassword()      const { return password; }
const char* Admin::getFullName()      const { return fullName; }
const char* Admin::getBadgeNumber()   const { return badgeNumber; }
const char* Admin::getPoliceStation() const { return policeStation; }
const char* Admin::getSecurityQuestion() const { return securityQuestion; }
const char* Admin::getSecurityAnswer() const { return securityAnswer; }
void Admin::display() const {
    cout << "\n----------------------------------------\n";
    cout << "Username       : " << username << "\n";
    cout << "Full Name      : " << fullName << "\n";
    cout << "Badge Number   : " << badgeNumber << "\n";
    cout << "Police Station : " << policeStation << "\n";
}
bool Admin::authenticate(const char* uname, const char* pwd) const {
    return (strcmp(username, uname) == 0 && strcmp(password, pwd) == 0);
}
Admin& Admin::operator=(const Admin& other) {
    if (this == &other) return *this;
    delete[] username; delete[] password; delete[] fullName; delete[] badgeNumber; delete[] policeStation; delete[] securityQuestion; delete[] securityAnswer;
    username = new char[strlen(other.username) + 1];
    password = new char[strlen(other.password) + 1];
    fullName = new char[strlen(other.fullName) + 1];
    badgeNumber = new char[strlen(other.badgeNumber) + 1];
    policeStation = new char[strlen(other.policeStation) + 1];
    securityQuestion = new char[strlen(other.securityQuestion) + 1];
    securityAnswer = new char[strlen(other.securityAnswer) + 1];
    strcpy(username, other.username); strcpy(password, other.password);
    strcpy(fullName, other.fullName); strcpy(badgeNumber, other.badgeNumber);
    strcpy(policeStation, other.policeStation); strcpy(securityQuestion, other.securityQuestion);
    strcpy(securityAnswer, other.securityAnswer);
    return *this;
}
//  SESSION MANAGER & DATA MANAGER IMPLEMENTATION
SessionManager* SessionManager::instance = nullptr;
DataManager* DataManager::instance = nullptr;

void DataManager::logAudit(const std::string& action, const std::string& details) {
    std::ofstream file("audit_log.txt", std::ios::app);
    if (!file) return;
    time_t now = time(0);
    char* dt = ctime(&now);
    dt[strlen(dt) - 1] = '\0'; // Remove newline
    file << "[" << dt << "] ACTION: " << action << " | DETAILS: " << details << "\n";
}

void DataManager::saveAdmins(Admin** admins, int count) const {
    std::ofstream file("admins.txt");
    if (!file) return;
    file << count << "\n";
    for (int i=0; i<count; i++) {
        Admin* a = admins[i];
        file << a->getUsername() << "\n" << a->getPassword() << "\n"
            << a->getFullName() << "\n" << a->getBadgeNumber() << "\n"
            << a->getPoliceStation() << "\n" << a->getSecurityQuestion() << "\n"
            << a->getSecurityAnswer() << "\n";
    }
}

void DataManager::loadAdmins(Admin**& admins, int& count, int& capacity) const {
    std::ifstream file("admins.txt");
    char buffer[MAX_STRING];
    if (file) {
        file >> count; file.ignore();
        while (capacity <= count) capacity = (capacity == 0) ? 4 : capacity * 2;
        admins = new Admin*[capacity];
        for (int i = 0; i < count; i++) {
            Admin* a = new Admin();
            file.getline(buffer, MAX_STRING); a->setUsername(buffer);
            file.getline(buffer, MAX_STRING); a->setPassword(buffer);
            file.getline(buffer, MAX_STRING); a->setFullName(buffer);
            file.getline(buffer, MAX_STRING); a->setBadgeNumber(buffer);
            file.getline(buffer, MAX_STRING); a->setPoliceStation(buffer);
            file.getline(buffer, MAX_STRING); a->setSecurityQuestion(buffer);
            file.getline(buffer, MAX_STRING); a->setSecurityAnswer(buffer);
            admins[i] = a;
        }
    }
}

void DataManager::saveReports(CrimeReport** reports, int count) const {
    std::ofstream file("crime_reports.txt");
    if (!file) return;
    file << count << "\n";
    for (int i=0; i<count; i++) {
        CrimeReport* r = reports[i];
        file << r->getReportID() << "\n"
            << r->getCrimeType() << "\n"
            << r->getLocation() << "\n"
            << r->getDate() << "\n"
            << r->getDescription() << "\n"
            << r->getReporterName() << "\n"
            << r->getStatus() << "\n"
            << r->getAssignedOfficer() << "\n"
            << r->getLastUpdated() << "\n";
    }
}

void DataManager::loadReports(CrimeReport**& reports, int& count, int& capacity) const {
    std::ifstream file("crime_reports.txt");
    char buffer[MAX_STRING];
    if (file) {
        file >> count; file.ignore();
        while (capacity <= count) capacity = (capacity == 0) ? 4 : capacity * 2;
        reports = new CrimeReport*[capacity];
        for (int i = 0; i < count; i++) {
            CrimeReport* r = new CrimeReport();
            int id; file >> id; file.ignore();
            r->setReportID(id);
            file.getline(buffer, MAX_STRING); r->setCrimeType(buffer);
            file.getline(buffer, MAX_STRING); r->setLocation(buffer);
            file.getline(buffer, MAX_STRING); r->setDate(buffer);
            file.getline(buffer, MAX_STRING); r->setDescription(buffer);
            file.getline(buffer, MAX_STRING); r->setReporterName(buffer);
            file.getline(buffer, MAX_STRING); r->setStatus(buffer);
            file.getline(buffer, MAX_STRING); r->setAssignedOfficer(buffer);
            file.getline(buffer, MAX_STRING); r->setLastUpdated(buffer);
            reports[i] = r;
        }
    }
}

void DataManager::saveUsers(User** users, int count) const {
    std::ofstream file("users.txt");
    if (!file) return;
    file << count << "\n";
    for (int i=0; i<count; i++) {
        User* u = users[i];
        file << u->getUsername() << "\n" << u->getPassword() << "\n"
            << u->getFullName() << "\n" << u->getPhone() << "\n"
            << u->getEmail() << "\n" << u->getSecurityQuestion() << "\n"
            << u->getSecurityAnswer() << "\n";
    }
}

void DataManager::loadUsers(User**& users, int& count, int& capacity) const {
    std::ifstream file("users.txt");
    char buffer[MAX_STRING];
    if (file) {
        file >> count; file.ignore();
        while (capacity <= count) capacity = (capacity == 0) ? 4 : capacity * 2;
        users = new User*[capacity];
        for (int i = 0; i < count; i++) {
            User* u = new User();
            file.getline(buffer, MAX_STRING); u->setUsername(buffer);
            file.getline(buffer, MAX_STRING); u->setPassword(buffer);
            file.getline(buffer, MAX_STRING); u->setFullName(buffer);
            file.getline(buffer, MAX_STRING); u->setPhone(buffer);
            file.getline(buffer, MAX_STRING); u->setEmail(buffer);
            file.getline(buffer, MAX_STRING); u->setSecurityQuestion(buffer);
            file.getline(buffer, MAX_STRING); u->setSecurityAnswer(buffer);
            users[i] = u;
        }
    }
}

//  CRIME MANAGEMENT SYSTEM 
std::unordered_map<std::string, std::vector<std::string>> syndicateNetwork;

CrimeManagementSystem::CrimeManagementSystem() {
    reportCount = 0; reportCapacity = 0;
    userCount = 0; userCapacity = 0;
    adminCount = 0; adminCapacity = 0;
    reports = nullptr; users = nullptr; admins = nullptr;
    suspects = new Suspect*[4];
    suspectCount = 0; suspectCapacity = 4;
    officers = new Officer*[4];
    officerCount = 0; officerCapacity = 4;
    evidenceLog = new Evidence*[4]; evidenceCount = 0; evidenceCapacity = 4;
    archivedSuspects = new ArchivedSuspect*[4]; archivedCount = 0; archivedCapacity = 4;
    archivedCases = new ArchivedCase*[4]; archivedCaseCount = 0; archivedCaseCapacity = 4;
    archivedOfficers = new ArchivedOfficer*[4]; archivedOfficerCount = 0; archivedOfficerCapacity = 4;
    sessionLogs = new LoginSession*[4]; sessionCount = 0; sessionCapacity = 4;
    cases = new Case*[4]; caseCount = 0; caseCapacity = 4;
    nextReportID = 1;
    loadData();
}
CrimeManagementSystem::~CrimeManagementSystem() {
    saveData();
    for (int i = 0; i < userCount; i++) delete users[i];
    delete[] users;
    
    for (int i = 0; i < adminCount; i++) delete admins[i];
    delete[] admins;

    for (int i = 0; i < reportCount; i++) delete reports[i];
    delete[] reports;

    for (int i = 0; i < suspectCount; i++) delete suspects[i];
    delete[] suspects;

    for (int i = 0; i < officerCount; i++) delete officers[i];
    delete[] officers;

    for (int i = 0; i < evidenceCount; i++) delete evidenceLog[i];
    delete[] evidenceLog;

    for (int i = 0; i < archivedCount; i++) delete archivedSuspects[i];
    delete[] archivedSuspects;

    for (int i = 0; i < archivedCaseCount; i++) delete archivedCases[i];
    delete[] archivedCases;

    for (int i = 0; i < archivedOfficerCount; i++) delete archivedOfficers[i];
    delete[] archivedOfficers;

    for (int i = 0; i < sessionCount; i++) delete sessionLogs[i];
    delete[] sessionLogs;

    for (int i = 0; i < caseCount; i++) delete cases[i];
    delete[] cases;
}
void CrimeManagementSystem::resizeUsers() {
    userCapacity = (userCapacity == 0) ? 4 : userCapacity * 2;
    User** temp = new User*[userCapacity];
    for (int i = 0; i < userCount; i++) temp[i] = users[i];
    delete[] users;
    users = temp;
}
void CrimeManagementSystem::resizeAdmins() {
    adminCapacity = (adminCapacity == 0) ? 4 : adminCapacity * 2;
    Admin** temp = new Admin*[adminCapacity];
    for (int i = 0; i < adminCount; i++) temp[i] = admins[i];
    delete[] admins;
    admins = temp;
}
void CrimeManagementSystem::resizeReports() {
    reportCapacity = (reportCapacity == 0) ? 4 : reportCapacity * 2;
    CrimeReport** temp = new CrimeReport*[reportCapacity];
    for (int i = 0; i < reportCount; i++) temp[i] = reports[i];
    delete[] reports;
    reports = temp;
}
void CrimeManagementSystem::resizeOfficers() {
    officerCapacity *= 2;
    Officer** temp = new Officer*[officerCapacity];
    for (int i = 0; i < officerCount; i++) temp[i] = officers[i];
    delete[] officers;
    officers = temp;
}
void CrimeManagementSystem::addNewOfficer(const char* name, int age, const char* rank) {
    if (officerCount == officerCapacity) resizeOfficers();
    officers[officerCount] = new Officer(name, age, rank);
    officerCount++;
    DataManager::getInstance().logAudit("RECRUIT_OFFICER", "Recruited Officer: " + std::string(name) + " - Rank: " + std::string(rank));
}
void CrimeManagementSystem::resizeSuspects() {
    suspectCapacity *= 2;
    Suspect** temp = new Suspect*[suspectCapacity];
    for (int i = 0; i < suspectCount; i++) temp[i] = suspects[i];
    delete[] suspects;
    suspects = temp;
}

void CrimeManagementSystem::resizeEvidenceLog() {
    evidenceCapacity = (evidenceCapacity == 0) ? 4 : evidenceCapacity * 2;
    Evidence** temp = new Evidence*[evidenceCapacity];
    for (int i = 0; i < evidenceCount; i++) temp[i] = evidenceLog[i];
    delete[] evidenceLog;
    evidenceLog = temp;
}

void CrimeManagementSystem::resizeArchivedSuspects() {
    archivedCapacity = (archivedCapacity == 0) ? 4 : archivedCapacity * 2;
    ArchivedSuspect** temp = new ArchivedSuspect*[archivedCapacity];
    for (int i = 0; i < archivedCount; i++) temp[i] = archivedSuspects[i];
    delete[] archivedSuspects;
    archivedSuspects = temp;
}

void CrimeManagementSystem::resizeArchivedCases() {
    archivedCaseCapacity = (archivedCaseCapacity == 0) ? 4 : archivedCaseCapacity * 2;
    ArchivedCase** temp = new ArchivedCase*[archivedCaseCapacity];
    for (int i = 0; i < archivedCaseCount; i++) temp[i] = archivedCases[i];
    delete[] archivedCases;
    archivedCases = temp;
}

void CrimeManagementSystem::resizeArchivedOfficers() {
    archivedOfficerCapacity = (archivedOfficerCapacity == 0) ? 4 : archivedOfficerCapacity * 2;
    ArchivedOfficer** temp = new ArchivedOfficer*[archivedOfficerCapacity];
    for (int i = 0; i < archivedOfficerCount; i++) temp[i] = archivedOfficers[i];
    delete[] archivedOfficers;
    archivedOfficers = temp;
}

void CrimeManagementSystem::resizeSessions() {
    sessionCapacity = (sessionCapacity == 0) ? 4 : sessionCapacity * 2;
    LoginSession** temp = new LoginSession*[sessionCapacity];
    for (int i = 0; i < sessionCount; i++) temp[i] = sessionLogs[i];
    delete[] sessionLogs;
    sessionLogs = temp;
}

void CrimeManagementSystem::logSecureEvidence(const char* suspectName, const char* description, unsigned int generatedHash) {
    if (evidenceCount == evidenceCapacity) resizeEvidenceLog();
    evidenceLog[evidenceCount] = new Evidence(100 + evidenceCount, suspectName, description, generatedHash);
    evidenceCount++;
}

void CrimeManagementSystem::archiveSuspectDirectly(const char* name, const char* crime, const char* reason, const char* date) {
    if (archivedCount == archivedCapacity) resizeArchivedSuspects();
    archivedSuspects[archivedCount] = new ArchivedSuspect();
    archivedSuspects[archivedCount]->name = name;
    archivedSuspects[archivedCount]->originalCrime = crime;
    archivedSuspects[archivedCount]->reasonForRemoval = reason;
    archivedSuspects[archivedCount]->dateRemoved = date;
    archivedCount++;

    DataManager::getInstance().logAudit("ARCHIVE_SUSPECT", "Archived Suspect: " + std::string(name) + " - Reason: " + std::string(reason));
}

void CrimeManagementSystem::archiveCaseDirectly(const char* caseID, const char* criminalName, const char* crimeType, const char* reason, const char* date) {
    if (archivedCaseCount == archivedCaseCapacity) resizeArchivedCases();
    archivedCases[archivedCaseCount] = new ArchivedCase();
    archivedCases[archivedCaseCount]->caseID = caseID;
    archivedCases[archivedCaseCount]->criminalName = criminalName;
    archivedCases[archivedCaseCount]->crimeType = crimeType;
    archivedCases[archivedCaseCount]->closingReason = reason;
    archivedCases[archivedCaseCount]->closingDate = date;
    archivedCaseCount++;

    DataManager::getInstance().logAudit("CLOSE_CASE", "Closed Case: " + std::string(caseID) + " - " + std::string(criminalName) + " - Reason: " + std::string(reason));
}

void CrimeManagementSystem::recordSession(const char* username, const char* role, const char* action) {
    if (sessionCount == sessionCapacity) resizeSessions();
    sessionLogs[sessionCount] = new LoginSession();
    sessionLogs[sessionCount]->username = username;
    sessionLogs[sessionCount]->role = role;
    
    time_t now = time(0);
    char* dt = ctime(&now);
    std::string timeStr(dt);
    if (!timeStr.empty() && timeStr[timeStr.length()-1] == '\n') timeStr.erase(timeStr.length()-1);
    
    if (std::string(action) == "login") {
        sessionLogs[sessionCount]->loginTime = timeStr;
        sessionLogs[sessionCount]->logoutTime = "Active";
    } else {
        sessionLogs[sessionCount]->loginTime = action; // Just using loginTime to store action/time
        sessionLogs[sessionCount]->logoutTime = timeStr;
    }
    sessionCount++;

    DataManager::getInstance().logAudit("SESSION_" + std::string(action), std::string(username) + " (" + std::string(role) + ") " + std::string(action));
}

void CrimeManagementSystem::archiveAndDeleteSuspect(int suspectIndex, const char* reason) {
    if (suspectIndex < 0 || suspectIndex >= suspectCount) return;
    if (archivedCount == archivedCapacity) resizeArchivedSuspects();

    archivedSuspects[archivedCount] = new ArchivedSuspect();
    archivedSuspects[archivedCount]->name = suspects[suspectIndex]->getName();
    archivedSuspects[archivedCount]->reasonForRemoval = reason;
    archivedCount++;

    delete suspects[suspectIndex];
    for (int i = suspectIndex; i < suspectCount - 1; i++) {
        suspects[i] = suspects[i + 1];
    }
    suspectCount--;
}

void CrimeManagementSystem::archiveCase(int caseIndex, const char* reason, const char* date) {
    if (caseIndex < 0 || caseIndex >= caseCount) return;
    if (archivedCaseCount == archivedCaseCapacity) resizeArchivedCases();

    archivedCases[archivedCaseCount] = new ArchivedCase();
    archivedCases[archivedCaseCount]->caseID = std::to_string(cases[caseIndex]->getCaseID());
    archivedCases[archivedCaseCount]->criminalName = cases[caseIndex]->getCriminalName();
    archivedCases[archivedCaseCount]->crimeType = cases[caseIndex]->getCrimeType();
    archivedCases[archivedCaseCount]->closingReason = reason;
    archivedCases[archivedCaseCount]->closingDate = date;
    archivedCaseCount++;

    delete cases[caseIndex];
    for (int i = caseIndex; i < caseCount - 1; i++) {
        cases[i] = cases[i + 1];
    }
    caseCount--;
}

void CrimeManagementSystem::archiveOfficer(int officerIndex, const char* reason, const char* date) {
    if (officerIndex < 0 || officerIndex >= officerCount) return;
    if (archivedOfficerCount == archivedOfficerCapacity) resizeArchivedOfficers();

    archivedOfficers[archivedOfficerCount] = new ArchivedOfficer();
    archivedOfficers[archivedOfficerCount]->name = officers[officerIndex]->getName();
    archivedOfficers[archivedOfficerCount]->finalRank = officers[officerIndex]->getRank();
    archivedOfficers[archivedOfficerCount]->departureReason = reason;
    archivedOfficers[archivedOfficerCount]->departureDate = date;
    archivedOfficerCount++;

    delete officers[officerIndex];
    for (int i = officerIndex; i < officerCount - 1; i++) {
        officers[i] = officers[i + 1];
    }
    officerCount--;
}

void CrimeManagementSystem::recordLogin(const char* username, const char* role, const char* timeIn) {
    if (sessionCount == sessionCapacity) resizeSessions();

    sessionLogs[sessionCount] = new LoginSession();
    sessionLogs[sessionCount]->username = username;
    sessionLogs[sessionCount]->role = role;
    sessionLogs[sessionCount]->loginTime = timeIn;
    sessionLogs[sessionCount]->logoutTime = "ACTIVE";
    sessionCount++;
}

void CrimeManagementSystem::recordLogout(const char* username, const char* timeOut) {
    for (int i = sessionCount - 1; i >= 0; i--) {
        if (sessionLogs[i]->username == username && sessionLogs[i]->logoutTime == "ACTIVE") {
            sessionLogs[i]->logoutTime = timeOut;
            break;
        }
    }
}
void CrimeManagementSystem::addNewSuspect(const char* name, int age, const char* crime) {
    if (suspectCount == suspectCapacity) resizeSuspects();
    suspects[suspectCount] = new Suspect(name, age, crime);
    suspectCount++;
}
void CrimeManagementSystem::loadData() {
    DataManager::getInstance().loadUsers(users, userCount, userCapacity);
    DataManager::getInstance().loadAdmins(admins, adminCount, adminCapacity);
    DataManager::getInstance().loadReports(reports, reportCount, reportCapacity);

    ifstream offFile("officers.txt");
    if (offFile) {
        int n; offFile >> n; offFile.ignore();
        for (int i = 0; i < n; i++) {
            int badge, age; char name[MAX_STRING], rank[MAX_STRING];
            offFile >> badge; offFile.ignore();
            offFile.getline(name, MAX_STRING);
            offFile >> age; offFile.ignore();
            offFile.getline(rank, MAX_STRING);
            if (officerCount == officerCapacity) resizeOfficers();
            officers[officerCount] = new Officer(name, age, rank);
            officers[officerCount]->setBadgeNumber(badge);
            officerCount++;
        }
        offFile.close();
    }

    ifstream asFile("archived_suspects.txt");
    if (asFile) {
        int n; asFile >> n; asFile.ignore();
        for (int i = 0; i < n; i++) {
            if (archivedCount == archivedCapacity) resizeArchivedSuspects();
            archivedSuspects[archivedCount] = new ArchivedSuspect();
            getline(asFile, archivedSuspects[archivedCount]->name);
            getline(asFile, archivedSuspects[archivedCount]->originalCrime);
            getline(asFile, archivedSuspects[archivedCount]->reasonForRemoval);
            getline(asFile, archivedSuspects[archivedCount]->dateRemoved);
            archivedCount++;
        }
        asFile.close();
    }

    ifstream acFile("archived_cases.txt");
    if (acFile) {
        int n; acFile >> n; acFile.ignore();
        for (int i = 0; i < n; i++) {
            if (archivedCaseCount == archivedCaseCapacity) resizeArchivedCases();
            archivedCases[archivedCaseCount] = new ArchivedCase();
            getline(acFile, archivedCases[archivedCaseCount]->caseID);
            getline(acFile, archivedCases[archivedCaseCount]->criminalName);
            getline(acFile, archivedCases[archivedCaseCount]->crimeType);
            getline(acFile, archivedCases[archivedCaseCount]->closingReason);
            getline(acFile, archivedCases[archivedCaseCount]->closingDate);
            archivedCaseCount++;
        }
        acFile.close();
    }

    ifstream slFile("session_logs.txt");
    if (slFile) {
        int n; slFile >> n; slFile.ignore();
        for (int i = 0; i < n; i++) {
            if (sessionCount == sessionCapacity) resizeSessions();
            sessionLogs[sessionCount] = new LoginSession();
            getline(slFile, sessionLogs[sessionCount]->username);
            getline(slFile, sessionLogs[sessionCount]->role);
            getline(slFile, sessionLogs[sessionCount]->loginTime);
            getline(slFile, sessionLogs[sessionCount]->logoutTime);
            sessionCount++;
        }
        slFile.close();
    }

    if (reportCount > 0) {
        nextReportID = reports[reportCount-1]->getReportID() + 1;
    }
    if (adminCount == 0) {
        if (adminCount == adminCapacity) resizeAdmins();
        Admin* a = new Admin();
        a->setUsername("admin"); a->setPassword("admin123");
        a->setFullName("System Administrator");
        a->setBadgeNumber("ADMIN001"); a->setPoliceStation("Central Police Station");
        admins[adminCount++] = a;
    }

    ifstream evFile("evidence_log.txt");
    if (evFile) {
        int n; evFile >> n; evFile.ignore();
        for (int i = 0; i < n; i++) {
            if (evidenceCount == evidenceCapacity) resizeEvidenceLog();
            int id; unsigned int hash; char susp[MAX_STRING], desc[MAX_STRING];
            evFile >> id; evFile.ignore();
            evFile.getline(susp, MAX_STRING);
            evFile.getline(desc, MAX_STRING);
            evFile >> hash; evFile.ignore();
            evidenceLog[evidenceCount] = new Evidence(id, susp, desc, hash);
            evidenceCount++;
        }
        evFile.close();
    }
}
void CrimeManagementSystem::saveData() const {
    DataManager::getInstance().saveUsers(users, userCount);
    DataManager::getInstance().saveAdmins(admins, adminCount);
    DataManager::getInstance().saveReports(reports, reportCount);

    ofstream offFile("officers.txt");
    if (offFile) {
        offFile << officerCount << "\n";
        for (int i = 0; i < officerCount; i++) {
            offFile << officers[i]->getBadgeNumber() << "\n"
                    << officers[i]->getName() << "\n"
                    << officers[i]->getAge() << "\n"
                    << officers[i]->getRank() << "\n";
        }
        offFile.close();
    }

    ofstream asFile("archived_suspects.txt");
    if (asFile) {
        asFile << archivedCount << "\n";
        for (int i = 0; i < archivedCount; i++) {
            asFile << archivedSuspects[i]->name << "\n"
                   << archivedSuspects[i]->originalCrime << "\n"
                   << archivedSuspects[i]->reasonForRemoval << "\n"
                   << archivedSuspects[i]->dateRemoved << "\n";
        }
        asFile.close();
    }

    ofstream acFile("archived_cases.txt");
    if (acFile) {
        acFile << archivedCaseCount << "\n";
        for (int i = 0; i < archivedCaseCount; i++) {
            acFile << archivedCases[i]->caseID << "\n"
                   << archivedCases[i]->criminalName << "\n"
                   << archivedCases[i]->crimeType << "\n"
                   << archivedCases[i]->closingReason << "\n"
                   << archivedCases[i]->closingDate << "\n";
        }
        acFile.close();
    }

    ofstream slFile("session_logs.txt");
    if (slFile) {
        slFile << sessionCount << "\n";
        for (int i = 0; i < sessionCount; i++) {
            slFile << sessionLogs[i]->username << "\n"
                   << sessionLogs[i]->role << "\n"
                   << sessionLogs[i]->loginTime << "\n"
                   << sessionLogs[i]->logoutTime << "\n";
        }
        slFile.close();
    }

    ofstream evFile("evidence_log.txt");
    if (evFile) {
        evFile << evidenceCount << "\n";
        for (int i = 0; i < evidenceCount; i++) {
            evFile << evidenceLog[i]->getEvidenceID() << "\n"
                   << evidenceLog[i]->getLinkedSuspect() << "\n"
                   << evidenceLog[i]->getDescription() << "\n"
                   << evidenceLog[i]->getSecureHash() << "\n";
        }
        evFile.close();
    }
}
bool CrimeManagementSystem::isUsernameTaken(const char* username) const {
    for (int i=0; i<userCount; i++) {
        if (strcmp(users[i]->getUsername(), username) == 0) return true;
    }
    return false;
}
void CrimeManagementSystem::userRegistration() {
    char username[MAX_STRING];
    cout << "\n=== USER REGISTRATION ===\n";
    cout << "Enter Username: "; cin.getline(username, MAX_STRING);
    if (isUsernameTaken(username)) { cout << "Username already exists!\n"; return; }
    if (userCount == userCapacity) resizeUsers();
    User* u = new User();
    u->setUsername(username);
    char buf[MAX_STRING];
    cout << "Enter Password: "; cin.getline(buf, MAX_STRING); u->setPassword(buf);
    cout << "Enter Full Name: "; cin.getline(buf, MAX_STRING); u->setFullName(buf);
    cout << "Enter Phone Number: "; cin.getline(buf, MAX_STRING); u->setPhone(buf);
    cout << "Enter Email: "; cin.getline(buf, MAX_STRING); u->setEmail(buf);
    cout << "Enter Security Question: "; cin.getline(buf, MAX_STRING); u->setSecurityQuestion(buf);
    cout << "Enter Security Answer: "; cin.getline(buf, MAX_STRING); u->setSecurityAnswer(buf);
    users[userCount++] = u;
    cout << "Registration successful!\n";
}
void CrimeManagementSystem::userRegistration(const char* username, const char* pass, const char* name, const char* phone, const char* email, const char* sq, const char* sa) {
    if (isUsernameTaken(username)) return;
    if (userCount == userCapacity) resizeUsers();
    User* u = new User();
    u->setUsername(username);
    u->setPassword(pass);
    u->setFullName(name);
    u->setPhone(phone);
    u->setEmail(email);
    u->setSecurityQuestion(sq);
    u->setSecurityAnswer(sa);
    users[userCount++] = u;
    DataManager::getInstance().logAudit("ADD_USER", "Created user: " + std::string(username));
    saveData();
}
bool CrimeManagementSystem::userLogin(char* loggedInUsername) {
    char username[MAX_STRING], password[MAX_STRING];
    cout << "\n=== USER LOGIN ===\n";
    cout << "Username: "; cin.getline(username, MAX_STRING);
    cout << "Password: "; cin.getline(password, MAX_STRING);
    for (int i=0; i<userCount; i++) {
        if (users[i]->authenticate(username, password)) {
            strcpy(loggedInUsername, username);
            cout << "Login successful! Welcome " << users[i]->getFullName() << "!\n";
            SessionManager::getInstance().login(username, false);
            return true;
        }
    }
    cout << "Invalid username or password!\n"; return false;
}
bool CrimeManagementSystem::userLogin(const char* username, const char* password) {
    for (int i=0; i<userCount; i++) {
        if (users[i]->authenticate(username, password)) {
            SessionManager::getInstance().login(username, false);
            recordSession(username, "User", "login");
            DataManager::getInstance().logAudit("LOGIN", "User login: " + std::string(username));
            return true;
        }
    }
    return false;
}
void CrimeManagementSystem::adminLogin() {
    char username[MAX_STRING], password[MAX_STRING];
    cout << "\n=== ADMIN LOGIN ===\n";
    cout << "Username: "; cin.getline(username, MAX_STRING);
    cout << "Password: "; cin.getline(password, MAX_STRING);
    for (int i=0; i<adminCount; i++) {
        if (admins[i]->authenticate(username, password)) {
            cout << "Login successful! Welcome Admin " << admins[i]->getFullName() << "!\n";
            SessionManager::getInstance().login(username, true);
            adminMenu(); return;
        }
    }
    cout << "Invalid admin credentials!\n";
}
bool CrimeManagementSystem::adminLogin(const char* username, const char* password) {
    for (int i=0; i<adminCount; i++) {
        if (admins[i]->authenticate(username, password)) {
            SessionManager::getInstance().login(username, true);
            recordSession(username, "Admin", "login");
            DataManager::getInstance().logAudit("LOGIN", "Admin login: " + std::string(username));
            return true;
        }
    }
    return false;
}
void CrimeManagementSystem::fileCrimeReport(const char* username) {
    cout << "\n=== FILE CRIME REPORT ===\n";
    if (reportCount == reportCapacity) resizeReports();
    CrimeReport* r = new CrimeReport();
    r->setReportID(nextReportID++);
    char buf[MAX_STRING];
    cout << "Crime Type (Theft/Assault/Fraud/Murder/Other): "; cin.getline(buf, MAX_STRING); r->setCrimeType(buf);
    cout << "Location: "; cin.getline(buf, MAX_STRING); r->setLocation(buf);
    
    // Real-time clock for the date
    time_t now = time(0);
    char* dt = ctime(&now);
    dt[strlen(dt) - 1] = '\0'; // Remove newline
    r->setDate(dt);
    
    cout << "Description: "; cin.getline(buf, MAX_STRING); r->setDescription(buf);
    r->setReporterName(username);
    cout << "Report filed! Your Report ID: " << r->getReportID() << "\n";
    reports[reportCount++] = r;
    
    // Save reports immediately
    DataManager::getInstance().saveReports(reports, reportCount);
}
void CrimeManagementSystem::viewMyReports(const char* username) const {
    cout << "\n=== MY CRIME REPORTS ===\n"; bool found = false;
    for (int i=0; i<reportCount; i++) {
        if (strcmp(reports[i]->getReporterName(), username) == 0) { reports[i]->display(); found = true; }
    }
    if (!found) cout << "No reports found for this user.\n";
}
void CrimeManagementSystem::viewAllReports() const {
    cout << "\n=== ALL CRIME REPORTS ===\n";
    if (reportCount == 0) { cout << "No reports found.\n"; return; }
    for (int i=0; i<reportCount; i++) reports[i]->display();
}
void CrimeManagementSystem::updateReportStatus() {
    int id; cout << "\n=== UPDATE REPORT STATUS ===\nEnter Report ID: "; cin >> id; cin.ignore();
    for (int i=0; i<reportCount; i++) {
        if (reports[i]->getReportID() == id) {
            cout << "Current Status: " << reports[i]->getStatus() << "\n";
            char buf[MAX_STRING];
            cout << "New Status (Pending/Investigating/Resolved): "; cin.getline(buf, MAX_STRING);
            reports[i]->setStatus(buf); cout << "Status updated!\n";
            DataManager::getInstance().logAudit("UPDATE_REPORT_STATUS", "Report ID: " + std::to_string(id) + " to " + std::string(buf));
            return;
        }
    }
    cout << "Report ID not found!\n";
}
void CrimeManagementSystem::assignOfficerToReport() {
    int id; cout << "\n=== ASSIGN OFFICER ===\nEnter Report ID: "; cin >> id; cin.ignore();
    for (int i=0; i<reportCount; i++) {
        if (reports[i]->getReportID() == id) {
            char buf[MAX_STRING];
            cout << "Enter Officer Name: "; cin.getline(buf, MAX_STRING);
            reports[i]->setAssignedOfficer(buf); cout << "Officer assigned!\n";
            DataManager::getInstance().logAudit("ASSIGN_OFFICER", "Report ID: " + std::to_string(id) + " -> Officer: " + std::string(buf));
            return;
        }
    }
    cout << "Report ID not found!\n";
}
void CrimeManagementSystem::searchReportByID() const {
    int id; cout << "\n=== SEARCH REPORT ===\nEnter Report ID: "; cin >> id; cin.ignore();
    for (int i=0; i<reportCount; i++) if (reports[i]->getReportID() == id) { reports[i]->display(); return; }
    cout << "Report ID not found!\n";
}
void CrimeManagementSystem::viewAllUsers() const {
    cout << "\n=== ALL REGISTERED USERS ===\n";
    if (userCount == 0) { cout << "No users found.\n"; return; }
    for (int i=0; i<userCount; i++) users[i]->display();
}
void CrimeManagementSystem::deleteUser() {
    char username[MAX_STRING]; cout << "\n=== DELETE USER ===\nEnter username: "; cin.getline(username, MAX_STRING);
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i]->getUsername(), username) == 0) {
            delete users[i];
            for (int j = i; j < userCount - 1; ++j) {
                users[j] = users[j+1];
            }
            userCount--;
            cout << "User deleted!\n";
            DataManager::getInstance().logAudit("DELETE_USER", "Deleted user: " + std::string(username));
            return;
        }
    }
    cout << "User not found!\n";
}
void CrimeManagementSystem::viewAllAdmins() const {
    cout << "\n=== ALL ADMINS ===\n";
    for (int i=0; i<adminCount; i++) admins[i]->display();
}
void CrimeManagementSystem::addNewAdmin() {
    char username[MAX_STRING]; cout << "\n=== ADD NEW ADMIN ===\nEnter Username: "; cin.getline(username, MAX_STRING);
    for (int i=0; i<adminCount; i++) if (strcmp(admins[i]->getUsername(), username) == 0) { cout << "Username exists!\n"; return; }
    if (adminCount == adminCapacity) resizeAdmins();
    Admin* a = new Admin();
    a->setUsername(username);
    char buf[MAX_STRING];
    cout << "Password: "; cin.getline(buf, MAX_STRING); a->setPassword(buf);
    cout << "Full Name: "; cin.getline(buf, MAX_STRING); a->setFullName(buf);
    cout << "Badge Number: "; cin.getline(buf, MAX_STRING); a->setBadgeNumber(buf);
    cout << "Police Station: "; cin.getline(buf, MAX_STRING); a->setPoliceStation(buf);
    cout << "Security Question: "; cin.getline(buf, MAX_STRING); a->setSecurityQuestion(buf);
    cout << "Security Answer: "; cin.getline(buf, MAX_STRING); a->setSecurityAnswer(buf);
    admins[adminCount++] = a;
    cout << "New admin added!\n";
    DataManager::getInstance().logAudit("ADD_ADMIN", "Created admin: " + std::string(username));
    saveData();
}
void CrimeManagementSystem::addNewAdmin(const char* username, const char* pass, const char* name, const char* badge, const char* station, const char* sq, const char* sa) {
    for (int i=0; i<adminCount; i++) if (strcmp(admins[i]->getUsername(), username) == 0) return;
    if (adminCount == adminCapacity) resizeAdmins();
    Admin* a = new Admin();
    a->setUsername(username);
    a->setPassword(pass);
    a->setFullName(name);
    a->setBadgeNumber(badge);
    a->setPoliceStation(station);
    a->setSecurityQuestion(sq);
    a->setSecurityAnswer(sa);
    admins[adminCount++] = a;
    DataManager::getInstance().logAudit("ADD_ADMIN", "Created admin: " + std::string(username));
    saveData();
}
void CrimeManagementSystem::userMenu(char* username) {
    int choice;
    do {
        cout << "\n========================================\n";
        cout << "              USER MENU\n";
        cout << "========================================\n";
        cout << "1. File a Crime Report\n";
        cout << "2. View My Reports\n";
        cout << "3. Logout\n";
        cout << "Enter choice: "; cin >> choice; cin.ignore();
        switch (choice) {
        case 1: fileCrimeReport(username); break;
        case 2: viewMyReports(username);   break;
        case 3: cout << "Logging out...\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 3);
}
void CrimeManagementSystem::adminMenu() {
    int choice;
    do {
        cout << "\n========================================\n";
        cout << "             ADMIN MENU\n";
        cout << "========================================\n";
        cout << "1. View All Crime Reports\n";
        cout << "2. Update Report Status\n";
        cout << "3. Assign Officer to Report\n";
        cout << "4. Search Report by ID\n";
        cout << "5. View All Users\n";
        cout << "6. Delete User\n";
        cout << "7. View All Admins\n";
        cout << "8. Add New Admin\n";
        cout << "9. Logout\n";
        cout << "Enter choice: "; cin >> choice; cin.ignore();
        switch (choice) {
        case 1: viewAllReports();       break;
        case 2: updateReportStatus();   break;
        case 3: assignOfficerToReport(); break;
        case 4: searchReportByID();     break;
        case 5: viewAllUsers();         break;
        case 6: deleteUser();           break;
        case 7: viewAllAdmins();        break;
        case 8: addNewAdmin();          break;
        case 9: cout << "Logging out...\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 9);
}
void CrimeManagementSystem::run() {
    int choice; char loggedInUsername[MAX_STRING];
    do {
        cout << "\n========================================\n";
        cout << "      CRIME MANAGEMENT SYSTEM\n";
        cout << "========================================\n";
        cout << "1. User Registration\n2. User Login\n3. Admin Login\n4. Exit\n";
        cout << "Enter choice: "; cin >> choice; cin.ignore();
        switch (choice) {
        case 1: userRegistration(); break;
        case 2: if (userLogin(loggedInUsername)) userMenu(loggedInUsername); break;
        case 3: adminLogin(); break;
        case 4: cout << "Thank you for using Crime Management System!\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 4);
}

//  FEATURE SUB-MENU HELPERS
string getCurrentTime() {
    string dateTime;
    cout << "Enter date and time (YYYY-MM-DD HH:MM): "; cin >> dateTime;
    return dateTime;
}

void medicalMenu() {
    MedicalProfile profile; int choice;
    do {
        cout << "\n--- Medical Profile Management ---\n";
        cout << "1. Enter Medical Profile\n2. Display Medical Profile\n0. Back\nChoice: ";
        cin >> choice; cin.ignore();
        if (choice == 1) {
            string blood, allergy, mental, marks;
            cout << "Blood Group: "; getline(cin, blood);
            cout << "Allergies: "; getline(cin, allergy);
            cout << "Mental Health Status: "; getline(cin, mental);
            cout << "Physical Marks: "; getline(cin, marks);
            profile.setBloodGroup(blood).setAllergies(allergy).setMentalHealth(mental).setPhysicalMarks(marks);
            cout << "Medical profile saved!\n";
        }
        else if (choice == 2) { profile.displayMedicalProfile(); }
    } while (choice != 0);
}
void familyMenu() {
    FamilyRecord family; int choice;
    do {
        cout << "\n--- Kinship / Family Mapping ---\n";
        cout << "1. Add Family Member\n2. Display All Family Members\n0. Back\nChoice: ";
        cin >> choice; cin.ignore();
        if (choice == 1) {
            string name, relation, contact;
            cout << "Name: "; getline(cin, name);
            cout << "Relation: "; getline(cin, relation);
            cout << "Contact: "; getline(cin, contact);
            family.addFamilyMember(name, relation, contact);
            cout << "Family member added!\n";
        }
        else if (choice == 2) { family.displayFamily(); }
    } while (choice != 0);
}
void caseMenu() {
    CaseDatabase db; int choice;
    do {
        cout << "\n--- Case Management ---\n";
        cout << "1. Add Case\n2. Display All Cases\n3. Search by ID\n4. Assign Officer\n5. Update Status\n6. Most Wanted\n0. Back\nChoice: ";
        cin >> choice; cin.ignore();
        if (choice == 1) {
            char crimName[MAX_STRING], crime[MAX_STRING];
            cout << "Criminal Name: "; cin.getline(crimName, MAX_STRING);
            cout << "Crime Type: "; cin.getline(crime, MAX_STRING);
            db.addCase(crimName, crime); cout << "Case added!\n";
        }
        else if (choice == 2) { db.displayAll(); }
        else if (choice == 3) {
            int id; cout << "Enter Case ID: "; cin >> id; cin.ignore();
            Case* c = db.searchByID(id);
            if (c) c->display(); else cout << "Case not found.\n";
        }
        else if (choice == 5) {
            int id; cout << "Enter Case ID: "; cin >> id; cin.ignore();
            Case* c = db.searchByID(id);
            if (c) {
                char status[MAX_STRING];
                cout << "New Status (Open/Under Investigation/Closed): "; cin.getline(status, MAX_STRING);
                c->updateStatus(status); cout << "Status updated!\n";
            }
            else cout << "Case not found.\n";
        }
        else if (choice == 6) { db.displayMostWanted(); }
    } while (choice != 0);
}
void criminalRecordMenu(CriminalRecord records[], int& count) {
    int choice;
    do {
        cout << "\n--- Criminal Records ---\n";
        cout << "1. Add Criminal Record\n2. View All Records\n3. Search by ID\n0. Back\nChoice: ";
        cin >> choice; cin.ignore();
        if (choice == 1 && count < MAX_RECORDS) {
            char buf[MAX_STRING];
            cout << "Criminal ID: "; cin.getline(buf, MAX_STRING); records[count].setCriminalID(buf);
            cout << "Full Name: "; cin.getline(buf, MAX_STRING); records[count].setFullName(buf);
            cout << "CNIC: "; cin.getline(buf, MAX_STRING); records[count].setCNIC(buf);
            cout << "Crime Type: "; cin.getline(buf, MAX_STRING); records[count].setCrimeType(buf);
            cout << "FIR Number: "; cin.getline(buf, MAX_STRING); records[count].setFIRNumber(buf);
            cout << "Current Status (Active/Arrested/Convicted): "; cin.getline(buf, MAX_STRING); records[count].setCurrentStatus(buf);
            count++; cout << "Criminal record added!\n";
        }
        else if (choice == 2) {
            if (count == 0) { cout << "No criminal records.\n"; continue; }
            cout << left << setw(12) << "Criminal ID" << " | " << setw(22) << "Name" << " | " << setw(15) << "Crime" << " | Status\n";
            cout << string(65, '-') << "\n";
            for (int i = 0; i < count; i++) records[i].displayBrief();
        }
        else if (choice == 3) {
            char id[MAX_STRING]; cout << "Enter Criminal ID: "; cin.getline(id, MAX_STRING);
            bool found = false;
            for (int i = 0; i < count; i++) if (records[i] == id) { records[i].display(); found = true; break; }
            if (!found) cout << "Criminal ID not found.\n";
        }
    } while (choice != 0);
}
void complaintMenu(Complaint complaints[], int& count) {
    int choice;
    do {
        cout << "\n--- Complaint / FIR Management ---\n";
        cout << "1. File New Complaint\n2. View All Complaints\n3. View Full Complaint\n4. Update Status\n5. Assign Officer\n0. Back\nChoice: ";
        cin >> choice; cin.ignore();
        if (choice == 1 && count < MAX_COMPLAINTS) {
            char buf[MAX_STRING];
            long long track = 1000000000LL + count + 1;
            complaints[count].setTrackingNumber(track);
            cout << "Complainant Name: "; cin.getline(buf, MAX_STRING); complaints[count].setComplainantName(buf);
            cout << "CNIC: "; cin.getline(buf, MAX_STRING); complaints[count].setComplainantCNIC(buf);
            cout << "Phone: "; cin.getline(buf, MAX_STRING); complaints[count].setComplainantPhone(buf);
            cout << "Address: "; cin.getline(buf, MAX_STRING); complaints[count].setComplainantAddress(buf);
            cout << "Incident Type: "; cin.getline(buf, MAX_STRING); complaints[count].setIncidentType(buf);
            cout << "Incident Date: "; cin.getline(buf, MAX_STRING); complaints[count].setIncidentDate(buf);
            cout << "Incident Location: "; cin.getline(buf, MAX_STRING); complaints[count].setIncidentLocation(buf);
            cout << "Description: "; cin.getline(buf, MAX_STRING); complaints[count].setIncidentDescription(buf);
            cout << "Accused Name: "; cin.getline(buf, MAX_STRING); complaints[count].setAccusedName(buf);
            cout << "Witnesses: "; cin.getline(buf, MAX_STRING); complaints[count].setWitnesses(buf);
            cout << "Filing Date (YYYY-MM-DD): "; cin.getline(buf, MAX_STRING); complaints[count].setFilingDate(buf);
            count++; cout << "Complaint filed! Tracking #: " << track << "\n";
        }
        else if (choice == 2) {
            if (count == 0) { cout << "No complaints.\n"; continue; }
            cout << "| " << left << setw(12) << "Tracking#" << " | " << setw(20) << "Name" << " | " << setw(12) << "Type" << " | " << setw(14) << "Status" << " |\n";
            cout << string(65, '-') << "\n";
            for (int i = 0; i < count; i++) complaints[i].displayBrief();
        }
        else if (choice == 3) {
            long long id; cout << "Enter Tracking Number: "; cin >> id; cin.ignore();
            bool found = false;
            for (int i = 0; i < count; i++) if (complaints[i].getTrackingNumber() == id) { complaints[i].displayFull(); found = true; break; }
            if (!found) cout << "Complaint not found.\n";
        }
        else if (choice == 4) {
            long long id; cout << "Enter Tracking Number: "; cin >> id; cin.ignore();
            char buf[MAX_STRING];
            for (int i = 0; i < count; i++) if (complaints[i].getTrackingNumber() == id) {
                cout << "New Status (Pending/Under Investigation/Resolved): "; cin.getline(buf, MAX_STRING);
                complaints[i].setStatus(buf); cout << "Status updated!\n"; break;
            }
        }
        else if (choice == 5) {
            long long id; cout << "Enter Tracking Number: "; cin >> id; cin.ignore();
            char buf[MAX_STRING];
            for (int i = 0; i < count; i++) if (complaints[i].getTrackingNumber() == id) {
                cout << "Assign Officer: "; cin.getline(buf, MAX_STRING);
                complaints[i].setAssignedOfficer(buf); cout << "Officer assigned!\n"; break;
            }
        }
    } while (choice != 0);
}
void anonymousReportMenu(AnonymousReport reports[], int& count) {
    int choice;
    do {
        cout << "\n--- Anonymous Crime Reporting ---\n";
        cout << "1. Submit Tip\n2. View All\n3. Update Status\n0. Back\nChoice: ";
        cin >> choice; cin.ignore();
        if (choice == 1 && count < 10) { reports[count].setdata(); count++; cout << "Tip submitted!\n"; }
        else if (choice == 2) { for (int i = 0; i < count; i++) reports[i].display(); }
        else if (choice == 3) {
            int id; cout << "Enter Report ID: "; cin >> id; cin.ignore();
            for (int i = 0; i < count; i++) if (reports[i].getreportid() == id) reports[i].updatestatus();
        }
    } while (choice != 0);
}
void statsDashboardMenu(StatsDashboard& dash, int reportcount) {
    int open, inv, closed;
    cout << "\n--- Crime Statistics Dashboard ---\n";
    cout << "Enter Open Cases count: "; cin >> open;
    cout << "Enter Under Investigation count: "; cin >> inv;
    cout << "Enter Closed Cases count: "; cin >> closed;
    int newtotal = open + inv + closed;
    while (StatsDashboard::gettotal() < newtotal) StatsDashboard::addcase();
    dash.display(open, inv, closed, reportcount);
}
void filePersistenceMenu(MedicalProfileSimple profiles[], int& profilecount,
    AnonymousReport reports[], int& reportcount, FilePersistence& fp) {
    int choice;
    cout << "\n--- File Persistence ---\n1. Save All\n2. Load All\nChoice: "; cin >> choice; cin.ignore();
    if (choice == 1) { fp.savemedical(profiles, profilecount); fp.savereports(reports, reportcount); }
    else if (choice == 2) { fp.loadmedical(profiles, profilecount); fp.loadreports(reports, reportcount); }
}
void CriminalRecord::saveToFile(std::ofstream& file) const {
    auto writeStr = [&](const char* s) {
        size_t len = s ? strlen(s) : 0;
        file.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
        if (len > 0) file.write(s, len);
        };
    writeStr(criminalID);
    writeStr(fullName);
    writeStr(alias);
    writeStr(fatherName);
    writeStr(motherName);
    writeStr(dateOfBirth);
    writeStr(gender);
    writeStr(nationality);
    writeStr(cnic);
    writeStr(passportNumber);
    writeStr(height);
    writeStr(weight);
    writeStr(eyeColor);
    writeStr(hairColor);
    writeStr(bloodGroup);
    writeStr(distinguishingMarks);
    writeStr(permanentAddress);
    writeStr(currentAddress);
    writeStr(phoneNumber);
    writeStr(email);
    writeStr(criminalRecordNumber);
    writeStr(dateOfFirstArrest);
    writeStr(criminalHistorySummary);
    writeStr(crimeType);
    writeStr(sentence);
    file.write(reinterpret_cast<const char*>(&totalArrests), sizeof(int));
}

bool CriminalRecord::loadFromFile(std::ifstream& file) {
    auto readStr = [&](char*& dest) {
        size_t len = 0;
        file.read(reinterpret_cast<char*>(&len), sizeof(size_t));
        delete[] dest; dest = nullptr;
        if (len > 0) {
            dest = new char[len + 1];
            file.read(dest, len);
            dest[len] = '\0';
        }
        };
    if (!file) return false;
    readStr(criminalID);
    readStr(fullName);
    readStr(alias);
    readStr(fatherName);
    readStr(motherName);
    readStr(dateOfBirth);
    readStr(gender);
    readStr(nationality);
    readStr(cnic);
    readStr(passportNumber);
    readStr(height);
    readStr(weight);
    readStr(eyeColor);
    readStr(hairColor);
    readStr(bloodGroup);
    readStr(distinguishingMarks);
    readStr(permanentAddress);
    readStr(currentAddress);
    readStr(phoneNumber);
    readStr(email);
    readStr(criminalRecordNumber);
    readStr(dateOfFirstArrest);
    readStr(criminalHistorySummary);
    readStr(crimeType);
    readStr(sentence);
    file.read(reinterpret_cast<char*>(&totalArrests), sizeof(int));
    return file.good();
}
#pragma managed(pop)