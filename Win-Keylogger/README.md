
# Technical Analysis: Windows Persistence & Keystroke Logger (C++)

## 🔍 Overview
This project is a functional research sample of a Windows-based keystroke logger. It was developed to study common malware behaviors, specifically **Registry-based persistence**, **process hiding**, and **unprivileged data exfiltration**.

## 🛠️ Technical Features & Analysis

### 1. Persistence Mechanism (`RegisterPersistence`)
The sample implements persistence by modifying the Windows Registry.
*   **Technique:** T1547.001 (MITRE ATT&CK - Boot or Logon Autostart Execution).
*   **Implementation:** It utilizes the `RegSetValueExA` API to add the executable path to `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`.
*   **Analyst Note:** This allows the process to start automatically every time the current user logs in without requiring Administrator privileges.

### 2. Stealth & Evasion
*   **Window Hiding:** Uses `ShowWindow(GetConsoleWindow(), SW_HIDE)` to detach the process from the visible UI, making it a background process.
*   **Attribute Manipulation:** Uses `SetFileAttributesA` with `FILE_ATTRIBUTE_HIDDEN` on `data.txt`. This is a basic evasion technique to hide the log file from a standard user viewing the directory.

### 3. Keystroke Interception
*   **Mechanism:** Implements a polling loop using `GetAsyncKeyState`. 
*   **Logic:** It iterates through virtual key codes (8 to 190) and logs them to a local file. This demonstrates an understanding of the **Win32 Input System**.

## 🛡️ Defensive Indicators (IOCs)
An analyst or Blue Team member can detect this activity via:
- **Registry:** Monitoring for new values in the `...\CurrentVersion\Run` key, specifically the name `WindowsUpdateTask`.
- **Filesystem:** Presence of a hidden file named `data.txt` in the execution directory.
- **Behavioral:** High frequency of `GetAsyncKeyState` calls from a background process.

## ⚠️ Ethical & Safety Disclaimer
**FOR RESEARCH PURPOSES ONLY.** This repository is intended for Malware Analysts and Security Researchers to understand threat actor methodologies. Handling or executing this code outside of a controlled sandbox environment is not recommended.
