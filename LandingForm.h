
#pragma once
#include "UIHelper.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Drawing::Text;

namespace CrimeManagementGUI {

    public ref class LandingForm : public UserControl {
    private:
        Form^ mainForm;
        Timer^ animTimer;
        float scanY;
        float opacity;
        Button^ enterButton;

    public:
        LandingForm(Form^ parent) {
            mainForm = parent;
            scanY = 0;
            opacity = 0;
            InitializeComponent();
            StartAnimations();
        }

    private:
        void InitializeComponent() {
            this->Size = Drawing::Size(1280, 720);
            this->BackColor = Color::FromArgb(11, 17, 27);
            this->DoubleBuffered = true;

            this->Paint += gcnew PaintEventHandler(this, &LandingForm::OnPaint);

            // Enter Button - Centered
            enterButton = gcnew Button();
            enterButton->Text = " ENTER SYSTEM ";
            enterButton->Size = Drawing::Size(300, 80);
            enterButton->Location = Point((this->Width - 300) / 2, this->Height - 150);
            enterButton->Anchor = AnchorStyles::Bottom;
            enterButton->FlatStyle = FlatStyle::Flat;
            enterButton->BackColor = Color::FromArgb(220, 38, 38);
            enterButton->ForeColor = Color::White;
            enterButton->Font = gcnew Drawing::Font("Segoe UI", 16, FontStyle::Bold);
            enterButton->Cursor = Cursors::Hand;
            enterButton->FlatAppearance->BorderSize = 4;
            enterButton->FlatAppearance->BorderColor = Color::FromArgb(153, 27, 27);
            enterButton->Click += gcnew EventHandler(this, &LandingForm::OnEnterClick);

            // Hover effects
            enterButton->MouseEnter += gcnew EventHandler(this, &LandingForm::Button_MouseEnter);
            enterButton->MouseLeave += gcnew EventHandler(this, &LandingForm::Button_MouseLeave);

            this->Controls->Add(enterButton);
        }

        void StartAnimations() {
            animTimer = gcnew Timer();
            animTimer->Interval = 50;
            animTimer->Tick += gcnew EventHandler(this, &LandingForm::AnimTimer_Tick);
            animTimer->Start();
        }

        void AnimTimer_Tick(Object^ sender, EventArgs^ e) {
            scanY += 5;
            if (scanY > this->Height) scanY = 0;

            if (opacity < 1.0f) opacity += 0.02f;

            this->Invalidate();
        }

        void OnPaint(Object^ sender, PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            g->TextRenderingHint = TextRenderingHint::AntiAliasGridFit;

            // Background Grid
            Pen^ gridPen = gcnew Pen(Color::FromArgb(25, 220, 38, 38), 1);
            for (int x = 0; x < this->Width; x += 50) {
                g->DrawLine(gridPen, x, 0, x, this->Height);
            }
            for (int y = 0; y < this->Height; y += 50) {
                g->DrawLine(gridPen, 0, y, this->Width, y);
            }

            // Scanning Line
            Pen^ scanPen = gcnew Pen(Color::FromArgb(100, 220, 38, 38), 2);
            g->DrawLine(scanPen, 0, (int)scanY, this->Width, (int)scanY);

            // WANTED POSTER - Center
            DrawWantedPoster(g);

            // Stats at bottom
            DrawStats(g);

            // Warning text
            DrawWarning(g);
        }

        void DrawWantedPoster(Graphics^ g) {
            int posterW = 600;
            int posterH = 400;
            int posterX = (this->Width - posterW) / 2;
            int posterY = (this->Height - posterH) / 2 - 50;

            // Poster background - aged paper
            Drawing::Rectangle posterRect(posterX, posterY, posterW, posterH);
            LinearGradientBrush^ paperBrush = gcnew LinearGradientBrush(
                posterRect,
                Color::FromArgb(255, 245, 222), // Cream
                Color::FromArgb(255, 235, 205), // Beige
                90.0f
            );
            g->FillRectangle(paperBrush, posterRect);

            // Border - double line
            Pen^ borderPen = gcnew Pen(Color::FromArgb(101, 67, 33), 8);
            g->DrawRectangle(borderPen, posterRect);
            g->DrawRectangle(gcnew Pen(Color::FromArgb(139, 90, 43), 2),
                posterX + 10, posterY + 10, posterW - 20, posterH - 20);

            // WANTED header
            Drawing::Rectangle wantedRect(posterX, posterY + 20, posterW, 80);
            g->FillRectangle(gcnew SolidBrush(Color::FromArgb(139, 0, 0)), wantedRect);

            Drawing::Font^ wantedFont = gcnew Drawing::Font("Impact", 48, FontStyle::Bold);
            StringFormat^ centerFormat = gcnew StringFormat();
            centerFormat->Alignment = StringAlignment::Center;
            centerFormat->LineAlignment = StringAlignment::Center;

            g->DrawString("WANTED", wantedFont, Brushes::White,
                RectangleF((float)posterX, (float)(posterY + 20), (float)posterW, 80.0f),
                centerFormat);

            // CRIME RECORDS title
            Drawing::Font^ titleFont = gcnew Drawing::Font("Impact", 72, FontStyle::Bold);
            g->DrawString("CRIME", titleFont,
                gcnew SolidBrush(Color::FromArgb(0, 0, 0)),
                RectangleF((float)posterX, (float)(posterY + 120), (float)posterW, 80.0f),
                centerFormat);

            Drawing::Font^ titleFont2 = gcnew Drawing::Font("Impact", 56, FontStyle::Bold);
            g->DrawString("RECORDS", titleFont2,
                gcnew SolidBrush(Color::FromArgb(139, 0, 0)),
                RectangleF((float)posterX, (float)(posterY + 200), (float)posterW, 80.0f),
                centerFormat);

            // Divider lines
            Pen^ divPen = gcnew Pen(Color::FromArgb(139, 0, 0), 3);
            g->DrawLine(divPen, posterX + 150, posterY + 110, posterX + posterW - 150, posterY + 110);
            g->DrawLine(divPen, posterX + 100, posterY + 290, posterX + posterW - 100, posterY + 290);

            // Subtitle
            Drawing::Font^ subFont = gcnew Drawing::Font("Arial", 18, FontStyle::Bold);
            g->DrawString("MANAGEMENT SYSTEM", subFont,
                gcnew SolidBrush(Color::FromArgb(0, 0, 0)),
                RectangleF((float)posterX, (float)(posterY + 300), (float)posterW, 40.0f),
                centerFormat);

            // Warning
            Drawing::Font^ warnFont = gcnew Drawing::Font("Courier New", 10, FontStyle::Bold);
            g->DrawString(" CLASSIFIED ACCESS ONLY ", warnFont,
                gcnew SolidBrush(Color::FromArgb(101, 67, 33)),
                RectangleF((float)posterX, (float)(posterY + 350), (float)posterW, 30.0f),
                centerFormat);
        }

        void DrawStats(Graphics^ g) {
            array<String^>^ labels = { "Cases Tracked", "Suspects", "Solved" };
            array<String^>^ values = { "1,247", "856", "92%" };

            int startX = (this->Width - 500) / 2;
            int y = (this->Height - 400) / 2 + 380;

            Drawing::Font^ valFont = gcnew Drawing::Font("Impact", 32, FontStyle::Bold);
            Drawing::Font^ lblFont = gcnew Drawing::Font("Arial", 10, FontStyle::Bold);

            for (int i = 0; i < 3; i++) {
                int x = startX + (i * 200);

                // Value
                SizeF valSize = g->MeasureString(values[i], valFont);
                g->DrawString(values[i], valFont,
                    gcnew SolidBrush(Color::FromArgb(220, 38, 38)),
                    (float)x, (float)y);

                // Label
                g->DrawString(labels[i], lblFont,
                    gcnew SolidBrush(Color::FromArgb(156, 163, 175)),
                    (float)x, (float)(y + 40));
            }
        }

        void DrawWarning(Graphics^ g) {
            Drawing::Font^ warnFont = gcnew Drawing::Font("Consolas", 9, FontStyle::Bold);
            String^ warning = " AUTHORIZED PERSONNEL ONLY • SECURITY LEVEL: MAXIMUM ";
            SizeF textSize = g->MeasureString(warning, warnFont);

            g->DrawString(warning, warnFont,
                gcnew SolidBrush(Color::FromArgb(180, 245, 158, 11)),
                (this->Width - textSize.Width) / 2,
                this->Height - 40);
        }

        void Button_MouseEnter(Object^ sender, EventArgs^ e) {
            enterButton->BackColor = Color::FromArgb(239, 68, 68);
        }

        void Button_MouseLeave(Object^ sender, EventArgs^ e) {
            enterButton->BackColor = Color::FromArgb(220, 38, 38);
        }

        void OnEnterClick(Object^ sender, EventArgs^ e) {
            animTimer->Stop();
            System::Type^ formType = mainForm->GetType();
            System::Reflection::MethodInfo^ showLoginMethod = formType->GetMethod("ShowLogin");
            if (showLoginMethod != nullptr) {
                showLoginMethod->Invoke(mainForm, nullptr);
            }
        }
    };
}