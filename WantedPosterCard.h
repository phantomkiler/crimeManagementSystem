#pragma once
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Drawing::Text;
using namespace System::Collections::Generic;

namespace CrimeManagementGUI {

    // Suspect Data Structure
    public ref struct SuspectData {
        String^ id;
        String^ name;
        String^ alias;
        int age;
        String^ height;
        String^ crime;
        int bounty;
        String^ status; // "Most Wanted", "Active", "In Custody", "Released"
        String^ lastSeen;
        String^ dangerLevel; // "Extreme", "High", "Medium", "Low"
        String^ photoPath;
        
        // Specialized Subsystems (Feature 13, Feature 14)
        String^ bloodGroup;
        String^ physicalMarks;
        String^ knownRelatives;

        static SuspectData^ CreateMock(String^ id, String^ name, String^ alias, int age, String^ height, String^ crime, int bounty, String^ status, String^ lastSeen, String^ danger) {
            SuspectData^ s = gcnew SuspectData();
            s->id = id; s->name = name; s->alias = alias; s->age = age; s->height = height; s->crime = crime; s->bounty = bounty; s->status = status; s->lastSeen = lastSeen; s->dangerLevel = danger; s->photoPath = "";
            s->bloodGroup = "Unknown";
            s->physicalMarks = "None recorded";
            s->knownRelatives = "None on file";
            return s;
        }

        static void SaveSuspects(List<SuspectData^>^ suspects) {
            String^ dbPath = System::IO::Path::Combine(Application::StartupPath, "suspects_db.txt");
            List<String^>^ lines = gcnew List<String^>();
            for each (SuspectData^ s in suspects) {
                String^ line = String::Format("{0}|{1}|{2}|{3}|{4}|{5}|{6}|{7}|{8}|{9}|{10}|{11}|{12}|{13}",
                    s->id, s->name, s->alias, s->age, s->height, s->crime, s->bounty, s->status, s->lastSeen, s->dangerLevel, s->photoPath,
                    s->bloodGroup, s->physicalMarks, s->knownRelatives);
                lines->Add(line);
            }
            System::IO::File::WriteAllLines(dbPath, lines->ToArray());
        }

        static List<SuspectData^>^ LoadSuspects() {
            List<SuspectData^>^ suspects = gcnew List<SuspectData^>();
            String^ dbPath = System::IO::Path::Combine(Application::StartupPath, "suspects_db.txt");
            if (System::IO::File::Exists(dbPath)) {
                array<String^>^ lines = System::IO::File::ReadAllLines(dbPath);
                for each (String^ line in lines) {
                    if (String::IsNullOrWhiteSpace(line)) continue;
                    array<String^>^ parts = line->Split('|');
                    if (parts->Length >= 10) {
                        SuspectData^ s = gcnew SuspectData();
                        s->id = parts[0]; s->name = parts[1]; s->alias = parts[2];
                        s->age = Int32::Parse(parts[3]); s->height = parts[4]; s->crime = parts[5];
                        s->bounty = Int32::Parse(parts[6]); s->status = parts[7];
                        s->lastSeen = parts[8]; s->dangerLevel = parts[9];
                        if (parts->Length >= 11) s->photoPath = parts[10];
                        if (parts->Length >= 14) {
                            s->bloodGroup = parts[11];
                            s->physicalMarks = parts[12];
                            s->knownRelatives = parts[13];
                        } else {
                            s->bloodGroup = "Unknown";
                            s->physicalMarks = "None recorded";
                            s->knownRelatives = "None on file";
                        }
                        suspects->Add(s);
                    }
                }
            } else {
                suspects->Add(CreateMock("S001", "JACK MORRISON", "The Blade", 34, "6'2\"", "Armed Robbery", 150000, "Most Wanted", "Downtown District", "Extreme"));
                suspects->Add(CreateMock("S002", "MARIA SANTOS", "Ghost", 28, "5'7\"", "Cybercrime", 95000, "Most Wanted", "Tech Plaza", "High"));
                suspects->Add(CreateMock("S003", "VIKTOR VOLKOV", "The Bear", 42, "6'5\"", "Organized Crime", 250000, "Most Wanted", "Port District", "Extreme"));
                suspects->Add(CreateMock("S004", "SARAH CHEN", "Viper", 31, "5'6\"", "Drug Trafficking", 180000, "Most Wanted", "Warehouse 12", "Extreme"));
                suspects->Add(CreateMock("S005", "CARLOS RODRIGUEZ", "El Tigre", 29, "5'10\"", "Grand Theft Auto", 45000, "Active", "Highway 101", "Medium"));
                suspects->Add(CreateMock("S006", "DMITRI PETROV", "Red Fox", 36, "6'0\"", "Extortion", 65000, "Active", "Business District", "High"));
                suspects->Add(CreateMock("S007", "LISA YAMAMOTO", "Phantom", 26, "5'5\"", "Identity Theft", 35000, "Active", "Shopping Mall", "Medium"));
                suspects->Add(CreateMock("S008", "ANTONIO GARCIA", "El Lobo", 38, "5'11\"", "Weapons Trafficking", 125000, "Active", "Border Town", "High"));
                suspects->Add(CreateMock("S009", "JENNY BLACK", "Raven", 24, "5'4\"", "Art Theft", 55000, "In Custody", "City Museum", "Low"));
                suspects->Add(CreateMock("S010", "MARCUS JOHNSON", "Doc", 45, "6'1\"", "Medical Fraud", 40000, "Released", "Medical Center", "Low"));
                SaveSuspects(suspects);
            }
            return suspects;
        }

        static void UpdatePhotoPath(String^ targetId, String^ newPath) {
            List<SuspectData^>^ all = LoadSuspects();
            for each (SuspectData^ s in all) {
                if (s->id == targetId) {
                    s->photoPath = newPath;
                    break;
                }
            }
            SaveSuspects(all);
        }
    };
    public ref class ArchiveReasonForm : public Form {
    public:
        String^ SelectedReason;
        ArchiveReasonForm() {
            this->Text = "Archive Suspect";
            this->Size = Drawing::Size(300, 200);
            this->StartPosition = FormStartPosition::CenterParent;
            this->BackColor = Color::FromArgb(30, 41, 59);
            this->ForeColor = Color::White;

            Label^ lbl = gcnew Label();
            lbl->Text = "Reason for Archiving:";
            lbl->Location = Point(20, 20);
            lbl->AutoSize = true;

            ComboBox^ cbReason = gcnew ComboBox();
            cbReason->Name = "cbReason";
            cbReason->Items->AddRange(gcnew cli::array<String^>{"Deceased", "Exonerated", "Transferred Jurisdiction", "Case Closed"});
            cbReason->Location = Point(20, 50);
            cbReason->Size = Drawing::Size(240, 25);
            cbReason->DropDownStyle = ComboBoxStyle::DropDownList;
            if (cbReason->Items->Count > 0) cbReason->SelectedIndex = 0;

            Button^ btnConfirm = gcnew Button();
            btnConfirm->Text = "Confirm";
            btnConfirm->Location = Point(80, 100);
            btnConfirm->Size = Drawing::Size(120, 40);
            btnConfirm->BackColor = Color::FromArgb(220, 38, 38);
            btnConfirm->FlatStyle = FlatStyle::Flat;
            btnConfirm->Click += gcnew EventHandler(this, &ArchiveReasonForm::Confirm_Click);

            this->Controls->Add(lbl);
            this->Controls->Add(cbReason);
            this->Controls->Add(btnConfirm);
        }
    private:
        void Confirm_Click(Object^ sender, EventArgs^ e) {
            ComboBox^ cbReason = (ComboBox^)this->Controls->Find("cbReason", false)[0];
            SelectedReason = cbReason->SelectedItem->ToString();
            this->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->Close();
        }
    };

    public ref class SuspectEditForm : public Form {
    private:
        SuspectData^ suspect;
        TextBox^ tName;
        TextBox^ tAlias;
        TextBox^ tCrime;
        ComboBox^ cStatus;
        TextBox^ tBounty;
        TextBox^ tDanger;
    public:
        bool wasDeleted;
        bool wasSaved;

        SuspectEditForm(SuspectData^ s) {
            suspect = s;
            wasDeleted = false;
            wasSaved = false;
            InitializeComponent();
        }
    private:
        void InitializeComponent() {
            this->Text = "Edit Suspect: " + suspect->id;
            this->Size = Drawing::Size(400, 350);
            this->StartPosition = FormStartPosition::CenterParent;
            this->BackColor = Color::FromArgb(30, 41, 59);
            this->ForeColor = Color::White;

            Label^ lName = gcnew Label(); lName->Text = "Name:"; lName->Location = Point(20, 20);
            tName = gcnew TextBox(); tName->Text = suspect->name; tName->Location = Point(120, 20); tName->Size = Drawing::Size(240, 25);
            
            Label^ lAlias = gcnew Label(); lAlias->Text = "Alias:"; lAlias->Location = Point(20, 60);
            tAlias = gcnew TextBox(); tAlias->Text = suspect->alias; tAlias->Location = Point(120, 60); tAlias->Size = Drawing::Size(240, 25);
            
            Label^ lCrime = gcnew Label(); lCrime->Text = "Crime:"; lCrime->Location = Point(20, 100);
            tCrime = gcnew TextBox(); tCrime->Text = suspect->crime; tCrime->Location = Point(120, 100); tCrime->Size = Drawing::Size(240, 25);
            
            Label^ lStatus = gcnew Label(); lStatus->Text = "Status:"; lStatus->Location = Point(20, 140);
            cStatus = gcnew ComboBox(); 
            cStatus->Items->AddRange(gcnew cli::array<String^>{"Most Wanted", "Active", "In Custody", "Released"});
            cStatus->SelectedItem = suspect->status;
            cStatus->Location = Point(120, 140); cStatus->Size = Drawing::Size(240, 25);
            cStatus->DropDownStyle = ComboBoxStyle::DropDownList;

            Label^ lBounty = gcnew Label(); lBounty->Text = "Bounty:"; lBounty->Location = Point(20, 180);
            tBounty = gcnew TextBox(); tBounty->Text = suspect->bounty.ToString(); tBounty->Location = Point(120, 180); tBounty->Size = Drawing::Size(240, 25);

            Label^ lDanger = gcnew Label(); lDanger->Text = "Danger Lvl:"; lDanger->Location = Point(20, 220);
            tDanger = gcnew TextBox(); tDanger->Text = suspect->dangerLevel; tDanger->Location = Point(120, 220); tDanger->Size = Drawing::Size(240, 25);

            Button^ btnSave = gcnew Button(); btnSave->Text = "Save"; btnSave->Location = Point(50, 270); btnSave->Size = Drawing::Size(120, 40);
            btnSave->BackColor = Color::FromArgb(34, 197, 94); btnSave->FlatStyle = FlatStyle::Flat;
            btnSave->Click += gcnew EventHandler(this, &SuspectEditForm::Save_Click);
            
            Button^ btnDelete = gcnew Button(); btnDelete->Text = "Delete"; btnDelete->Location = Point(200, 270); btnDelete->Size = Drawing::Size(120, 40);
            btnDelete->BackColor = Color::FromArgb(220, 38, 38); btnDelete->FlatStyle = FlatStyle::Flat;
            btnDelete->Click += gcnew EventHandler(this, &SuspectEditForm::Delete_Click);

            this->Controls->Add(lName); this->Controls->Add(tName);
            this->Controls->Add(lAlias); this->Controls->Add(tAlias);
            this->Controls->Add(lCrime); this->Controls->Add(tCrime);
            this->Controls->Add(lStatus); this->Controls->Add(cStatus);
            this->Controls->Add(lBounty); this->Controls->Add(tBounty);
            this->Controls->Add(lDanger); this->Controls->Add(tDanger);
            this->Controls->Add(btnSave); this->Controls->Add(btnDelete);
        }

        void Save_Click(Object^ sender, EventArgs^ e) {
            suspect->name = tName->Text;
            suspect->alias = tAlias->Text;
            suspect->crime = tCrime->Text;
            if (cStatus->SelectedItem != nullptr) suspect->status = cStatus->SelectedItem->ToString();
            suspect->dangerLevel = tDanger->Text;
            int b;
            if (Int32::TryParse(tBounty->Text, b)) suspect->bounty = b;
            wasSaved = true;
            this->Close();
        }

        void Delete_Click(Object^ sender, EventArgs^ e) {
            ArchiveReasonForm^ archiveForm = gcnew ArchiveReasonForm();
            if (archiveForm->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                
                CrimeManagementSystem cms;
                String^ localReason = archiveForm->SelectedReason;
                std::string reason = msclr::interop::marshal_as<std::string>(localReason);
                
                msclr::interop::marshal_context ctx;
                const char* cName = ctx.marshal_as<const char*>(suspect->name);
                const char* cCrime = ctx.marshal_as<const char*>(suspect->crime);
                const char* cDate = ctx.marshal_as<const char*>(DateTime::Now.ToString("yyyy-MM-dd"));
                
                cms.archiveSuspectDirectly(cName, cCrime, reason.c_str(), cDate);

                wasDeleted = true;
                this->Close();
            }
        }
    };

    // ONE PIECE STYLE WANTED POSTER CONTROL
    public ref class WantedPosterCard : public UserControl {
    private:
        SuspectData^ suspect;
        Timer^ pulseTimer;
        float pulseAlpha;
        bool isHovered;

    public:
        WantedPosterCard(SuspectData^ suspectData) {
            suspect = suspectData;
            pulseAlpha = 0.0f;
            isHovered = false;
            InitializeComponent();

            if (suspect->status == "Most Wanted") {
                StartPulseAnimation();
            }
        }

    private:
        void InitializeComponent() {
            // Card Size - Wanted Poster proportions
            this->Size = Drawing::Size(280, 420);
            this->BackColor = Color::Transparent;
            this->DoubleBuffered = true;
            this->Cursor = Cursors::Hand;

            this->Paint += gcnew PaintEventHandler(this, &WantedPosterCard::OnPaint);
            this->MouseEnter += gcnew EventHandler(this, &WantedPosterCard::Card_MouseEnter);
            this->MouseLeave += gcnew EventHandler(this, &WantedPosterCard::Card_MouseLeave);
            this->MouseClick += gcnew MouseEventHandler(this, &WantedPosterCard::Card_MouseClick);
        }

        void StartPulseAnimation() {
            pulseTimer = gcnew Timer();
            pulseTimer->Interval = 50;
            pulseTimer->Tick += gcnew EventHandler(this, &WantedPosterCard::PulseTimer_Tick);
            pulseTimer->Start();
        }

        void PulseTimer_Tick(Object^ sender, EventArgs^ e) {
            pulseAlpha += 0.1f;
            if (pulseAlpha > 1.0f) pulseAlpha = 0.0f;
            this->Invalidate();
        }

        void OnPaint(Object^ sender, PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            g->TextRenderingHint = TextRenderingHint::AntiAliasGridFit;

            // Apply hover scale effect
            if (isHovered) {
                g->ScaleTransform(1.05f, 1.05f);
                g->TranslateTransform(-7, -10);
            }

            // WANTED POSTER BACKGROUND
            DrawPosterBackground(g);

            // TORN PAPER EFFECT TOP
            DrawTornEdge(g, true);

            // "WANTED" HEADER
            DrawWantedHeader(g);

            // PHOTO SECTION
            DrawPhotoSection(g);

            // NAME & ALIAS
            DrawNameSection(g);

            // DIVIDER
            DrawDivider(g);

            // CRIME
            DrawCrimeSection(g);

            // BOUNTY
            DrawBountySection(g);

            // DETAILS (Age, Height, Last Seen)
            DrawDetailsSection(g);

            // WARNING FOOTER
            DrawWarningFooter(g);

            // TORN PAPER EFFECT BOTTOM
            DrawTornEdge(g, false);

            // PIN AT TOP
            DrawPin(g);

            // GRUNGE TEXTURE OVERLAY
            DrawGrungeOverlay(g);
        }

        void DrawPosterBackground(Graphics^ g) {
            Drawing::Rectangle posterRect(10, 10, 260, 400);

            // Aged paper gradient
            LinearGradientBrush^ paperBrush = gcnew LinearGradientBrush(
                posterRect,
                Color::FromArgb(255, 250, 240), // Floral white
                Color::FromArgb(255, 245, 220), // Navajo white
                LinearGradientMode::Vertical
            );
            g->FillRectangle(paperBrush, posterRect);

            // Double border - outer
            Pen^ borderOuter = gcnew Pen(Color::FromArgb(101, 67, 33), 6);
            g->DrawRectangle(borderOuter, posterRect);

            // Double border - inner
            Pen^ borderInner = gcnew Pen(Color::FromArgb(139, 90, 43), 2);
            g->DrawRectangle(borderInner, 16, 16, 248, 388);
        }

        void DrawTornEdge(Graphics^ g, bool isTop) {
            // Create jagged torn paper effect
            GraphicsPath^ path = gcnew GraphicsPath();
            int y = isTop ? 10 : 404;

            path->StartFigure();
            for (int x = 10; x <= 270; x += 10) {
                int offset = (x % 20 == 0) ? 4 : 0;
                path->AddLine(x, y + offset, x + 10, y - offset);
            }
            path->CloseFigure();

            g->FillPath(gcnew SolidBrush(Color::FromArgb(101, 67, 33)), path);
        }

        void DrawWantedHeader(Graphics^ g) {
            Drawing::Rectangle headerRect(10, 18, 260, 70);

            // Red background
            LinearGradientBrush^ redBrush = gcnew LinearGradientBrush(
                headerRect,
                Color::FromArgb(139, 0, 0),   // Dark red
                Color::FromArgb(178, 34, 34), // Firebrick
                LinearGradientMode::Vertical
            );
            g->FillRectangle(redBrush, headerRect);

            // "WANTED" text
            Drawing::Font^ wantedFont = gcnew Drawing::Font("Impact", 36, FontStyle::Bold);
            StringFormat^ centerFormat = gcnew StringFormat();
            centerFormat->Alignment = StringAlignment::Center;
            centerFormat->LineAlignment = StringAlignment::Center;

            g->DrawString("WANTED", wantedFont,
                gcnew SolidBrush(Color::FromArgb(255, 245, 220)),
                RectangleF(10.0f, 18.0f, 260.0f, 70.0f),
                centerFormat);

            // "MOST WANTED" for high priority
            if (suspect->status == "Most Wanted") {
                Drawing::Font^ mostFont = gcnew Drawing::Font("Arial", 8, FontStyle::Bold);
                g->DrawString("[!] MOST WANTED [!]", mostFont,
                    gcnew SolidBrush(Color::FromArgb(255, 245, 220)),
                    RectangleF(10.0f, 70.0f, 260.0f, 18.0f),
                    centerFormat);

                // Pulsing glow for most wanted
                if (pulseTimer != nullptr) {
                    int alpha = (int)(100 * Math::Sin(pulseAlpha * Math::PI * 2));
                    if (alpha < 0) alpha = 0;
                    Pen^ glowPen = gcnew Pen(Color::FromArgb(alpha, 220, 38, 38), 3);
                    g->DrawRectangle(glowPen, 10, 10, 260, 400);
                }
            }
        }

        void DrawPhotoSection(Graphics^ g) {
            Drawing::Rectangle photoRect(50, 95, 180, 180);

            // Photo background (dark)
            g->FillRectangle(gcnew SolidBrush(Color::FromArgb(30, 30, 30)), photoRect);

            bool photoLoaded = false;
            if (!String::IsNullOrEmpty(suspect->photoPath) && System::IO::File::Exists(suspect->photoPath)) {
                try {
                    Bitmap^ photo = gcnew Bitmap(suspect->photoPath);
                    g->DrawImage(photo, photoRect);
                    photoLoaded = true;
                }
                catch (Exception^) { }
            }

            StringFormat^ centerFormat = gcnew StringFormat();
            centerFormat->Alignment = StringAlignment::Center;
            centerFormat->LineAlignment = StringAlignment::Center;

            if (!photoLoaded) {
                // Placeholder icon
                Drawing::Font^ iconFont = gcnew Drawing::Font("Segoe UI", 48);

                String^ icon = suspect->status == "Most Wanted" ? "!" : "?";
                g->DrawString(icon, iconFont, Brushes::White,
                    RectangleF(50.0f, 95.0f, 180.0f, 180.0f), centerFormat);
                
                Drawing::Font^ uploadFont = gcnew Drawing::Font("Arial", 8);
                g->DrawString("Click to add photo", uploadFont, Brushes::Gray,
                    RectangleF(50.0f, 240.0f, 180.0f, 30.0f), centerFormat);
            }

            // Border
            Pen^ photoBorder = gcnew Pen(Color::FromArgb(0, 0, 0), 4);
            g->DrawRectangle(photoBorder, photoRect);

            // Danger Level Badge
            String^ dangerText = GetDangerLevel();
            Color dangerColor = GetDangerColor();

            Drawing::Rectangle badgeRect(190, 100, 55, 22);
            GraphicsPath^ badgePath = GetRoundedRect(badgeRect, 11);
            g->FillPath(gcnew SolidBrush(dangerColor), badgePath);

            Drawing::Font^ badgeFont = gcnew Drawing::Font("Arial", 7, FontStyle::Bold);
            g->DrawString(dangerText, badgeFont, Brushes::White,
                RectangleF(190.0f, 103.0f, 55.0f, 18.0f), centerFormat);

            // Crosshair overlay for Most Wanted
            if (suspect->status == "Most Wanted") {
                Pen^ crossPen = gcnew Pen(Color::FromArgb(100, 220, 38, 38), 2);
                g->DrawEllipse(crossPen, 85, 130, 110, 110);
                g->DrawLine(crossPen, 140, 130, 140, 240);
                g->DrawLine(crossPen, 85, 185, 195, 185);
            }
        }

        void DrawNameSection(Graphics^ g) {
            StringFormat^ centerFormat = gcnew StringFormat();
            centerFormat->Alignment = StringAlignment::Center;

            // Name
            Drawing::Font^ nameFont = gcnew Drawing::Font("Impact", 18, FontStyle::Bold);
            SizeF nameSize = g->MeasureString(suspect->name, nameFont);
            float fontSize = 18.0f;

            // Auto-resize if name is too long
            if (nameSize.Width > 240) {
                fontSize = 14.0f;
                nameFont = gcnew Drawing::Font("Impact", fontSize, FontStyle::Bold);
            }

            g->DrawString(suspect->name->ToUpper(), nameFont,
                Brushes::Black,
                RectangleF(10.0f, 282.0f, 260.0f, 25.0f),
                centerFormat);

            // Alias
            if (!String::IsNullOrEmpty(suspect->alias)) {
                Drawing::Font^ aliasFont = gcnew Drawing::Font("Arial", 9, FontStyle::Bold);
                g->DrawString("\"" + suspect->alias + "\"", aliasFont,
                    gcnew SolidBrush(Color::FromArgb(60, 60, 60)),
                    RectangleF(10.0f, 306.0f, 260.0f, 15.0f),
                    centerFormat);
            }
        }

        void DrawDivider(Graphics^ g) {
            Pen^ divPen = gcnew Pen(Color::FromArgb(139, 0, 0), 2);

            // Left line
            g->DrawLine(divPen, 30, 325, 110, 325);

            // Skull icon
            Drawing::Font^ skullFont = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
            g->DrawString("[!]", skullFont, Brushes::Black, 120, 318);

            // Right line
            g->DrawLine(divPen, 150, 325, 240, 325);
        }

        void DrawCrimeSection(Graphics^ g) {
            Drawing::Rectangle crimeRect(30, 330, 220, 32);
            g->FillRectangle(gcnew SolidBrush(Color::FromArgb(139, 0, 0)), crimeRect);

            StringFormat^ centerFormat = gcnew StringFormat();
            centerFormat->Alignment = StringAlignment::Center;
            centerFormat->LineAlignment = StringAlignment::Center;

            // Label
            Drawing::Font^ labelFont = gcnew Drawing::Font("Arial", 7, FontStyle::Bold);
            g->DrawString("CRIME", labelFont,
                gcnew SolidBrush(Color::FromArgb(255, 245, 220)),
                RectangleF(30.0f, 332.0f, 220.0f, 10.0f), centerFormat);

            // Crime type
            Drawing::Font^ crimeFont = gcnew Drawing::Font("Arial", 11, FontStyle::Bold);
            g->DrawString(suspect->crime->ToUpper(), crimeFont,
                gcnew SolidBrush(Color::FromArgb(255, 245, 220)),
                RectangleF(30.0f, 345.0f, 220.0f, 17.0f), centerFormat);
        }

        void DrawBountySection(Graphics^ g) {
            Drawing::Rectangle bountyRect(30, 367, 220, 42);

            // Gold gradient
            LinearGradientBrush^ goldBrush = gcnew LinearGradientBrush(
                bountyRect,
                Color::FromArgb(255, 223, 0),   // Gold
                Color::FromArgb(255, 193, 37),  // Dark gold
                LinearGradientMode::Vertical
            );
            g->FillRectangle(goldBrush, bountyRect);

            // Border
            Pen^ borderPen = gcnew Pen(Color::FromArgb(184, 134, 11), 2);
            g->DrawRectangle(borderPen, bountyRect);

            StringFormat^ centerFormat = gcnew StringFormat();
            centerFormat->Alignment = StringAlignment::Center;
            centerFormat->LineAlignment = StringAlignment::Center;

            // Label
            Drawing::Font^ labelFont = gcnew Drawing::Font("Arial", 7, FontStyle::Bold);
            g->DrawString("BOUNTY", labelFont, Brushes::Black,
                RectangleF(30.0f, 370.0f, 220.0f, 12.0f), centerFormat);

            // Amount
            Drawing::Font^ bountyFont = gcnew Drawing::Font("Impact", 20, FontStyle::Bold);
            String^ bountyText = "$" + suspect->bounty.ToString("N0");
            g->DrawString(bountyText, bountyFont, Brushes::Black,
                RectangleF(30.0f, 385.0f, 220.0f, 24.0f), centerFormat);

            // Warning icon
            g->DrawString("[!]", gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold),
                gcnew SolidBrush(Color::FromArgb(139, 0, 0)), 230, 370);
        }

        void DrawDetailsSection(Graphics^ g) {
            // Compact details
            Drawing::Font^ detailFont = gcnew Drawing::Font("Arial", 7);
            SolidBrush^ labelBrush = gcnew SolidBrush(Color::FromArgb(80, 80, 80));
            SolidBrush^ valueBrush = gcnew SolidBrush(Color::Black);

            // Age
            g->DrawString("Age:", detailFont, labelBrush, 35, 415);
            g->DrawString(suspect->age.ToString(), detailFont, valueBrush, 95, 415);

            // Height
            g->DrawString("Height:", detailFont, labelBrush, 35, 427);
            g->DrawString(suspect->height, detailFont, valueBrush, 95, 427);

            // Last Seen
            g->DrawString("Last Seen:", detailFont, labelBrush, 140, 415);
            g->DrawString(suspect->lastSeen, detailFont, valueBrush, 140, 427);
        }

        void DrawWarningFooter(Graphics^ g) {
            Drawing::Rectangle footerRect(20, 445, 240, 28);
            g->FillRectangle(gcnew SolidBrush(Color::FromArgb(139, 0, 0)), footerRect);

            StringFormat^ centerFormat = gcnew StringFormat();
            centerFormat->Alignment = StringAlignment::Center;
            centerFormat->LineAlignment = StringAlignment::Center;

            Drawing::Font^ warnFont = gcnew Drawing::Font("Arial", 7, FontStyle::Bold);
            g->DrawString("[!] ARMED AND DANGEROUS [!]", warnFont,
                gcnew SolidBrush(Color::FromArgb(255, 245, 220)),
                RectangleF(20.0f, 447.0f, 240.0f, 12.0f), centerFormat);

            Drawing::Font^ subFont = gcnew Drawing::Font("Arial", 6, FontStyle::Bold);
            g->DrawString("DO NOT APPROACH • CONTACT AUTHORITIES", subFont,
                gcnew SolidBrush(Color::FromArgb(255, 245, 220)),
                RectangleF(20.0f, 460.0f, 240.0f, 10.0f), centerFormat);
        }

        void DrawPin(Graphics^ g) {
            // Pin at top center
            Drawing::Rectangle pinRect(130, 0, 20, 20);
            g->FillEllipse(gcnew SolidBrush(Color::FromArgb(70, 70, 70)), pinRect);

            // Pin border
            Pen^ pinBorder = gcnew Pen(Color::FromArgb(50, 50, 50), 3);
            g->DrawEllipse(pinBorder, pinRect);

            // Highlight
            g->FillEllipse(gcnew SolidBrush(Color::FromArgb(100, 255, 255, 255)),
                133, 3, 6, 6);
        }

        void DrawGrungeOverlay(Graphics^ g) {
            // Add subtle noise/texture for aged look
            SolidBrush^ grungeBrush = gcnew SolidBrush(Color::FromArgb(10, 101, 67, 33));
            Random^ rand = gcnew Random();

            for (int i = 0; i < 50; i++) {
                int x = rand->Next(10, 270);
                int y = rand->Next(10, 410);
                int size = rand->Next(2, 5);
                g->FillEllipse(grungeBrush, x, y, size, size);
            }
        }

        GraphicsPath^ GetRoundedRect(Drawing::Rectangle rect, int radius) {
            GraphicsPath^ path = gcnew GraphicsPath();
            path->AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
            path->AddArc(rect.Right - radius * 2, rect.Y, radius * 2, radius * 2, 270, 90);
            path->AddArc(rect.Right - radius * 2, rect.Bottom - radius * 2, radius * 2, radius * 2, 0, 90);
            path->AddArc(rect.X, rect.Bottom - radius * 2, radius * 2, radius * 2, 90, 90);
            path->CloseFigure();
            return path;
        }

        String^ GetDangerLevel() {
            if (suspect->dangerLevel == "Extreme") return "EXTREME";
            if (suspect->dangerLevel == "High") return "HIGH";
            if (suspect->dangerLevel == "Medium") return "MEDIUM";
            return "LOW";
        }

        Color GetDangerColor() {
            if (suspect->dangerLevel == "Extreme") return Color::FromArgb(220, 38, 38);
            if (suspect->dangerLevel == "High") return Color::FromArgb(249, 115, 22);
            if (suspect->dangerLevel == "Medium") return Color::FromArgb(234, 179, 8);
            return Color::FromArgb(34, 197, 94);
        }

        void Card_MouseEnter(Object^ sender, EventArgs^ e) {
            isHovered = true;
            this->Invalidate();
        }

        void Card_MouseLeave(Object^ sender, EventArgs^ e) {
            isHovered = false;
            this->Invalidate();
        }

        void Card_MouseClick(Object^ sender, MouseEventArgs^ e) {
            Drawing::Rectangle photoRect(50, 95, 180, 180);
            if (photoRect.Contains(e->Location)) {
                OpenFileDialog^ ofd = gcnew OpenFileDialog();
                ofd->Title = "Select Suspect Photo";
                ofd->Filter = "Image Files(*.jpg; *.jpeg; *.gif; *.bmp; *.png)|*.jpg; *.jpeg; *.gif; *.bmp; *.png";
                
                if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    String^ imagesDir = System::IO::Path::Combine(Application::StartupPath, "Images");
                    if (!System::IO::Directory::Exists(imagesDir)) {
                        System::IO::Directory::CreateDirectory(imagesDir);
                    }
                    
                    String^ ext = System::IO::Path::GetExtension(ofd->FileName);
                    String^ newPath = System::IO::Path::Combine(imagesDir, suspect->id + ext);
                    
                    try {
                        System::IO::File::Copy(ofd->FileName, newPath, true);
                        suspect->photoPath = newPath;
                        SuspectData::UpdatePhotoPath(suspect->id, newPath); // Persist to DB
                        this->Invalidate(); // Redraw the card with new photo
                    }
                    catch (Exception^ ex) {
                        MessageBox::Show("Error saving photo: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                    }
                }
            } else {
                SuspectEditForm^ form = gcnew SuspectEditForm(suspect);
                form->ShowDialog();
                if (form->wasDeleted || form->wasSaved) {
                    List<SuspectData^>^ all = SuspectData::LoadSuspects();
                    if (form->wasDeleted) {
                        for (int i = 0; i < all->Count; i++) {
                            if (all[i]->id == suspect->id) { all->RemoveAt(i); break; }
                        }
                        this->Hide(); // Hide deleted card immediately
                    } else if (form->wasSaved) {
                        for (int i = 0; i < all->Count; i++) {
                            if (all[i]->id == suspect->id) { 
                                all[i] = suspect; 
                                break; 
                            }
                        }
                        this->Invalidate(); // Refresh the card drawing
                    }
                    SuspectData::SaveSuspects(all);
                }
            }
        }
    };
}

