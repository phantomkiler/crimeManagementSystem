#pragma once
#include "UIHelper.h"
#include "CrimeManagement.h"
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace CrimeManagementGUI {
    public ref class AssignOfficerForm : public Form {
    private:
        ComboBox^ cbAvailableOfficers;
        String^ caseID;

    public:
        String^ SelectedOfficerName;

        AssignOfficerForm(String^ caseIdStr) {
            caseID = caseIdStr;

            this->Size = Drawing::Size(400, 250);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::FromArgb(30, 41, 59);

            BuildUI();
            this->Paint += gcnew PaintEventHandler(this, &AssignOfficerForm::OnPaint);
            
            LoadOfficersIntoDropdown();
        }

    private:
        void BuildUI() {
            int pad = 30;

            // Title
            Label^ title = gcnew Label();
            title->Text = "ASSIGN OFFICER";
            title->Font = gcnew Drawing::Font("Segoe UI", 14.0f, FontStyle::Bold);
            title->ForeColor = Color::FromArgb(37, 99, 235);
            title->Location = Point(pad, 20);
            title->AutoSize = true;
            this->Controls->Add(title);

            // Subtitle
            Label^ subtitle = gcnew Label();
            subtitle->Text = "Select an officer for Case: " + caseID;
            subtitle->ForeColor = Color::FromArgb(156, 163, 175);
            subtitle->Location = Point(pad, 60);
            subtitle->AutoSize = true;
            this->Controls->Add(subtitle);

            // ComboBox
            cbAvailableOfficers = gcnew ComboBox();
            cbAvailableOfficers->Location = Point(pad, 90);
            cbAvailableOfficers->Size = Drawing::Size(340, 30);
            cbAvailableOfficers->Font = gcnew Drawing::Font("Segoe UI", 12);
            cbAvailableOfficers->DropDownStyle = ComboBoxStyle::DropDownList;
            this->Controls->Add(cbAvailableOfficers);

            // Buttons
            Button^ btnConfirm = gcnew Button();
            btnConfirm->Text = "CONFIRM ASSIGNMENT";
            btnConfirm->BackColor = Color::FromArgb(37, 99, 235);
            btnConfirm->ForeColor = Color::White;
            btnConfirm->FlatStyle = FlatStyle::Flat;
            btnConfirm->Size = Drawing::Size(200, 40);
            btnConfirm->Location = Point(pad, 150);
            btnConfirm->Click += gcnew EventHandler(this, &AssignOfficerForm::Confirm_Click);
            this->Controls->Add(btnConfirm);

            Button^ btnCancel = gcnew Button();
            btnCancel->Text = "CANCEL";
            btnCancel->BackColor = Color::FromArgb(40, 50, 70);
            btnCancel->ForeColor = Color::White;
            btnCancel->FlatStyle = FlatStyle::Flat;
            btnCancel->Size = Drawing::Size(100, 40);
            btnCancel->Location = Point(pad + 220, 150);
            btnCancel->Click += gcnew EventHandler(this, &AssignOfficerForm::Cancel_Click);
            this->Controls->Add(btnCancel);
        }

        void LoadOfficersIntoDropdown() {
            cbAvailableOfficers->Items->Clear();

            CrimeManagementSystem crimeSystem;
            // CRITICAL FIX: Loop only to officerCount, NOT the array capacity!
            for (int i = 0; i < crimeSystem.getOfficerCount(); i++) {
                String^ name = gcnew String(crimeSystem.getOfficers()[i]->getName().c_str());
                String^ rank = gcnew String(crimeSystem.getOfficers()[i]->getRank().c_str());
                cbAvailableOfficers->Items->Add(name + " - " + rank);
            }

            if (cbAvailableOfficers->Items->Count > 0) {
                cbAvailableOfficers->SelectedIndex = 0;
            }
        }

        void OnPaint(Object^ sender, PaintEventArgs^ e) {
            e->Graphics->DrawRectangle(gcnew Pen(Color::FromArgb(37, 99, 235), 2.0f), 1, 1, this->Width - 2, this->Height - 2);
        }

        void Confirm_Click(Object^ sender, EventArgs^ e) {
            if (cbAvailableOfficers->SelectedIndex == -1) {
                MessageBox::Show("Please select an officer.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            String^ selection = cbAvailableOfficers->SelectedItem->ToString();
            // Extract just the name (part before " - ")
            int dashIndex = selection->IndexOf(" - ");
            if (dashIndex != -1) {
                SelectedOfficerName = selection->Substring(0, dashIndex);
            } else {
                SelectedOfficerName = selection;
            }

            this->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->Close();
        }

        void Cancel_Click(Object^ sender, EventArgs^ e) {
            this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Close();
        }
    };
}
