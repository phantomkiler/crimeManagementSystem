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
    }
        else if (choice != 0) {
            cout << "Invalid choice!" << endl;
        }

    } while (choice != 0);
    {
        cout << "Exiting system..." << endl;
    }

    void menu() {
        cout << "\n===== CRIME RECORD MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Case\n";
        cout << "2. Display All Cases\n";
        cout << "3. Search Case\n";
        cout << "4. Assign Officer\n";
        cout << "5. Update Status\n";
        cout << "6. Most Wanted\n";
        cout << "0. Exit\n";
        cout << "==========================================\n";
    }

    int main() {

        CaseDatabase db;

        int choice;

        while (true) {
            menu();
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 0) break;


            switch (choice) {

            case 1:
                cout << "Add Case feature (to implement)\n";
                break;

            case 2:
                db.displayAll();
                break;

            case 3:
                cout << "Search feature (to implement)\n";
                break;

            case 4:
                cout << "Assign officer feature (to implement)\n";
                break;

            case 5:
                cout << "Update status feature (to implement)\n";
                break;

            case 6:
                db.displayMostWanted();
                break;

            default:
                cout << "Invalid choice\n";
            }
        }
          medicalprofile  profiles[10];
  anonymousreport reports[10];
  int profilecount = 0;
  int reportcount = 0;

  filepersistence fp;
  statsdashboard  dash;

  
  fp.loadmedical(profiles, profilecount);
  fp.loadreports(reports, reportcount);

  int choice;
  do {
      cout << "\n          main menu         " << endl;
      cout << "1 anonymous crime reporting " << endl;
      cout << "2 crime statistics dashboard " << endl;
      cout << "3 file persistence " << endl;
      cout << "4 medical profile management " << endl;
      cout << "0 exit" << endl;
      cout << "Choice: ";
      cin >> choice;

      // Feature 10
      if (choice == 1) {
          int c;
          cout << "1|Submit Tip  2|View All  3|Update Status" << endl;
          cout << "choice: ";
          cin >> c;

          if (c == 1 && reportcount < 10) {
              reports[reportcount].setdata();
              reportcount++;
              cout << "tip submitted!" << endl;

          }
          else if (c == 2) {
              for (int i = 0; i < reportcount; i++)
                  reports[i].display();

          }
          else if (c == 3) {
              int id;
              cout << "enter report id: ";
              cin >> id;
              for (int i = 0; i < reportcount; i++)
                  if (reports[i].getreportid() == id)
                      reports[i].updatestatus();
          }

          // Feature 11
      }
      else if (choice == 2) {
          int open, inv, closed;
          cout << "enter open cases count: ";           cin >> open;
          cout << "enter under investigation count: ";  cin >> inv;
          cout << "enter closed cases count: ";         cin >> closed;

          int newtotal = open + inv + closed;
          while (statsdashboard::gettotal() < newtotal)
              statsdashboard::addcase();

          dash.display(open, inv, closed, reportcount);

          // Feature 12
      }
      else if (choice == 3) {
          int c;
          cout << "1|Save All  2|Load All" << endl;
          cout << "choice: ";
          cin >> c;
          if (c == 1) {
              fp.savemedical(profiles, profilecount);
              fp.savereports(reports, reportcount);
          }
          else if (c == 2) {
              fp.loadmedical(profiles, profilecount);
              fp.loadreports(reports, reportcount);
          }

          // Feature 13
      }
      else if (choice == 4) {
          int c;
          cout << "1|add profile  2|view all  3|search by id" << endl;
          cout << "choice: ";
          cin >> c;

          if (c == 1 && profilecount < 10) {
              profiles[profilecount].setdata();
              profilecount++;
              cout << "profile added.." << endl;

          }
          else if (c == 2) {
              for (int i = 0; i < profilecount; i++)
                  profiles[i].display();

          }
          else if (c == 3) {
              string id;
              cout << "enter criminal id: ";
              cin >> id;
              for (int i = 0; i < profilecount; i++)
                  if (profiles[i].getcriminalid() == id)
                      profiles[i].display();
          }

      }
      else if (choice == 0) {
          fp.savemedical(profiles, profilecount);
          fp.savereports(reports, reportcount);
          cout << "data saved. goodbye pookie!" << endl;
      }

  } while (choice != 0);

    return 0;
}
