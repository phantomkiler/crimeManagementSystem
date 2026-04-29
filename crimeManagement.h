#ifndef crimeManagement_h
#define crimeManagement_h
#include<iostream>
#include <fstream>
#include <string>
using namespace std;
//  FEATURE 7 Evidence Cataloguing
#ifndef CRIME_SYSTEM_H
#define CRIME_SYSTEM_H

const int MAX_STRING = 100;
const int MAX_REPORTS = 100;
const int MAX_USERS = 50;
const int MAX_ADMINS = 10;

class Admin {
private:
    char* username;
    char* password;
    char* fullName;
    char* badgeNumber;
    char* policeStation;
public:
    Admin();
    Admin(const Admin& other);
    ~Admin();
    void setUsername(const char* uname);
    void setPassword(const char* pwd);
    void setFullName(const char* name);
    void setBadgeNumber(const char* badge);
    void setPoliceStation(const char* station);
    const char* getUsername() const;
    const char* getPassword() const;
    const char* getFullName() const;
    const char* getBadgeNumber() const;
    const char* getPoliceStation() const;
    void display() const;
    bool authenticate(const char* uname, const char* pwd) const;
    Admin& operator=(const Admin& other);
};

class User {
private:
    char* username;
    char* password;
    char* fullName;
    char* phone;
    char* email;
public:
    User();
    User(const User& other);
    ~User();
    void setUsername(const char* uname);
    void setPassword(const char* pwd);
    void setFullName(const char* name);
    void setPhone(const char* ph);
    void setEmail(const char* mail);
    const char* getUsername() const;
    const char* getPassword() const;
    const char* getFullName() const;
    const char* getPhone() const;
    const char* getEmail() const;
    void display() const;
    bool authenticate(const char* uname, const char* pwd) const;
    User& operator=(const User& other);
};

class CrimeReport {
private:
    int reportID;
    char* crimeType;
    char* location;
    char* date;
    char* description;
    char* reporterName;
    char* status;
    char* assignedOfficer;
public:
    CrimeReport();
    CrimeReport(const CrimeReport& other);
    ~CrimeReport();
    void setReportID(int id);
    void setCrimeType(const char* type);
    void setLocation(const char* loc);
    void setDate(const char* d);
    void setDescription(const char* desc);
    void setReporterName(const char* name);
    void setStatus(const char* s);
    void setAssignedOfficer(const char* officer);
    int getReportID() const;
    const char* getCrimeType() const;
    const char* getLocation() const;
    const char* getDate() const;
    const char* getDescription() const;
    const char* getReporterName() const;
    const char* getStatus() const;
    const char* getAssignedOfficer() const;
    void display() const;
    CrimeReport& operator=(const CrimeReport& other);
};

class CrimeManagementSystem {
private:
    CrimeReport** reports;
    User** users;
    Admin** admins;
    int reportCount;
    int userCount;
    int adminCount;
    int nextReportID;
    void loadData();
    void saveData() const;
    bool isUsernameTaken(const char* username) const;
    void getCurrentDate(char* date) const;
public:
    CrimeManagementSystem();
    ~CrimeManagementSystem();
    void userRegistration();
    bool userLogin(char* loggedInUsername);
    void adminLogin();
    void fileCrimeReport(const char* username);
    void viewMyReports(const char* username) const;
    void viewAllReports() const;
    void updateReportStatus();
    void assignOfficerToReport();
    void searchReportByID() const;
    void viewAllUsers() const;
    void deleteUser();
    void viewAllAdmins() const;
    void addNewAdmin();
    void userMenu(char* username);
    void adminMenu();
    void run();
};

#endif

class Evidence {
private:
    int    evidenceID;
    string description;
    string timestamp;
    string linkedFIR_ID;    // which FIR this belongs to

    static int evidenceCounter;

public:
    Evidence();
    Evidence(string desc, string time, string firID);
    Evidence& setDescription(string desc);//cascaded function 
    Evidence& setTimestamp(string time);
    Evidence& setLinkedFIR(string firID);
    int    getEvidenceID()   const; // Getters
    string getDescription()  const;
    string getTimestamp()    const;
    string getLinkedFIR()    const;
    Evidence operator+(const Evidence& other) const; //merge two evidence logs

    void displayEvidence() const;
};
//  FEATURE 13 Medical Profile Management
class MedicalProfile {
private:
    string bloodGroup;
    string allergies;
    string mentalHealthStatus;   //"Stable", "Unstable"
    string physicalMarks;        // scars, tattoos, etc
public:
    MedicalProfile();
    MedicalProfile(string blood, string allergy, string mental, string marks);
    MedicalProfile& setBloodGroup(string bg);// Cascaded setters
    MedicalProfile& setAllergies(string allergy);
    MedicalProfile& setMentalHealth(string status);
    MedicalProfile& setPhysicalMarks(string marks);
    string getBloodGroup()       const;// Getters
    string getAllergies()         const;
    string getMentalHealth()     const;
    string getPhysicalMarks()    const;

    void displayMedicalProfile() const;
};
//  FEATURE 14  Kinship,Family Mapping
class FamilyMember {
public:
    string name;
    string relation;   // Father, Mother, Spouse, Sibling
    string contact;
};

class FamilyRecord {
private:
    FamilyMember* members;   // dynamic array (pointer usage)
    int count;
    int capacity;

public:
    FamilyRecord();
    ~FamilyRecord();   // destructor frees dynamic memory

    void addFamilyMember(string name, string relation, string contact);
    void displayFamily() const;
    int  getMemberCount() const;
};

// ───────── PERSON ─────────
class Person {
protected:
    string name;
    int age;

public:
    Person();
    Person(const string& n, int a);
    virtual ~Person();

    string getName() const;
    int getAge() const;

    Person& setName(const string& n);
    Person& setAge(int a);

    virtual void display() const = 0;
};

// ───────── OFFICER ─────────
class Officer : public Person {
private:
    int badgeNumber;
    string rank;
    static int badgeCounter;

public:
    Officer();
    Officer(const string& n, int a, const string& r);
    ~Officer();

    int getBadgeNumber() const;
    string getRank() const;

    Officer& setRank(const string& r);

    void display() const override;

    void setBadgeNumber(int b);
    static void setCounter(int value);
};

// ───────── CASE ─────────
class Case {
private:
    int caseID;
    string criminalName;
    string crimeType;
    string status;
    bool isMostWanted;
    Officer* assignedOfficer;

    static int caseCounter;

public:
    Case(const string& crimName, const string& crime);
    ~Case();

    static int getTotalCases();

    int getCaseID() const;
    string getCriminalName() const;
    string getCrimeType() const;
    string getStatus() const;
    bool getIsMostWanted() const;
    Officer* getOfficer() const;

    Case& setCriminalName(const string& n);
    Case& setCrimeType(const string& c);
    Case& setMostWanted(bool val);

    Case& updateStatus(const string& newStatus);

    void assignOfficer(Officer* officer);
    void unassignOfficer();

    bool operator==(int id) const;
    bool operator==(const string& name) const;

    void setCaseID(int id);
    static void setCounter(int value);

    void display() const;
};

// ───────── DATABASE ─────────
class CaseDatabase {
private:
    Case** cases;
    int count;
    int capacity;

    void resize();

public:
    CaseDatabase();
    ~CaseDatabase();

    void addCase(const string& crimName, const string& crime);

    Case* getCase(int index);

    Case* searchByID(int id);
    Case* searchByName(const string& name);

    void displayMostWanted() const;
    void displayAll() const;

    int getCount() const;

    void saveToFile(const string& recordFile) const;
    void loadFromFile(const string& recordFile, Officer** officers, int officerCount);
};

// ───────── FILE HELPERS ─────────
void saveOfficersToFile(const string& officerFile, Officer** officers, int officerCount);
int loadOfficersFromFile(const string& officerFile, Officer** officers);

class Criminal {};
class FIR {};
class MedicalHistory {};

#endif // !crimeManagement.h
