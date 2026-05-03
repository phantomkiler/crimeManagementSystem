#pragma once
#include "UIHelper.h"
#include "InstructionForm.h"
#define WIN32_LEAN_AND_MEAN
#pragma managed(push, off)
#include "CrimeManagement.h"
#pragma managed(pop)
#include <msclr/marshal_cppstd.h>

namespace CrimeManagementGUI {

    public ref class LoginForm : public UserControl {
    private:
        Form^ mainForm;
        bool isAdmin;
        Panel^ rolePanel;
        Panel^ authPanel;
        TextBox^ txtUsername;
        TextBox^ txtPassword;
        Timer^ scanTimer;
        int scanLine;

    public:
        LoginForm(Form^ parent) {
            mainForm = parent;
            isAdmin = false;
            scanLine = 0;
            InitializeComponent();
        }

    private:
        void InitializeComponent() {
            this->Size = Drawing::Size(1280, 720);
            this->BackColor = Color::FromArgb(11, 17, 27);
            this->DoubleBuffered = true;
            this->Paint += gcnew PaintEventHandler(this, &LoginForm::OnPaint);

            ShowInitialChoice();
        }

        void ShowInitialChoice() {
            rolePanel = gcnew Panel();
            rolePanel->Size = Drawing::Size(880, 500);
            rolePanel->BackColor = Color::Transparent;
            rolePanel->Location = Point((this->Width - 880) / 2, (this->Height - 500) / 2);
            rolePanel->Anchor = AnchorStyles::None;

            Label^ title = gcnew Label();
            title->Text = "PHANTOM KILLER SYSTEM";
            title->Font = gcnew Drawing::Font("Impact", 42, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(160, 20);
            rolePanel->Controls->Add(title);

            // SIGN IN Card
            Panel^ signInCard = CreateRoleCard("SIGN IN", "Login with existing credentials", Color::FromArgb(37, 99, 235), 40, 150);
            signInCard->Click += gcnew EventHandler(this, &LoginForm::InitialSignIn_Click);
            rolePanel->Controls->Add(signInCard);

            // SIGN UP Card
            Panel^ signUpCard = CreateRoleCard("SIGN UP", "Create a new account", Color::FromArgb(34, 197, 94), 440, 150);
            signUpCard->Click += gcnew EventHandler(this, &LoginForm::InitialSignUp_Click);
            rolePanel->Controls->Add(signUpCard);

            this->Controls->Add(rolePanel);
        }

        void InitialSignIn_Click(Object^ sender, EventArgs^ e) {
            this->Controls->Remove(rolePanel);
            ShowRoleSelection();
        }

        void InitialSignUp_Click(Object^ sender, EventArgs^ e) {
            this->Controls->Remove(rolePanel);
            ShowRegistrationPanel();
        }

        void ShowRoleSelection() {
            rolePanel = gcnew Panel();
            rolePanel->Size = Drawing::Size(880, 500);
            rolePanel->BackColor = Color::Transparent;
            rolePanel->Location = Point((this->Width - 880) / 2, (this->Height - 500) / 2);
            rolePanel->Anchor = AnchorStyles::None;

            // Title
            Label^ title = gcnew Label();
            title->Text = "SELECT ACCESS LEVEL";
            title->Font = gcnew Drawing::Font("Impact", 42, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(200, 20);
            rolePanel->Controls->Add(title);

            // User Card
            Panel^ userCard = CreateRoleCard("USER", "Standard Officer Access",
                Color::FromArgb(37, 99, 235), 40, 150);
            userCard->Click += gcnew EventHandler(this, &LoginForm::UserCard_Click);
            rolePanel->Controls->Add(userCard);

            // Admin Card
            Panel^ adminCard = CreateRoleCard("ADMIN", "Full System Control",
                Color::FromArgb(220, 38, 38), 440, 150);
            adminCard->Click += gcnew EventHandler(this, &LoginForm::AdminCard_Click);
            rolePanel->Controls->Add(adminCard);

            // Create Account Button
            Button^ btnCreate = gcnew Button();
            btnCreate->Text = "CREATE NEW ACCOUNT";
            btnCreate->Size = Drawing::Size(200, 40);
            btnCreate->Location = Point(220, 520); // Moved lower
            btnCreate->FlatStyle = FlatStyle::Flat;
            btnCreate->ForeColor = Color::White;
            btnCreate->BackColor = Color::Transparent;
            btnCreate->FlatAppearance->BorderColor = Color::FromArgb(37, 99, 235);
            btnCreate->Cursor = Cursors::Hand;
            btnCreate->Click += gcnew EventHandler(this, &LoginForm::CreateAccount_Click);
            
            // Anonymous Tip Button
            Button^ btnAnon = gcnew Button();
            btnAnon->Text = "SUBMIT ANONYMOUS TIP";
            btnAnon->Size = Drawing::Size(200, 40);
            btnAnon->Location = Point(440, 520); // Moved lower
            btnAnon->FlatStyle = FlatStyle::Flat;
            btnAnon->ForeColor = Color::White;
            btnAnon->BackColor = Color::Transparent;
            btnAnon->FlatAppearance->BorderColor = Color::FromArgb(245, 158, 11);
            btnAnon->Cursor = Cursors::Hand;
            btnAnon->Click += gcnew EventHandler(this, &LoginForm::SubmitAnon_Click);

            rolePanel->Controls->Add(btnCreate);
            rolePanel->Controls->Add(btnAnon);

            this->Controls->Add(rolePanel);
        }

        void CreateAccount_Click(Object^ sender, EventArgs^ e) {
            this->Controls->Remove(rolePanel);
            ShowRegistrationPanel();
        }

        void BackToInitial_Click(Object^ sender, EventArgs^ e) {
            this->Controls->Remove(rolePanel);
            ShowInitialChoice();
        }

        void SubmitAnon_Click(Object^ sender, EventArgs^ e) {
            this->Controls->Remove(rolePanel);
            ShowAnonymousTipPanel();
        }

        void ShowRegistrationPanel() {
            Panel^ regPanel = gcnew Panel();
            regPanel->Size = Drawing::Size(500, 550);
            regPanel->Location = Point((this->Width - 500) / 2, (this->Height - 550) / 2);
            regPanel->BackColor = Color::FromArgb(30, 41, 59);
            regPanel->Name = "regPanel";

            Label^ title = gcnew Label();
            title->Text = "USER REGISTRATION";
            title->Font = gcnew Drawing::Font("Impact", 28, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(90, 40);
            regPanel->Controls->Add(title);

            array<String^>^ labels = { "USERNAME", "FULL NAME", "PHONE NUMBER", "EMAIL ADDRESS", "PASSWORD", "SECURITY QUESTION", "SECURITY ANSWER" };
            int y = 120;
            for (int i = 0; i < 7; i++) {
                Label^ l = gcnew Label();
                l->Text = labels[i];
                l->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
                l->ForeColor = Color::FromArgb(156, 163, 175);
                l->AutoSize = true;
                l->Location = Point(50, y);
                regPanel->Controls->Add(l);

                TextBox^ t = gcnew TextBox();
                t->Name = "txtReg" + i;
                t->Size = Drawing::Size(400, 30);
                t->Location = Point(50, y + 20);
                t->Font = gcnew Drawing::Font("Segoe UI", 11);
                t->BackColor = Color::FromArgb(15, 23, 42);
                t->ForeColor = Color::White;
                t->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
                if (i == 4) t->PasswordChar = '*';
                regPanel->Controls->Add(t);
                
                y += 55;
            }

            Button^ btnReg = gcnew Button();
            btnReg->Text = "REGISTER ACCOUNT";
            btnReg->Size = Drawing::Size(400, 50);
            btnReg->Location = Point(50, y + 10);
            btnReg->BackColor = Color::FromArgb(37, 99, 235);
            btnReg->ForeColor = Color::White;
            btnReg->FlatStyle = FlatStyle::Flat;
            btnReg->Font = gcnew Drawing::Font("Segoe UI", 14, FontStyle::Bold);
            btnReg->Click += gcnew EventHandler(this, &LoginForm::DoRegister_Click);
            regPanel->Controls->Add(btnReg);

            Button^ btnBack = gcnew Button();
            btnBack->Text = "< Back to Login";
            btnBack->Size = Drawing::Size(150, 35);
            btnBack->Location = Point(20, 20); // Top left
            btnBack->FlatStyle = FlatStyle::Flat;
            btnBack->FlatAppearance->BorderSize = 0;
            btnBack->BackColor = Color::FromArgb(30, 41, 59); // Dark clean color
            btnBack->ForeColor = Color::FromArgb(156, 163, 175);
            btnBack->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
            btnBack->Cursor = Cursors::Hand;
            btnBack->Click += gcnew EventHandler(this, &LoginForm::CloseReg_Click);
            regPanel->Controls->Add(btnBack);

            this->Controls->Add(regPanel);
        }

        void DoRegister_Click(Object^ sender, EventArgs^ e) {
            Control^ p = this->Controls->Find("regPanel", false)[0];
            String^ username = ((TextBox^)p->Controls->Find("txtReg0", false)[0])->Text;
            String^ fullName = ((TextBox^)p->Controls->Find("txtReg1", false)[0])->Text;
            String^ phone = ((TextBox^)p->Controls->Find("txtReg2", false)[0])->Text;
            String^ email = ((TextBox^)p->Controls->Find("txtReg3", false)[0])->Text;
            String^ pass = ((TextBox^)p->Controls->Find("txtReg4", false)[0])->Text;
            String^ secQ = ((TextBox^)p->Controls->Find("txtReg5", false)[0])->Text;
            String^ secA = ((TextBox^)p->Controls->Find("txtReg6", false)[0])->Text;

            if (username == "" || pass == "") {
                MessageBox::Show("Username and Password are required.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            if (phone->Length != 11) {
                MessageBox::Show("Phone number must be exactly 11 characters.",
                    "Invalid Phone", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            std::string u = msclr::interop::marshal_as<std::string>(username);
            std::string psw = msclr::interop::marshal_as<std::string>(pass);
            std::string f = msclr::interop::marshal_as<std::string>(fullName);
            std::string ph = msclr::interop::marshal_as<std::string>(phone);
            std::string em = msclr::interop::marshal_as<std::string>(email);
            std::string sq = msclr::interop::marshal_as<std::string>(secQ);
            std::string sa = msclr::interop::marshal_as<std::string>(secA);

            CrimeManagementSystem cms;
            cms.userRegistration(u.c_str(), psw.c_str(), f.c_str(), ph.c_str(), em.c_str(), sq.c_str(), sa.c_str());

            MessageBox::Show("Account created successfully!\nYou are now being logged in.", "Registration Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            
            SessionManager::getInstance().login(u.c_str(), false);

            InstructionForm^ guide = gcnew InstructionForm(false);
            guide->ShowDialog();

            System::Type^ formType = mainForm->GetType();
            System::Reflection::MethodInfo^ showUserMethod = formType->GetMethod("ShowUserDashboard");
            if (showUserMethod != nullptr) {
                showUserMethod->Invoke(mainForm, nullptr);
            }
        }

        void CloseReg_Click(Object^ sender, EventArgs^ e) {
            Control^ p = this->Controls->Find("regPanel", false)[0];
            this->Controls->Remove(p);
            ShowInitialChoice();
        }

        void ShowAnonymousTipPanel() {
            Panel^ tipPanel = gcnew Panel();
            tipPanel->Size = Drawing::Size(600, 450);
            tipPanel->Location = Point((this->Width - 600) / 2, (this->Height - 450) / 2);
            tipPanel->BackColor = Color::FromArgb(30, 41, 59);
            tipPanel->Name = "tipPanel";

            Label^ title = gcnew Label();
            title->Text = "ANONYMOUS REPORT";
            title->Font = gcnew Drawing::Font("Impact", 28, FontStyle::Bold);
            title->ForeColor = Color::FromArgb(245, 158, 11);
            title->AutoSize = true;
            title->Location = Point(130, 40);
            tipPanel->Controls->Add(title);

            Label^ lblDesc = gcnew Label();
            lblDesc->Text = "Details of the incident or tip:";
            lblDesc->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
            lblDesc->ForeColor = Color::White;
            lblDesc->AutoSize = true;
            lblDesc->Location = Point(50, 110);
            tipPanel->Controls->Add(lblDesc);

            TextBox^ txtTip = gcnew TextBox();
            txtTip->Name = "txtTipDesc";
            txtTip->Multiline = true;
            txtTip->Size = Drawing::Size(500, 150);
            txtTip->Location = Point(50, 140);
            txtTip->Font = gcnew Drawing::Font("Segoe UI", 12);
            txtTip->BackColor = Color::FromArgb(15, 23, 42);
            txtTip->ForeColor = Color::White;
            tipPanel->Controls->Add(txtTip);

            Button^ btnSub = gcnew Button();
            btnSub->Text = "SUBMIT TIP SECURELY";
            btnSub->Size = Drawing::Size(500, 50);
            btnSub->Location = Point(50, 310);
            btnSub->BackColor = Color::FromArgb(245, 158, 11);
            btnSub->ForeColor = Color::White;
            btnSub->FlatStyle = FlatStyle::Flat;
            btnSub->Font = gcnew Drawing::Font("Segoe UI", 14, FontStyle::Bold);
            btnSub->Click += gcnew EventHandler(this, &LoginForm::DoSubmitTip_Click);
            tipPanel->Controls->Add(btnSub);

            Button^ btnBack = gcnew Button();
            btnBack->Text = "← Back";
            btnBack->Size = Drawing::Size(100, 35);
            btnBack->Location = Point(250, 380);
            btnBack->FlatStyle = FlatStyle::Flat;
            btnBack->ForeColor = Color::FromArgb(156, 163, 175);
            btnBack->Click += gcnew EventHandler(this, &LoginForm::CloseTip_Click);
            tipPanel->Controls->Add(btnBack);

            this->Controls->Add(tipPanel);
        }

        void DoSubmitTip_Click(Object^ sender, EventArgs^ e) {
            TextBox^ txtTip = (TextBox^)this->Controls->Find("tipPanel", false)[0]->Controls->Find("txtTipDesc", false)[0];
            if (txtTip->Text->Trim() == "") {
                MessageBox::Show("Tip description cannot be empty.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            // Append to anon_reports_db.txt
            String^ dbPath = System::IO::Path::Combine(Application::StartupPath, "anon_reports_db.txt");
            String^ dateStr = DateTime::Now.ToString("yyyy-MM-dd");
            String^ newId = "AR-" + DateTime::Now.Ticks.ToString()->Substring(8, 5);
            String^ entry = newId + "|" + txtTip->Text->Replace("\n", " ")->Replace("\r", "") + "|Pending|" + dateStr + "\n";
            System::IO::File::AppendAllText(dbPath, entry);

            MessageBox::Show("Your anonymous tip has been submitted. Tracking ID: " + newId, "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            CloseTip_Click(nullptr, nullptr);
        }

        void CloseTip_Click(Object^ sender, EventArgs^ e) {
            Control^ p = this->Controls->Find("tipPanel", false)[0];
            this->Controls->Remove(p);
            this->Controls->Add(rolePanel);
        }

        Panel^ CreateRoleCard(String^ title, String^ subtitle, Color accentColor, int x, int y) {
            Panel^ card = gcnew Panel();
            card->Size = Drawing::Size(380, 350);
            card->Location = Point(x, y);
            card->BackColor = Color::FromArgb(30, 41, 59);
            card->Cursor = Cursors::Hand;
            card->Tag = title->Contains("ADMIN");

            // Border
            card->Paint += gcnew PaintEventHandler(this, &LoginForm::RoleCard_Paint);

            // Title
            Label^ lblTitle = gcnew Label();
            lblTitle->Text = title;
            lblTitle->Font = gcnew Drawing::Font("Segoe UI", 28, FontStyle::Bold);
            lblTitle->ForeColor = Color::White;
            lblTitle->AutoSize = true;
            lblTitle->Location = Point(90, 120);
            lblTitle->BackColor = Color::Transparent;
            card->Controls->Add(lblTitle);

            // Subtitle
            Label^ lblSub = gcnew Label();
            lblSub->Text = subtitle;
            lblSub->Font = gcnew Drawing::Font("Segoe UI", 12);
            lblSub->ForeColor = accentColor;
            lblSub->AutoSize = true;
            lblSub->Location = Point(70, 170);
            lblSub->BackColor = Color::Transparent;
            card->Controls->Add(lblSub);

            // Demo creds
            Label^ lblDemo = gcnew Label();
            if (title == "SIGN IN") lblDemo->Text = "Proceed to Authentication";
            else if (title == "SIGN UP") lblDemo->Text = "Join the system";
            else if (title == "ADMIN") lblDemo->Text = "Demo: admin / admin123";
            else lblDemo->Text = "Standard Access";
            lblDemo->Font = gcnew Drawing::Font("Consolas", 9);
            lblDemo->ForeColor = Color::FromArgb(100, 100, 100);
            lblDemo->AutoSize = true;
            lblDemo->Location = Point(90, 300);
            lblDemo->BackColor = Color::Transparent;
            card->Controls->Add(lblDemo);

            // Hover effect
            card->MouseEnter += gcnew EventHandler(this, &LoginForm::Card_MouseEnter);
            card->MouseLeave += gcnew EventHandler(this, &LoginForm::Card_MouseLeave);

            return card;
        }

        void RoleCard_Paint(Object^ sender, PaintEventArgs^ e) {
            Panel^ card = (Panel^)sender;
            bool isAdminCard = (bool)card->Tag;

            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;

            Color borderColor = isAdminCard ?
                Color::FromArgb(220, 38, 38) : Color::FromArgb(37, 99, 235);

            Pen^ borderPen = gcnew Pen(borderColor, 3);
            g->DrawRectangle(borderPen, 0, 0, card->Width - 1, card->Height - 1);
        }

        void Card_MouseEnter(Object^ sender, EventArgs^ e) {
            Panel^ card = (Panel^)sender;
            card->BackColor = Color::FromArgb(45, 55, 72);
        }

        void Card_MouseLeave(Object^ sender, EventArgs^ e) {
            Panel^ card = (Panel^)sender;
            card->BackColor = Color::FromArgb(30, 41, 59);
        }

        void UserCard_Click(Object^ sender, EventArgs^ e) {
            isAdmin = false;
            ShowAuthPanel();
        }

        void AdminCard_Click(Object^ sender, EventArgs^ e) {
            isAdmin = true;
            ShowAuthPanel();
        }

        void ShowAuthPanel() {
            this->Controls->Remove(rolePanel);
            delete rolePanel;

            authPanel = gcnew Panel();
            authPanel->Size = Drawing::Size(500, 500);
            authPanel->Location = Point((this->Width - 500) / 2, (this->Height - 500) / 2);
            authPanel->Anchor = AnchorStyles::None;
            authPanel->BackColor = Color::FromArgb(30, 41, 59);
            authPanel->Paint += gcnew PaintEventHandler(this, &LoginForm::AuthPanel_Paint);

            // Icon
            Label^ icon = gcnew Label();
            icon->Text = isAdmin ? "" : "";
            icon->Font = gcnew Drawing::Font("Impact", 28, FontStyle::Bold);
            icon->AutoSize = true;
            icon->Location = Point(220, 40);
            icon->BackColor = Color::Transparent;
            authPanel->Controls->Add(icon);

            // Title
            Label^ title = gcnew Label();
            title->Text = isAdmin ? "ADMIN LOGIN" : "USER LOGIN";
            title->Font = gcnew Drawing::Font("Impact", 28, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(130, 120);
            title->BackColor = Color::Transparent;
            authPanel->Controls->Add(title);

            // Username
            Label^ lblUser = gcnew Label();
            lblUser->Text ="USERNAME";
            lblUser->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            lblUser->ForeColor = Color::FromArgb(156, 163, 175);
            lblUser->AutoSize = true;
            lblUser->Location = Point(50, 190);
            lblUser->BackColor = Color::Transparent;
            authPanel->Controls->Add(lblUser);

            txtUsername = gcnew TextBox();
            txtUsername->Size = Drawing::Size(400, 35);
            txtUsername->Location = Point(50, 215);
            txtUsername->Font = gcnew Drawing::Font("Segoe UI", 12);
            txtUsername->BackColor = Color::FromArgb(15, 23, 42);
            txtUsername->ForeColor = Color::White;
            txtUsername->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            authPanel->Controls->Add(txtUsername);

            // Password
            Label^ lblPass = gcnew Label();
            lblPass->Text = "PASSWORD";
            lblPass->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            lblPass->ForeColor = Color::FromArgb(156, 163, 175);
            lblPass->AutoSize = true;
            lblPass->Location = Point(50, 270);
            lblPass->BackColor = Color::Transparent;
            authPanel->Controls->Add(lblPass);

            txtPassword = gcnew TextBox();
            txtPassword->Size = Drawing::Size(350, 35);
            txtPassword->Location = Point(50, 295);
            txtPassword->Font = gcnew Drawing::Font("Segoe UI", 12);
            txtPassword->BackColor = Color::FromArgb(15, 23, 42);
            txtPassword->ForeColor = Color::White;
            txtPassword->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            txtPassword->PasswordChar = L'*';
            authPanel->Controls->Add(txtPassword);

            Button^ btnTogglePass = gcnew Button();
            btnTogglePass->Text = "SHOW";
            btnTogglePass->Size = Drawing::Size(60, 29);
            btnTogglePass->Location = Point(410, 295);
            btnTogglePass->FlatStyle = FlatStyle::Flat;
            btnTogglePass->ForeColor = Color::White;
            btnTogglePass->BackColor = Color::FromArgb(30, 41, 59);
            btnTogglePass->FlatAppearance->BorderSize = 1;
            btnTogglePass->FlatAppearance->BorderColor = Color::FromArgb(100, 100, 100);
            btnTogglePass->Cursor = Cursors::Hand;
            btnTogglePass->Click += gcnew EventHandler(this, &LoginForm::TogglePass_Click);
            authPanel->Controls->Add(btnTogglePass);

            // Login Button
            Button^ btnLogin = gcnew Button();
            btnLogin->Text = "AUTHENTICATE";
            btnLogin->Size = Drawing::Size(400, 50);
            btnLogin->Location = Point(50, 360);
            btnLogin->FlatStyle = FlatStyle::Flat;
            btnLogin->BackColor = isAdmin ?
                Color::FromArgb(220, 38, 38) : Color::FromArgb(37, 99, 235);
            btnLogin->ForeColor = Color::White;
            btnLogin->Font = gcnew Drawing::Font("Segoe UI", 14, FontStyle::Bold);
            btnLogin->Cursor = Cursors::Hand;
            btnLogin->FlatAppearance->BorderSize = 0;
            btnLogin->Click += gcnew EventHandler(this, &LoginForm::Login_Click);
            authPanel->Controls->Add(btnLogin);

            // Forgot Password Button
            Button^ btnForgot = gcnew Button();
            btnForgot->Text = "FORGOT PASSWORD?";
            btnForgot->Size = Drawing::Size(150, 25);
            btnForgot->Location = Point(50, 332);
            btnForgot->FlatStyle = FlatStyle::Flat;
            btnForgot->FlatAppearance->BorderSize = 0;
            btnForgot->ForeColor = Color::FromArgb(156, 163, 175);
            btnForgot->BackColor = Color::Transparent;
            btnForgot->Cursor = Cursors::Hand;
            btnForgot->Click += gcnew EventHandler(this, &LoginForm::ForgotPassword_Click);
            authPanel->Controls->Add(btnForgot);

            // Back Button
            Button^ btnBack = gcnew Button();
            btnBack->Text = "← Back to Role Selection";
            btnBack->Size = Drawing::Size(200, 35);
            btnBack->Location = Point(150, 430);
            btnBack->FlatStyle = FlatStyle::Flat;
            btnBack->BackColor = Color::Transparent;
            btnBack->ForeColor = Color::FromArgb(156, 163, 175);
            btnBack->Font = gcnew Drawing::Font("Segoe UI", 10);
            btnBack->Cursor = Cursors::Hand;
            btnBack->FlatAppearance->BorderColor = Color::FromArgb(71, 85, 105);
            btnBack->Click += gcnew EventHandler(this, &LoginForm::Back_Click);
            authPanel->Controls->Add(btnBack);

            this->Controls->Add(authPanel);

            // Start scan animation
            scanTimer = gcnew Timer();
            scanTimer->Interval = 30;
            scanTimer->Tick += gcnew EventHandler(this, &LoginForm::ScanTimer_Tick);
            scanTimer->Start();
        }

        void ForgotPassword_Click(Object^ sender, EventArgs^ e) {
            String^ username = txtUsername->Text->Trim();
            if (username == "") {
                MessageBox::Show("Please enter your username first.", "Forgot Password", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            String^ secQ = "";
            bool found = false;

            if (isAdmin) {
                Admin** admins = nullptr; int count = 0, capacity = 0;
                DataManager::getInstance().loadAdmins(admins, count, capacity);
                for(int i=0; i<count; i++) {
                    if (gcnew String(admins[i]->getUsername()) == username) {
                        secQ = gcnew String(admins[i]->getSecurityQuestion());
                        found = true; break;
                    }
                }
                for(int i=0; i<count; i++) delete admins[i]; delete[] admins;
            } else {
                User** users = nullptr; int count = 0, capacity = 0;
                DataManager::getInstance().loadUsers(users, count, capacity);
                for(int i=0; i<count; i++) {
                    if (gcnew String(users[i]->getUsername()) == username) {
                        secQ = gcnew String(users[i]->getSecurityQuestion());
                        found = true; break;
                    }
                }
                for(int i=0; i<count; i++) delete users[i]; delete[] users;
            }

            if (!found) {
                MessageBox::Show("Username not found.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            scanTimer->Stop();
            this->Controls->Remove(authPanel);
            ShowRecoveryPanel(username, secQ);
        }

        void ShowRecoveryPanel(String^ username, String^ secQ) {
            Panel^ recPanel = gcnew Panel();
            recPanel->Size = Drawing::Size(500, 450);
            recPanel->Location = Point((this->Width - 500) / 2, (this->Height - 450) / 2);
            recPanel->BackColor = Color::FromArgb(30, 41, 59);
            recPanel->Name = "recPanel";
            
            Label^ title = gcnew Label();
            title->Text = "PASSWORD RECOVERY";
            title->Font = gcnew Drawing::Font("Impact", 28, FontStyle::Bold);
            title->ForeColor = Color::White;
            title->AutoSize = true;
            title->Location = Point(60, 40);
            recPanel->Controls->Add(title);

            Label^ lQ = gcnew Label();
            lQ->Text = "Security Question:\n" + secQ;
            lQ->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
            lQ->ForeColor = Color::FromArgb(245, 158, 11);
            lQ->AutoSize = true;
            lQ->Location = Point(50, 120);
            recPanel->Controls->Add(lQ);

            Label^ lA = gcnew Label();
            lA->Text = "Your Answer:";
            lA->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            lA->ForeColor = Color::FromArgb(156, 163, 175);
            lA->AutoSize = true;
            lA->Location = Point(50, 190);
            recPanel->Controls->Add(lA);

            TextBox^ tA = gcnew TextBox();
            tA->Name = "txtRecAns";
            tA->Size = Drawing::Size(400, 30);
            tA->Location = Point(50, 215);
            tA->Font = gcnew Drawing::Font("Segoe UI", 12);
            tA->BackColor = Color::FromArgb(15, 23, 42);
            tA->ForeColor = Color::White;
            tA->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            recPanel->Controls->Add(tA);

            Label^ lP = gcnew Label();
            lP->Text = "New Password:";
            lP->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            lP->ForeColor = Color::FromArgb(156, 163, 175);
            lP->AutoSize = true;
            lP->Location = Point(50, 260);
            recPanel->Controls->Add(lP);

            TextBox^ tP = gcnew TextBox();
            tP->Name = "txtRecPass";
            tP->Size = Drawing::Size(400, 30);
            tP->Location = Point(50, 285);
            tP->Font = gcnew Drawing::Font("Segoe UI", 12);
            tP->BackColor = Color::FromArgb(15, 23, 42);
            tP->ForeColor = Color::White;
            tP->PasswordChar = '*';
            tP->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            recPanel->Controls->Add(tP);

            Button^ btnSub = gcnew Button();
            btnSub->Text = "CHANGE PASSWORD";
            btnSub->Tag = username;
            btnSub->Size = Drawing::Size(400, 45);
            btnSub->Location = Point(50, 340);
            btnSub->BackColor = Color::FromArgb(37, 99, 235);
            btnSub->ForeColor = Color::White;
            btnSub->FlatStyle = FlatStyle::Flat;
            btnSub->Font = gcnew Drawing::Font("Segoe UI", 14, FontStyle::Bold);
            btnSub->Click += gcnew EventHandler(this, &LoginForm::DoRecovery_Click);
            recPanel->Controls->Add(btnSub);

            Button^ btnBack = gcnew Button();
            btnBack->Text = "← Cancel";
            btnBack->Size = Drawing::Size(100, 30);
            btnBack->Location = Point(200, 400);
            btnBack->FlatStyle = FlatStyle::Flat;
            btnBack->ForeColor = Color::FromArgb(156, 163, 175);
            btnBack->Click += gcnew EventHandler(this, &LoginForm::CancelRecovery_Click);
            recPanel->Controls->Add(btnBack);

            this->Controls->Add(recPanel);
        }

        void CancelRecovery_Click(Object^ sender, EventArgs^ e) {
            Control^ p = this->Controls->Find("recPanel", false)[0];
            this->Controls->Remove(p);
            ShowAuthPanel();
        }

        void DoRecovery_Click(Object^ sender, EventArgs^ e) {
            Button^ btn = (Button^)sender;
            String^ username = (String^)btn->Tag;
            
            Control^ p = this->Controls->Find("recPanel", false)[0];
            String^ ans = ((TextBox^)p->Controls->Find("txtRecAns", false)[0])->Text->Trim();
            String^ newPass = ((TextBox^)p->Controls->Find("txtRecPass", false)[0])->Text;

            if (ans == "" || newPass == "") {
                MessageBox::Show("Please provide both answer and new password.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            bool success = false;
            if (isAdmin) {
                Admin** admins = nullptr; int count = 0, capacity = 0;
                DataManager::getInstance().loadAdmins(admins, count, capacity);
                for(int i=0; i<count; i++) {
                    if (gcnew String(admins[i]->getUsername()) == username) {
                        if (gcnew String(admins[i]->getSecurityAnswer()) == ans) {
                            std::string np = msclr::interop::marshal_as<std::string>(newPass);
                            admins[i]->setPassword(np.c_str());
                            success = true;
                        }
                        break;
                    }
                }
                if(success) DataManager::getInstance().saveAdmins(admins, count);
                for(int i=0; i<count; i++) delete admins[i]; delete[] admins;
            } else {
                User** users = nullptr; int count = 0, capacity = 0;
                DataManager::getInstance().loadUsers(users, count, capacity);
                for(int i=0; i<count; i++) {
                    if (gcnew String(users[i]->getUsername()) == username) {
                        if (gcnew String(users[i]->getSecurityAnswer()) == ans) {
                            std::string np = msclr::interop::marshal_as<std::string>(newPass);
                            users[i]->setPassword(np.c_str());
                            success = true;
                        }
                        break;
                    }
                }
                if(success) DataManager::getInstance().saveUsers(users, count);
                for(int i=0; i<count; i++) delete users[i]; delete[] users;
            }

            if (success) {
                MessageBox::Show("Password changed successfully!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
                CancelRecovery_Click(nullptr, nullptr);
            } else {
                MessageBox::Show("Incorrect Security Answer.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void AuthPanel_Paint(Object^ sender, PaintEventArgs^ e) {
            Panel^ panel = (Panel^)sender;
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;

            // Border
            Color borderColor = isAdmin ?
                Color::FromArgb(220, 38, 38) : Color::FromArgb(37, 99, 235);
            Pen^ borderPen = gcnew Pen(borderColor, 3);
            g->DrawRectangle(borderPen, 0, 0, panel->Width - 1, panel->Height - 1);

            // Scan line
            Pen^ scanPen = gcnew Pen(Color::FromArgb(100, 0, 255, 255), 2);
            g->DrawLine(scanPen, 0, scanLine, panel->Width, scanLine);
        }

        void ScanTimer_Tick(Object^ sender, EventArgs^ e) {
            scanLine += 5;
            if (scanLine > authPanel->Height) scanLine = 0;
            authPanel->Invalidate();
        }

        void OnPaint(Object^ sender, PaintEventArgs^ e) {
            // Background effects can be added here
        }

        void TogglePass_Click(Object^ sender, EventArgs^ e) {
            if (txtPassword->PasswordChar == '*') {
                txtPassword->PasswordChar = '\0';
            } else {
                txtPassword->PasswordChar = '*';
            }
        }

        void Login_Click(Object^ sender, EventArgs^ e) {
            String^ username = txtUsername->Text->Trim()->ToLower();
            String^ password = txtPassword->Text;

            bool success = false;
            
            std::string u = msclr::interop::marshal_as<std::string>(username);
            std::string p = msclr::interop::marshal_as<std::string>(password);
            
            CrimeManagementSystem cms;
            if (isAdmin) {
                success = cms.adminLogin(u.c_str(), p.c_str());
            } else {
                success = cms.userLogin(u.c_str(), p.c_str());
            }

            // Demo logic fallback
            if (!success) {
                if (isAdmin && username == "admin" && password == "admin123") success = true;
            }

            if (success) {
                scanTimer->Stop();

                InstructionForm^ guide = gcnew InstructionForm(isAdmin);
                guide->ShowDialog();

                System::Type^ formType = mainForm->GetType();
                if (isAdmin) {
                    System::Reflection::MethodInfo^ showAdminMethod = formType->GetMethod("ShowAdminDashboard");
                    if (showAdminMethod != nullptr) {
                        showAdminMethod->Invoke(mainForm, nullptr);
                    }
                }
                else {
                    System::Reflection::MethodInfo^ showUserMethod = formType->GetMethod("ShowUserDashboard");
                    if (showUserMethod != nullptr) {
                        showUserMethod->Invoke(mainForm, nullptr);
                    }
                }
            }
            else {
                MessageBox::Show("Invalid credentials. Try again.", "Access Denied",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void Back_Click(Object^ sender, EventArgs^ e) {
            scanTimer->Stop();
            this->Controls->Remove(authPanel);
            delete authPanel;
            ShowRoleSelection();
        }
    };
}
