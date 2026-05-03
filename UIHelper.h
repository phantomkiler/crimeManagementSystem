#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Drawing::Text;
using namespace System::Windows::Forms;

namespace CrimeManagementGUI {

    public ref class UIHelper {
    public:
        // ── Colour Palette ──────────────────────────────────────────────────
        static property Color BgDeep { Color get() { return Color::FromArgb(11, 14, 20); } }   // #0B0E14
        static property Color BgDark { Color get() { return Color::FromArgb(16, 20, 28); } }
        static property Color BgCard { Color get() { return Color::FromArgb(22, 28, 36); } }
        static property Color Border { Color get() { return Color::FromArgb(45, 55, 72); } }
        static property Color AccentBlue { Color get() { return Color::FromArgb(0, 240, 255); } }  // Cyan
        static property Color AccentRed { Color get() { return Color::FromArgb(230, 57, 70); } }  // #E63946
        static property Color AccentGreen { Color get() { return Color::FromArgb(57, 255, 20); } }  // Neon Green
        static property Color TextPrimary { Color get() { return Color::White; } }
        static property Color TextSec { Color get() { return Color::FromArgb(156, 163, 175); } }
        static initonly Color BgRow = Color::FromArgb(18, 22, 30);
        static initonly Color AccentAmber = Color::FromArgb(245, 158, 11);
        static initonly Color AccentPurple = Color::FromArgb(139, 92, 246);


        // ── Rounded Rectangle Path ──────────────────────────────────────────
        static GraphicsPath^ RoundedRect(Drawing::Rectangle r, int rad) {
            GraphicsPath^ p = gcnew GraphicsPath();
            if (rad <= 0) {
                p->AddRectangle(r);
                return p;
            }
            p->AddArc(r.X, r.Y, rad * 2, rad * 2, 180, 90);
            p->AddArc(r.Right - rad * 2, r.Y, rad * 2, rad * 2, 270, 90);
            p->AddArc(r.Right - rad * 2, r.Bottom - rad * 2, rad * 2, rad * 2, 0, 90);
            p->AddArc(r.X, r.Bottom - rad * 2, rad * 2, rad * 2, 90, 90);
            p->CloseFigure();
            return p;
        }

        // ── Fill Rounded Panel ──────────────────────────────────────────────
        static void FillRounded(Graphics^ g, Drawing::Rectangle r, int rad,
            Color fill, bool border, Color bColor, float bw) {
            g->SmoothingMode = SmoothingMode::AntiAlias;

            GraphicsPath^ path = RoundedRect(r, rad);
            SolidBrush^ brush = gcnew SolidBrush(fill);

            g->FillPath(brush, path);

            delete brush;

            if (border) {
                Pen^ pen = gcnew Pen(bColor, bw);
                g->DrawPath(pen, path);
                delete pen;
            }

            delete path;
        }

        // Overload without border
        static void FillRounded(Graphics^ g, Drawing::Rectangle r, int rad, Color fill) {
            FillRounded(g, r, rad, fill, false, Color::Transparent, 0);
        }

        // ── Draw Card Background (call from Paint event) ────────────────────
        static void DrawCard(Graphics^ g, int w, int h, int rad) {
            g->SmoothingMode = SmoothingMode::AntiAlias;
            FillRounded(g, Drawing::Rectangle(0, 0, w - 1, h - 1), rad, BgCard, true, Border, 1.0f);
        }

        // ── Soft Tag Badge ──────────────────────────────────────────────────
        static void PaintBadge(DataGridViewCellPaintingEventArgs^ e, Color baseColor, String^ text, int width) {
            e->PaintBackground(e->CellBounds, true);
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;

            Color softBg = Color::FromArgb(35, baseColor);

            Drawing::Rectangle r = e->CellBounds;
            r.Y += 8;
            r.Height -= 16;
            r.X += 5;
            r.Width = width;

            GraphicsPath^ p = RoundedRect(r, r.Height / 2);
            g->FillPath(gcnew SolidBrush(softBg), p);
            g->DrawPath(gcnew Pen(Color::FromArgb(80, baseColor), 1.0f), p);

            Drawing::Font^ f = gcnew Drawing::Font("Segoe UI", 7.5f, FontStyle::Bold);
            StringFormat^ sf = gcnew StringFormat();
            sf->Alignment = StringAlignment::Center;
            sf->LineAlignment = StringAlignment::Center;
            g->DrawString(text, f, gcnew SolidBrush(baseColor), r, sf);

            e->Handled = true;
        }

        static void PaintBadge(DataGridViewCellPaintingEventArgs^ e, Color baseColor, String^ text) {
            PaintBadge(e, baseColor, text, 120);
        }

        // ── Styled DataGridView ─────────────────────────────────────────────
        static DataGridView^ StyledGrid() {
            DataGridView^ g = gcnew DataGridView();
            g->BackgroundColor = BgCard;
            g->BorderStyle = BorderStyle::None;
            g->CellBorderStyle = DataGridViewCellBorderStyle::SingleHorizontal;
            g->GridColor = Border;
            g->RowHeadersVisible = false;
            g->AllowUserToAddRows = false;
            g->AllowUserToDeleteRows = false;
            g->AllowUserToResizeRows = false;
            g->ReadOnly = true;
            g->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            g->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            g->Font = gcnew Drawing::Font("Segoe UI", 9.0f);
            g->RowTemplate->Height = 45;
            g->EnableHeadersVisualStyles = false;
            g->ColumnHeadersBorderStyle = DataGridViewHeaderBorderStyle::None;
            g->ColumnHeadersHeight = 45;
            g->ScrollBars = ScrollBars::Vertical;

            g->DefaultCellStyle->BackColor = BgCard;
            g->DefaultCellStyle->ForeColor = TextPrimary;
            g->DefaultCellStyle->SelectionBackColor = BgRow;
            g->DefaultCellStyle->SelectionForeColor = TextPrimary;
            g->DefaultCellStyle->Padding = Padding(10, 0, 0, 0);

            g->AlternatingRowsDefaultCellStyle->BackColor = BgRow;
            g->AlternatingRowsDefaultCellStyle->ForeColor = TextPrimary;
            g->AlternatingRowsDefaultCellStyle->SelectionBackColor = BgRow;
            g->AlternatingRowsDefaultCellStyle->SelectionForeColor = TextPrimary;

            g->ColumnHeadersDefaultCellStyle->BackColor = BgDeep;
            g->ColumnHeadersDefaultCellStyle->ForeColor = TextSec;
            g->ColumnHeadersDefaultCellStyle->Font = gcnew Drawing::Font("Segoe UI", 9.0f, FontStyle::Bold);
            g->ColumnHeadersDefaultCellStyle->Padding = Padding(10, 0, 0, 0);
            g->ColumnHeadersDefaultCellStyle->SelectionBackColor = BgDeep;
            return g;
        }

        // ── Text Column Helper ──────────────────────────────────────────────
        static DataGridViewTextBoxColumn^ Col(String^ header, int width) {
            DataGridViewTextBoxColumn^ c = gcnew DataGridViewTextBoxColumn();
            c->HeaderText = header;
            c->Width = width;
            return c;
        }

        // ── Action Button ───────────────────────────────────────────────────
        static Button^ ActionBtn(String^ text, Color bg, int x, int y, int w, int h) {
            Button^ b = gcnew Button();
            b->Text = text;
            b->Location = Point(x, y);
            b->Size = Drawing::Size(w, h);
            b->FlatStyle = FlatStyle::Flat;
            b->BackColor = bg;
            b->ForeColor = Color::White;
            b->Font = gcnew Drawing::Font("Segoe UI", 9.0f, FontStyle::Bold);
            b->Cursor = Cursors::Hand;
            b->TextAlign = ContentAlignment::MiddleCenter;
            b->FlatAppearance->BorderSize = 0;
            b->FlatAppearance->MouseOverBackColor = ControlPaint::Light(bg, 0.15f);
            return b;
        }

        // ── Styled TextBox ──────────────────────────────────────────────────
        static TextBox^ StyledTB(int x, int y, int w, String^ ph, bool ro) {
            TextBox^ t = gcnew TextBox();
            t->Location = Point(x, y);
            t->Size = Drawing::Size(w, 36);
            t->BorderStyle = BorderStyle::FixedSingle;
            t->BackColor = ro ? Color::FromArgb(12, 18, 32) : Color::FromArgb(15, 23, 42);
            t->ForeColor = ro ? TextSec : TextPrimary;
            t->Font = gcnew Drawing::Font("Segoe UI", 10.0f);

            t->ReadOnly = ro;
            return t;
        }

        static TextBox^ StyledTB(int x, int y, int w, String^ ph) {
            return StyledTB(x, y, w, ph, false);
        }

        // ── Styled ComboBox ─────────────────────────────────────────────────
        static ComboBox^ StyledCB(int x, int y, int w, array<String^>^ items) {
            ComboBox^ cb = gcnew ComboBox();
            cb->Location = Point(x, y);
            cb->Size = Drawing::Size(w, 36);
            cb->FlatStyle = FlatStyle::Flat;
            cb->BackColor = Color::FromArgb(15, 23, 42);
            cb->ForeColor = TextPrimary;
            cb->Font = gcnew Drawing::Font("Segoe UI", 10.0f);
            cb->DropDownStyle = ComboBoxStyle::DropDownList;
            for each (String ^ item in items) cb->Items->Add(item);
            if (cb->Items->Count > 0) cb->SelectedIndex = 0;
            return cb;
        }

        // ── Labels ──────────────────────────────────────────────────────────
        static Label^ SectionTitle(String^ text, int x, int y) {
            Label^ l = gcnew Label();
            l->Text = text;
            l->Location = Point(x, y);
            l->AutoSize = true;
            l->Font = gcnew Drawing::Font("Segoe UI", 13.0f, FontStyle::Bold);
            l->ForeColor = TextPrimary;
            l->BackColor = Color::Transparent;
            return l;
        }

        static Label^ SubLabel(String^ text, int x, int y) {
            Label^ l = gcnew Label();
            l->Text = text;
            l->Location = Point(x, y);
            l->AutoSize = true;
            l->Font = gcnew Drawing::Font("Segoe UI", 9.0f);
            l->ForeColor = TextSec;
            l->BackColor = Color::Transparent;
            return l;
        }

        static Label^ FieldLabel(String^ text, int x, int y) {
            Label^ l = gcnew Label();
            l->Text = text;
            l->Location = Point(x, y);
            l->AutoSize = true;
            l->Font = gcnew Drawing::Font("Segoe UI", 8.0f, FontStyle::Bold);
            l->ForeColor = TextSec;
            l->BackColor = Color::Transparent;
            return l;
        }
    };
}
