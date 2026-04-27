#ifndef crimeManagement_h
#define crimeManagement_h
#include<iostream>
#include <fstream>
#include <string>
using namespace std;

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
//  FEATURE 7 Evidence Cataloguing
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
    MedicalProfile(string blood, string allergy,string mental, string marks);
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

#endif // !crimeManagement.h
