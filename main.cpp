#include "crimeManagement_h"

string getCurrentTime() {
    string dateTime;
    cout << "Enter date and time ";
    cin >> dateTime;
    return dateTime;
}
// Feature 7 Menu
void evidenceMenu() {
    int choice;
    Evidence e1, e2;
    do {
        cout << "\n Evidence Cataloguing " << endl;
        cout << "1 Add Evidence" << endl;
        cout << "2 Display Evidence" << endl;
        cout << "3 Merge Two Evidence Logs" << endl;
        cout << "0 Back" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string desc, firID;
            cout << "Enter FIR ID: ";
            getline(cin, firID);
            cout << "Enter Description: "; 
            getline(cin, desc);
            e1.setLinkedFIR(firID).setDescription(desc).setTimestamp(getCurrentTime());
            cout << "Evidence added!" << endl;

        }
        else if (choice == 2) {
            e1.displayEvidence();

        }
        else if (choice == 3) {
            string desc2, firID2;
            cout << "Enter 2nd Evidence FIR ID: ";
            getline(cin, firID2);
            cout << "Enter 2nd Evidence Description: ";
            getline(cin, desc2);
            e2.setLinkedFIR(firID2).setDescription(desc2).setTimestamp(getCurrentTime());
            Evidence merged = e1 + e2;  // operator+ overload
            cout << "\nMerged Evidence:" << endl;
            merged.displayEvidence();
        }
    } while (choice != 0);
}
//Feature 13 Menu
void medicalMenu() {
    MedicalProfile profile;
    int choice;
    do {
        cout << "\nMedical Profile Management " << endl;
        cout << "1 Enter Medical Profile" << endl;
        cout << "2 Display Medical Profile" << endl;
        cout << "0 Back" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            string blood, allergy, mental, marks;
            cout << "Blood Group: ";
            getline(cin, blood);
            cout << "Allergies: ";
            getline(cin, allergy);
            cout << "Mental Health Status: "; 
            getline(cin, mental);
            cout << "Physical Marks: "; 
            getline(cin, marks);
            profile.setBloodGroup(blood).setAllergies(allergy).setMentalHealth(mental).setPhysicalMarks(marks);
            cout << "Medical profile saved!" << endl;
        }
        else if (choice == 2) {
            profile.displayMedicalProfile();
        }
    } while (choice != 0);
}
// Feature 14 Menu
void familyMenu() {
    FamilyRecord family;
    int choice;
    do {
        cout << "\n Kinship / Family Mapping" << endl;
        cout << "1 Add Family Member" << endl;
        cout << "2 Display All Family Members" << endl;
        cout << "0 Back" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            string name, relation, contact;
            cout << "Name: ";
            getline(cin, name);
            cout << "Relation: ";
            getline(cin, relation);
            cout << "Contact: ";
            getline(cin, contact);
            family.addFamilyMember(name, relation, contact);
            cout << "Family member added!" << endl;
        }
        else if (choice == 2) {
            family.displayFamily();
        }
    } while (choice != 0);
}
// Main Menu 
int main() {
    int choice;
    do {
        cout << "CRIME RECORD MANAGEMENT SYSTEM" << endl;
        cout << "7  Evidence Cataloguing" << endl;
        cout << "13 Medical Profile Management" << endl;
        cout << "14 Kinship / Family Mapping" << endl;
        cout << "0  Exit" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 7){
            evidenceMenu();
         }
        else if (choice == 13) {
            medicalMenu();
        }
        else if (choice == 14) {
            familyMenu();
        }
        else if (choice != 0) {
            cout << "Invalid choice!" << endl;
        }

    } while (choice != 0);
    {
        cout << "Exiting system..." << endl;
    }
    return 0;
}