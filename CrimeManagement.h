#ifndef CRIME_MANAGEMENT_SYSTEM_H
#define CRIME_MANAGEMENT_SYSTEM_H
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <cctype>
#include <ctime>
#include <vector>
#include <memory>

//  GLOBAL CONSTANTS
const int MAX_STRING = 200;
const int MAX_REPORTS = 100;
const int MAX_USERS = 50;
const int MAX_ADMINS = 10;
const int MAX_COMPLAINTS = 100;
const int MAX_RECORDS = 100;
//  FEATURE 7 — Evidence Cataloguing 
class Evidence {
private:
    int evidenceID;
    std::string description;
    std::string linkedSuspect;
    unsigned int secureHash;
public:
    Evidence() : evidenceID(0), secureHash(0) {}
    Evidence(int id, const std::string& suspect, const std::string& desc, unsigned int hash)
        : evidenceID(id), linkedSuspect(suspect), description(desc), secureHash(hash) {}
    int getEvidenceID() const { return evidenceID; }
    std::string getDescription() const { return description; }
    std::string getLinkedSuspect() const { return linkedSuspect; }
    unsigned int getSecureHash() const { return secureHash; }
};
//  FEATURE 13 — Medical Profile Management 
class MedicalProfile {
private:
    std::string bloodGroup;
    std::string allergies;
    std::string mentalHealthStatus;
    std::string physicalMarks;
public:
    MedicalProfile();
    MedicalProfile(std::string blood, std::string allergy, std::string mental, std::string marks);
    MedicalProfile& setBloodGroup(std::string bg);
    MedicalProfile& setAllergies(std::string allergy);
    MedicalProfile& setMentalHealth(std::string status);
    MedicalProfile& setPhysicalMarks(std::string marks);
    std::string getBloodGroup()    const;
    std::string getAllergies()      const;
    std::string getMentalHealth()  const;
    std::string getPhysicalMarks() const;
    void displayMedicalProfile() const;
};
//  FEATURE 14 — Kinship / Family Mapping  
class FamilyMember {
public:
    std::string name;
    std::string relation;
    std::string contact;
};

class FamilyRecord {
private:
    FamilyMember* members;
    int count;
    int capacity;
public:
    FamilyRecord();
    ~FamilyRecord();
    void addFamilyMember(std::string name, std::string relation, std::string contact);
    void displayFamily() const;
    int  getMemberCount() const;
};
//  PERSON
class Person {
protected:
    std::string name;
    int age;
public:
    Person();
    Person(const std::string& n, int a);
    virtual ~Person();
    std::string getName() const;
    int    getAge()  const;
    Person& setName(const std::string& n);
    Person& setAge(int a);
    virtual void display() const = 0;
};
//  SUSPECT
class Suspect : public Person {
private:
    std::string crimeCommitted;
public:
    Suspect();
    Suspect(const std::string& n, int a, const std::string& c);
    std::string getCrime() const;
    void setCrime(const std::string& c);
    void display() const override;
};
//  OFFICER
class Officer : public Person {
private:
    int    badgeNumber;
    std::string rank;
    bool isAvailable;
    static int badgeCounter;
public:
    Officer();
    Officer(const std::string& n, int a, const std::string& r);
    ~Officer();
    int    getBadgeNumber() const;
    std::string getRank()        const;
    bool   getIsAvailable()  const;
    Officer& setRank(const std::string& r);
    Officer& setAvailable(bool avail);
    void display() const override;
    void setBadgeNumber(int b);
    static void setCounter(int value);
};
//  CASE  
class Case {
private:
    int    caseID;
    std::string criminalName;
    std::string crimeType;
    std::string status;
    bool   isMostWanted;
    Officer* assignedOfficer;
    static int caseCounter;
public:
    Case(const std::string& crimName, const std::string& crime);
    ~Case();
    static int getTotalCases();
    int    getCaseID()       const;
    std::string getCriminalName() const;
    std::string getCrimeType()    const;
    std::string getStatus()       const;
    bool   getIsMostWanted() const;
    Officer* getOfficer()    const;
    Case& updateStatus(const std::string& newStatus);
    void assignOfficer(Officer* officer);
    void unassignOfficer();
    bool operator==(int id)           const;
    bool operator==(const std::string& name) const;
    void setCaseID(int id);
    static void setCounter(int value);
    void display() const;
};
//  CASE DATABASE 
class CaseDatabase {
private:
    Case** cases;
    int count;
    int capacity;
    void resize();
public:
    CaseDatabase();
    ~CaseDatabase();
    void addCase(const std::string& crimName, const std::string& crime);
    Case* getCase(int index);
    Case* searchByID(int id);
    Case* searchByName(const std::string& name);
    void displayMostWanted() const;
    void displayAll()        const;
    int  getCount()          const;
    void saveToFile(const std::string& recordFile) const;
    void loadFromFile(const std::string& recordFile, Officer** officers, int officerCount);
};

void saveOfficersToFile(const std::string& officerFile, Officer** officers, int officerCount);
int  loadOfficersFromFile(const std::string& officerFile, Officer** officers);

//  FEATURE 10 — Anonymous Crime Reporting 
class AnonymousReport {
private:
    static int counter;
    int    reportid;
    std::string description;
    std::string location;
    std::string status;
public:
    AnonymousReport();
    void setdata();
    void display();
    void updatestatus();
    int  getreportid();
    void savetofile(std::ofstream& file);
    void loadfromfile(std::ifstream& file);
};
//  FEATURE 11 — Crime Statistics Dashboard 
class StatsDashboard {
private:
    static int totalcases;
public:
    static void addcase();
    static int  gettotal();
    void display(int open, int underinvestigation, int closed, int tips);
};

//  FEATURE 12 — File Persistence  (
class MedicalProfileSimple {
private:
    std::string criminalid;
    std::string bloodgroup;
    std::string allergies;
    std::string mentalhealth;
    std::string physicalmarks;
public:
    void setdata();
    void display();
    std::string getcriminalid();
    void savetofile(std::ofstream& file);
    void loadfromfile(std::ifstream& file);
};

class FilePersistence {
public:
    void savemedical(MedicalProfileSimple profiles[], int count);
    void loadmedical(MedicalProfileSimple profiles[], int& count);
    void savereports(AnonymousReport reports[], int count);
    void loadreports(AnonymousReport reports[], int& count);
};
//  CRIMINAL RECORD
class CriminalRecord {
private:
    char* criminalID;      char* fullName;     char* alias;
    char* fatherName;      char* motherName;   char* dateOfBirth;
    char* gender;          char* nationality;  char* cnic;
    char* passportNumber;
    char* height;          char* weight;       char* eyeColor;
    char* hairColor;       char* bloodGroup;   char* distinguishingMarks;
    char* permanentAddress; char* currentAddress; char* phoneNumber;
    char* email;
    char* criminalRecordNumber; char* dateOfFirstArrest;
    char* criminalHistorySummary;
    int   totalArrests;    int totalConvictions;
    char* knownAssociates; char* gangAffiliation;
    char* firNumber;       char* policeStation;
    char* crimeType;       char* crimeDate;
    char* crimeLocation;   char* crimeDescription;
    char* arrestingOfficer; char* arrestingOfficerBadge;
    char* currentStatus;   char* courtName;    char* caseNumber;
    char* verdict;         char* sentence;     char* bailStatus;
    char* fingerprints;    char* dnaProfile;   char* photographPath;
    char* additionalNotes; char* createdBy;    char* creationDate;
    char* lastModifiedBy;  char* lastModifiedDate;

    void allocateAndCopy(char*& destination, const char* source);
    void deallocate(char*& ptr);
    bool isValidDate(const char* date);
    bool isValidPhone(const char* phone);
public:
    CriminalRecord();
    CriminalRecord(const CriminalRecord& other);
    ~CriminalRecord();

    // Cascaded Setters
    CriminalRecord& setCriminalID(const char* id);
    CriminalRecord& setFullName(const char* name);
    CriminalRecord& setAlias(const char* a);
    CriminalRecord& setFatherName(const char* name);
    CriminalRecord& setMotherName(const char* name);
    CriminalRecord& setDateOfBirth(const char* dob);
    CriminalRecord& setGender(const char* g);
    CriminalRecord& setNationality(const char* nation);
    CriminalRecord& setCNIC(const char* c);
    CriminalRecord& setPassportNumber(const char* passport);
    CriminalRecord& setHeight(const char* h);
    CriminalRecord& setWeight(const char* w);
    CriminalRecord& setEyeColor(const char* color);
    CriminalRecord& setHairColor(const char* color);
    CriminalRecord& setBloodGroup(const char* bg);
    CriminalRecord& setDistinguishingMarks(const char* marks);
    CriminalRecord& setPermanentAddress(const char* addr);
    CriminalRecord& setCurrentAddress(const char* addr);
    CriminalRecord& setPhoneNumber(const char* phone);
    CriminalRecord& setEmail(const char* email);
    CriminalRecord& setCriminalRecordNumber(const char* recordNum);
    CriminalRecord& setDateOfFirstArrest(const char* date);
    CriminalRecord& setCriminalHistorySummary(const char* summary);
    CriminalRecord& setTotalArrests(int arrests);
    CriminalRecord& setTotalConvictions(int convictions);
    CriminalRecord& setKnownAssociates(const char* associates);
    CriminalRecord& setGangAffiliation(const char* gang);
    CriminalRecord& setFIRNumber(const char* fir);
    CriminalRecord& setPoliceStation(const char* station);
    CriminalRecord& setCrimeType(const char* type);
    CriminalRecord& setCrimeDate(const char* date);
    CriminalRecord& setCrimeLocation(const char* location);
    CriminalRecord& setCrimeDescription(const char* description);
    CriminalRecord& setArrestingOfficer(const char* officer);
    CriminalRecord& setArrestingOfficerBadge(const char* badge);
    CriminalRecord& setCurrentStatus(const char* status);
    CriminalRecord& setCourtName(const char* court);
    CriminalRecord& setCaseNumber(const char* caseNum);
    CriminalRecord& setVerdict(const char* v);
    CriminalRecord& setSentence(const char* s);
    CriminalRecord& setBailStatus(const char* bail);
    CriminalRecord& setFingerprints(const char* fp);
    CriminalRecord& setDNAProfile(const char* dna);
    CriminalRecord& setPhotographPath(const char* path);
    CriminalRecord& setAdditionalNotes(const char* notes);
    CriminalRecord& setCreatedBy(const char* creator);
    CriminalRecord& setCreationDate(const char* date);
    CriminalRecord& setLastModifiedBy(const char* modifier);
    CriminalRecord& setLastModifiedDate(const char* date);

    // Getters
    const char* getCriminalID()             const;
    const char* getFullName()               const;
    const char* getAlias()                  const;
    const char* getDateOfBirth()            const;
    const char* getGender()                 const;
    const char* getCNIC()                   const;
    const char* getHeight()                 const;
    const char* getWeight()                 const;
    const char* getBloodGroup()             const;
    const char* getDistinguishingMarks()    const;
    const char* getPermanentAddress()       const;
    const char* getPhoneNumber()            const;
    const char* getEmail()                  const;
    const char* getCriminalRecordNumber()   const;
    int         getTotalArrests()           const;
    int         getTotalConvictions()       const;
    const char* getKnownAssociates()        const;
    const char* getGangAffiliation()        const;
    const char* getFIRNumber()              const;
    const char* getCrimeType()              const;
    const char* getCrimeDate()              const;
    const char* getCrimeLocation()          const;
    const char* getArrestingOfficer()       const;
    const char* getCurrentStatus()          const;
    const char* getVerdict()                const;
    const char* getSentence()               const;

    void display() const;
    void displayBrief() const;
    void saveToFile(std::ofstream& file) const;
    bool loadFromFile(std::ifstream& file);
    CriminalRecord& operator=(const CriminalRecord& other);
    bool operator==(const char* id) const;
};
//  COMPLAINT 
class Complaint {
private:
    long long trackingNumber;
    char* complainantName;
    char* complainantCNIC;
    char* complainantPhone;
    char* complainantAddress;
    char* incidentType;
    char* incidentDate;
    char* incidentLocation;
    char* incidentDescription;
    char* accusedName;
    char* witnesses;
    char* status;
    char* assignedOfficer;
    char* filingDate;

    void setDynamic(char*& dest, const char* src);
public:
    Complaint();
    Complaint(const Complaint& other);
    ~Complaint();

    Complaint& setTrackingNumber(long long num);
    Complaint& setComplainantName(const char* n);
    Complaint& setComplainantCNIC(const char* c);
    Complaint& setComplainantPhone(const char* p);
    Complaint& setComplainantAddress(const char* a);
    Complaint& setIncidentType(const char* t);
    Complaint& setIncidentDate(const char* d);
    Complaint& setIncidentLocation(const char* l);
    Complaint& setIncidentDescription(const char* d);
    Complaint& setAccusedName(const char* n);
    Complaint& setWitnesses(const char* w);
    Complaint& setStatus(const char* s);
    Complaint& setAssignedOfficer(const char* o);
    Complaint& setFilingDate(const char* d);

    long long   getTrackingNumber()    const;
    const char* getComplainantName()   const;
    const char* getComplainantCNIC()   const;
    const char* getStatus()            const;
    const char* getAssignedOfficer()   const;
    const char* getIncidentType()      const;
    const char* getFilingDate()        const;

    void displayBrief() const;
    void displayFull()  const;
    Complaint& operator=(const Complaint& other);
};

//  CRIME REPORT 
class CrimeReport {
private:
    int   reportID;
    char* crimeType;
    char* location;
    char* date;
    char* description;
    char* reporterName;
    char* status;
    char* assignedOfficer;
    char* lastUpdated;
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
    void setLastUpdated(const char* updated);

    int         getReportID()       const;
    const char* getCrimeType()      const;
    const char* getLocation()       const;
    const char* getDate()           const;
    const char* getDescription()    const;
    const char* getReporterName()   const;
    const char* getStatus()         const;
    const char* getAssignedOfficer()const;
    const char* getLastUpdated()    const;

    void display() const;
    CrimeReport& operator=(const CrimeReport& other);
};

//  USER  
class User {
private:
    char* username;
    char* password;
    char* fullName;
    char* phone;
    char* email;
    char* securityQuestion;
    char* securityAnswer;
public:
    User();
    User(const User& other);
    ~User();

    void setUsername(const char* uname);
    void setPassword(const char* pwd);
    void setFullName(const char* name);
    void setPhone(const char* ph);
    void setEmail(const char* mail);
    void setSecurityQuestion(const char* sq);
    void setSecurityAnswer(const char* sa);

    const char* getUsername() const;
    const char* getPassword() const;
    const char* getFullName() const;
    const char* getPhone()    const;
    const char* getEmail()    const;
    const char* getSecurityQuestion() const;
    const char* getSecurityAnswer() const;

    void display() const;
    bool authenticate(const char* uname, const char* pwd) const;
    User& operator=(const User& other);
};

//  ADMIN  =
class Admin {
private:
    char* username;
    char* password;
    char* fullName;
    char* badgeNumber;
    char* policeStation;
    char* securityQuestion;
    char* securityAnswer;
public:
    Admin();
    Admin(const Admin& other);
    ~Admin();

    void setUsername(const char* uname);
    void setPassword(const char* pwd);
    void setFullName(const char* name);
    void setBadgeNumber(const char* badge);
    void setPoliceStation(const char* station);
    void setSecurityQuestion(const char* sq);
    void setSecurityAnswer(const char* sa);

    const char* getUsername()      const;
    const char* getPassword()      const;
    const char* getFullName()      const;
    const char* getBadgeNumber()   const;
    const char* getPoliceStation() const;
    const char* getSecurityQuestion() const;
    const char* getSecurityAnswer() const;

    void display() const;
    bool authenticate(const char* uname, const char* pwd) const;
    Admin& operator=(const Admin& other);
};

//  SESSION MANAGER (SINGLETON)
class SessionManager {
private:
    static SessionManager* instance;
    std::string currentUser;
    bool isAdmin;
    SessionManager() : isAdmin(false) {}
public:
    static SessionManager& getInstance() {
        if (!instance) instance = new SessionManager();
        return *instance;
    }
    void login(const std::string& user, bool adminFlag) {
        currentUser = user;
        isAdmin = adminFlag;
    }
    void logout() {
        currentUser = "";
        isAdmin = false;
    }
    bool isAdminSession() const { return isAdmin; }
    const char* getCurrentUser() const { return currentUser.c_str(); }
};

//  DATA MANAGER (SINGLETON)
class DataManager {
private:
    static DataManager* instance;
    DataManager() {}
public:
    static DataManager& getInstance() {
        if (!instance) instance = new DataManager();
        return *instance;
    }
    void logAudit(const std::string& action, const std::string& details);
    void saveAdmins(Admin** admins, int count) const;
    void loadAdmins(Admin**& admins, int& count, int& capacity) const;
    void saveReports(CrimeReport** reports, int count) const;
    void loadReports(CrimeReport**& reports, int& count, int& capacity) const;
    void saveUsers(User** users, int count) const;
    void loadUsers(User**& users, int& count, int& capacity) const;
};

// This stores all connections. Example: network["Afwan"] = {"Ali", "Viktor"}
#include <unordered_map>
#include <vector>
extern std::unordered_map<std::string, std::vector<std::string>> syndicateNetwork;


class ArchivedSuspect {
public:
    std::string name;
    std::string originalCrime;
    std::string reasonForRemoval;
    std::string dateRemoved;
};

class ArchivedCase {
public:
    std::string caseID;
    std::string criminalName;
    std::string crimeType;
    std::string closingReason;
    std::string closingDate;
};

class ArchivedOfficer {
public:
    std::string name;
    std::string finalRank;
    std::string departureReason;
    std::string departureDate;
};

class LoginSession {
public:
    std::string username;
    std::string role;
    std::string loginTime;
    std::string logoutTime;
};

//  CRIME MANAGEMENT SYSTEM  
class CrimeManagementSystem {
private:
    CrimeReport** reports; // Array of pointers to CrimeReport
    User** users;         // Array of pointers to User
    Admin** admins;       // Array of pointers to Admin
    
    int reportCount, reportCapacity;
    int userCount, userCapacity;
    int adminCount, adminCapacity;
    int nextReportID;

    Suspect** suspects;
    int suspectCount;
    int suspectCapacity;
    void resizeSuspects();

    Officer** officers;
    int officerCount;
    int officerCapacity;
    void resizeOfficers();

    Evidence** evidenceLog;
    int evidenceCount;
    int evidenceCapacity;
    void resizeEvidenceLog();

    ArchivedSuspect** archivedSuspects;
    int archivedCount;
    int archivedCapacity;
    void resizeArchivedSuspects();

    ArchivedCase** archivedCases;
    int archivedCaseCount;
    int archivedCaseCapacity;
    void resizeArchivedCases();

    ArchivedOfficer** archivedOfficers;
    int archivedOfficerCount;
    int archivedOfficerCapacity;
    void resizeArchivedOfficers();

    LoginSession** sessionLogs;
    int sessionCount;
    int sessionCapacity;



    void resizeSessions();

    Case** cases; // To avoid compilation errors, need pointer to Case if we archive cases
    int caseCount;
    int caseCapacity;

    // Helper to resize arrays manually
    void resizeUsers();
    void resizeAdmins();
    void resizeReports();

    void loadData();
    void saveData() const;
    bool isUsernameTaken(const char* username) const;

public:
    CrimeManagementSystem();
    ~CrimeManagementSystem();
    
    void archiveSuspectDirectly(const char* name, const char* crime, const char* reason, const char* date);
    void archiveCaseDirectly(const char* caseID, const char* criminalName, const char* crimeType, const char* reason, const char* date);
    void recordSession(const char* username, const char* role, const char* action);

    void addNewSuspect(const char* name, int age, const char* crime);
    void addNewOfficer(const char* name, int age, const char* rank);

    void logSecureEvidence(const char* suspectName, const char* description, unsigned int generatedHash);
    void archiveAndDeleteSuspect(int suspectIndex, const char* reason);
    void archiveCase(int caseIndex, const char* reason, const char* date);
    void archiveOfficer(int officerIndex, const char* reason, const char* date);
    void recordLogin(const char* username, const char* role, const char* timeIn);
    void recordLogout(const char* username, const char* timeOut);

    int getEvidenceCount() const { return evidenceCount; }
    Evidence** getEvidenceLog() const { return evidenceLog; }
    int getSessionCount() const { return sessionCount; }
    LoginSession** getSessionLogs() const { return sessionLogs; }
    int getArchivedCaseCount() const { return archivedCaseCount; }
    ArchivedCase** getArchivedCases() const { return archivedCases; }
    int getArchivedOfficerCount() const { return archivedOfficerCount; }
    ArchivedOfficer** getArchivedOfficers() const { return archivedOfficers; }
    int getArchivedSuspectCount() const { return archivedCount; }
    ArchivedSuspect** getArchivedSuspects() const { return archivedSuspects; }
    int getOfficerCount() const { return officerCount; }
    Officer** getOfficers() const { return officers; }
    int getReportCount() const { return reportCount; }
    CrimeReport** getReports() const { return reports; }

    void userRegistration();
    void userRegistration(const char* username, const char* pass, const char* name, const char* phone, const char* email, const char* sq, const char* sa);
    bool userLogin(char* loggedInUsername);
    bool userLogin(const char* username, const char* password);
    void adminLogin();
    bool adminLogin(const char* username, const char* password);

    void fileCrimeReport(const char* username);
    void viewMyReports(const char* username) const;
    void viewAllReports()       const;
    void updateReportStatus();
    void assignOfficerToReport();
    void searchReportByID()     const;
    void viewAllUsers()         const;
    void deleteUser();
    void viewAllAdmins()        const;
    void addNewAdmin();
    void addNewAdmin(const char* username, const char* pass, const char* name, const char* badge, const char* station, const char* sq, const char* sa);

    void userMenu(char* username);
    void adminMenu();
    void run();


};

//  MENU HELPERS 
std::string getCurrentTime();

void medicalMenu();
void familyMenu();
void caseMenu();
void criminalRecordMenu(CriminalRecord records[], int& count);
void complaintMenu(Complaint complaints[], int& count);
void anonymousReportMenu(AnonymousReport reports[], int& count);
void statsDashboardMenu(StatsDashboard& dash, int reportcount);
void filePersistenceMenu(MedicalProfileSimple profiles[], int& profilecount,
    AnonymousReport reports[], int& reportcount,
    FilePersistence& fp);

#endif // CRIME_MANAGEMENT_SYSTEM_H