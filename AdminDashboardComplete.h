#pragma once
#include "UIHelper.h"
#include "AdvancedFeatures.h"
#include "WantedPosterCard.h"
#include "AddSuspectForm.h"
#include "AssignOfficerForm.h"
#include "AddOfficerForm.h"
#define WIN32_LEAN_AND_MEAN
#pragma managed(push, off)
#include "CrimeManagement.h"
#pragma managed(pop)
#include <msclr/marshal_cppstd.h>
#include <vector>
using namespace System::Windows::Forms;
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Collections::Generic;

namespace CrimeManagementGUI {

    public ref class AdminDashboard : public UserControl {
    private:
        Form^ mainForm;
        Panel^ headerPanel;
        Panel^ tabPanel;
        Panel^ contentPanel;
        String^ activeTab;
        List<SuspectData^>^ suspects;
        FlowLayoutPanel^ suspectFlow;
        TextBox^ txtSuspectSearch;
        List<String^>^ adminUsers;
        List<String^>^ evidenceLog;
        List<String^>^ anonReports;
        List<String^>^ casesList;

    public:
        AdminDashboard(Form^ parent) {
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

            // Load/init admin users
            adminUsers = gcnew List<String^>();
            String^ admPath = System::IO::Path::Combine(Application::StartupPath, "admins_db.txt");
            if (System::IO::File::Exists(admPath)) {
                adminUsers->AddRange(System::IO::File::ReadAllLines(admPath));
            } else {
                adminUsers->Add("admin|admin123|System Administrator");
                System::IO::File::WriteAllLines(admPath, adminUsers->ToArray());
            }

            // Load/init evidence log
            evidenceLog = gcnew List<String^>();
            String^ evPath = System::IO::Path::Combine(Application::StartupPath, "evidence_db.txt");
            if (System::IO::File::Exists(evPath)) {
                evidenceLog->AddRange(System::IO::File::ReadAllLines(evPath));
            } else {
                evidenceLog->Add("EV-001|S001|CCTV Footage from Downtown|2026-01-10");
                evidenceLog->Add("EV-002|S001|Witness Statement - John Doe|2026-01-11");
                evidenceLog->Add("EV-003|S002|Encrypted Hard Drive|2026-01-15");
                System::IO::File::WriteAllLines(evPath, evidenceLog->ToArray());
            }

            // Load/init anonymous reports
            anonReports = gcnew List<String^>();
            String^ arPath = System::IO::Path::Combine(Application::StartupPath, "anon_reports_db.txt");
            if (System::IO::File::Exists(arPath)) {
                anonReports->AddRange(System::IO::File::ReadAllLines(arPath));
            } else {
                anonReports->Add("AR-001|Suspicious activity near Park Street|Pending|2026-04-01");
                anonReports->Add("AR-002|Unknown vehicle seen near warehouse|Reviewed|2026-04-05");
                System::IO::File::WriteAllLines(arPath, anonReports->ToArray());
            }

            // Load/init cases
            casesList = gcnew List<String^>();
            String^ cPath = System::IO::Path::Combine(Application::StartupPath, "cases_db.txt");
            if (System::IO::File::Exists(cPath)) {
                casesList->AddRange(System::IO::File::ReadAllLines(cPath));
            } else {
                casesList->Add("CR-2026-001|Armed Robbery - JACK MORRISON|Under Investigation|Unassigned");
                casesList->Add("CR-2026-002|Corporate Data Breach - MARIA SANTOS|Under Investigation|Unassigned");
                casesList->Add("CR-2026-003|Racketeering - VIKTOR VOLKOV|Closed|Officer Smith");
                System::IO::File::WriteAllLines(cPath, casesList->ToArray());
            }
        }

        void CreateHeader() {
            headerPanel = gcnew Panel();
            headerPanel->Size = Drawing::Size(this->Width, 80);
            headerPanel->Location = Point(0, 0);
            headerPanel->Anchor = AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right;
            headerPanel->BackColor = Color::FromArgb(18, 26, 40);
            headerPanel->Paint += gcnew PaintEventHandler(this, &AdminDashboard::Header_Paint);

            // Icon
            Label^ icon = gcnew Label();
            icon->Text = "";
            icon->Font = gcnew Drawing::Font("Impact", 28, FontStyle::Bold);
            icon->AutoSize = true;
            icon->Location = Point(30, 20);
            icon->BackColor = Color::Transparent;
            headerPanel->Controls->Add(icon);

            // Title
            Label^ title = gcnew Label();
            title->Text = "ADMIN COMMAND CENTER";
            title->Font = gcnew Drawing::Font("Impact", 28, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(90, 20);
            title->BackColor = Color::Transparent;
            headerPanel->Controls->Add(title);

            // Subtitle
            Label^ subtitle = gcnew Label();
            subtitle->Text = "Welcome, System Administrator";
            subtitle->Font = gcnew Drawing::Font("Consolas", 10);
            subtitle->ForeColor = Color::FromArgb(220, 38, 38);
            subtitle->AutoSize = true;
            subtitle->Location = Point(90, 55);
            subtitle->BackColor = Color::Transparent;
            headerPanel->Controls->Add(subtitle);

            // Logout Button
            Button^ btnLogout = gcnew Button();
            btnLogout->Text = "LOGOUT";
            btnLogout->Size = Drawing::Size(140, 40);
            btnLogout->Location = Point(this->Width - 170, 20);
            btnLogout->Anchor = AnchorStyles::Top | AnchorStyles::Right;
            btnLogout->FlatStyle = FlatStyle::Flat;
            btnLogout->BackColor = Color::Transparent;
            btnLogout->ForeColor = Color::FromArgb(220, 38, 38);
            btnLogout->Font = gcnew Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            btnLogout->Cursor = Cursors::Hand;
            btnLogout->FlatAppearance->BorderColor = Color::FromArgb(220, 38, 38);
            btnLogout->FlatAppearance->BorderSize = 2;
            btnLogout->Click += gcnew EventHandler(this, &AdminDashboard::Logout_Click);
            headerPanel->Controls->Add(btnLogout);

            this->Controls->Add(headerPanel);
        }

        void Header_Paint(Object^ sender, PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            Pen^ borderPen = gcnew Pen(Color::FromArgb(220, 38, 38), 4);
            g->DrawLine(borderPen, 0, 76, headerPanel->Width, 76);
        }

        void CreateTabBar() {
            tabPanel = gcnew Panel();
            tabPanel->Size = Drawing::Size(this->Width, 60);
            tabPanel->Location = Point(0, 80);
            tabPanel->Anchor = AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right;
            tabPanel->BackColor = Color::FromArgb(15, 23, 42);

            array<String^>^ tabs    = { "Overview", "Suspects", "Cases", "Reports", "Evidence", "Anon Tips", "Users", "Intel Map", "Master Archives" };
            array<String^>^ tabIds  = { "overview", "suspects", "cases", "reports", "evidence", "anon", "users", "intel", "archives" };

            int x = 30;
            for (int i = 0; i < tabs->Length; i++) {
                Button^ tabBtn = CreateTabButton(tabs[i], tabIds[i], x);
                tabBtn->Size = Drawing::Size(130, 44);
                tabPanel->Controls->Add(tabBtn);
                x += 140;
            }

            this->Controls->Add(tabPanel);
        }

        Button^ CreateTabButton(String^ text, String^ tabId, int x) {
            Button^ btn = gcnew Button();
            btn->Text = text;
            btn->Tag = tabId;
            btn->Size = Drawing::Size(130, 44);
            btn->Location = Point(x, 8);
            btn->FlatStyle = FlatStyle::Flat;
            btn->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
            btn->Cursor = Cursors::Hand;
            btn->FlatAppearance->BorderSize = 0;

            if (tabId == activeTab) {
                btn->BackColor = Color::FromArgb(220, 38, 38);
                btn->ForeColor = Color::White;
            }
            else {
                btn->BackColor = Color::Transparent;
                btn->ForeColor = Color::FromArgb(156, 163, 175);
            }

            btn->Click += gcnew EventHandler(this, &AdminDashboard::Tab_Click);
            btn->MouseEnter += gcnew EventHandler(this, &AdminDashboard::TabBtn_MouseEnter);
            btn->MouseLeave += gcnew EventHandler(this, &AdminDashboard::TabBtn_MouseLeave);

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

            // Update button states
            for each (Control ^ ctrl in tabPanel->Controls) {
                Button^ tabBtn = dynamic_cast<Button^>(ctrl);
                if (tabBtn != nullptr) {
                    if ((String^)tabBtn->Tag == tabId) {
                        tabBtn->BackColor = Color::FromArgb(220, 38, 38);
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

            // Switch content
            if (tabId == "overview")  ShowOverview();
            else if (tabId == "suspects") ShowSuspects("");
            else if (tabId == "cases")    ShowCases();
            else if (tabId == "reports") ShowReports();
            else if (tabId == "evidence") ShowEvidence();
            else if (tabId == "anon")    ShowAnonReports();
            else if (tabId == "users")   ShowUserManagement();
            else if (tabId == "archives") ShowMasterArchives();
            else if (tabId == "intel") {
                System::Type^ formType = mainForm->GetType();
                System::Reflection::MethodInfo^ showIntelMethod = formType->GetMethod("ShowAdvancedFeatures");
                if (showIntelMethod != nullptr) {
                    showIntelMethod->Invoke(mainForm, nullptr);
                }
            }
            else if (tabId == "archives") ShowMasterArchives();
        }

        void ShowOverview() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "SYSTEM OVERVIEW";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            CrimeReport** reports = nullptr;
            int repCount = 0; int repCap = 0;
            DataManager::getInstance().loadReports(reports, repCount, repCap);
            int activeCount = 0;
            for(int i = 0; i < repCount; i++) {
                String^ status = gcnew String(reports[i]->getStatus());
                if (status == "Pending" || status == "Under Investigation") {
                    activeCount++;
                }
            }
            for(int i = 0; i < repCount; i++) delete reports[i];
            delete[] reports;

            // Stats Cards
            int cardY = 100;
            AddStatCard("Total Suspects", suspects->Count.ToString(), Color::FromArgb(220, 38, 38), 40, cardY);
            AddStatCard("Most Wanted", GetMostWantedCount().ToString(), Color::FromArgb(245, 158, 11), 340, cardY);
            AddStatCard("Active Cases", activeCount.ToString(), Color::FromArgb(37, 99, 235), 640, cardY);
            AddStatCard("Solved", "92%", Color::FromArgb(34, 197, 94), 940, cardY);

            // Recent Activity
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
            card->Paint += gcnew PaintEventHandler(this, &AdminDashboard::StatCard_Paint);

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

        void ShowSuspects(String^ filter) {
            contentPanel->Controls->Clear();

            // Title
            Label^ title = gcnew Label();
            title->Text = "SUSPECT DATABASE";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            // Search Bar
            txtSuspectSearch = gcnew TextBox();
            txtSuspectSearch->Size = Drawing::Size(300, 35);
            txtSuspectSearch->Location = Point(400, 40);
            txtSuspectSearch->Font = gcnew Drawing::Font("Segoe UI", 12);
            txtSuspectSearch->BackColor = Color::FromArgb(30, 41, 59);
            txtSuspectSearch->ForeColor = Color::White;
            if (filter != "") txtSuspectSearch->Text = filter;
            contentPanel->Controls->Add(txtSuspectSearch);

            Button^ btnSearch = gcnew Button();
            btnSearch->Text = "SEARCH BY NAME/ID";
            btnSearch->Size = Drawing::Size(180, 32);
            btnSearch->Location = Point(710, 39);
            btnSearch->BackColor = Color::FromArgb(37, 99, 235);
            btnSearch->ForeColor = Color::White;
            btnSearch->FlatStyle = FlatStyle::Flat;
            btnSearch->Click += gcnew EventHandler(this, &AdminDashboard::SearchSuspect_Click);
            contentPanel->Controls->Add(btnSearch);

            Button^ btnAddSuspect = gcnew Button();
            btnAddSuspect->Text = "ADD SUSPECT";
            btnAddSuspect->Size = Drawing::Size(180, 32);
            btnAddSuspect->Location = Point(910, 39);
            btnAddSuspect->BackColor = Color::FromArgb(220, 38, 38);
            btnAddSuspect->ForeColor = Color::White;
            btnAddSuspect->FlatStyle = FlatStyle::Flat;
            btnAddSuspect->Click += gcnew EventHandler(this, &AdminDashboard::btnAddSuspect_Click);
            contentPanel->Controls->Add(btnAddSuspect);

            // Most Wanted Section
            Label^ mwTitle = gcnew Label();
            mwTitle->Text = "[!] MOST WANTED ALERTS";
            mwTitle->Font = gcnew Drawing::Font("Segoe UI", 20, FontStyle::Bold);
            mwTitle->ForeColor = Color::FromArgb(220, 38, 38);
            mwTitle->AutoSize = true;
            mwTitle->Location = Point(40, 100);
            contentPanel->Controls->Add(mwTitle);

            // Most Wanted Flow
            FlowLayoutPanel^ mwFlow = gcnew FlowLayoutPanel();
            mwFlow->Location = Point(40, 150);
            mwFlow->Size = Drawing::Size(1200, 480);
            mwFlow->AutoScroll = false;
            mwFlow->WrapContents = true;
            mwFlow->BackColor = Color::Transparent;

            String^ lowerFilter = filter->ToLower();

            for each (SuspectData ^ s in suspects) {
                if (s->status == "Most Wanted") {
                    if (filter != "" && !s->name->ToLower()->Contains(lowerFilter) && !s->id->ToLower()->Contains(lowerFilter)) continue;
                    WantedPosterCard^ card = gcnew WantedPosterCard(s);
                    mwFlow->Controls->Add(card);
                }
            }
            contentPanel->Controls->Add(mwFlow);

            // All Suspects Section
            Label^ allTitle = gcnew Label();
            allTitle->Text = "ALL SUSPECTS";
            allTitle->Font = gcnew Drawing::Font("Segoe UI", 18, FontStyle::Bold);
            allTitle->ForeColor = Color::White;
            allTitle->AutoSize = true;
            allTitle->Location = Point(40, 650);
            contentPanel->Controls->Add(allTitle);

            // All Suspects Flow
            FlowLayoutPanel^ allFlow = gcnew FlowLayoutPanel();
            allFlow->Location = Point(40, 700);
            allFlow->Size = Drawing::Size(1200, 900);
            allFlow->AutoScroll = false;
            allFlow->WrapContents = true;
            allFlow->BackColor = Color::Transparent;

            for each (SuspectData ^ s in suspects) {
                if (s->status != "Most Wanted") {
                    if (filter != "" && !s->name->ToLower()->Contains(lowerFilter) && !s->id->ToLower()->Contains(lowerFilter)) continue;
                    WantedPosterCard^ card = gcnew WantedPosterCard(s);
                    allFlow->Controls->Add(card);
                }
            }
            contentPanel->Controls->Add(allFlow);
        }

        void SearchSuspect_Click(Object^ sender, EventArgs^ e) {
            String^ filter = txtSuspectSearch->Text->Trim();
            ShowSuspects(filter);
        }

        void btnAddSuspect_Click(Object^ sender, EventArgs^ e) {
            AddSuspectForm^ popup = gcnew AddSuspectForm();
            if (popup->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                String^ newName = popup->SuspectName;
                String^ newCrime = popup->SuspectCrime;
                int newAge = popup->SuspectAge;

                std::string c_name = msclr::interop::marshal_as<std::string>(newName);
                std::string c_crime = msclr::interop::marshal_as<std::string>(newCrime);

                CrimeManagementSystem cms;
                cms.addNewSuspect(c_name.c_str(), newAge, c_crime.c_str());

                // Add to GUI mock data array so it displays instantly
                SuspectData^ sd = gcnew SuspectData();
                sd->id = "S-" + (suspects->Count + 100).ToString();
                sd->name = newName;
                sd->crime = newCrime;
                sd->bounty = 5000;
                sd->status = "Active";
                suspects->Add(sd);
                
                SuspectData::SaveSuspects(suspects);

                MessageBox::Show("Wanted Poster Issued Successfully!", "System Alert", MessageBoxButtons::OK, MessageBoxIcon::Information);
                ShowSuspects("");
            }
        }

        void ShowCases() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "CASE MANAGEMENT";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            Label^ info = gcnew Label();
            info->Text = "Track and manage all active criminal cases";
            info->Font = gcnew Drawing::Font("Segoe UI", 12);
            info->ForeColor = Color::FromArgb(156, 163, 175);
            info->AutoSize = true;
            info->Location = Point(40, 80);
            contentPanel->Controls->Add(info);

            int y = 140;
            for (int i = 0; i < casesList->Count; i++) {
                array<String^>^ parts = casesList[i]->Split('|');
                if (parts->Length < 4) continue;

                Panel^ casePanel = gcnew Panel();
                casePanel->Size = Drawing::Size(1200, 80);
                casePanel->Location = Point(40, y);
                casePanel->BackColor = Color::FromArgb(30, 41, 59);

                Label^ caseLabel = gcnew Label();
                caseLabel->Text = parts[0] + ": " + parts[1];
                caseLabel->Font = gcnew Drawing::Font("Segoe UI", 14, FontStyle::Bold);
                caseLabel->ForeColor = Color::White;
                caseLabel->AutoSize = true;
                caseLabel->Location = Point(20, 20);
                casePanel->Controls->Add(caseLabel);

                Label^ statusLabel = gcnew Label();
                statusLabel->Text = "Status: " + parts[2] + " | Assigned to: " + parts[3];
                statusLabel->Font = gcnew Drawing::Font("Segoe UI", 10);
                statusLabel->ForeColor = parts[2] == "Closed" ? Color::FromArgb(34, 197, 94) : Color::FromArgb(245, 158, 11);
                statusLabel->AutoSize = true;
                statusLabel->Location = Point(20, 48);
                casePanel->Controls->Add(statusLabel);

                if (parts[3] == "Unassigned") {
                    Button^ btnAssign = gcnew Button();
                    btnAssign->Text = "ASSIGN OFFICER";
                    btnAssign->Tag = i;
                    btnAssign->Size = Drawing::Size(150, 30);
                    btnAssign->Location = Point(850, 25);
                    btnAssign->BackColor = Color::FromArgb(30, 41, 59);
                    btnAssign->ForeColor = Color::White;
                    btnAssign->FlatStyle = FlatStyle::Flat;
                    btnAssign->Click += gcnew EventHandler(this, &AdminDashboard::AssignOfficer_Click);
                    casePanel->Controls->Add(btnAssign);
                }

                if (parts[2] != "Closed") {
                    Button^ btnUpdate = gcnew Button();
                    btnUpdate->Text = "MARK CLOSED";
                    btnUpdate->Tag = i;
                    btnUpdate->Size = Drawing::Size(150, 30);
                    btnUpdate->Location = Point(1020, 25);
                    btnUpdate->BackColor = Color::FromArgb(37, 99, 235);
                    btnUpdate->ForeColor = Color::White;
                    btnUpdate->FlatStyle = FlatStyle::Flat;
                    btnUpdate->Click += gcnew EventHandler(this, &AdminDashboard::UpdateCaseStatus_Click);
                    casePanel->Controls->Add(btnUpdate);
                }

                contentPanel->Controls->Add(casePanel);
                y += 100;
            }
        }

        void AssignOfficer_Click(Object^ sender, EventArgs^ e) {
            Button^ btn = (Button^)sender;
            int idx = (int)btn->Tag;
            array<String^>^ parts = casesList[idx]->Split('|');
            if (parts->Length >= 4) {
                AssignOfficerForm^ popup = gcnew AssignOfficerForm(parts[0]);
                if (popup->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    String^ selectedOfficer = popup->SelectedOfficerName;
                    casesList[idx] = parts[0] + "|" + parts[1] + "|" + parts[2] + "|" + selectedOfficer;
                    SaveCases();
                    
                    // Update corresponding FIR in CrimeReport
                    CrimeReport** reports = nullptr;
                    int count = 0; int capacity = 0;
                    DataManager::getInstance().loadReports(reports, count, capacity);
                    
                    for (int i = 0; i < count; i++) {
                        String^ cType = gcnew String(reports[i]->getCrimeType());
                        String^ desc = gcnew String(reports[i]->getDescription());
                        String^ accused = "UNKNOWN";
                        int aIdx = desc->IndexOf("Accused: ");
                        if (aIdx != -1) {
                            int endIdx = desc->IndexOf(")", aIdx);
                            if (endIdx != -1) accused = desc->Substring(aIdx + 9, endIdx - (aIdx + 9));
                        }
                        String^ matchString = cType + " - " + accused;
                        if (matchString == parts[1]) {
                            msclr::interop::marshal_context ctx;
                            const char* c_officer = ctx.marshal_as<const char*>(selectedOfficer);
                            reports[i]->setAssignedOfficer(c_officer);
                        }
                    }
                    DataManager::getInstance().saveReports(reports, count);
                    for(int i=0; i<count; i++) delete reports[i];
                    delete[] reports;

                    ShowCases();
                    MessageBox::Show("Officer successfully assigned!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
            }
        }

        void UpdateCaseStatus_Click(Object^ sender, EventArgs^ e) {
            Button^ btn = (Button^)sender;
            int idx = (int)btn->Tag;
            array<String^>^ parts = casesList[idx]->Split('|');
            if (parts->Length >= 4) {
                ArchiveReasonForm^ popup = gcnew ArchiveReasonForm();
                if (popup->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    String^ closingReason = popup->SelectedReason;
                    String^ date = DateTime::Now.ToString("yyyy-MM-dd");

                    msclr::interop::marshal_context ctx;
                    const char* c_caseID = ctx.marshal_as<const char*>(parts[0]);
                    const char* c_crimName = ctx.marshal_as<const char*>(parts[1]);
                    const char* c_reason = ctx.marshal_as<const char*>(closingReason);
                    const char* c_date = ctx.marshal_as<const char*>(date);

                    CrimeManagementSystem cms;
                    cms.archiveCaseDirectly(c_caseID, c_crimName, "Unknown Crime", c_reason, c_date);

                    casesList[idx] = parts[0] + "|" + parts[1] + "|Closed|" + parts[3];
                    SaveCases();

                    // Update corresponding FIR in CrimeReport
                    CrimeReport** reports = nullptr;
                    int count = 0; int capacity = 0;
                    DataManager::getInstance().loadReports(reports, count, capacity);
                    
                    for (int i = 0; i < count; i++) {
                        String^ cType = gcnew String(reports[i]->getCrimeType());
                        String^ desc = gcnew String(reports[i]->getDescription());
                        String^ accused = "UNKNOWN";
                        int aIdx = desc->IndexOf("Accused: ");
                        if (aIdx != -1) {
                            int endIdx = desc->IndexOf(")", aIdx);
                            if (endIdx != -1) accused = desc->Substring(aIdx + 9, endIdx - (aIdx + 9));
                        }
                        String^ matchString = cType + " - " + accused;
                        if (matchString == parts[1]) {
                            reports[i]->setStatus("Closed");
                            
                            msclr::interop::marshal_context updateCtx;
                            const char* c_upDate = updateCtx.marshal_as<const char*>(date);
                            reports[i]->setLastUpdated(c_upDate);
                        }
                    }
                    DataManager::getInstance().saveReports(reports, count);
                    for(int i=0; i<count; i++) delete reports[i];
                    delete[] reports;

                    ShowCases();
                    MessageBox::Show("Case has been closed and successfully archived.", "Case Closed", MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
            }
        }

        ListBox^ lbArchivedSuspects;
        ListBox^ lbArchivedCases;
        ListBox^ lbSessionLogs;

        void ShowMasterArchives() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "MASTER ARCHIVES";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            // Suspect Archives
            Label^ lSus = gcnew Label();
            lSus->Text = "Archived Suspects:";
            lSus->Font = gcnew Drawing::Font("Segoe UI", 16, FontStyle::Bold);
            lSus->ForeColor = Color::White;
            lSus->AutoSize = true;
            lSus->Location = Point(40, 90);
            contentPanel->Controls->Add(lSus);

            lbArchivedSuspects = gcnew ListBox();
            lbArchivedSuspects->Size = Drawing::Size(350, 400);
            lbArchivedSuspects->Location = Point(40, 130);
            lbArchivedSuspects->BackColor = Color::FromArgb(30, 41, 59);
            lbArchivedSuspects->ForeColor = Color::White;
            lbArchivedSuspects->Font = gcnew Drawing::Font("Segoe UI", 12);
            contentPanel->Controls->Add(lbArchivedSuspects);

            // Case Archives
            Label^ lCase = gcnew Label();
            lCase->Text = "Archived Cases:";
            lCase->Font = gcnew Drawing::Font("Segoe UI", 16, FontStyle::Bold);
            lCase->ForeColor = Color::White;
            lCase->AutoSize = true;
            lCase->Location = Point(420, 90);
            contentPanel->Controls->Add(lCase);

            lbArchivedCases = gcnew ListBox();
            lbArchivedCases->Size = Drawing::Size(350, 400);
            lbArchivedCases->Location = Point(420, 130);
            lbArchivedCases->BackColor = Color::FromArgb(30, 41, 59);
            lbArchivedCases->ForeColor = Color::White;
            lbArchivedCases->Font = gcnew Drawing::Font("Segoe UI", 12);
            contentPanel->Controls->Add(lbArchivedCases);

            // Session Logs
            Label^ lSess = gcnew Label();
            lSess->Text = "Session Logs:";
            lSess->Font = gcnew Drawing::Font("Segoe UI", 16, FontStyle::Bold);
            lSess->ForeColor = Color::White;
            lSess->AutoSize = true;
            lSess->Location = Point(800, 90);
            contentPanel->Controls->Add(lSess);

            lbSessionLogs = gcnew ListBox();
            lbSessionLogs->Size = Drawing::Size(350, 400);
            lbSessionLogs->Location = Point(800, 130);
            lbSessionLogs->BackColor = Color::FromArgb(30, 41, 59);
            lbSessionLogs->ForeColor = Color::White;
            lbSessionLogs->Font = gcnew Drawing::Font("Segoe UI", 12);
            contentPanel->Controls->Add(lbSessionLogs);

            RefreshMasterArchives();
        }

        void RefreshMasterArchives() {
            if (lbArchivedSuspects == nullptr || lbArchivedCases == nullptr || lbSessionLogs == nullptr) return;

            lbArchivedSuspects->Items->Clear();
            lbArchivedCases->Items->Clear();
            lbSessionLogs->Items->Clear();

            CrimeManagementSystem crimeSystem;

            // 1. Load Archived Suspects
            for (int i = 0; i < crimeSystem.getArchivedSuspectCount(); i++) {
                String^ entry = gcnew String(crimeSystem.getArchivedSuspects()[i]->name.c_str()) + " | Reason: " + 
                              gcnew String(crimeSystem.getArchivedSuspects()[i]->reasonForRemoval.c_str());
                lbArchivedSuspects->Items->Add(entry);
            }

            // 2. Load Archived Cases
            for (int i = 0; i < crimeSystem.getArchivedCaseCount(); i++) {
                String^ entry = "Case: " + gcnew String(crimeSystem.getArchivedCases()[i]->caseID.c_str()) + " | Closed due to: " + 
                              gcnew String(crimeSystem.getArchivedCases()[i]->closingReason.c_str());
                lbArchivedCases->Items->Add(entry);
            }

            // 3. Load Session Logs
            for (int i = 0; i < crimeSystem.getSessionCount(); i++) {
                String^ entry = "[" + gcnew String(crimeSystem.getSessionLogs()[i]->loginTime.c_str()) + "] " +
                                   gcnew String(crimeSystem.getSessionLogs()[i]->username.c_str()) + 
                                   " logged in.";
                lbSessionLogs->Items->Add(entry);
            }
        }


        void SaveCases() {
            String^ cPath = System::IO::Path::Combine(Application::StartupPath, "cases_db.txt");
            System::IO::File::WriteAllLines(cPath, casesList->ToArray());
        }

        void ShowReports() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "INCIDENT REPORTS";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            CrimeReport** reports = nullptr;
            int count = 0; int capacity = 0;
            DataManager::getInstance().loadReports(reports, count, capacity);

            int y = 100;
            for (int i = 0; i < count; i++) {
                Panel^ p = gcnew Panel();
                p->Size = Drawing::Size(1200, 80);
                p->Location = Point(40, y);
                p->BackColor = Color::FromArgb(30, 41, 59);

                String^ status = gcnew String(reports[i]->getStatus());

                Label^ l = gcnew Label();
                l->Text = "REP-" + reports[i]->getReportID().ToString("D3") + ": [" + 
                          gcnew String(reports[i]->getCrimeType()) + "] at " + 
                          gcnew String(reports[i]->getLocation()) + "\n" +
                          gcnew String(reports[i]->getDescription());
                l->Font = gcnew Drawing::Font("Segoe UI", 12);
                l->ForeColor = Color::White;
                l->Location = Point(20, 15);
                l->AutoSize = true;
                p->Controls->Add(l);
                
                Label^ lStatus = gcnew Label();
                lStatus->Text = "Status: " + status;
                lStatus->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
                lStatus->ForeColor = (status == "Pending") ? Color::FromArgb(245, 158, 11) : Color::FromArgb(34, 197, 94);
                lStatus->Location = Point(800, 30);
                lStatus->AutoSize = true;
                p->Controls->Add(lStatus);

                if (status == "Pending") {
                    Button^ btnConvert = gcnew Button();
                    btnConvert->Text = "CONVERT TO CASE";
                    btnConvert->Tag = reports[i]->getReportID();
                    btnConvert->Size = Drawing::Size(180, 35);
                    btnConvert->Location = Point(980, 22);
                    btnConvert->BackColor = Color::FromArgb(37, 99, 235);
                    btnConvert->ForeColor = Color::White;
                    btnConvert->FlatStyle = FlatStyle::Flat;
                    btnConvert->Click += gcnew EventHandler(this, &AdminDashboard::ConvertReport_Click);
                    p->Controls->Add(btnConvert);
                }

                contentPanel->Controls->Add(p);
                y += 100;
            }

            for(int i=0; i<count; i++) delete reports[i];
            delete[] reports;
        }

        void ConvertReport_Click(Object^ sender, EventArgs^ e) {
            Button^ btn = (Button^)sender;
            int rId = (int)btn->Tag;

            CrimeReport** reports = nullptr;
            int count = 0; int capacity = 0;
            DataManager::getInstance().loadReports(reports, count, capacity);

            for (int i = 0; i < count; i++) {
                if (reports[i]->getReportID() == rId) {
                    reports[i]->setStatus("Under Investigation");
                    
                    int caseIdNum = casesList->Count + 1;
                    String^ cType = gcnew String(reports[i]->getCrimeType());
                    
                    String^ desc = gcnew String(reports[i]->getDescription());
                    String^ accused = "UNKNOWN";
                    int idx = desc->IndexOf("Accused: ");
                    if (idx != -1) {
                        int endIdx = desc->IndexOf(")", idx);
                        if (endIdx != -1) accused = desc->Substring(idx + 9, endIdx - (idx + 9));
                    }

                    String^ newCase = "CR-2026-" + caseIdNum.ToString("D3") + "|" + cType + " - " + accused + "|Under Investigation|Unassigned";
                    casesList->Add(newCase);
                    SaveCases();
                    break;
                }
            }

            DataManager::getInstance().saveReports(reports, count);

            for(int i=0; i<count; i++) delete reports[i];
            delete[] reports;

            MessageBox::Show("Report successfully converted to active case.", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            ShowReports();
        }

        int GetMostWantedCount() {
            int count = 0;
            for each (SuspectData ^ s in suspects) {
                if (s->status == "Most Wanted") count++;
            }
            return count;
        }

        void ShowEvidence() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "CRYPTO EVIDENCE VERIFICATION";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            Label^ lblEvSelector = gcnew Label();
            lblEvSelector->Text = "Select Evidence File:";
            lblEvSelector->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblEvSelector->ForeColor = Color::White;
            lblEvSelector->AutoSize = true;
            lblEvSelector->Location = Point(40, 90);
            contentPanel->Controls->Add(lblEvSelector);

            ComboBox^ cbEvidenceSelector = gcnew ComboBox();
            cbEvidenceSelector->Name = "cbEvidenceSelector";
            cbEvidenceSelector->Location = Point(40, 120);
            cbEvidenceSelector->Size = Drawing::Size(400, 35);
            cbEvidenceSelector->Font = gcnew Drawing::Font("Segoe UI", 12);
            cbEvidenceSelector->DropDownStyle = ComboBoxStyle::DropDownList;
            
            CrimeManagementSystem crimeSystem;
            for (int i = 0; i < crimeSystem.getEvidenceCount(); i++) {
                String^ evID = crimeSystem.getEvidenceLog()[i]->getEvidenceID().ToString();
                String^ suspect = gcnew String(crimeSystem.getEvidenceLog()[i]->getLinkedSuspect().c_str());
                cbEvidenceSelector->Items->Add("EVD-" + evID + " | Suspect: " + suspect);
            }
            if (cbEvidenceSelector->Items->Count > 0) cbEvidenceSelector->SelectedIndex = 0;
            contentPanel->Controls->Add(cbEvidenceSelector);

            Label^ lblEvData = gcnew Label();
            lblEvData->Text = "Paste Evidence Description to Audit:";
            lblEvData->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblEvData->ForeColor = Color::White;
            lblEvData->AutoSize = true;
            lblEvData->Location = Point(40, 160);
            contentPanel->Controls->Add(lblEvData);

            TextBox^ tbEvidenceData = gcnew TextBox();
            tbEvidenceData->Name = "tbEvidenceData";
            tbEvidenceData->Location = Point(40, 190);
            tbEvidenceData->Size = Drawing::Size(600, 150);
            tbEvidenceData->Multiline = true;
            tbEvidenceData->Font = gcnew Drawing::Font("Segoe UI", 12);
            tbEvidenceData->BackColor = Color::FromArgb(30, 41, 59);
            tbEvidenceData->ForeColor = Color::White;
            contentPanel->Controls->Add(tbEvidenceData);

            Button^ btnVerifyEvidence = gcnew Button();
            btnVerifyEvidence->Text = "VERIFY INTEGRITY";
            btnVerifyEvidence->Size = Drawing::Size(200, 40);
            btnVerifyEvidence->Location = Point(40, 360);
            btnVerifyEvidence->BackColor = Color::FromArgb(245, 158, 11);
            btnVerifyEvidence->ForeColor = Color::White;
            btnVerifyEvidence->FlatStyle = FlatStyle::Flat;
            btnVerifyEvidence->Click += gcnew EventHandler(this, &AdminDashboard::btnVerifyEvidence_Click);
            contentPanel->Controls->Add(btnVerifyEvidence);

            Label^ lblGeneratedHash = gcnew Label();
            lblGeneratedHash->Name = "lblGeneratedHash";
            lblGeneratedHash->Text = "Generated FNV-1a Hash: [Waiting...]";
            lblGeneratedHash->Font = gcnew Drawing::Font("Consolas", 12);
            lblGeneratedHash->ForeColor = Color::White;
            lblGeneratedHash->AutoSize = true;
            lblGeneratedHash->Location = Point(40, 420);
            contentPanel->Controls->Add(lblGeneratedHash);

            Label^ lblStoredHashDisplay = gcnew Label();
            lblStoredHashDisplay->Name = "lblStoredHashDisplay";
            lblStoredHashDisplay->Text = "Database Hash Record: [Waiting...]";
            lblStoredHashDisplay->Font = gcnew Drawing::Font("Consolas", 12);
            lblStoredHashDisplay->ForeColor = Color::White;
            lblStoredHashDisplay->AutoSize = true;
            lblStoredHashDisplay->Location = Point(40, 450);
            contentPanel->Controls->Add(lblStoredHashDisplay);

            Label^ lblVerificationStatus = gcnew Label();
            lblVerificationStatus->Name = "lblVerificationStatus";
            lblVerificationStatus->Text = "Verification Status: [Waiting...]";
            lblVerificationStatus->Font = gcnew Drawing::Font("Segoe UI", 14, FontStyle::Bold);
            lblVerificationStatus->ForeColor = Color::White;
            lblVerificationStatus->AutoSize = true;
            lblVerificationStatus->Location = Point(40, 490);
            contentPanel->Controls->Add(lblVerificationStatus);
        }

        void btnVerifyEvidence_Click(Object^ sender, EventArgs^ e) {
            ComboBox^ cbEvidenceSelector = (ComboBox^)contentPanel->Controls->Find("cbEvidenceSelector", false)[0];
            TextBox^ tbEvidenceData = (TextBox^)contentPanel->Controls->Find("tbEvidenceData", false)[0];
            Label^ lblGeneratedHash = (Label^)contentPanel->Controls->Find("lblGeneratedHash", false)[0];
            Label^ lblStoredHashDisplay = (Label^)contentPanel->Controls->Find("lblStoredHashDisplay", false)[0];
            Label^ lblVerificationStatus = (Label^)contentPanel->Controls->Find("lblVerificationStatus", false)[0];

            if (cbEvidenceSelector->SelectedIndex == -1) {
                MessageBox::Show("Please select an Evidence File from the dropdown to audit.");
                return;
            }

            String^ evidenceData = tbEvidenceData->Text->Trim();
            if (evidenceData == "") {
                MessageBox::Show("Please paste the evidence text you want to verify.");
                return;
            }

            CrimeManagementSystem crimeSystem;
            int selectedIndex = cbEvidenceSelector->SelectedIndex;
            unsigned int storedHash = crimeSystem.getEvidenceLog()[selectedIndex]->getSecureHash();

            std::string c_evidenceData = msclr::interop::marshal_as<std::string>(evidenceData);
            unsigned int generatedHash = AdvancedFeatures::CryptoEvidence::calculateFNV1a(c_evidenceData);

            lblGeneratedHash->Text = "Generated FNV-1a Hash: " + generatedHash.ToString();
            lblStoredHashDisplay->Text = "Database Hash Record: " + storedHash.ToString();

            if (generatedHash == storedHash) {
                lblVerificationStatus->Text = "Verification Status: VERIFIED (SECURE)";
                lblVerificationStatus->ForeColor = Color::LimeGreen;
            } else {
                lblVerificationStatus->Text = "Verification Status: TAMPERED (CRITICAL ALERT)";
                lblVerificationStatus->ForeColor = Color::Red;
                
                MessageBox::Show("CRITICAL ALERT: The generated hash does not match the database record. The evidence text has been secretly modified!", 
                                 "Tamper Warning", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void ShowAnonReports() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "ANONYMOUS TIPS";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            int y = 90;
            for (int i = 0; i < anonReports->Count; i++) {
                String^ repLine = anonReports[i];
                array<String^>^ parts = repLine->Split('|');
                if (parts->Length < 4) continue;

                Panel^ rPanel = gcnew Panel();
                rPanel->Size = Drawing::Size(1000, 80);
                rPanel->Location = Point(40, y);
                rPanel->BackColor = Color::FromArgb(30, 41, 59);

                Label^ lRep = gcnew Label();
                lRep->Text = parts[0] + " | " + parts[3] + "\n" + parts[1];
                lRep->Font = gcnew Drawing::Font("Segoe UI", 12);
                lRep->ForeColor = Color::White;
                lRep->AutoSize = true;
                lRep->Location = Point(20, 15);
                rPanel->Controls->Add(lRep);

                Label^ lStat = gcnew Label();
                lStat->Text = "Status: " + parts[2];
                lStat->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
                lStat->ForeColor = parts[2] == "Pending" ? Color::FromArgb(220, 38, 38) : Color::FromArgb(34, 197, 94);
                lStat->AutoSize = true;
                lStat->Location = Point(700, 30);
                rPanel->Controls->Add(lStat);

                if (parts[2] == "Pending") {
                    Button^ btnRev = gcnew Button();
                    btnRev->Text = "MARK REVIEWED";
                    btnRev->Tag = i;
                    btnRev->Size = Drawing::Size(150, 30);
                    btnRev->Location = Point(830, 25);
                    btnRev->BackColor = Color::FromArgb(37, 99, 235);
                    btnRev->ForeColor = Color::White;
                    btnRev->FlatStyle = FlatStyle::Flat;
                    btnRev->Click += gcnew EventHandler(this, &AdminDashboard::ReviewAnonReport_Click);
                    rPanel->Controls->Add(btnRev);
                }

                contentPanel->Controls->Add(rPanel);
                y += 90;
            }
        }

        void ReviewAnonReport_Click(Object^ sender, EventArgs^ e) {
            Button^ btn = (Button^)sender;
            int idx = (int)btn->Tag;
            
            array<String^>^ parts = anonReports[idx]->Split('|');
            if (parts->Length >= 4) {
                anonReports[idx] = parts[0] + "|" + parts[1] + "|Reviewed|" + parts[3];
                String^ arPath = System::IO::Path::Combine(Application::StartupPath, "anon_reports_db.txt");
                System::IO::File::WriteAllLines(arPath, anonReports->ToArray());
                ShowAnonReports();
            }
        }

        void ShowUserManagement() {
            contentPanel->Controls->Clear();

            Label^ title = gcnew Label();
            title->Text = "USER MANAGEMENT";
            title->Font = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            // Add new admin form
            Label^ lblAdd = gcnew Label();
            lblAdd->Text = "ADD NEW ADMIN";
            lblAdd->Font = gcnew Drawing::Font("Segoe UI", 18, FontStyle::Bold);
            lblAdd->ForeColor = Color::FromArgb(37, 99, 235);
            lblAdd->AutoSize = true;
            lblAdd->Location = Point(40, 90);
            contentPanel->Controls->Add(lblAdd);

            TextBox^ txtUser = gcnew TextBox();
            txtUser->Name = "txtNewUser";
            txtUser->Size = Drawing::Size(200, 30);
            txtUser->Location = Point(40, 130);
            txtUser->Font = gcnew Drawing::Font("Segoe UI", 12);
            contentPanel->Controls->Add(txtUser);

            TextBox^ txtPass = gcnew TextBox();
            txtPass->Name = "txtNewPass";
            txtPass->Size = Drawing::Size(200, 30);
            txtPass->Location = Point(250, 130);
            txtPass->Font = gcnew Drawing::Font("Segoe UI", 12);
            contentPanel->Controls->Add(txtPass);

            ComboBox^ cmbRole = gcnew ComboBox();
            cmbRole->Name = "cmbNewRole";
            cmbRole->Items->Add("Admin");
            cmbRole->Items->Add("User");
            cmbRole->SelectedIndex = 0;
            cmbRole->Size = Drawing::Size(200, 30);
            cmbRole->Location = Point(460, 130);
            cmbRole->Font = gcnew Drawing::Font("Segoe UI", 12);
            cmbRole->DropDownStyle = ComboBoxStyle::DropDownList;
            contentPanel->Controls->Add(cmbRole);

            Button^ btnAdd = gcnew Button();
            btnAdd->Text = "ADD ADMIN";
            btnAdd->Size = Drawing::Size(150, 32);
            btnAdd->Location = Point(670, 129);
            btnAdd->BackColor = Color::FromArgb(34, 197, 94);
            btnAdd->ForeColor = Color::White;
            btnAdd->FlatStyle = FlatStyle::Flat;
            btnAdd->Click += gcnew EventHandler(this, &AdminDashboard::AddAdmin_Click);
            contentPanel->Controls->Add(btnAdd);

            Button^ btnAddOfficer = gcnew Button();
            btnAddOfficer->Text = "RECRUIT OFFICER";
            btnAddOfficer->Size = Drawing::Size(180, 32);
            btnAddOfficer->Location = Point(830, 129);
            btnAddOfficer->BackColor = Color::FromArgb(37, 99, 235);
            btnAddOfficer->ForeColor = Color::White;
            btnAddOfficer->FlatStyle = FlatStyle::Flat;
            btnAddOfficer->Click += gcnew EventHandler(this, &AdminDashboard::btnAddOfficer_Click);
            contentPanel->Controls->Add(btnAddOfficer);

            // List users
            Label^ lblList = gcnew Label();
            lblList->Text = "SYSTEM ADMINISTRATORS";
            lblList->Font = gcnew Drawing::Font("Segoe UI", 18, FontStyle::Bold);
            lblList->ForeColor = Color::White;
            lblList->AutoSize = true;
            lblList->Location = Point(40, 180);
            contentPanel->Controls->Add(lblList);

            int y = 230;
            
            // Render Admins
            Admin** admins = nullptr;
            int adCount = 0; int adCap = 0;
            DataManager::getInstance().loadAdmins(admins, adCount, adCap);
            for (int i = 0; i < adCount; i++) {
                Panel^ uPanel = gcnew Panel();
                uPanel->Size = Drawing::Size(800, 50);
                uPanel->Location = Point(40, y);
                uPanel->BackColor = Color::FromArgb(30, 41, 59);

                Label^ lUser = gcnew Label();
                lUser->Text = gcnew String(admins[i]->getUsername()) + " (Admin)";
                lUser->Font = gcnew Drawing::Font("Segoe UI", 12);
                lUser->ForeColor = Color::White;
                lUser->AutoSize = true;
                lUser->Location = Point(20, 12);
                uPanel->Controls->Add(lUser);

                // Cannot delete main admin safely from simple UI
                y += 60;
            }
            if (admins) { for(int i=0; i<adCount; i++) delete admins[i]; delete[] admins; }
            
            Label^ lblListUsers = gcnew Label();
            lblListUsers->Text = "STANDARD USERS";
            lblListUsers->Font = gcnew Drawing::Font("Segoe UI", 18, FontStyle::Bold);
            lblListUsers->ForeColor = Color::White;
            lblListUsers->AutoSize = true;
            lblListUsers->Location = Point(40, y + 20);
            contentPanel->Controls->Add(lblListUsers);
            y += 70;

            User** users = nullptr;
            int uCount = 0; int uCap = 0;
            DataManager::getInstance().loadUsers(users, uCount, uCap);
            for (int i = 0; i < uCount; i++) {
                Panel^ uPanel = gcnew Panel();
                uPanel->Size = Drawing::Size(800, 50);
                uPanel->Location = Point(40, y);
                uPanel->BackColor = Color::FromArgb(30, 41, 59);

                Label^ lUser = gcnew Label();
                lUser->Text = gcnew String(users[i]->getUsername()) + " (User)";
                lUser->Font = gcnew Drawing::Font("Segoe UI", 12);
                lUser->ForeColor = Color::White;
                lUser->AutoSize = true;
                lUser->Location = Point(20, 12);
                uPanel->Controls->Add(lUser);

                Button^ btnDel = gcnew Button();
                btnDel->Text = "DELETE";
                btnDel->Tag = i;
                btnDel->Size = Drawing::Size(100, 30);
                btnDel->Location = Point(680, 10);
                btnDel->BackColor = Color::FromArgb(220, 38, 38);
                btnDel->ForeColor = Color::White;
                btnDel->FlatStyle = FlatStyle::Flat;
                btnDel->Click += gcnew EventHandler(this, &AdminDashboard::DeleteAdmin_Click);
                uPanel->Controls->Add(btnDel);

                contentPanel->Controls->Add(uPanel);
                y += 60;
            }
        }

        void AddAdmin_Click(Object^ sender, EventArgs^ e) {
            TextBox^ tUser = (TextBox^)contentPanel->Controls->Find("txtNewUser", false)[0];
            TextBox^ tPass = (TextBox^)contentPanel->Controls->Find("txtNewPass", false)[0];
            ComboBox^ cRole = (ComboBox^)contentPanel->Controls->Find("cmbNewRole", false)[0];

            if (tUser->Text->Trim() == "" || tPass->Text->Trim() == "") {
                MessageBox::Show("Please fill all fields.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            CrimeManagementSystem cms;
            if (cRole->SelectedItem->ToString() == "Admin") {
                cms.addNewAdmin(
                    msclr::interop::marshal_as<std::string>(tUser->Text).c_str(),
                    msclr::interop::marshal_as<std::string>(tPass->Text).c_str(),
                    msclr::interop::marshal_as<std::string>(tUser->Text).c_str(),
                    "BADGE-000", "HQ", "Q", "A"
                );
            } else {
                cms.userRegistration(
                    msclr::interop::marshal_as<std::string>(tUser->Text).c_str(),
                    msclr::interop::marshal_as<std::string>(tPass->Text).c_str(),
                    msclr::interop::marshal_as<std::string>(tUser->Text).c_str(),
                    "555-0000", "user@crms.gov", "Q", "A"
                );
            }

            MessageBox::Show("Account created.", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            ShowUserManagement();
        }

        void btnAddOfficer_Click(Object^ sender, EventArgs^ e) {
            AddOfficerForm^ popup = gcnew AddOfficerForm();
            if (popup->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                String^ newName = popup->OfficerName;
                String^ newRank = popup->OfficerRank;
                int newAge = popup->OfficerAge;

                msclr::interop::marshal_context ctx;
                const char* c_name = ctx.marshal_as<const char*>(newName);
                const char* c_rank = ctx.marshal_as<const char*>(newRank);

                CrimeManagementSystem cms;
                cms.addNewOfficer(c_name, newAge, c_rank);

                MessageBox::Show("New Officer successfully added to the roster!", "System Alert", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void DeleteAdmin_Click(Object^ sender, EventArgs^ e) {
            Button^ btn = (Button^)sender;
            int idx = (int)btn->Tag;
            
            User** users = nullptr;
            int count = 0; int capacity = 0;
            DataManager::getInstance().loadUsers(users, count, capacity);
            
            if (idx >= 0 && idx < count) {
                String^ delUser = gcnew String(users[idx]->getUsername());
                
                // Shift array left
                for (int i = idx; i < count - 1; i++) {
                    users[i] = users[i + 1];
                }
                count--;
                
                DataManager::getInstance().saveUsers(users, count);
                MessageBox::Show("Standard user '" + delUser + "' deleted successfully.", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            
            for(int i=0; i<count; i++) delete users[i];
            delete[] users;
            
            ShowUserManagement();
        }

        void SaveAdmins() {
            // Deprecated mock logic
        }

        void Logout_Click(Object^ sender, EventArgs^ e) {
            CrimeManagementSystem cms;
            const char* user = SessionManager::getInstance().getCurrentUser();
            if (user && strlen(user) > 0) {
                cms.recordSession(user, "Admin", "logout");
                DataManager::getInstance().logAudit("LOGOUT", "Admin logged out: " + std::string(user));
            }
            SessionManager::getInstance().logout();

            System::Type^ formType = mainForm->GetType();
            System::Reflection::MethodInfo^ showLoginMethod = formType->GetMethod("ShowLogin");
            if (showLoginMethod != nullptr) {
                showLoginMethod->Invoke(mainForm, nullptr);
            }
        }
    };
}
