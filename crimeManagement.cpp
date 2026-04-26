#include "crimeManagement.h"




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