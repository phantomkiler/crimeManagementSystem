#pragma once
#include "UIHelper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace CrimeManagementGUI {

public ref class InstructionForm : public Form {
private:
    bool _isAdmin;
    RichTextBox^ _rtbInstructions;
    Button^ _btnOk;

public:
    InstructionForm(bool isAdmin) {
        _isAdmin = isAdmin;

        this->Size = Drawing::Size(600, 500);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
        this->StartPosition = FormStartPosition::CenterScreen;
        this->BackColor = Color::FromArgb(30, 41, 59);
        this->DoubleBuffered = true;

        BuildUI();
        this->Paint += gcnew PaintEventHandler(this, &InstructionForm::OnPaint);
    }

private:
    void BuildUI() {
        int pad = 30;

        // 1. Title
        Label^ title = gcnew Label();
        title->Text = _isAdmin ? "ADMINISTRATOR QUICK START GUIDE" : "OFFICER QUICK START GUIDE";
        title->Font = gcnew Drawing::Font("Segoe UI", 16.0f, FontStyle::Bold);
        title->ForeColor = _isAdmin ? Color::FromArgb(220, 38, 38) : Color::FromArgb(37, 99, 235);
        title->Location = Point(pad, pad);
        title->AutoSize = true;
        this->Controls->Add(title);

        // 2. RichTextBox for the instructions
        _rtbInstructions = gcnew RichTextBox();
        _rtbInstructions->Location = Point(pad, pad + 40);
        _rtbInstructions->Size = Drawing::Size(this->Width - (pad * 2), 330);
        _rtbInstructions->BackColor = Color::FromArgb(16, 24, 38);
        _rtbInstructions->ForeColor = Color::White;
        _rtbInstructions->Font = gcnew Drawing::Font("Segoe UI", 10.0f);
        _rtbInstructions->BorderStyle = BorderStyle::None;
        _rtbInstructions->ReadOnly = true; 
        
        // Load the appropriate text based on the role
        if (_isAdmin) {
            _rtbInstructions->Text = GetAdminInstructions();
        } else {
            _rtbInstructions->Text = GetUserInstructions();
        }
        
        this->Controls->Add(_rtbInstructions);

        // 3. OK Button
        _btnOk = gcnew Button();
        _btnOk->Text = "I UNDERSTAND -> PROCEED";
        _btnOk->BackColor = _isAdmin ? Color::FromArgb(220, 38, 38) : Color::FromArgb(37, 99, 235);
        _btnOk->ForeColor = Color::White;
        _btnOk->FlatStyle = FlatStyle::Flat;
        _btnOk->Size = Drawing::Size(250, 45);
        _btnOk->Location = Point((this->Width - 250) / 2, this->Height - 70);
        _btnOk->Font = gcnew Drawing::Font("Segoe UI", 11.0f, FontStyle::Bold);
        _btnOk->Click += gcnew EventHandler(this, &InstructionForm::Ok_Click);
        this->Controls->Add(_btnOk);
    }

    void OnPaint(Object^ sender, PaintEventArgs^ e) {
        Color borderCol = _isAdmin ? Color::FromArgb(220, 38, 38) : Color::FromArgb(37, 99, 235);
        e->Graphics->DrawRectangle(gcnew Pen(borderCol, 2.0f), 1, 1, this->Width - 2, this->Height - 2);
    }

    void Ok_Click(Object^ sender, EventArgs^ e) {
        this->DialogResult = System::Windows::Forms::DialogResult::OK;
        this->Close();
    }

    String^ GetAdminInstructions() {
        return "Welcome to the CRMS Admin Command Center.\n\n" +
               "1. OVERVIEW: View a real-time dashboard of active cases and system activity.\n\n" +
               "2. SUSPECTS: View wanted posters. Click 'Add Suspect' to issue new warrants to the database.\n\n" +
               "3. CASES: Review cases filed by users. Click 'Assign Officer' to allocate resources or 'Mark Closed' to end an investigation.\n\n" +
               "4. ADVANCED INTEL:\n" +
               "   - Syndicate Map: Input a suspect to see a Breadth-First Search graph of their associates.\n" +
               "   - Crypto Evidence: Use FNV-1a hashing to verify if evidence text has been tampered with.\n" +
               "   - Biometrics: Use Levenshtein distance to find partial DNA matches.\n\n" +
               "Remember: All actions are recorded in the audit log.";
    }

    String^ GetUserInstructions() {
        return "Welcome to the CRMS Standard Officer Portal.\n\n" +
               "1. DASHBOARD: View your active case load.\n\n" +
               "2. FILE FIR: Submit a new First Information Report. Be sure to include the Crime Type, Location, and detailed description. This will be sent to the Admin Command Center.\n\n" +
               "3. TRACK FIR: Use the search box to find your previously filed reports and check if an Admin has assigned an officer to them.\n\n" +
               "4. KINSHIP: Link known associates to suspect IDs to assist the Admin's Syndicate Mapping tool.\n\n" +
               "5. PROFILE: Update your contact information (Email/Phone) here.";
    }
};

} // namespace CrimeManagementGUI
