#include "crimeManagement.h"

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

    return 0;
