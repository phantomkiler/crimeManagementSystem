#pragma once
#include "UIHelper.h"
#include "WantedPosterCard.h"
#define WIN32_LEAN_AND_MEAN
#pragma managed(push, off)
#include "AdvancedFeatures.h"
#pragma managed(pop)
#include <msclr/marshal_cppstd.h>
#include <vector>

using namespace System::Windows::Forms;
using namespace System;
using namespace System::Drawing;

namespace CrimeManagementGUI {

    public ref class AdvancedFeaturesDashboard : public UserControl {
    private:
        Form^ mainForm;
        Panel^ headerPanel;
        Panel^ menuPanel;
        Panel^ contentPanel;
        
        // Dynamic Controls for Syndicate Map
        ComboBox^ cbTargetSuspect;
        TreeView^ tvSyndicate;

        // Dynamic Controls for Predictive Mapping
        ComboBox^ cbCrimeFilter;
        ListBox^ lbPredictiveResults;

        // Dynamic Controls for Biometrics
        TextBox^ tbDatabaseDNA;
        TextBox^ tbSceneDNA;
        Label^ lblDistanceResult;
        Label^ lblPercentResult;

        // Dynamic Controls for Threat Triage
        TextBox^ tbThreatTip;
        Label^ lblThreatScore;

        // Dynamic Controls for Crypto Evidence
        ComboBox^ cbCryptoData;
        Label^ lblCryptoHash;
        Label^ lblCryptoStatus;

    public:
        AdvancedFeaturesDashboard(Form^ parent) {
            mainForm = parent;
            InitializeComponent();
        }

    private:
        void InitializeComponent() {
            this->Size = Drawing::Size(1280, 720);
            this->BackColor = Color::FromArgb(11, 17, 27);
            this->DoubleBuffered = true;

            CreateHeader();
            CreateMenu();
            
            contentPanel = gcnew Panel();
            contentPanel->Location = Point(250, 80);
            contentPanel->Size = Drawing::Size(1030, 640);
            contentPanel->Anchor = AnchorStyles::Top | AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right;
            contentPanel->AutoScroll = true;
            this->Controls->Add(contentPanel);

            ShowIntelMap();
        }

        void CreateHeader() {
            headerPanel = gcnew Panel();
            headerPanel->Size = Drawing::Size(this->Width, 80);
            headerPanel->Location = Point(0, 0);
            headerPanel->Anchor = AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right;
            headerPanel->BackColor = Color::FromArgb(18, 26, 40);

            Label^ title = gcnew Label();
            title->Text = "ADVANCED INTEL SYSTEM";
            title->Font = gcnew Drawing::Font("Impact", 28, FontStyle::Bold);
            title->ForeColor = Color::FromArgb(220, 38, 38);
            title->AutoSize = true;
            title->Location = Point(30, 20);
            headerPanel->Controls->Add(title);

            Button^ btnBack = gcnew Button();
            btnBack->Text = "← BACK TO COMMAND CENTER";
            btnBack->Size = Drawing::Size(250, 40);
            btnBack->Location = Point(this->Width - 280, 20);
            btnBack->Anchor = AnchorStyles::Top | AnchorStyles::Right;
            btnBack->FlatStyle = FlatStyle::Flat;
            btnBack->BackColor = Color::Transparent;
            btnBack->ForeColor = Color::FromArgb(156, 163, 175);
            btnBack->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
            btnBack->Cursor = Cursors::Hand;
            btnBack->Click += gcnew EventHandler(this, &AdvancedFeaturesDashboard::Back_Click);
            headerPanel->Controls->Add(btnBack);

            this->Controls->Add(headerPanel);
        }

        void CreateMenu() {
            menuPanel = gcnew Panel();
            menuPanel->Size = Drawing::Size(250, this->Height - 80);
            menuPanel->Location = Point(0, 80);
            menuPanel->Anchor = AnchorStyles::Top | AnchorStyles::Bottom | AnchorStyles::Left;
            menuPanel->BackColor = Color::FromArgb(15, 23, 42);

            array<String^>^ menus = { "Syndicate Map", "Predictive Mapping", "Threat Triage", "Crypto Evidence", "Biometrics" };
            int y = 20;
            for (int i = 0; i < menus->Length; i++) {
                Button^ btn = gcnew Button();
                btn->Text = menus[i];
                btn->Tag = menus[i];
                btn->Size = Drawing::Size(210, 50);
                btn->Location = Point(20, y);
                btn->FlatStyle = FlatStyle::Flat;
                btn->FlatAppearance->BorderSize = 0;
                btn->ForeColor = Color::White;
                btn->BackColor = Color::FromArgb(30, 41, 59);
                btn->Font = gcnew Drawing::Font("Segoe UI", 11, FontStyle::Bold);
                btn->Cursor = Cursors::Hand;
                btn->Click += gcnew EventHandler(this, &AdvancedFeaturesDashboard::Menu_Click);
                menuPanel->Controls->Add(btn);
                y += 60;
            }

            this->Controls->Add(menuPanel);
        }

        void Menu_Click(Object^ sender, EventArgs^ e) {
            Button^ btn = (Button^)sender;
            String^ tag = (String^)btn->Tag;
            if (tag == "Syndicate Map") ShowIntelMap();
            else if (tag == "Predictive Mapping") ShowPredictiveMapping();
            else if (tag == "Threat Triage") ShowThreatTriage();
            else if (tag == "Crypto Evidence") ShowCryptoEvidence();
            else if (tag == "Biometrics") ShowBiometrics();
        }

        void ShowIntelMap() {
            contentPanel->Controls->Clear();
            Label^ title = gcnew Label();
            title->Text = "GRAPH-BASED SYNDICATE MAPPING";
            title->Font = gcnew Drawing::Font("Impact", 24);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            Label^ lblInst = gcnew Label();
            lblInst->Text = "Select Primary Target:";
            lblInst->ForeColor = Color::White;
            lblInst->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblInst->AutoSize = true;
            lblInst->Location = Point(40, 90);
            contentPanel->Controls->Add(lblInst);

            cbTargetSuspect = gcnew ComboBox();
            cbTargetSuspect->Location = Point(40, 120);
            cbTargetSuspect->Size = Drawing::Size(300, 35);
            cbTargetSuspect->Font = gcnew Drawing::Font("Segoe UI", 14);
            cbTargetSuspect->DropDownStyle = ComboBoxStyle::DropDownList;
            
            System::Collections::Generic::List<SuspectData^>^ suspects = SuspectData::LoadSuspects();
            for each (SuspectData^ s in suspects) {
                cbTargetSuspect->Items->Add(s->name);
            }

            if (cbTargetSuspect->Items->Count > 0) cbTargetSuspect->SelectedIndex = 0;
            contentPanel->Controls->Add(cbTargetSuspect);

            Button^ btnAnalyze = gcnew Button();
            btnAnalyze->Text = "ANALYZE";
            btnAnalyze->Size = Drawing::Size(120, 35);
            btnAnalyze->Location = Point(360, 120);
            btnAnalyze->BackColor = Color::FromArgb(220, 38, 38);
            btnAnalyze->ForeColor = Color::White;
            btnAnalyze->FlatStyle = FlatStyle::Flat;
            btnAnalyze->Click += gcnew EventHandler(this, &AdvancedFeaturesDashboard::btnAnalyzeSyndicate_Click);
            contentPanel->Controls->Add(btnAnalyze);

            tvSyndicate = gcnew TreeView();
            tvSyndicate->Location = Point(40, 180);
            tvSyndicate->Size = Drawing::Size(800, 400);
            tvSyndicate->BackColor = Color::FromArgb(30, 41, 59);
            tvSyndicate->ForeColor = Color::White;
            tvSyndicate->Font = gcnew Drawing::Font("Consolas", 14);
            contentPanel->Controls->Add(tvSyndicate);
        }

        void btnAnalyzeSyndicate_Click(Object^ sender, EventArgs^ e) {
            tvSyndicate->Nodes->Clear();

            if (cbTargetSuspect->SelectedIndex == -1) {
                MessageBox::Show("Please select a Target Suspect.");
                return;
            }
            
            String^ target = cbTargetSuspect->SelectedItem->ToString();
            std::string startNode = msclr::interop::marshal_as<std::string>(target);

            TreeNode^ rootNode = gcnew TreeNode(target + " [PRIMARY TARGET]");
            rootNode->ForeColor = Color::Red;
            rootNode->NodeFont = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);

            if (syndicateNetwork.find(startNode) != syndicateNetwork.end()) {
                for (const std::string& associate : syndicateNetwork[startNode]) {
                    TreeNode^ deg1Node = gcnew TreeNode(gcnew String(associate.c_str()));
                    rootNode->Nodes->Add(deg1Node);
                }
            }

            tvSyndicate->Nodes->Add(rootNode);
            tvSyndicate->ExpandAll();
        }

        void ShowPredictiveMapping() {
            contentPanel->Controls->Clear();
            Label^ title = gcnew Label();
            title->Text = "PREDICTIVE CRIME MAPPING";
            title->Font = gcnew Drawing::Font("Impact", 24);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            Label^ lblFilter = gcnew Label();
            lblFilter->Text = "Select Crime Filter:";
            lblFilter->ForeColor = Color::White;
            lblFilter->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblFilter->AutoSize = true;
            lblFilter->Location = Point(40, 90);
            contentPanel->Controls->Add(lblFilter);

            cbCrimeFilter = gcnew ComboBox();
            cbCrimeFilter->Location = Point(40, 120);
            cbCrimeFilter->Size = Drawing::Size(300, 35);
            cbCrimeFilter->Font = gcnew Drawing::Font("Segoe UI", 14);
            cbCrimeFilter->DropDownStyle = ComboBoxStyle::DropDownList;
            cbCrimeFilter->Items->Clear();
            cbCrimeFilter->Items->AddRange(gcnew array<String^> { "All Crimes", "Theft", "Assault", "Cybercrime" });
            if (cbCrimeFilter->Items->Count > 0) cbCrimeFilter->SelectedIndex = 0;
            contentPanel->Controls->Add(cbCrimeFilter);

            Button^ btnRunPrediction = gcnew Button();
            btnRunPrediction->Text = "RUN PREDICTION MODEL";
            btnRunPrediction->Size = Drawing::Size(220, 35);
            btnRunPrediction->Location = Point(360, 120);
            btnRunPrediction->BackColor = Color::FromArgb(245, 158, 11);
            btnRunPrediction->ForeColor = Color::White;
            btnRunPrediction->FlatStyle = FlatStyle::Flat;
            btnRunPrediction->Click += gcnew EventHandler(this, &AdvancedFeaturesDashboard::btnRunPrediction_Click);
            contentPanel->Controls->Add(btnRunPrediction);

            lbPredictiveResults = gcnew ListBox();
            lbPredictiveResults->Location = Point(40, 180);
            lbPredictiveResults->Size = Drawing::Size(800, 400);
            lbPredictiveResults->BackColor = Color::FromArgb(30, 41, 59);
            lbPredictiveResults->ForeColor = Color::FromArgb(245, 158, 11);
            lbPredictiveResults->Font = gcnew Drawing::Font("Consolas", 14);
            contentPanel->Controls->Add(lbPredictiveResults);
        }

        void btnRunPrediction_Click(Object^ sender, EventArgs^ e) {
            lbPredictiveResults->Items->Clear();
            
            if (cbCrimeFilter->SelectedIndex == -1) {
                MessageBox::Show("Please select a Crime Filter first.");
                return;
            }
            String^ filter = cbCrimeFilter->SelectedItem->ToString();

            int gulbergCount = 0; int dhaCount = 0; int modelTownCount = 0;
            int walledCityCount = 0; int canttCount = 0; int joharTownCount = 0;
            int cliftonCount = 0; int saddarCount = 0; int f8Count = 0; int bahriaCount = 0;
            int totalFilteredCases = 0;

            CrimeReport** reports = nullptr;
            int count = 0; int capacity = 0;
            DataManager::getInstance().loadReports(reports, count, capacity);

            for (int i = 0; i < count; i++) {
                String^ crimeType = gcnew String(reports[i]->getCrimeType());
                String^ loc = gcnew String(reports[i]->getLocation());

                if (filter == "All Crimes" || crimeType->Contains(filter)) {
                    totalFilteredCases++;
                    String^ locLower = loc->ToLower();
                    
                    if (locLower->Contains("gulberg")) gulbergCount++;
                    else if (locLower->Contains("dha") || locLower->Contains("defence")) dhaCount++;
                    else if (locLower->Contains("model town")) modelTownCount++;
                    else if (locLower->Contains("walled city") || locLower->Contains("androon")) walledCityCount++;
                    else if (locLower->Contains("cantt")) canttCount++;
                    else if (locLower->Contains("johar town")) joharTownCount++;
                    else if (locLower->Contains("clifton")) cliftonCount++;
                    else if (locLower->Contains("saddar")) saddarCount++;
                    else if (locLower->Contains("f8")) f8Count++;
                    else if (locLower->Contains("bahria")) bahriaCount++;
                }
            }

            for (int i = 0; i < count; i++) delete reports[i];
            delete[] reports;

            if (totalFilteredCases == 0) {
                lbPredictiveResults->Items->Add("No historical data found for " + filter + ".");
                return;
            }

            double gulbergProb = ((double)gulbergCount / totalFilteredCases) * 100;
            double dhaProb = ((double)dhaCount / totalFilteredCases) * 100;
            double modelTownProb = ((double)modelTownCount / totalFilteredCases) * 100;
            double walledCityProb = ((double)walledCityCount / totalFilteredCases) * 100;
            double canttProb = ((double)canttCount / totalFilteredCases) * 100;
            double joharTownProb = ((double)joharTownCount / totalFilteredCases) * 100;
            double cliftonProb = ((double)cliftonCount / totalFilteredCases) * 100;
            double saddarProb = ((double)saddarCount / totalFilteredCases) * 100;
            double f8Prob = ((double)f8Count / totalFilteredCases) * 100;
            double bahriaProb = ((double)bahriaCount / totalFilteredCases) * 100;

            lbPredictiveResults->Items->Add("=== PREDICTIVE MODEL: " + filter + " ===");
            lbPredictiveResults->Items->Add("Lahore - Gulberg:       " + gulbergProb.ToString("0.0") + "% Risk Factor");
            lbPredictiveResults->Items->Add("Lahore - DHA / Defence: " + dhaProb.ToString("0.0") + "% Risk Factor");
            lbPredictiveResults->Items->Add("Lahore - Model Town:    " + modelTownProb.ToString("0.0") + "% Risk Factor");
            lbPredictiveResults->Items->Add("Lahore - Walled City:   " + walledCityProb.ToString("0.0") + "% Risk Factor");
            lbPredictiveResults->Items->Add("Lahore - Cantt:         " + canttProb.ToString("0.0") + "% Risk Factor");
            lbPredictiveResults->Items->Add("Lahore - Johar Town:    " + joharTownProb.ToString("0.0") + "% Risk Factor");
            lbPredictiveResults->Items->Add("Karachi - Clifton:      " + cliftonProb.ToString("0.0") + "% Risk Factor");
            lbPredictiveResults->Items->Add("Karachi/Peshawar Saddar:" + saddarProb.ToString("0.0") + "% Risk Factor");
            lbPredictiveResults->Items->Add("Islamabad - F8:         " + f8Prob.ToString("0.0") + "% Risk Factor");
            lbPredictiveResults->Items->Add("Rawalpindi - Bahria:    " + bahriaProb.ToString("0.0") + "% Risk Factor");
        }

        ListBox^ lbThreatResults;

        void ShowThreatTriage() {
            contentPanel->Controls->Clear();
            Label^ title = gcnew Label();
            title->Text = "AUTOMATED THREAT TRIAGE (NLP)";
            title->Font = gcnew Drawing::Font("Impact", 24);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            Button^ btnAnalyzeTip = gcnew Button();
            btnAnalyzeTip->Text = "Analyze Threat Levels from Cases";
            btnAnalyzeTip->Size = Drawing::Size(300, 40);
            btnAnalyzeTip->Location = Point(40, 90);
            btnAnalyzeTip->BackColor = Color::FromArgb(220, 38, 38);
            btnAnalyzeTip->ForeColor = Color::White;
            btnAnalyzeTip->FlatStyle = FlatStyle::Flat;
            btnAnalyzeTip->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
            btnAnalyzeTip->Click += gcnew EventHandler(this, &AdvancedFeaturesDashboard::btnAnalyzeTip_Click);
            contentPanel->Controls->Add(btnAnalyzeTip);

            lbThreatResults = gcnew ListBox();
            lbThreatResults->Location = Point(40, 150);
            lbThreatResults->Size = Drawing::Size(800, 400);
            lbThreatResults->BackColor = Color::FromArgb(30, 41, 59);
            lbThreatResults->ForeColor = Color::FromArgb(245, 158, 11);
            lbThreatResults->Font = gcnew Drawing::Font("Consolas", 14);
            contentPanel->Controls->Add(lbThreatResults);
        }

        void btnAnalyzeTip_Click(Object^ sender, EventArgs^ e) {
            lbThreatResults->Items->Clear();

            CrimeReport** reports = nullptr;
            int count = 0; int capacity = 0;
            DataManager::getInstance().loadReports(reports, count, capacity);

            for (int i = 0; i < count; i++) {
                String^ crime = gcnew String(reports[i]->getCrimeType());
                String^ crimeLower = crime->ToLower();
                int threatScore = 0;

                if (crimeLower->Contains("murder") || crimeLower->Contains("homicide")) {
                    threatScore += 100;
                } else if (crimeLower->Contains("armed robbery") || crimeLower->Contains("kidnapping")) {
                    threatScore += 80;
                } else if (crimeLower->Contains("assault")) {
                    threatScore += 60;
                } else if (crimeLower->Contains("theft") || crimeLower->Contains("fraud")) {
                    threatScore += 30;
                } else {
                    threatScore += 10;
                }

                lbThreatResults->Items->Add("REP-" + reports[i]->getReportID().ToString("D3") + " | " + crime + " | Score: " + threatScore);
            }

            for (int i = 0; i < count; i++) delete reports[i];
            delete[] reports;
        }

        void ShowCryptoEvidence() {
            contentPanel->Controls->Clear();
            Label^ title = gcnew Label();
            title->Text = "CRYPTOGRAPHIC EVIDENCE CHAIN (FNV-1a)";
            title->Font = gcnew Drawing::Font("Impact", 24);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            Label^ lblInst = gcnew Label();
            lblInst->Text = "Select Evidence to Verify:";
            lblInst->ForeColor = Color::White;
            lblInst->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblInst->AutoSize = true;
            lblInst->Location = Point(60, 90);
            contentPanel->Controls->Add(lblInst);

            cbCryptoData = gcnew ComboBox();
            cbCryptoData->Size = Drawing::Size(600, 30);
            cbCryptoData->Location = Point(60, 120);
            cbCryptoData->Font = gcnew Drawing::Font("Segoe UI", 12);
            cbCryptoData->DropDownStyle = ComboBoxStyle::DropDownList;
            
            CrimeManagementSystem cms;
            Evidence** evLog = cms.getEvidenceLog();
            int evCount = cms.getEvidenceCount();
            for(int i = 0; i < evCount; i++) {
                String^ item = "EV-" + evLog[i]->getEvidenceID().ToString() + " | " + gcnew String(evLog[i]->getLinkedSuspect().c_str()) + " | " + gcnew String(evLog[i]->getDescription().c_str());
                cbCryptoData->Items->Add(item);
            }
            if(cbCryptoData->Items->Count > 0) cbCryptoData->SelectedIndex = 0;
            
            contentPanel->Controls->Add(cbCryptoData);

            Button^ btnHash = gcnew Button();
            btnHash->Text = "Generate Secure Hash";
            btnHash->Size = Drawing::Size(200, 40);
            btnHash->Location = Point(60, 170);
            btnHash->BackColor = Color::FromArgb(37, 99, 235);
            btnHash->ForeColor = Color::White;
            btnHash->FlatStyle = FlatStyle::Flat;
            btnHash->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
            btnHash->Click += gcnew EventHandler(this, &AdvancedFeaturesDashboard::btnHash_Click);
            contentPanel->Controls->Add(btnHash);

            lblCryptoHash = gcnew Label();
            lblCryptoHash->Text = "Generated FNV-1a Hash: --";
            lblCryptoHash->ForeColor = Color::FromArgb(37, 99, 235);
            lblCryptoHash->Font = gcnew Drawing::Font("Consolas", 14);
            lblCryptoHash->AutoSize = true;
            lblCryptoHash->Location = Point(60, 230);
            contentPanel->Controls->Add(lblCryptoHash);
            
            lblCryptoStatus = gcnew Label();
            lblCryptoStatus->Text = "Verification Status: --";
            lblCryptoStatus->ForeColor = Color::White;
            lblCryptoStatus->Font = gcnew Drawing::Font("Consolas", 14, FontStyle::Bold);
            lblCryptoStatus->AutoSize = true;
            lblCryptoStatus->Location = Point(60, 270);
            contentPanel->Controls->Add(lblCryptoStatus);
        }

        void btnHash_Click(Object^ sender, EventArgs^ e) {
            if (cbCryptoData->SelectedIndex == -1) {
                MessageBox::Show("Please select an evidence item.");
                return;
            }
            String^ dataStr = cbCryptoData->SelectedItem->ToString();
            
            CrimeManagementSystem cms;
            Evidence** evLog = cms.getEvidenceLog();
            int idx = cbCryptoData->SelectedIndex;
            if (idx < 0 || idx >= cms.getEvidenceCount()) return;
            
            std::string c_desc = msclr::interop::marshal_as<std::string>(gcnew String(evLog[idx]->getDescription().c_str()));
            unsigned int generatedHash = AdvancedFeatures::CryptoEvidence::calculateFNV1a(c_desc);
            unsigned int storedHash = evLog[idx]->getSecureHash();
            bool verified = (generatedHash == storedHash);

            lblCryptoHash->Text = "Generated Hash: " + generatedHash.ToString() + " | Stored Hash: " + storedHash.ToString();
            lblCryptoStatus->Text = "Verification Status: " + (verified ? "VERIFIED (SECURE)" : "TAMPERED");
            lblCryptoStatus->ForeColor = verified ? Color::FromArgb(34, 197, 94) : Color::FromArgb(220, 38, 38);
        }

        void ShowBiometrics() {
            contentPanel->Controls->Clear();
            Label^ title = gcnew Label();
            title->Text = "FUZZY BIOMETRIC MATCHING";
            title->Font = gcnew Drawing::Font("Impact", 24);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(40, 30);
            contentPanel->Controls->Add(title);

            Label^ l1 = gcnew Label();
            l1->Text = "Database DNA Sequence:";
            l1->ForeColor = Color::White;
            l1->Font = gcnew Drawing::Font("Segoe UI", 12);
            l1->AutoSize = true;
            l1->Location = Point(60, 90);
            contentPanel->Controls->Add(l1);

            tbDatabaseDNA = gcnew TextBox();
            tbDatabaseDNA->Size = Drawing::Size(400, 30);
            tbDatabaseDNA->Location = Point(60, 120);
            tbDatabaseDNA->Font = gcnew Drawing::Font("Consolas", 12);
            tbDatabaseDNA->BackColor = Color::FromArgb(30, 41, 59);
            tbDatabaseDNA->ForeColor = Color::White;
            tbDatabaseDNA->CharacterCasing = CharacterCasing::Upper;
            contentPanel->Controls->Add(tbDatabaseDNA);

            Label^ l2 = gcnew Label();
            l2->Text = "Crime Scene DNA Sequence:";
            l2->ForeColor = Color::White;
            l2->Font = gcnew Drawing::Font("Segoe UI", 12);
            l2->AutoSize = true;
            l2->Location = Point(60, 160);
            contentPanel->Controls->Add(l2);

            tbSceneDNA = gcnew TextBox();
            tbSceneDNA->Size = Drawing::Size(400, 30);
            tbSceneDNA->Location = Point(60, 190);
            tbSceneDNA->Font = gcnew Drawing::Font("Consolas", 12);
            tbSceneDNA->BackColor = Color::FromArgb(30, 41, 59);
            tbSceneDNA->ForeColor = Color::White;
            tbSceneDNA->CharacterCasing = CharacterCasing::Upper;
            contentPanel->Controls->Add(tbSceneDNA);

            Button^ btnAnalyzeDNA = gcnew Button();
            btnAnalyzeDNA->Text = "Run Biometric Scan";
            btnAnalyzeDNA->Size = Drawing::Size(200, 40);
            btnAnalyzeDNA->Location = Point(60, 240);
            btnAnalyzeDNA->BackColor = Color::FromArgb(34, 197, 94);
            btnAnalyzeDNA->ForeColor = Color::White;
            btnAnalyzeDNA->FlatStyle = FlatStyle::Flat;
            btnAnalyzeDNA->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
            btnAnalyzeDNA->Click += gcnew EventHandler(this, &AdvancedFeaturesDashboard::btnAnalyzeDNA_Click);
            contentPanel->Controls->Add(btnAnalyzeDNA);

            lblDistanceResult = gcnew Label();
            lblDistanceResult->Text = "Levenshtein Distance: --";
            lblDistanceResult->ForeColor = Color::FromArgb(245, 158, 11);
            lblDistanceResult->Font = gcnew Drawing::Font("Consolas", 14);
            lblDistanceResult->AutoSize = true;
            lblDistanceResult->Location = Point(60, 300);
            contentPanel->Controls->Add(lblDistanceResult);

            lblPercentResult = gcnew Label();
            lblPercentResult->Text = "Similarity Percentage: --%";
            lblPercentResult->ForeColor = Color::White;
            lblPercentResult->Font = gcnew Drawing::Font("Consolas", 16, FontStyle::Bold);
            lblPercentResult->AutoSize = true;
            lblPercentResult->Location = Point(60, 340);
            contentPanel->Controls->Add(lblPercentResult);
        }

        void btnAnalyzeDNA_Click(Object^ sender, EventArgs^ e) {
            String^ dbDna = tbDatabaseDNA->Text->Trim()->ToUpper();
            String^ sceneDna = tbSceneDNA->Text->Trim()->ToUpper();

            if (dbDna == "" || sceneDna == "") {
                MessageBox::Show("Please enter both DNA sequences.");
                return;
            }

            std::string c_dbDna = msclr::interop::marshal_as<std::string>(dbDna);
            std::string c_sceneDna = msclr::interop::marshal_as<std::string>(sceneDna);

            int dist = AdvancedFeatures::BiometricMatcher::calculateLevenshtein(c_dbDna, c_sceneDna);
            float sim = AdvancedFeatures::BiometricMatcher::getSimilarityPercentage(c_dbDna, c_sceneDna);

            lblDistanceResult->Text = "Levenshtein Distance: " + dist.ToString();
            lblPercentResult->Text = "Similarity Percentage: " + sim.ToString("0.00") + "%";

            if (sim > 80.0) {
                lblPercentResult->ForeColor = Color::FromArgb(34, 197, 94); // High match
            } else {
                lblPercentResult->ForeColor = Color::FromArgb(220, 38, 38);   // Low match
            }
        }

        void Back_Click(Object^ sender, EventArgs^ e) {
            System::Type^ formType = mainForm->GetType();
            System::Reflection::MethodInfo^ showAdminMethod = formType->GetMethod("ShowAdminDashboard");
            if (showAdminMethod != nullptr) {
                showAdminMethod->Invoke(mainForm, nullptr);
            }
        }
    };
}
