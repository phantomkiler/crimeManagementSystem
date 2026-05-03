#pragma once
#include "UIHelper.h"
#include "CrimeManagement.h"
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace CrimeManagementGUI {
    public ref class AddSuspectForm : public Form {
    private:
        TextBox^ _tbName;
        TextBox^ _tbAlias;
        TextBox^ _tbAge;
        TextBox^ _tbCrime;
        TextBox^ _tbBounty;
        ComboBox^ _cbRiskLevel;

    public:
        // Properties to pass data back to the main form
        String^ SuspectName;
        String^ SuspectCrime;
        int SuspectAge;

        AddSuspectForm() {
            this->Size = Drawing::Size(450, 550);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::FromArgb(30, 41, 59);

            BuildUI();
            this->Paint += gcnew PaintEventHandler(this, &AddSuspectForm::OnPaint);
        }

    private:
        void BuildUI() {
            int pad = 30, fw = 390, fy = 20;

            // Title
            Label^ title = gcnew Label();
            title->Text = "ISSUE NEW WANTED POSTER";
            title->Font = gcnew Drawing::Font("Segoe UI", 14.0f, FontStyle::Bold);
            title->ForeColor = Color::FromArgb(220, 38, 38);
            title->Location = Point(pad, fy);
            title->AutoSize = true;
            this->Controls->Add(title);
            fy += 50;

            // Fields
            _tbName = MakeLabeledField("Full Name / Identity:", pad, fy, fw); fy += 60;
            _tbAlias = MakeLabeledField("Known Alias (e.g., 'The Ghost'):", pad, fy, fw); fy += 60;
            _tbAge = MakeLabeledField("Estimated Age:", pad, fy, 100); fy += 60;
            _tbCrime = MakeLabeledField("Primary Charge / Crime:", pad, fy, fw); fy += 60;
            _tbBounty = MakeLabeledField("Assigned Bounty ($):", pad, fy, 150); fy += 60;

            // Risk Level Dropdown
            Label^ lblRisk = gcnew Label();
            lblRisk->Text = "Threat / Risk Level:";
            lblRisk->ForeColor = Color::FromArgb(156, 163, 175);
            lblRisk->Location = Point(pad, fy);
            lblRisk->AutoSize = true;
            this->Controls->Add(lblRisk);

            _cbRiskLevel = gcnew ComboBox();
            _cbRiskLevel->Location = Point(pad, fy + 20);
            _cbRiskLevel->Size = Drawing::Size(150, 30);
            _cbRiskLevel->Items->AddRange(gcnew array<String^> { "Low", "Medium", "High", "EXTREME" });
            _cbRiskLevel->SelectedIndex = 1;
            this->Controls->Add(_cbRiskLevel);
            fy += 70;

            // Buttons
            Button^ btnSave = gcnew Button();
            btnSave->Text = "ADD SUSPECT";
            btnSave->BackColor = Color::FromArgb(220, 38, 38);
            btnSave->ForeColor = Color::White;
            btnSave->FlatStyle = FlatStyle::Flat;
            btnSave->Size = Drawing::Size(180, 40);
            btnSave->Location = Point(pad, fy);
            btnSave->Click += gcnew EventHandler(this, &AddSuspectForm::Save_Click);
            this->Controls->Add(btnSave);

            Button^ btnCancel = gcnew Button();
            btnCancel->Text = "CANCEL";
            btnCancel->BackColor = Color::FromArgb(40, 50, 70);
            btnCancel->ForeColor = Color::White;
            btnCancel->FlatStyle = FlatStyle::Flat;
            btnCancel->Size = Drawing::Size(100, 40);
            btnCancel->Location = Point(pad + 190, fy);
            btnCancel->Click += gcnew EventHandler(this, &AddSuspectForm::Cancel_Click);
            this->Controls->Add(btnCancel);
        }

        TextBox^ MakeLabeledField(String^ labelText, int x, int y, int w) {
            Label^ lbl = gcnew Label();
            lbl->Text = labelText;
            lbl->ForeColor = Color::FromArgb(156, 163, 175);
            lbl->Location = Point(x, y);
            lbl->AutoSize = true;
            this->Controls->Add(lbl);

            TextBox^ tb = gcnew TextBox();
            tb->Location = Point(x, y + 20);
            tb->Size = Drawing::Size(w, 28);
            tb->BackColor = Color::FromArgb(16, 24, 38);
            tb->ForeColor = Color::White;
            tb->BorderStyle = BorderStyle::FixedSingle;
            this->Controls->Add(tb);
            return tb;
        }

        void OnPaint(Object^ sender, PaintEventArgs^ e) {
            // Draw a nice border around the popup
            e->Graphics->DrawRectangle(gcnew Pen(Color::FromArgb(220, 38, 38), 2.0f), 1, 1, this->Width - 2, this->Height - 2);
        }

        void Save_Click(Object^ sender, EventArgs^ e) {
            if (_tbName->Text->Trim() == "" || _tbCrime->Text->Trim() == "") {
                MessageBox::Show("Name and Crime are required fields!");
                return;
            }

            SuspectName = _tbName->Text->Trim();
            SuspectCrime = _tbCrime->Text->Trim();
            
            try {
                SuspectAge = Convert::ToInt32(_tbAge->Text->Trim());
            } catch (...) {
                SuspectAge = 0; // Default to 0 if they type letters instead of numbers
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
