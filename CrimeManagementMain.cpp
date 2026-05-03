#pragma managed(push, off)   // ← ADD THIS as first line
#include "CrimeManagement.h"
using namespace std;

void printMainMenu() {
    cout << "\n";
    cout << "+=================================================================+\n";
    cout << "|         CRIME RECORD MANAGEMENT SYSTEM  -  MAIN MENU           |\n";
    cout << "+=================================================================+\n";
    cout << "|  1.  Crime Management System  (Login / File Crime Reports)      |\n";
    cout << "|  2.  Case Management                                            |\n";
    cout << "|  3.  Criminal Records                                           |\n";
    cout << "|  4.  Complaint / FIR Management                                 |\n";
    cout << "|-----------------------------------------------------------------|\n";
    cout << "|  10. Anonymous Crime Reporting      (Feature 10)                |\n";
    cout << "|  11. Crime Statistics Dashboard     (Feature 11)                |\n";
    cout << "|  12. File Persistence               (Feature 12)                |\n";
    cout << "|  13. Medical Profile Management     (Feature 13)                |\n";
    cout << "|  14. Kinship / Family Mapping       (Feature 14)                |\n";
    cout << "|-----------------------------------------------------------------|\n";
    cout << "|  0.  Exit                                                       |\n";
    cout << "+=================================================================+\n";
    cout << "  Choice: ";
}

int runConsoleApp() {

    //  Subsystem 1 – Crime Management System (user/admin login + crime reports)
    CrimeManagementSystem crimeSystem;          // loads data from files in constructor

    //  Subsystem 3 – Criminal Records  (passed to criminalRecordMenu)
    CriminalRecord criminalRecords[MAX_RECORDS];
    int            criminalRecordCount = 0;

    //  Subsystem 4 – Complaints / FIR  (passed to complaintMenu)
    Complaint complaints[MAX_COMPLAINTS];
    int       complaintCount = 0;

    //  Features 10-12 – Anonymous reports, stats, file persistence
    AnonymousReport      anonReports[10];
    int                  anonCount = 0;

    MedicalProfileSimple medProfiles[10];
    int                  medCount = 0;

    StatsDashboard       dash;
    FilePersistence      fp;

    // Auto-load saved data on startup
    fp.loadmedical(medProfiles, medCount);
    fp.loadreports(anonReports, anonCount);

    //  Main loop
    int choice;
    do {
        printMainMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {

            // 1 – Crime Management System (user registration, login, crime reports)
        case 1:
            crimeSystem.run();
            break;

            // 2 – Case Management
        case 2:
            caseMenu();
            break;

            // 3 – Criminal Records
        case 3:
            criminalRecordMenu(criminalRecords, criminalRecordCount);
            break;

            // 4 – Complaint / FIR Management
        case 4:
            complaintMenu(complaints, complaintCount);
            break;

            // 10 – Anonymous Crime Reporting
        case 10:
            anonymousReportMenu(anonReports, anonCount);
            break;

            // 11 – Crime Statistics Dashboard
        case 11:
            statsDashboardMenu(dash, anonCount);
            break;

            // 12 – File Persistence (save / load medical profiles + anon reports
        case 12:
            filePersistenceMenu(medProfiles, medCount, anonReports, anonCount, fp);
            break;

            // 13 – Medical Profile Management
        case 13:
            medicalMenu();
            break;

            // 14 – Kinship / Family Mapping
        case 14:
            familyMenu();
            break;

            // 0 – Exit: auto-save before quitting
        case 0:
            fp.savemedical(medProfiles, medCount);
            fp.savereports(anonReports, anonCount);
            cout << "\nData saved. Exiting system. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice. Please select a valid option from the menu.\n";
            break;
        }

    } while (choice != 0);
    // Release heap memory
    delete[] criminalRecords;
    delete[] complaints;
    delete[] anonReports;
    delete[] medProfiles;
    return 0;
}
#pragma managed(pop)