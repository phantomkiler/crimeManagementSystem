#pragma once
#include "LandingForm.h"
#include "LoginFormEnhanced.h"
#include "AdminDashboardComplete.h"
#include "UserDashboard.h"
#include "AdvancedFeaturesDashboard.h"

namespace CrimeManagementGUI {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class MainForm : public Form {
    private:
        Panel^ containerPanel;
        UserControl^ currentView;

    public:
        MainForm() {
            InitializeComponent();
            ShowLanding();
        }

    private:
        void InitializeComponent() {
            this->Text = L"Crime Record Management System - One Piece Edition";
            this->Size = Drawing::Size(1280, 720);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::FromArgb(11, 17, 27);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Sizable;
            this->MaximizeBox = true;
            this->WindowState = FormWindowState::Maximized;
            this->Icon = nullptr; // Add your icon here

            containerPanel = gcnew Panel();
            containerPanel->Dock = DockStyle::Fill;
            this->Controls->Add(containerPanel);
        }

        void ShowLanding() {
            SwitchView(gcnew LandingForm(this));
        }

    public:
        void ShowLogin() {
            SwitchView(gcnew LoginForm(this));
        }

        void ShowAdminDashboard() {
            SwitchView(gcnew AdminDashboard(this));
        }

        void ShowUserDashboard() {
            SwitchView(gcnew UserDashboard(this));
        }

        void ShowAdvancedFeatures() {
            SwitchView(gcnew AdvancedFeaturesDashboard(this));
        }

    private:
        void SwitchView(UserControl^ newView) {
            if (currentView != nullptr) {
                containerPanel->Controls->Remove(currentView);
                delete currentView;
            }
            currentView = newView;
            currentView->Dock = DockStyle::Fill;
            containerPanel->Controls->Add(currentView);
        }
    };
}