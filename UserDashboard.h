#pragma once
#include "UIHelper.h"
#include "AdvancedFeatures.h"
#include "WantedPosterCard.h"
#include <vector>

#define WIN32_LEAN_AND_MEAN
#pragma managed(push, off)
#include "CrimeManagement.h"
#pragma managed(pop)
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Collections::Generic;

namespace CrimeManagementGUI {

    public ref class UserDashboard : public UserControl {
    private:
        Form^ mainForm;
        Panel^ headerPanel;
        Panel^ tabPanel;
        Panel^ contentPanel;
        String^ activeTab;
        List<SuspectData^>^ suspects;
        FlowLayoutPanel^ suspectFlow;
        List<String^>^ myReports;
        TextBox^ txtReportDesc;
        ComboBox^ cbCrimeType;
        ComboBox^ cbLocation;
        ComboBox^ cbEvidenceSuspect;

    public:
        UserDashboard(Form^ parent) {
            mainForm = parent;
            activeTab = "overview";
            InitializeMockData();
            InitializeComponent();
        }

    private:
        void InitializeComponent() {
            this->Size = Drawing::Size(1280, 720);
            this->BackColor = Color::FromArgb(11, 17, 27);
            this->DoubleBuffered = true;

            CreateHeader();
            CreateTabBar();
            CreateContentArea();

            ShowOverview();
        }

        void InitializeMockData() {
            suspects = SuspectData::LoadSuspects();

            myReports = gcnew List<String^>();
            String^ dbPath = System::IO::Path::Combine(Application::StartupPath, "reports_db.txt");
            if (System::IO::File::Exists(dbPath)) {
                array<String^>^ lines = System::IO::File::ReadAllLines(dbPath);
                myReports->AddRange(lines);
            } else {
                myReports->Add("REP-001: Theft in Downtown - Status: Under Investigation");
                myReports->Add("REP-002: Vandalism - Status: Closed");
                System::IO::File::WriteAllLines(dbPath, myReports->ToArray());
            }
        }

        void CreateHeader() {
            headerPanel = gcnew Panel();
            headerPanel->Size = Drawing::Size(this->Width, 80);
            headerPanel->Location = Point(0, 0);
            headerPanel->Anchor = AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right;
            headerPanel->BackColor = Color::FromArgb(18, 26, 40);
            headerPanel->Paint += gcnew PaintEventHandler(this, &UserDashboard::Header_Paint);

            Label^ icon = gcnew Label();
            icon->Text = "";
            icon->Font = gcnew Drawing::Font("Impact", 28, FontStyle::Bold);
            icon->AutoSize = true;
            icon->Location = Point(30, 20);
            icon->BackColor = Color::Transparent;
            headerPanel->Controls->Add(icon);

            Label^ title = gcnew Label();
            title->Text = "OFFICER PORTAL";
            title->Font = gcnew Drawing::Font("Impact", 28, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(90, 20);
            title->BackColor = Color::Transparent;
            headerPanel->Controls->Add(title);

            Label^ subtitle = gcnew Label();
            subtitle->Text = "Welcome, Standard Officer";
            subtitle->Font = gcnew Drawing::Font("Consolas", 10);
            subtitle->ForeColor = Color::FromArgb(37, 99, 235);
            subtitle->AutoSize = true;
            subtitle->Location = Point(90, 55);
            subtitle->BackColor = Color::Transparent;
            headerPanel->Controls->Add(subtitle);

            Button^ btnLogout = gcnew Button();
            btnLogout->Text = "LOGOUT";
            btnLogout->Size = Drawing::Size(140, 40);
            btnLogout->Location = Point(this->Width - 170, 20);
            btnLogout->Anchor = AnchorStyles::Top | AnchorStyles::Right;
            btnLogout->FlatStyle = FlatStyle::Flat;
            btnLogout->BackColor = Color::Transparent;
            btnLogout->ForeColor = Color::FromArgb(37, 99, 235);
            btnLogout->Font = gcnew Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            btnLogout->Cursor = Cursors::Hand;
            btnLogout->FlatAppearance->BorderColor = Color::FromArgb(37, 99, 235);
            btnLogout->FlatAppearance->BorderSize = 2;
            btnLogout->Click += gcnew EventHandler(this, &UserDashboard::Logout_Click);
            headerPanel->Controls->Add(btnLogout);

            Button^ btnProfile = gcnew Button();
            btnProfile->Text = "PROFILE";
            btnProfile->Size = Drawing::Size(120, 40);
            btnProfile->Location = Point(this->Width - 300, 20);
            btnProfile->Anchor = AnchorStyles::Top | AnchorStyles::Right;
            btnProfile->FlatStyle = FlatStyle::Flat;
            btnProfile->BackColor = Color::Transparent;
            btnProfile->ForeColor = Color::White;
            btnProfile->Font = gcnew Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            btnProfile->Cursor = Cursors::Hand;
            btnProfile->FlatAppearance->BorderColor = Color::White;
            btnProfile->FlatAppearance->BorderSize = 1;
            btnProfile->Click += gcnew EventHandler(this, &UserDashboard::Profile_Click);
            headerPanel->Controls->Add(btnProfile);

            this->Controls->Add(headerPanel);
        }

        void Header_Paint(Object^ sender, PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            Pen^ borderPen = gcnew Pen(Color::FromArgb(37, 99, 235), 4);
            g->DrawLine(borderPen, 0, 76, headerPanel->Width, 76);
        }

        void CreateTabBar() {
            tabPanel = gcnew Panel();
            tabPanel->Size = Drawing::Size(this->Width, 60);
            tabPanel->Location = Point(0, 80);
            tabPanel->Anchor = AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right;
            tabPanel->BackColor = Color::FromArgb(15, 23, 42);

            array<String^>^ tabs = { "Dashboard", "File FIR", "Track FIR", "Evidence", "Kinship", "My History", "Anon Tips" };
            array<String^>^ tabIds = { "overview", "report", "track", "evidence", "kinship", "history", "anon" };

            int x = 20;
            for (int i = 0; i < tabs->Length; i++) {
                Button^ tabBtn = CreateTabButton(tabs[i], tabIds[i], x);
                tabBtn->Size = Drawing::Size(150, 44);
                tabPanel->Controls->Add(tabBtn);
                x += 160;
            }

            this->Controls->Add(tabPanel);
        }

        Button^ CreateTabButton(String^ text, String^ tabId, int x) {
            Button^ btn = gcnew Button();
            btn->Text = text;
            btn->Tag = tabId;
            btn->Size = Drawing::Size(150, 44);
            btn->Location = Point(x, 8);
            btn->FlatStyle = FlatStyle::Flat;
            btn->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
            btn->Cursor = Cursors::Hand;
            btn->FlatAppearance->BorderSize = 0;

            if (tabId == activeTab) {
                btn->BackColor = Color::FromArgb(37, 99, 235);
                btn->ForeColor = Color::White;
            }
            else {
                btn->BackColor = Color::Transparent;
                btn->ForeColor = Color::FromArgb(156, 163, 175);
            }

            btn->Click += gcnew EventHandler(this, &UserDashboard::Tab_Click);
            btn->MouseEnter += gcnew EventHandler(this, &UserDashboard::TabBtn_MouseEnter);
            btn->MouseLeave += gcnew EventHandler(this, &UserDashboard::TabBtn_MouseLeave);

            return btn;
        }

        void CreateContentArea() {
            contentPanel = gcnew Panel();
            contentPanel->Size = Drawing::Size(this->Width, this->Height - 140);
            contentPanel->Location = Point(0, 140);
            contentPanel->Anchor = AnchorStyles::Top | AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right;
            contentPanel->BackColor = Color::FromArgb(11, 17, 27);
            contentPanel->AutoScroll = true;

            this->Controls->Add(contentPanel);
        }

        void TabBtn_MouseEnter(Object^ sender, EventArgs^ e) {
            Button^ btn = (Button^)sender;
            if ((String^)btn->Tag != activeTab) {
                btn->BackColor = Color::FromArgb(30, 41, 59);
            }
        }

        void TabBtn_MouseLeave(Object^ sender, EventArgs^ e) {
            Button^ btn = (Button^)sender;
            if ((String^)btn->Tag != activeTab) {
                btn->BackColor = Color::Transparent;
            }
        }

        void Tab_Click(Object^ sender, EventArgs^ e) {
            Button^ btn = (Button^)sender;
            String^ tabId = (String^)btn->Tag;

            if (tabId == activeTab) return;

            for each (Control ^ ctrl in tabPanel->Controls) {
                Button^ tabBtn = dynamic_cast<Button^>(ctrl);
                if (tabBtn != nullptr) {
                    if ((String^)tabBtn->Tag == tabId) {
                        tabBtn->BackColor = Color::FromArgb(37, 99, 235);
                        tabBtn->ForeColor = Color::White;
                    }
                    else {
                        tabBtn->BackColor = Color::Transparent;
                        tabBtn->ForeColor = Color::FromArgb(156, 163, 175);
                    }
                }
            }

            activeTab = tabId;

            // Fetch latest data to ensure dashboard is real-time
            InitializeMockData();

            if (tabId == "overview") ShowOverview();
            else if (tabId == "report") ShowReportCrime();
            else if (tabId == "track") ShowTrackFIR();
            else if (tabId == "evidence") ShowEvidence();
            else if (tabId == "kinship") ShowKinship();
            else if (tabId == "history") ShowMyHistory();
            else if (tabId == "anon") ShowAnonTips();
        }

        void ShowOverview() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "OFFICER DASHBOARD";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            int cardY = 100;
            AddStatCard("Active Suspects", suspects->Count.ToString(), Color::FromArgb(37, 99, 235), 40, cardY);
            AddStatCard("My Cases", "3", Color::FromArgb(37, 99, 235), 340, cardY);

            Label^ activityTitle = gcnew Label();
            activityTitle->Text = "RECENT ACTIVITY";
            activityTitle->Font = gcnew Drawing::Font("Segoe UI", 18, FontStyle::Bold);
            activityTitle->ForeColor = Color::White;
            activityTitle->AutoSize = true;
            activityTitle->Location = Point(40, 280);
            contentPanel->Controls->Add(activityTitle);

            ListBox^ lbRecentActivity = gcnew ListBox();
            lbRecentActivity->Location = Point(40, 330);
            lbRecentActivity->Size = Drawing::Size(1200, 300);
            lbRecentActivity->BackColor = Color::FromArgb(30, 41, 59);
            lbRecentActivity->ForeColor = Color::White;
            lbRecentActivity->Font = gcnew Drawing::Font("Segoe UI", 11);
            lbRecentActivity->BorderStyle = System::Windows::Forms::BorderStyle::None;
            contentPanel->Controls->Add(lbRecentActivity);

            lbRecentActivity->Items->Clear();
            if (System::IO::File::Exists("audit_log.txt")) {
                array<String^>^ logs = System::IO::File::ReadAllLines("audit_log.txt");
                int count = 0;
                for (int i = logs->Length - 1; i >= 0 && count < 8; i--) {
                    lbRecentActivity->Items->Add(logs[i]);
                    count++;
                }
            } else {
                lbRecentActivity->Items->Add("No recent activity found.");
            }
        }

        void AddStatCard(String^ label, String^ value, Color accentColor, int x, int y) {
            Panel^ card = gcnew Panel();
            card->Size = Drawing::Size(280, 140);
            card->Location = Point(x, y);
            card->BackColor = Color::FromArgb(30, 41, 59);
            card->Tag = accentColor;
            card->Paint += gcnew PaintEventHandler(this, &UserDashboard::StatCard_Paint);

            Label^ valLabel = gcnew Label();
            valLabel->Text = value;
            valLabel->Font = gcnew Drawing::Font("Impact", 42, FontStyle::Bold);
            valLabel->ForeColor = accentColor;
            valLabel->AutoSize = true;
            valLabel->Location = Point(20, 30);
            valLabel->BackColor = Color::Transparent;
            card->Controls->Add(valLabel);

            Label^ lblLabel = gcnew Label();
            lblLabel->Text = label;
            lblLabel->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
            lblLabel->ForeColor = Color::FromArgb(156, 163, 175);
            lblLabel->AutoSize = true;
            lblLabel->Location = Point(20, 90);
            lblLabel->BackColor = Color::Transparent;
            card->Controls->Add(lblLabel);

            contentPanel->Controls->Add(card);
        }

        void StatCard_Paint(Object^ sender, PaintEventArgs^ e) {
            Panel^ card = (Panel^)sender;
            Color accent = (Color)card->Tag;

            Graphics^ g = e->Graphics;
            Pen^ borderPen = gcnew Pen(accent, 3);
            g->DrawRectangle(borderPen, 0, 0, card->Width - 1, card->Height - 1);
        }

        void ShowReportCrime() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "FILE FORMAL COMPLAINT (FIR)";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            // Complainant CNIC
            Label^ lblCnic = gcnew Label();
            lblCnic->Text = "Complainant CNIC:";
            lblCnic->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblCnic->ForeColor = Color::White;
            lblCnic->AutoSize = true;
            lblCnic->Location = Point(40, 90);
            contentPanel->Controls->Add(lblCnic);

            TextBox^ txtCnic = gcnew TextBox();
            txtCnic->Name = "txtCnic";
            txtCnic->Size = Drawing::Size(250, 30);
            txtCnic->Location = Point(40, 120);
            txtCnic->Font = gcnew Drawing::Font("Segoe UI", 12);
            txtCnic->BackColor = Color::FromArgb(30, 41, 59);
            txtCnic->ForeColor = Color::White;
            contentPanel->Controls->Add(txtCnic);

            // Accused Name
            Label^ lblAcc = gcnew Label();
            lblAcc->Text = "Accused Name:";
            lblAcc->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblAcc->ForeColor = Color::White;
            lblAcc->AutoSize = true;
            lblAcc->Location = Point(320, 90);
            contentPanel->Controls->Add(lblAcc);

            TextBox^ txtAcc = gcnew TextBox();
            txtAcc->Name = "txtAcc";
            txtAcc->Size = Drawing::Size(250, 30);
            txtAcc->Location = Point(320, 120);
            txtAcc->Font = gcnew Drawing::Font("Segoe UI", 12);
            txtAcc->BackColor = Color::FromArgb(30, 41, 59);
            txtAcc->ForeColor = Color::White;
            contentPanel->Controls->Add(txtAcc);

            // Witnesses
            Label^ lblWit = gcnew Label();
            lblWit->Text = "Witnesses (Comma separated):";
            lblWit->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblWit->ForeColor = Color::White;
            lblWit->AutoSize = true;
            lblWit->Location = Point(600, 90);
            contentPanel->Controls->Add(lblWit);

            TextBox^ txtWit = gcnew TextBox();
            txtWit->Name = "txtWit";
            txtWit->Size = Drawing::Size(300, 30);
            txtWit->Location = Point(600, 120);
            txtWit->Font = gcnew Drawing::Font("Segoe UI", 12);
            txtWit->BackColor = Color::FromArgb(30, 41, 59);
            txtWit->ForeColor = Color::White;
            contentPanel->Controls->Add(txtWit);

            // Crime Type
            Label^ lblType = gcnew Label();
            lblType->Text = "Crime Type:";
            lblType->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblType->ForeColor = Color::White;
            lblType->AutoSize = true;
            lblType->Location = Point(40, 160);
            contentPanel->Controls->Add(lblType);

            cbCrimeType = gcnew ComboBox();
            cbCrimeType->DropDownStyle = ComboBoxStyle::DropDownList;
            cbCrimeType->Font = gcnew Drawing::Font("Segoe UI", 12);
            cbCrimeType->Items->AddRange(gcnew cli::array<Object^> { 
                "Theft", "Assault", "Vandalism", "Cybercrime", "Fraud", "Other" 
            });
            cbCrimeType->Location = Point(40, 190);
            cbCrimeType->Size = Drawing::Size(250, 30);
            cbCrimeType->SelectedIndex = 0;
            contentPanel->Controls->Add(cbCrimeType);

            // Location
            Label^ lblLoc = gcnew Label();
            lblLoc->Text = "Location of Incident:";
            lblLoc->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblLoc->ForeColor = Color::White;
            lblLoc->AutoSize = true;
            lblLoc->Location = Point(320, 160);
            contentPanel->Controls->Add(lblLoc);

            cbLocation = gcnew ComboBox();
            cbLocation->Font = gcnew Drawing::Font("Segoe UI", 12);
            cbLocation->Size = Drawing::Size(320, 30);
            cbLocation->Location = Point(320, 190);
            cbLocation->DropDownStyle = ComboBoxStyle::DropDownList;
            cbLocation->Items->AddRange(gcnew cli::array<Object^> {
                "Lahore - Gulberg", "Lahore - DHA", "Lahore - Model Town", 
                "Karachi - Clifton", "Karachi - Saddar", 
                "Islamabad - F8", "Rawalpindi - Bahria Town", "Peshawar - Saddar"
            });
            if (cbLocation->Items->Count > 0) cbLocation->SelectedIndex = 0;
            contentPanel->Controls->Add(cbLocation);

            // Description
            Label^ lblDesc = gcnew Label();
            lblDesc->Text = "Description of the incident:";
            lblDesc->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblDesc->ForeColor = Color::White;
            lblDesc->AutoSize = true;
            lblDesc->Location = Point(40, 240);
            contentPanel->Controls->Add(lblDesc);

            txtReportDesc = gcnew TextBox();
            txtReportDesc->Multiline = true;
            txtReportDesc->Size = Drawing::Size(860, 120);
            txtReportDesc->Location = Point(40, 270);
            txtReportDesc->BackColor = Color::FromArgb(30, 41, 59);
            txtReportDesc->ForeColor = Color::White;
            contentPanel->Controls->Add(txtReportDesc);

            Button^ btnSubmit = gcnew Button();
            btnSubmit->Text = "SUBMIT FIR";
            btnSubmit->Size = Drawing::Size(200, 50);
            btnSubmit->Location = Point(40, 410);
            btnSubmit->BackColor = Color::FromArgb(37, 99, 235);
            btnSubmit->ForeColor = Color::White;
            btnSubmit->FlatStyle = FlatStyle::Flat;
            btnSubmit->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
            btnSubmit->Click += gcnew EventHandler(this, &UserDashboard::SubmitReport_Click);
            contentPanel->Controls->Add(btnSubmit);
        }

        void SubmitReport_Click(Object^ sender, EventArgs^ e) {
            String^ type = cbCrimeType->SelectedItem->ToString();
            String^ loc = cbLocation->SelectedItem->ToString();
            String^ desc = txtReportDesc->Text->Trim();
            
            TextBox^ txtAcc = (TextBox^)contentPanel->Controls->Find("txtAcc", false)[0];
            String^ acc = txtAcc->Text->Trim();
            
            TextBox^ txtCnic = (TextBox^)contentPanel->Controls->Find("txtCnic", false)[0];
            String^ cnic = txtCnic->Text->Trim();

            if (cnic->Length != 14) {
                MessageBox::Show("CNIC must be exactly 14 characters long.", "Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            if (String::IsNullOrEmpty(desc) || String::IsNullOrEmpty(loc)) {
                MessageBox::Show("Please complete all fields (Location and Description).", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            CrimeReport** reports = nullptr;
            int count = 0; int capacity = 0;
            DataManager::getInstance().loadReports(reports, count, capacity);

            int nextId = count + 1;
            CrimeReport* newRep = new CrimeReport();
            newRep->setReportID(nextId);
            
            std::string typeStd = msclr::interop::marshal_as<std::string>(type);
            std::string locStd = msclr::interop::marshal_as<std::string>(loc);
            std::string descFull = msclr::interop::marshal_as<std::string>(desc + " (Accused: " + acc + ")");
            
            newRep->setCrimeType(typeStd.c_str());
            newRep->setLocation(locStd.c_str());
            newRep->setDescription(descFull.c_str());
            newRep->setStatus("Pending");
            newRep->setAssignedOfficer("Unassigned");
            newRep->setDate("2026-05-02");
            newRep->setReporterName(SessionManager::getInstance().getCurrentUser());

            String^ currentTime = DateTime::Now.ToString("yyyy-MM-dd HH:mm:ss");
            std::string timeStd = msclr::interop::marshal_as<std::string>(currentTime);
            newRep->setLastUpdated(timeStd.c_str());

            CrimeReport** newReports = new CrimeReport*[count + 1];
            for(int i=0; i<count; i++) newReports[i] = reports[i];
            newReports[count] = newRep;

            DataManager::getInstance().saveReports(newReports, count + 1);

            for(int i=0; i<=count; i++) delete newReports[i];
            delete[] newReports;
            if (reports) delete[] reports;

            MessageBox::Show("Crime report submitted successfully!\nReport ID: REP-" + nextId.ToString("D3"), "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            txtReportDesc->Text = "";
            cbLocation->SelectedIndex = 0;
            cbCrimeType->SelectedIndex = 0;
        }

        void ShowMyHistory() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "MY FILED REPORTS";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            CrimeReport** reports = nullptr;
            int count = 0; int capacity = 0;
            DataManager::getInstance().loadReports(reports, count, capacity);
            String^ currentUser = gcnew String(SessionManager::getInstance().getCurrentUser());

            int y = 100;
            // Iterate backwards to show newest first
            for (int i = count - 1; i >= 0; i--) {
                if (gcnew String(reports[i]->getReporterName()) != currentUser) continue;

                String^ h = "REP-" + reports[i]->getReportID().ToString("D3") + ": [" + 
                            gcnew String(reports[i]->getCrimeType()) + "] at " + 
                            gcnew String(reports[i]->getLocation()) + " | Status: " + 
                            gcnew String(reports[i]->getStatus());

                Panel^ p = gcnew Panel();
                p->Size = Drawing::Size(800, 60);
                p->Location = Point(40, y);
                p->BackColor = Color::FromArgb(30, 41, 59);

                Label^ l = gcnew Label();
                l->Text = h;
                l->Font = gcnew Drawing::Font("Segoe UI", 12);
                l->ForeColor = Color::White;
                l->Location = Point(20, 15);
                l->AutoSize = true;
                p->Controls->Add(l);

                contentPanel->Controls->Add(p);
                y += 80;
            }

            for(int i=0; i<count; i++) delete reports[i];
            delete[] reports;
        }

        void Logout_Click(Object^ sender, EventArgs^ e) {
            CrimeManagementSystem cms;
            const char* user = SessionManager::getInstance().getCurrentUser();
            if (user && strlen(user) > 0) {
                cms.recordSession(user, "User", "logout");
                DataManager::getInstance().logAudit("LOGOUT", "User logged out: " + std::string(user));
            }
            SessionManager::getInstance().logout();

            System::Type^ formType = mainForm->GetType();
            System::Reflection::MethodInfo^ showLoginMethod = formType->GetMethod("ShowLogin");
            if (showLoginMethod != nullptr) {
                showLoginMethod->Invoke(mainForm, nullptr);
            }
        }

        void Profile_Click(Object^ sender, EventArgs^ e) {
            ShowProfile();
        }

        void ShowProfile() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "PROFILE MANAGEMENT";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            User** users = nullptr;
            int count = 0; int capacity = 0;
            DataManager::getInstance().loadUsers(users, count, capacity);
            
            String^ currentUser = gcnew String(SessionManager::getInstance().getCurrentUser());
            String^ currentPhone = "555-0199";
            String^ currentEmail = "officer@crms.gov";
            for(int i = 0; i < count; i++) {
                if (gcnew String(users[i]->getUsername()) == currentUser) {
                    currentPhone = gcnew String(users[i]->getPhone());
                    currentEmail = gcnew String(users[i]->getEmail());
                    break;
                }
            }
            for(int i=0; i<count; i++) delete users[i];
            delete[] users;

            Label^ lPhone = gcnew Label();
            lPhone->Text = "Update Phone Number:";
            lPhone->Font = gcnew Drawing::Font("Segoe UI", 12);
            lPhone->ForeColor = Color::White;
            lPhone->AutoSize = true;
            lPhone->Location = Point(40, 100);
            contentPanel->Controls->Add(lPhone);

            TextBox^ tPhone = gcnew TextBox();
            tPhone->Name = "txtProfilePhone";
            tPhone->Text = currentPhone;
            tPhone->Font = gcnew Drawing::Font("Segoe UI", 12);
            tPhone->Location = Point(40, 130);
            tPhone->Size = Drawing::Size(300, 30);
            contentPanel->Controls->Add(tPhone);

            Label^ lEmail = gcnew Label();
            lEmail->Text = "Update Email Address:";
            lEmail->Font = gcnew Drawing::Font("Segoe UI", 12);
            lEmail->ForeColor = Color::White;
            lEmail->AutoSize = true;
            lEmail->Location = Point(40, 180);
            contentPanel->Controls->Add(lEmail);

            TextBox^ tEmail = gcnew TextBox();
            tEmail->Name = "txtProfileEmail";
            tEmail->Text = currentEmail;
            tEmail->Font = gcnew Drawing::Font("Segoe UI", 12);
            tEmail->Location = Point(40, 210);
            tEmail->Size = Drawing::Size(300, 30);
            contentPanel->Controls->Add(tEmail);

            Label^ lOldPass = gcnew Label();
            lOldPass->Text = "Old Password:";
            lOldPass->Font = gcnew Drawing::Font("Segoe UI", 12);
            lOldPass->ForeColor = Color::White;
            lOldPass->AutoSize = true;
            lOldPass->Location = Point(380, 100);
            contentPanel->Controls->Add(lOldPass);

            TextBox^ tbOldPass = gcnew TextBox();
            tbOldPass->Name = "tbOldPassword";
            tbOldPass->PasswordChar = '*';
            tbOldPass->Font = gcnew Drawing::Font("Segoe UI", 12);
            tbOldPass->Location = Point(380, 130);
            tbOldPass->Size = Drawing::Size(300, 30);
            contentPanel->Controls->Add(tbOldPass);

            Label^ lNewPass = gcnew Label();
            lNewPass->Text = "New Password:";
            lNewPass->Font = gcnew Drawing::Font("Segoe UI", 12);
            lNewPass->ForeColor = Color::White;
            lNewPass->AutoSize = true;
            lNewPass->Location = Point(380, 180);
            contentPanel->Controls->Add(lNewPass);

            TextBox^ tbNewPass = gcnew TextBox();
            tbNewPass->Name = "tbNewPassword";
            tbNewPass->PasswordChar = '*';
            tbNewPass->Font = gcnew Drawing::Font("Segoe UI", 12);
            tbNewPass->Location = Point(380, 210);
            tbNewPass->Size = Drawing::Size(300, 30);
            contentPanel->Controls->Add(tbNewPass);

            Button^ btnUpdate = gcnew Button();
            btnUpdate->Text = "SAVE PROFILE";
            btnUpdate->Size = Drawing::Size(150, 40);
            btnUpdate->Location = Point(40, 270);
            btnUpdate->BackColor = Color::FromArgb(37, 99, 235);
            btnUpdate->ForeColor = Color::White;
            btnUpdate->FlatStyle = FlatStyle::Flat;
            btnUpdate->Click += gcnew EventHandler(this, &UserDashboard::UpdateProfile_Click);
            contentPanel->Controls->Add(btnUpdate);
        }

        void UpdateProfile_Click(Object^ sender, EventArgs^ e) {
            TextBox^ tPhone = (TextBox^)contentPanel->Controls->Find("txtProfilePhone", false)[0];
            TextBox^ tEmail = (TextBox^)contentPanel->Controls->Find("txtProfileEmail", false)[0];
            TextBox^ tbOldPass = (TextBox^)contentPanel->Controls->Find("tbOldPassword", false)[0];
            TextBox^ tbNewPass = (TextBox^)contentPanel->Controls->Find("tbNewPassword", false)[0];
            
            String^ newPhone = tPhone->Text->Trim();
            String^ newEmail = tEmail->Text->Trim();
            String^ oldPassword = tbOldPass->Text;
            String^ newPassword = tbNewPass->Text;

            if (newPhone != "" && newPhone->Length != 12) {
                MessageBox::Show("Phone number must be exactly 12 characters long.", "Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            if (newPassword != "" && newPassword == oldPassword) {
                MessageBox::Show("Error: New password cannot be the same as your old password!", "Security Alert", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            if (newPassword != "" && newPassword->Length < 6) {
                MessageBox::Show("Error: Password must be at least 6 characters.", "Security Alert", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            User** users = nullptr;
            int count = 0; int capacity = 0;
            DataManager::getInstance().loadUsers(users, count, capacity);
            
            String^ currentUser = gcnew String(SessionManager::getInstance().getCurrentUser());
            bool found = false;
            for(int i = 0; i < count; i++) {
                if (gcnew String(users[i]->getUsername()) == currentUser) {
                    if (oldPassword != "" && gcnew String(users[i]->getPassword()) != oldPassword) {
                        MessageBox::Show("Error: Incorrect old password.", "Security Alert", MessageBoxButtons::OK, MessageBoxIcon::Error);
                        for(int j=0; j<count; j++) delete users[j];
                        delete[] users;
                        return;
                    }

                    std::string phoneStd = msclr::interop::marshal_as<std::string>(newPhone);
                    std::string emailStd = msclr::interop::marshal_as<std::string>(newEmail);
                    users[i]->setPhone(phoneStd.c_str());
                    users[i]->setEmail(emailStd.c_str());
                    if (newPassword != "") {
                        std::string passStd = msclr::interop::marshal_as<std::string>(newPassword);
                        users[i]->setPassword(passStd.c_str());
                    }
                    found = true;
                    break;
                }
            }
            if (found) {
                DataManager::getInstance().saveUsers(users, count);
                MessageBox::Show("Profile updated successfully.", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
                tbOldPass->Text = ""; tbNewPass->Text = "";
            } else {
                MessageBox::Show("Error: User not found in database.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            for(int i=0; i<count; i++) delete users[i];
            delete[] users;
        }

        void ShowTrackFIR() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "TRACK FIR STATUS";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            ComboBox^ cbTrack = gcnew ComboBox();
            cbTrack->Name = "cbTrackFIR";
            cbTrack->Font = gcnew Drawing::Font("Segoe UI", 14);
            cbTrack->Location = Point(40, 100);
            cbTrack->Size = Drawing::Size(300, 35);
            cbTrack->DropDownStyle = ComboBoxStyle::DropDownList;
            contentPanel->Controls->Add(cbTrack);

            CrimeReport** reports = nullptr;
            int count = 0; int capacity = 0;
            DataManager::getInstance().loadReports(reports, count, capacity);

            String^ loggedInUser = gcnew String(SessionManager::getInstance().getCurrentUser());
            Label^ lblFIRResult = gcnew Label();
            lblFIRResult->Name = "lblFIRResult";
            lblFIRResult->Text = "Result for REP-XXX: Unknown";
            lblFIRResult->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblFIRResult->ForeColor = Color::White;
            lblFIRResult->AutoSize = true;
            lblFIRResult->Location = Point(40, 160);
            contentPanel->Controls->Add(lblFIRResult);

            Label^ lblFIRAssigned = gcnew Label();
            lblFIRAssigned->Name = "lblFIRAssigned";
            lblFIRAssigned->Text = "Assigned Officer: Unknown";
            lblFIRAssigned->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblFIRAssigned->ForeColor = Color::White;
            lblFIRAssigned->AutoSize = true;
            lblFIRAssigned->Location = Point(40, 190);
            contentPanel->Controls->Add(lblFIRAssigned);

            Label^ lblFIRUpdated = gcnew Label();
            lblFIRUpdated->Name = "lblFIRUpdated";
            lblFIRUpdated->Text = "Last updated: Unknown";
            lblFIRUpdated->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblFIRUpdated->ForeColor = Color::White;
            lblFIRUpdated->AutoSize = true;
            lblFIRUpdated->Location = Point(40, 220);
            contentPanel->Controls->Add(lblFIRUpdated);
            
            cbTrack->SelectedIndexChanged += gcnew EventHandler(this, &UserDashboard::cbTrackFIR_SelectedIndexChanged);

            bool foundAny = false;
            for (int i = 0; i < count; i++) {
                if (gcnew String(reports[i]->getReporterName()) == loggedInUser) {
                    String^ entry = "REP-" + reports[i]->getReportID().ToString("D3") + " : " + 
                                    gcnew String(reports[i]->getCrimeType());
                    cbTrack->Items->Add(entry);
                    foundAny = true;
                }
            }

            if (!foundAny) {
                cbTrack->Items->Add("No FIRs found for your account.");
            } else {
                cbTrack->SelectedIndex = 0;
            }

            for(int i=0; i<count; i++) delete reports[i];
            delete[] reports;
            contentPanel->Controls->Add(lblFIRUpdated);
        }

        void cbTrackFIR_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
            ComboBox^ cbTrackFIR = (ComboBox^)sender;
            if (cbTrackFIR->SelectedIndex == -1 || cbTrackFIR->SelectedItem->ToString() == "No FIRs found for your account.") return;

            String^ selectedItem = cbTrackFIR->SelectedItem->ToString();
            String^ idStr = selectedItem->Substring(4, 3); 

            Label^ lblFIRResult = (Label^)contentPanel->Controls->Find("lblFIRResult", false)[0];
            Label^ lblFIRAssigned = (Label^)contentPanel->Controls->Find("lblFIRAssigned", false)[0];
            Label^ lblFIRUpdated = (Label^)contentPanel->Controls->Find("lblFIRUpdated", false)[0];

            CrimeReport** reports = nullptr;
            int count = 0; int capacity = 0;
            DataManager::getInstance().loadReports(reports, count, capacity);

            for (int i = 0; i < count; i++) {
                String^ currentID = reports[i]->getReportID().ToString("D3");
                if (currentID == idStr) {
                    lblFIRResult->Text = "Result for REP-" + currentID + ": " + gcnew String(reports[i]->getStatus());
                    String^ assignedOff = gcnew String(reports[i]->getAssignedOfficer());
                    lblFIRAssigned->Text = "Assigned Officer: " + (assignedOff == "" ? "Unassigned" : assignedOff);
                    lblFIRUpdated->Text = "Last updated: " + gcnew String(reports[i]->getLastUpdated());
                    break;
                }
            }

            for(int i=0; i<count; i++) delete reports[i];
            delete[] reports;
        }


        void ShowEvidence() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "LOG DIGITAL EVIDENCE";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            Label^ lblSuspect = gcnew Label();
            lblSuspect->Text = "Linked Suspect:";
            lblSuspect->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblSuspect->ForeColor = Color::White;
            lblSuspect->AutoSize = true;
            lblSuspect->Location = Point(40, 90);
            contentPanel->Controls->Add(lblSuspect);

            cbEvidenceSuspect = gcnew ComboBox();
            cbEvidenceSuspect->Location = Point(40, 120);
            cbEvidenceSuspect->Size = Drawing::Size(300, 35);
            cbEvidenceSuspect->Font = gcnew Drawing::Font("Segoe UI", 12);
            cbEvidenceSuspect->DropDownStyle = ComboBoxStyle::DropDownList;
            
            System::Collections::Generic::List<SuspectData^>^ suspectsList = SuspectData::LoadSuspects();
            for each (SuspectData^ s in suspectsList) {
                cbEvidenceSuspect->Items->Add(s->name);
            }
            if (cbEvidenceSuspect->Items->Count > 0) cbEvidenceSuspect->SelectedIndex = 0;
            contentPanel->Controls->Add(cbEvidenceSuspect);

            TextBox^ tbEvidenceDescription = gcnew TextBox();
            tbEvidenceDescription->Name = "tbEvidenceDescription";
            tbEvidenceDescription->Multiline = true;
            tbEvidenceDescription->Size = Drawing::Size(600, 200);
            tbEvidenceDescription->Location = Point(40, 180);
            tbEvidenceDescription->BackColor = Color::FromArgb(30, 41, 59);
            tbEvidenceDescription->ForeColor = Color::White;
            tbEvidenceDescription->Font = gcnew Drawing::Font("Segoe UI", 12);
            contentPanel->Controls->Add(tbEvidenceDescription);

            Button^ btnSubmitEvidence = gcnew Button();
            btnSubmitEvidence->Text = "SUBMIT & SECURE HASH";
            btnSubmitEvidence->Size = Drawing::Size(250, 40);
            btnSubmitEvidence->Location = Point(40, 400);
            btnSubmitEvidence->BackColor = Color::FromArgb(37, 99, 235);
            btnSubmitEvidence->ForeColor = Color::White;
            btnSubmitEvidence->FlatStyle = FlatStyle::Flat;
            btnSubmitEvidence->Click += gcnew EventHandler(this, &UserDashboard::btnSubmitEvidence_Click);
            contentPanel->Controls->Add(btnSubmitEvidence);
        }

        void btnSubmitEvidence_Click(Object^ sender, EventArgs^ e) {
            if (cbEvidenceSuspect->SelectedIndex == -1) {
                MessageBox::Show("Error: You must select a Suspect to attach this evidence to!", "Missing Data", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }
            
            String^ targetSuspect = cbEvidenceSuspect->SelectedItem->ToString();
            TextBox^ tbEvidenceDescription = (TextBox^)contentPanel->Controls->Find("tbEvidenceDescription", false)[0];
            String^ evidenceDesc = tbEvidenceDescription->Text;
            
            if (evidenceDesc->Trim() == "") {
                MessageBox::Show("Please provide an evidence description.");
                return;
            }

            std::string sSuspect = msclr::interop::marshal_as<std::string>(targetSuspect);
            std::string sDesc = msclr::interop::marshal_as<std::string>(evidenceDesc);

            unsigned int generatedHash = AdvancedFeatures::CryptoEvidence::calculateFNV1a(sDesc);

            CrimeManagementSystem cms;
            cms.logSecureEvidence(sSuspect.c_str(), sDesc.c_str(), generatedHash);

            MessageBox::Show("Evidence securely attached to " + targetSuspect + "\nHash Generated: " + generatedHash.ToString(), "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            tbEvidenceDescription->Text = "";
            cbEvidenceSuspect->SelectedIndex = -1;
        }

        void ShowAnonTips() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "SUBMIT ANONYMOUS TIP";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            Label^ lblLoc = gcnew Label();
            lblLoc->Text = "Location / Area:";
            lblLoc->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblLoc->ForeColor = Color::White;
            lblLoc->AutoSize = true;
            lblLoc->Location = Point(40, 90);
            contentPanel->Controls->Add(lblLoc);

            TextBox^ tbTipLocation = gcnew TextBox();
            tbTipLocation->Name = "tbTipLocation";
            tbTipLocation->Size = Drawing::Size(400, 30);
            tbTipLocation->Location = Point(40, 120);
            tbTipLocation->Font = gcnew Drawing::Font("Segoe UI", 12);
            tbTipLocation->BackColor = Color::FromArgb(30, 41, 59);
            tbTipLocation->ForeColor = Color::White;
            contentPanel->Controls->Add(tbTipLocation);

            Label^ lblDesc = gcnew Label();
            lblDesc->Text = "Description:";
            lblDesc->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblDesc->ForeColor = Color::White;
            lblDesc->AutoSize = true;
            lblDesc->Location = Point(40, 160);
            contentPanel->Controls->Add(lblDesc);

            RichTextBox^ rtbTipDescription = gcnew RichTextBox();
            rtbTipDescription->Name = "rtbTipDescription";
            rtbTipDescription->Size = Drawing::Size(600, 200);
            rtbTipDescription->Location = Point(40, 190);
            rtbTipDescription->BackColor = Color::FromArgb(30, 41, 59);
            rtbTipDescription->ForeColor = Color::White;
            rtbTipDescription->Font = gcnew Drawing::Font("Segoe UI", 12);
            contentPanel->Controls->Add(rtbTipDescription);

            Button^ btnSubmitTip = gcnew Button();
            btnSubmitTip->Text = "SUBMIT TIP SECURELY";
            btnSubmitTip->Size = Drawing::Size(200, 40);
            btnSubmitTip->Location = Point(40, 410);
            btnSubmitTip->BackColor = Color::FromArgb(37, 99, 235);
            btnSubmitTip->ForeColor = Color::White;
            btnSubmitTip->FlatStyle = FlatStyle::Flat;
            btnSubmitTip->Click += gcnew EventHandler(this, &UserDashboard::btnSubmitTip_Click);
            contentPanel->Controls->Add(btnSubmitTip);
        }

        void btnSubmitTip_Click(Object^ sender, EventArgs^ e) {
            TextBox^ tbTipLocation = (TextBox^)contentPanel->Controls->Find("tbTipLocation", false)[0];
            RichTextBox^ rtbTipDescription = (RichTextBox^)contentPanel->Controls->Find("rtbTipDescription", false)[0];

            if (tbTipLocation->Text == "" || rtbTipDescription->Text == "") {
                MessageBox::Show("Please provide both a location and description.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            String^ newReport = "ID-ANON-" + (DateTime::Now.Ticks % 10000).ToString() + "|" + tbTipLocation->Text->Replace("|", "") + "|Pending|" + rtbTipDescription->Text->Replace("|", "");
            String^ arPath = System::IO::Path::Combine(Application::StartupPath, "anon_reports_db.txt");
            
            System::IO::StreamWriter^ sw = System::IO::File::AppendText(arPath);
            sw->WriteLine(newReport);
            sw->Close();

            MessageBox::Show("Anonymous tip submitted securely.", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            tbTipLocation->Clear();
            rtbTipDescription->Clear();
        }

        void ShowKinship() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "FAMILY & KINSHIP ENTRY";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            Label^ lblSuspect = gcnew Label();
            lblSuspect->Text = "Suspect Name:";
            lblSuspect->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblSuspect->ForeColor = Color::White;
            lblSuspect->AutoSize = true;
            lblSuspect->Location = Point(40, 90);
            contentPanel->Controls->Add(lblSuspect);

            ComboBox^ cbKinshipSuspects = gcnew ComboBox();
            cbKinshipSuspects->Name = "cbKinshipSuspects";
            cbKinshipSuspects->Location = Point(40, 120);
            cbKinshipSuspects->Size = Drawing::Size(300, 35);
            cbKinshipSuspects->Font = gcnew Drawing::Font("Segoe UI", 12);
            cbKinshipSuspects->DropDownStyle = ComboBoxStyle::DropDownList;
            
            System::Collections::Generic::List<SuspectData^>^ suspectsList = SuspectData::LoadSuspects();
            for each (SuspectData^ s in suspectsList) {
                cbKinshipSuspects->Items->Add(s->name);
            }
            if (cbKinshipSuspects->Items->Count > 0) cbKinshipSuspects->SelectedIndex = 0;
            contentPanel->Controls->Add(cbKinshipSuspects);

            Label^ lblRelative = gcnew Label();
            lblRelative->Text = "Relative Name:";
            lblRelative->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblRelative->ForeColor = Color::White;
            lblRelative->AutoSize = true;
            lblRelative->Location = Point(40, 160);
            contentPanel->Controls->Add(lblRelative);

            TextBox^ tbRelativeName = gcnew TextBox();
            tbRelativeName->Name = "tbRelativeName";
            tbRelativeName->Location = Point(40, 190);
            tbRelativeName->Size = Drawing::Size(300, 30);
            tbRelativeName->Font = gcnew Drawing::Font("Segoe UI", 12);
            contentPanel->Controls->Add(tbRelativeName);

            Label^ lblRelType = gcnew Label();
            lblRelType->Text = "Relationship (e.g. Brother):";
            lblRelType->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblRelType->ForeColor = Color::White;
            lblRelType->AutoSize = true;
            lblRelType->Location = Point(40, 230);
            contentPanel->Controls->Add(lblRelType);

            TextBox^ tbRelationship = gcnew TextBox();
            tbRelationship->Name = "tbRelationship";
            tbRelationship->Location = Point(40, 260);
            tbRelationship->Size = Drawing::Size(300, 30);
            tbRelationship->Font = gcnew Drawing::Font("Segoe UI", 12);
            contentPanel->Controls->Add(tbRelationship);

            Button^ btnMapRelation = gcnew Button();
            btnMapRelation->Text = "MAP RELATION";
            btnMapRelation->Size = Drawing::Size(150, 40);
            btnMapRelation->Location = Point(40, 320);
            btnMapRelation->BackColor = Color::FromArgb(37, 99, 235);
            btnMapRelation->ForeColor = Color::White;
            btnMapRelation->FlatStyle = FlatStyle::Flat;
            btnMapRelation->Click += gcnew EventHandler(this, &UserDashboard::btnMapRelation_Click);
            contentPanel->Controls->Add(btnMapRelation);
        }

        void btnMapRelation_Click(Object^ sender, EventArgs^ e) {
            ComboBox^ cbKinshipSuspects = (ComboBox^)contentPanel->Controls->Find("cbKinshipSuspects", false)[0];
            TextBox^ tbRelativeName = (TextBox^)contentPanel->Controls->Find("tbRelativeName", false)[0];
            TextBox^ tbRelationship = (TextBox^)contentPanel->Controls->Find("tbRelationship", false)[0];

            if (cbKinshipSuspects->SelectedIndex == -1) {
                MessageBox::Show("Please select a main suspect.");
                return;
            }
            if (tbRelativeName->Text->Trim() == "" || tbRelationship->Text->Trim() == "") {
                MessageBox::Show("Please fill out relative name and relationship.");
                return;
            }

            String^ suspectName = cbKinshipSuspects->SelectedItem->ToString();
            String^ relativeName = tbRelativeName->Text->Trim();
            String^ relationType = tbRelationship->Text->Trim();

            std::string sName = msclr::interop::marshal_as<std::string>(suspectName);
            std::string rName = msclr::interop::marshal_as<std::string>(relativeName + " [" + relationType + "]");

            syndicateNetwork[sName].push_back(rName);
            syndicateNetwork[rName].push_back(sName);

            MessageBox::Show("Kinship relation added to the Syndicate Database!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            tbRelativeName->Text = "";
            tbRelationship->Text = "";
        }


    };
}
