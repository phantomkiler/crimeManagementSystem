#pragma once
#include "UIHelper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace CrimeManagementGUI {

public ref class AddOfficerForm : public Form {
private:
    TextBox^ _tbName;
    TextBox^ _tbAge;
    ComboBox^ _cbRank;

public:
    String^ OfficerName;
    String^ OfficerRank;
    int OfficerAge;

    AddOfficerForm() {
        this->Size = Drawing::Size(400, 450);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
        this->StartPosition = FormStartPosition::CenterParent;
        this->BackColor = Color::FromArgb(30, 41, 59);

        BuildUI();
        this->Paint += gcnew PaintEventHandler(this, &AddOfficerForm::OnPaint);
    }

private:
    void BuildUI() {
        int pad = 30, fw = 340, fy = 30;

        Label^ title = gcnew Label();
        title->Text = "RECRUIT NEW OFFICER";
        title->Font = gcnew Drawing::Font("Segoe UI", 14.0f, FontStyle::Bold);
        title->ForeColor = Color::FromArgb(37, 99, 235);
        title->Location = Point(pad, fy);
        title->AutoSize = true;
        this->Controls->Add(title);
        fy += 60;

        Label^ lblName = gcnew Label();
        lblName->Text = "Officer Full Name:";
        lblName->ForeColor = Color::FromArgb(156, 163, 175);
        lblName->Location = Point(pad, fy);
        lblName->AutoSize = true;
        this->Controls->Add(lblName);
        
        _tbName = gcnew TextBox();
        _tbName->Location = Point(pad, fy + 20);
        _tbName->Size = Drawing::Size(fw, 28);
        _tbName->BackColor = Color::FromArgb(16, 24, 38);
        _tbName->ForeColor = Color::White;
        _tbName->BorderStyle = BorderStyle::FixedSingle;
        this->Controls->Add(_tbName);
        fy += 70;

        Label^ lblAge = gcnew Label();
        lblAge->Text = "Age:";
        lblAge->ForeColor = Color::FromArgb(156, 163, 175);
        lblAge->Location = Point(pad, fy);
        lblAge->AutoSize = true;
        this->Controls->Add(lblAge);
        
        _tbAge = gcnew TextBox();
        _tbAge->Location = Point(pad, fy + 20);
        _tbAge->Size = Drawing::Size(100, 28);
        _tbAge->BackColor = Color::FromArgb(16, 24, 38);
        _tbAge->ForeColor = Color::White;
        _tbAge->BorderStyle = BorderStyle::FixedSingle;
        this->Controls->Add(_tbAge);
        fy += 70;

        Label^ lblRank = gcnew Label();
        lblRank->Text = "Assign Rank/Skill:";
        lblRank->ForeColor = Color::FromArgb(156, 163, 175);
        lblRank->Location = Point(pad, fy);
        lblRank->AutoSize = true;
        this->Controls->Add(lblRank);

        _cbRank = gcnew ComboBox();
        _cbRank->Location = Point(pad, fy + 20);
        _cbRank->Size = Drawing::Size(fw, 30);
        _cbRank->Items->AddRange(gcnew array<String^> { 
            "Standard Officer", 
            "Senior Detective", 
            "Cybercrime Specialist", 
            "Forensics Expert",
            "Marine Captain"
        });
        _cbRank->SelectedIndex = 0;
        this->Controls->Add(_cbRank);
        fy += 80;

        Button^ btnSave = gcnew Button();
        btnSave->Text = "RECRUIT";
        btnSave->BackColor = Color::FromArgb(37, 99, 235);
        btnSave->ForeColor = Color::White;
        btnSave->FlatStyle = FlatStyle::Flat;
        btnSave->Location = Point(pad, fy);
        btnSave->Size = Drawing::Size(150, 40);
        btnSave->Click += gcnew EventHandler(this, &AddOfficerForm::Save_Click);
        this->Controls->Add(btnSave);

        Button^ btnCancel = gcnew Button();
        btnCancel->Text = "CANCEL";
        btnCancel->BackColor = Color::FromArgb(40, 50, 70);
        btnCancel->ForeColor = Color::White;
        btnCancel->FlatStyle = FlatStyle::Flat;
        btnCancel->Location = Point(pad + 170, fy);
        btnCancel->Size = Drawing::Size(100, 40);
        btnCancel->Click += gcnew EventHandler(this, &AddOfficerForm::Cancel_Click);
        this->Controls->Add(btnCancel);
    }

    void OnPaint(Object^ sender, PaintEventArgs^ e) {
        e->Graphics->DrawRectangle(gcnew Pen(Color::FromArgb(37, 99, 235), 2.0f), 1, 1, this->Width - 2, this->Height - 2);
    }

    void Save_Click(Object^ sender, EventArgs^ e) {
        if (_tbName->Text->Trim() == "") {
            MessageBox::Show("Officer Name is required!");
            return;
        }

        OfficerName = _tbName->Text->Trim();
        OfficerRank = _cbRank->SelectedItem->ToString();
        
        try {
            OfficerAge = Convert::ToInt32(_tbAge->Text->Trim());
        } catch (...) {
            OfficerAge = 25;
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
