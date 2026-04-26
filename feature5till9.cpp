#include <iostream>
#include<fstream>
#include <string>
using namespace std;
//person class
class Person {
protected:
    string name;
    int    age;

public:
    Person() : name("Unknown"), age(0) {}
    Person(const string& n, int a) : name(n), age(a) {}
    virtual ~Person() {}
    string getName() const { return name; }
    int    getAge()  const { return age; }
    Person& setName(const string& n) {
        name = n;
        return *this;
    }
    Person& setAge(int a) {//change to setter
        age = a;
        return *this;
    }

    virtual void display() const = 0;
};


//class officer derived from person
class Officer : public Person {
private:
    int    badgeNumber;
    string rank;
    static int badgeCounter;

public:
    Officer() : Person(), rank("Constable") {
        badgeNumber = ++badgeCounter;
    }
    Officer(const string& n, int a, const string& r) : Person(n, a), rank(r) {
        badgeNumber = ++badgeCounter;
    }
    ~Officer() {}
    int    getBadgeNumber() const { return badgeNumber; }
    string getRank()        const { return rank; }
    Officer& setRank(const string& r) { rank = r; return *this; }
    void display() const override {
        cout << "  [Officer] Badge #" << badgeNumber << " | " << rank << " " << name << " | Age: " << age << endl;
    }
    void setBadgeNumber(int b) {
        badgeNumber = b;
    }
    static void setCounter(int value) {
        badgeCounter = value;
    }

};

int Officer::badgeCounter = 1000;

//  CLASS: Case
//  - Tracks status, evidence log, assigned officer
//  - Operator overloading: ==  and  +
//  - Static FIR counter
class Case {
private:
    int         caseID;
    string      criminalName;
    string      crimeType;
    string      status;          // "Open" / "Under Investigation" / "Closed"
    bool        isMostWanted;

    Officer* assignedOfficer; // Pointer to officer (dynamic linkage)

    static int caseCounter;      // Static member for unique IDs

public:
    Case(const string& crimName, const string& crime) : criminalName(crimName), crimeType(crime), status("Open"), isMostWanted(false), assignedOfficer(nullptr)
    {
        caseID = ++caseCounter;
    }

    //  Destructor  We do NOT delete assignedOfficer here because the Officer object is owned externally. We just sever the link.

    ~Case() {
        assignedOfficer = nullptr;
    }

    // Static function to get total cases 
    static int getTotalCases() { return caseCounter; }
    int         getCaseID()       const { return caseID; }
    string      getCriminalName() const { return criminalName; }
    string      getCrimeType()    const { return crimeType; }
    string      getStatus()       const { return status; }
    bool        getIsMostWanted() const { return isMostWanted; }
    Officer* getOfficer()      const { return assignedOfficer; }

    Case& setCriminalName(const string& n) { criminalName = n; return *this; }
    Case& setCrimeType(const string& c) { crimeType = c; return *this; }
    Case& setMostWanted(bool val) { isMostWanted = val; return *this; }

    // ── Feature 1: Investigation Status Tracker ──
    // Update status with validation
    Case& updateStatus(const string& newStatus) {
        // Allowed transitions 
        if (newStatus == "Open" || newStatus == "Under Investigation" || newStatus == "Closed")
        {
            string old = status;
            status = newStatus;
            cout << "  >> Case #" << caseID << " [" << criminalName << "]"
                << "  Status: [" << old << "] --> [" << status << "]\n";
        }
        else {
            cout << "  !! Invalid status: " << newStatus << endl;
        }
        return *this;
    }

    // ── Feature 2: Officer Assignment via Pointer ──
    void assignOfficer(Officer* officer) {
        assignedOfficer = officer;           // pointer assignment
        if (assignedOfficer != nullptr) {
            cout << "  >> Case #" << caseID << " assigned to: " << officer->getRank() << " " << officer->getName() << " (Badge #" << officer->getBadgeNumber() << ")\n";
        }
    }

    void unassignOfficer() {
        if (assignedOfficer != nullptr) {
            cout << "  >> Officer " << assignedOfficer->getName() << " removed from Case #" << caseID << "\n";
            assignedOfficer = nullptr;
        }
    }

    // ── Operator Overloading ==  (Feature 3: Search) ──
    // Compare by Case ID
    bool operator==(int id) const {
        return caseID == id;
    }

    // Compare by criminal name (case-sensitive)
    bool operator==(const string& name) const {
        return criminalName == name;
    }
    void setCaseID(int id) {
        caseID = id;
    }
    static void setCounter(int value) {
        caseCounter = value;
    }

    // ── Display a single case record ──────────
    void display() const {
        cout << "\n";
        cout << " Case ID     : " << caseID << "\n";
        cout << " Criminal    : " << criminalName << "\n";
        cout << " Crime Type  : " << crimeType << "\n";
        cout << " Status      : " << status << "\n";
        cout << "Most Wanted : ";

        if (isMostWanted) {
            cout << "YES ";
        }
        else {
            cout << "No";
        }

        cout << "\n";
        if (assignedOfficer != nullptr) {
            cout << " Lead Officer: " << assignedOfficer->getRank()
                << " " << assignedOfficer->getName()
                << " (Badge #" << assignedOfficer->getBadgeNumber() << ")\n";
        }
        else {
            cout << " Lead Officer: Unassigned\n";
        }
    }
};

int Case::caseCounter = 0; // Static member initialisation


// ─────────────────────────────────────────────
//  CLASS: CaseDatabase
//  - Manages a dynamic array of Case pointers
//  - Provides search, most-wanted listing
// ─────────────────────────────────────────────
class CaseDatabase {
private:
    Case** cases;     // dynamic array of Case pointers
    int    count;
    int    capacity;

    // Internal resize helper
    void resize() {
        int newCap;

        if (capacity == 0) {
            newCap = 4;
        }
        else {
            newCap = capacity * 2;
        }
        Case** temp = new Case * [newCap];
        for (int i = 0; i < count && i < newCap; i++) {
            temp[i] = cases[i];
        }
        delete[] cases;
        cases = temp;
        capacity = newCap;
    }

public:
    CaseDatabase() : cases(nullptr), count(0), capacity(0) {}

    // Destructor — frees all dynamically allocated Case objects
    ~CaseDatabase() {
        for (int i = 0; i < count; i++) delete cases[i];
        delete[] cases;
    }

    // Add a new case (heap-allocated)
    void addCase(const string& crimName, const string& crime) {
        if (count == capacity) resize();
        cases[count++] = new Case(crimName, crime);
        cout << "  >> New case registered. Total cases: "
            << Case::getTotalCases() << "\n";
    }

    // Get pointer to case by index (for chaining operations)
    Case* getCase(int index) {
        if (index >= 0 && index < count) return cases[index];
        return nullptr;
    }

    // ── Feature 3: Search Engine ─────────────
    // Search by ID using overloaded ==
    Case* searchByID(int id) {
        for (int i = 0; i < count; i++) {
            if (cases[i] != nullptr && *cases[i] == id) return cases[i]; // uses operator==(int)
        }
        return nullptr;
    }

    // Search by criminal name using overloaded ==
    Case* searchByName(const string& name) {
        for (int i = 0; i < count; i++) {
            if (cases[i] != nullptr && *cases[i] == name) {
                return cases[i];
            }
        }
        return nullptr;
    }
    // ── Feature 4: Most Wanted Gallery ───────
    void displayMostWanted() const {
        cout << "\nMOST WANTED GALLERY\n";

        bool found = false;

        for (int i = 0; i < count; i++) {
            if (cases[i] != nullptr &&
                cases[i]->getIsMostWanted() &&
                cases[i]->getStatus() != "Closed")
            {
                cases[i]->display();
                found = true;
            }
        }

        if (!found)
            cout << "No most-wanted records.\n";
    }
    //change by add of display
    void displayAll() const {
        cout << "\nALL CASE RECORDS\n";

        for (int i = 0; i < count; i++) {
            if (cases[i] != nullptr) {
                cases[i]->display();
            }
        }
    }

    int getCount() const { return count; }
    // record.txt(cases+assingn to officer in same file)  :caseID|criminalName|crimeType|status|isMostWanted|badgeNumber
    void saveToFile(const string& recordFile) const {
        ofstream file(recordFile);

        for (int i = 0; i < count; i++) {

            int badge = 0;

            if (cases[i]->getOfficer() != nullptr)
                badge = cases[i]->getOfficer()->getBadgeNumber();

            file << cases[i]->getCaseID() << "|"
                << cases[i]->getCriminalName() << "|"
                << cases[i]->getCrimeType() << "|"
                << cases[i]->getStatus() << "|"
                << cases[i]->getIsMostWanted() << "|"
                << badge << "\n";
        }

        file.close();
    }
    void loadFromFile(const string& recordFile, Officer** officers, int officerCount) {
        ifstream file(recordFile);

        if (!file) return;
        if (cases != nullptr) {
            for (int i = 0; i < count; i++)
                if (cases[i] != nullptr)
                    delete cases[i];

            delete[] cases;
        }

        cases = nullptr;
        count = 0;
        capacity = 0;

        string line;

        while (getline(file, line)) {

            if (line.empty()) continue;

            string id = "", name = "", crime = "", status = "", wanted = "", badge = "";
            int i = 0;

            while (i < line.size() && line[i] != '|')
                id += line[i++];
            i++;
            if (i >= line.size()) continue;

            while (i < line.size() && line[i] != '|')
                name += line[i++];
            i++;
            if (i >= line.size()) continue;

            while (i < line.size() && line[i] != '|')
                crime += line[i++];
            i++;
            if (i >= line.size()) continue;

            while (i < line.size() && line[i] != '|')
                status += line[i++];
            i++;
            if (i >= line.size()) continue;

            while (i < line.size() && line[i] != '|')
                wanted += line[i++];
            i++;
            if (i >= line.size()) continue;

            while (i < line.size())
                badge += line[i++];

            int cid = 0, bid = 0;


            for (int i = 0; i < id.length(); i++) {
                char c = id[i];

                if (c >= '0' && c <= '9') {
                    cid = cid * 10 + (c - '0');
                }
            }


            for (int i = 0; i < badge.length(); i++) {
                char c = badge[i];

                if (c >= '0' && c <= '9') {
                    bid = bid * 10 + (c - '0');
                }
            }

            if (count == capacity) resize();

            Case* c = new Case(name, crime);

            c->setCaseID(cid);
            c->updateStatus(status);
            c->setMostWanted(wanted == "1");

            // assign officer safely
            if (bid != 0) {
                for (int j = 0; j < officerCount; j++) {
                    if (officers[j] != nullptr &&
                        officers[j]->getBadgeNumber() == bid) {
                        c->assignOfficer(officers[j]);
                        break;
                    }
                }
            }

            cases[count++] = c;
        }

        file.close();

        Case::setCounter(count);
    }
};
// officer.txt:  badgeNumber|name|age|rank

void saveOfficersToFile(const string& officerFile, Officer** officers, int officerCount) {
    ofstream file(officerFile);

    for (int i = 0; i < officerCount; i++) {
        file << officers[i]->getBadgeNumber() << "|"
            << officers[i]->getName() << "|"
            << officers[i]->getAge() << "|"
            << officers[i]->getRank() << "\n";
    }

    file.close();
}
int loadOfficersFromFile(const string& officerFile, Officer** officers) {
    ifstream file(officerFile);
    string line;
    int count = 0;
    int maxBadge = 0;

    if (!file) return 0;

    while (getline(file, line)) {

        if (line.empty()) continue;

        string badge = "", name = "", age = "", rank = "";
        int i = 0;

        while (i < line.size() && line[i] != '|')
            badge += line[i++];
        i++;
        if (i >= line.size()) continue;

        while (i < line.size() && line[i] != '|')
            name += line[i++];
        i++;
        if (i >= line.size()) continue;

        while (i < line.size() && line[i] != '|')
            age += line[i++];
        i++;
        if (i >= line.size()) continue;

        while (i < line.size())
            rank += line[i++];

        int b = 0, a = 0;


        for (int i = 0; i < badge.length(); i++) {
            char c = badge[i];

            if (c >= '0' && c <= '9') {
                b = b * 10 + (c - '0');
            }
        }


        for (int i = 0; i < age.length(); i++) {
            char c = age[i];

            if (c >= '0' && c <= '9') {
                a = a * 10 + (c - '0');
            }
        }
        officers[count] = new Officer(name, a, rank);
        officers[count]->setBadgeNumber(b);

        if (b > maxBadge) maxBadge = b;

        count++;
    }

    file.close();

    Officer::setCounter(maxBadge);

    return count;
}

int main() {
    return 0;
}