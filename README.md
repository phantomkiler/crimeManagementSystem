# 🔍 Crime Record Management System
### *One Piece Edition*

> A full-featured desktop application for managing criminal records, suspects, officers, cases, evidence, and crime reports — built with **C++/CLI** and **Windows Forms**.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Screenshots & UI](#screenshots--ui)
- [Project Structure](#project-structure)
- [Classes & Architecture](#classes--architecture)
- [Advanced Algorithms](#advanced-algorithms)
- [Data Storage](#data-storage)
- [Getting Started](#getting-started)
- [OOP Concepts Used](#oop-concepts-used)
- [Team & Credits](#team--credits)

---

## Overview

The **Crime Record Management System** is a Windows desktop application that gives law enforcement a centralized platform to:

- Register and authenticate **admins** and **citizen users**
- File, track, and update **crime reports**
- Manage **suspects**, **officers**, and **cases**
- Log and verify **evidence** cryptographically
- Run **advanced AI-inspired algorithms** (threat triage, syndicate mapping, biometric matching, crime prediction)
- Maintain full **audit trails** and **session logs**

The system has two layers:
- **GUI Layer** — A dark-themed Windows Forms interface (C++/CLI managed code)
- **Backend Layer** — A pure native C++ engine with file-based persistence

---

## Features

| # | Feature | Description |
|---|---------|-------------|
| 1 | **User & Admin Authentication** | Separate login flows for citizens and administrators with security Q&A password recovery |
| 2 | **Crime Report Filing** | Citizens file reports; admins assign officers and update statuses |
| 3 | **Suspect Management** | Add, view, archive suspects with wanted-poster style cards |
| 4 | **Officer Management** | Recruit officers, track availability, assign to cases |
| 5 | **Case Management** | Open/close cases, link officers, track investigation status |
| 6 | **Criminal Records** | Detailed 50-field criminal profiles with full legal history |
| 7 | **Evidence Cataloguing** | Log evidence items with FNV-1a cryptographic hashes |
| 8 | **Complaint / FIR System** | Formal complaint filing with tracking numbers |
| 9 | **Wanted Poster Cards** | Visual suspect display with photo, bounty, and danger level |
| 10 | **Anonymous Crime Reporting** | Tip submission with no identity required |
| 11 | **Crime Statistics Dashboard** | Real-time counts of open, closed, and active cases |
| 12 | **File Persistence** | All data saved to and loaded from structured text files |
| 13 | **Medical Profile Management** | Blood group, allergies, mental health, physical marks per suspect |
| 14 | **Kinship / Family Mapping** | Record and display family members linked to suspects |
| 15 | **Audit Logging** | Every admin action timestamped and appended to audit_log.txt |
| 16 | **Session Tracking** | Login/logout history with role and timestamps |
| 17 | **Archive System** | Safe archival of removed suspects, cases, and retired officers |

---

## Screenshots & UI

The application uses a custom dark cyberpunk theme built entirely with GDI+ paint code (no image assets for UI chrome):

```
Colour Palette:
  Background Deep  →  RGB(11,  14,  20)   #0B0E14
  Background Card  →  RGB(22,  28,  36)   #161C24
  Accent Cyan      →  RGB(0,  240, 255)   #00F0FF
  Accent Red       →  RGB(230, 57,  70)   #E63946
  Accent Green     →  RGB(57,  255, 20)   #39FF14
  Accent Amber     →  RGB(245, 158, 11)   #F59E0B
```

**Screens:**
- `Landing Form` — Animated scan-line splash screen with fade-in title
- `Login Form` — Role selection (Admin / User / Sign Up) with animated background
- `Admin Dashboard` — Sidebar navigation with 9 management panels
- `User Dashboard` — Simplified view for filing and tracking own reports
- `Advanced Features Dashboard` — GUI for all 6 algorithm tools
- `Wanted Poster Cards` — Photo cards for each suspect with danger badges

---

## Project Structure

```
CrimeManagementSystem/
│
├── main.cpp                        # App entry point — starts Windows Forms
├── CrimeManagementMain.cpp         # Console subsystem with text menus
│
├── CrimeManagement.h               # ALL backend class declarations
├── CrimeManagement.cpp             # ALL backend class implementations (~2000 lines)
│
├── AdvancedFeatures.h              # Advanced algorithm class declarations
├── AdvancedFeatures.cpp            # Advanced algorithm implementations
│
├── MainForm.h                      # Root window — swaps views in container
├── LandingForm.h                   # Animated intro/splash screen
├── LoginFormEnhanced.h             # Login, sign-up, password recovery
├── AdminDashboardComplete.h        # Full admin panel
├── UserDashboard.h                 # Citizen/officer panel
├── AdvancedFeaturesDashboard.h     # GUI for 6 advanced algorithms
├── WantedPosterCard.h              # Suspect card renderer + SuspectData struct
│
├── UIHelper.h                      # Shared colours, rounded-rect helpers, factories
├── AddOfficerForm.h                # Pop-up dialog: recruit new officer
├── AddSuspectForm.h                # Pop-up dialog: add new suspect
├── AssignOfficerForm.h             # Pop-up dialog: assign officer to case
├── InstructionForm.h               # Pop-up quick-start guide
│
├── admins.txt                      # Saved admin accounts
├── users.txt                       # Saved user accounts
├── crime_reports.txt               # Filed crime reports
├── officers.txt                    # Active officers
├── suspects_db.txt                 # Suspect records (pipe-separated)
├── cases_db.txt                    # Case records
├── evidence_log.txt                # Evidence entries with hashes
├── audit_log.txt                   # Timestamped admin action log
├── session_logs.txt                # Login/logout history
├── archived_suspects.txt           # Archived (removed) suspects
├── archived_cases.txt              # Archived (closed) cases
│
└── x64/Debug/
    ├── CrimeManagementSystem.exe   # Compiled executable
    └── Images/                     # Suspect photographs (S001.png, S002.png …)
```

---

## Classes & Architecture

### Backend Classes (`CrimeManagement.h / .cpp`)

```
Person  (abstract)
├── Suspect          — name, age, crime committed
└── Officer          — badge number (auto), rank, availability

Case                 — caseID (auto), criminal, crime type, status, officer pointer
CaseDatabase         — dynamic array of Case*, auto-resizing

CriminalRecord       — 50+ fields (personal, physical, legal, audit) using char*
Complaint            — FIR with tracking number and full complainant details
CrimeReport          — Lightweight citizen-filed report

User                 — username, password, security Q&A
Admin                — extends User with badge number and police station

SessionManager       — Singleton: tracks current logged-in user and role
DataManager          — Singleton: handles all file I/O for users/admins/reports

CrimeManagementSystem  — Master controller owning all arrays and menus
Evidence             — evidenceID, suspect link, description, FNV-1a hash
MedicalProfile       — blood group, allergies, mental health, physical marks
FamilyRecord         — dynamic array of FamilyMember (name, relation, contact)
AnonymousReport      — anonymous tip with auto-ID and status
StatsDashboard       — displays case count statistics
FilePersistence      — save/load for medical profiles and anon reports
```

### GUI Classes (`*.h` Windows Forms)

```
MainForm             — root Form; holds containerPanel; calls SwitchView()
LandingForm          — animated splash; Timer-driven scan line + fade-in
LoginForm            — role cards → login/register form → credential check
AdminDashboard       — sidebar nav; 9 content panels loaded on demand
UserDashboard        — 4-tab panel for citizen interactions
AdvancedFeaturesDashboard — 6 algorithm tabs with inputs and result display
WantedPosterCard     — custom-painted suspect tile with photo and badges
UIHelper             — static colour constants + drawing/factory helpers
AddOfficerForm       — modal dialog returning OfficerName/Age/Rank
AddSuspectForm       — modal dialog returning SuspectName/Age/Crime
AssignOfficerForm    — modal dialog returning SelectedOfficerName
InstructionForm      — role-aware help text in a RichTextBox
```

---

## Advanced Algorithms

All six live in the `AdvancedFeatures` namespace:

### 1. 🕸️ Graph-Based Syndicate Mapping
Uses an **undirected graph** (adjacency list via `unordered_map`) and **Breadth-First Search** to map criminal networks. Given any person's name, traces all connected associates level by level.

```
addEdge("Luffy", "Zoro")  →  bidirectional link
getNetworkBFS("Luffy")    →  ["Luffy", "Zoro", "Nami", ...]
```

### 2. 🔐 Cryptographic Evidence Chain (FNV-1a)
Implements the **FNV-1a** (Fowler–Noll–Vo) hash to fingerprint evidence descriptions. The hash is stored alongside the evidence; re-hashing later detects any tampering.

```
hash = calculateFNV1a("Blood sample from scene 4")
verifyEvidence("Blood sample from scene 4", hash)  →  true / false
```

### 3. 🧬 Fuzzy Biometric Matching (Levenshtein)
Computes **edit distance** between two strings using dynamic programming, then converts to a similarity percentage. Used to match partial or misspelled names.

```
getSimilarityPercentage("Monkey D. Luffy", "Monkey D Lufy")  →  93.3%
```

### 4. 🗺️ Predictive Crime Mapping
Counts crime frequency per location using a frequency map, sorts by count, and returns the **top 5 hotspot zones**.

```
getHighRiskZones(["Market St", "Port", "Market St", "Alley 3", "Port", "Market St"])
→ ["Market St", "Port", "Alley 3"]
```

### 5. ⚠️ Automated Threat Triage
Scores a crime description using a **weighted keyword dictionary**. High-risk words like "murder" (90 pts) or "gun" (50 pts) push the score up; vague words like "suspicious" (10 pts) add minimally.

```
calculateThreatScore("Armed robbery with a gun")  →  80
calculateThreatScore("Suspicious noise heard")    →  15
```

### 6. 👁️ RBAC Activity Anomaly Detection
Tracks action counts per user within a **2-minute sliding window**. If a user exceeds 10 actions in that window, an anomaly is flagged — indicating a bot or insider threat.

```
logAndCheckAnomaly("admin1", "DELETE_RECORD")  →  false (1st action)
... (11th action within 2 min) ...             →  true  (ANOMALY!)
```

---

## Data Storage

All data is persisted as **plain text files** in the executable's directory. No database engine is required.

| File | Format | Contents |
|------|--------|----------|
| `admins.txt` | One field per line | Admin credentials + badge + station |
| `users.txt` | One field per line | User credentials + security Q&A |
| `crime_reports.txt` | One field per line | Crime report details |
| `officers.txt` | One field per line | Badge, name, age, rank |
| `cases_db.txt` | One field per line | Case ID, criminal, crime, status |
| `suspects_db.txt` | Pipe-separated `\|` | All 14 suspect fields per line |
| `evidence_log.txt` | One field per line | Evidence ID, suspect, description, hash |
| `audit_log.txt` | Append-only | `[timestamp] ACTION : details` |
| `session_logs.txt` | One field per line | Username, role, login time, logout time |
| `archived_suspects.txt` | One field per line | Name, crime, reason, date removed |
| `archived_cases.txt` | One field per line | Case ID, criminal, crime, reason, date |
| `anon_reports_db.txt` | One field per line | Anonymous report ID, description, location, status |

---

## Getting Started

### Requirements

- **Windows 10/11** (x64)
- **Visual Studio 2022** with:
  - Desktop development with C++ workload
  - .NET Framework 4.7.2 SDK
  - C++/CLI support component

### Build & Run

```
1. Open CrimeManagementSystem.slnx in Visual Studio 2022

2. Set configuration to:  Debug | x64

3. Build → Build Solution   (Ctrl + Shift + B)

4. Run →  Start Without Debugging   (Ctrl + F5)
```

The executable is placed at:
```
x64/Debug/CrimeManagementSystem.exe
```

Make sure the `Images/` folder (with suspect photos) is in the same directory as the `.exe` before running.

### Default Login Credentials

| Role | Username | Password |
|------|----------|----------|
| Admin | `admin` | `admin123` |
| User  | (register via Sign Up screen) | — |

> **Note:** Credentials are stored as plain text in `admins.txt` and `users.txt`. This is intentional for an academic/demo project.

---

## OOP Concepts Used

| Concept | Where Used |
|---------|-----------|
| **Inheritance** | `Person → Suspect`, `Person → Officer` |
| **Polymorphism** | `Person::display()` is pure virtual; overridden in both subclasses |
| **Encapsulation** | All data members private; accessed via public getters/setters |
| **Abstraction** | `Person` is an abstract class (cannot be instantiated directly) |
| **Method Chaining** | `MedicalProfile`, `CriminalRecord`, `Case` setters return `*this` |
| **Singleton Pattern** | `SessionManager` and `DataManager` — one global instance each |
| **Dynamic Memory** | `new` / `delete` with manual resize (doubling strategy) throughout |
| **Deep Copy** | Custom copy constructor + `operator=` in `CriminalRecord`, `Complaint`, `User`, `Admin` |
| **Operator Overloading** | `Case::operator==(int)` and `Case::operator==(string)` for search |
| **Static Members** | `Officer::badgeCounter`, `Case::caseCounter`, `AnonymousReport::counter` |
| **Templates / STL** | `vector`, `unordered_map`, `queue` in AdvancedFeatures |
| **File I/O** | `fstream` for reading/writing all persistence files |
| **C++/CLI Managed** | GUI layer uses `^` handles, `gcnew`, `ref class` for .NET interop |

---

## Team & Credits

- Built as a **university final project** — Object-Oriented Programming course
- Theme inspired by the **One Piece** anime universe
- UI designed with a custom **dark cyberpunk** aesthetic using GDI+ painting

---

*"Justice will prevail, you say? Of course it will! Whoever wins this war becomes justice!"*
