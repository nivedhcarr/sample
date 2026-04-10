# Malware Analysis & Reverse Engineering Research Lab 🛡️

## 🔍 Professional Overview
This repository serves as a technical portfolio focused on **Windows Internals**, **Malware Evasion**, and **Dynamic Binary Instrumentation**. Each project is designed to deconstruct the methodologies used by modern threat actors and explore defensive countermeasures.

---

## 📂 Project Directory

### 1. [Custom API Hooking Tool](./Custom%20API%20Hooking%20Tool)
*   **Purpose:** Real-time interception of System Calls (NTAPI/Win32).
*   **Highlights:** Intercepts `NtQuerySystemInformation` and `CreateFileW`.

### 2. [Anti Debugging Library](./Anti%20Debugging%20Library)
*   **Purpose:** A multi-vector protection library to detect and neutralize analysis environments.
*   **Techniques:** Uses `readgsqword` for direct PEB access, Heap Flag analysis, and `ThreadHideFromDebugger` active defense.
*   **Target:** Bypassing researcher instrumentation and automated sandboxes.

### 3. [Win-Keylogger](./Win-Keylogger)
*   **Purpose:** Research into User-mode API hooking and registry-based persistence.
*   **Techniques:** Implements `SetWindowsHookEx` and `RegSetValueExA` for boot-time survival.
*   **Focus:** Understanding low-level input interception and stealthy data logging.

---

## 🛠️ Technical Skillset
*   **Languages:** C++, C, Assembly (x64)
*   **Windows Internals:** PEB/TEB Parsing, Win32 API, Native API (NTAPI), Process Injection.
*   **Reversing Tools:** Ghidra, x64dbg, PE-bear, Process Hacker etc
*   **Research:** Technical Writing, MITRE ATT&CK Mapping, IOC Extraction.
  
## 🤖 AI-Augmented Research Methodology
These projects were developed using an **AI-Assisted Workflow**. I utilized advanced Large Language Models (LLMs) as research collaborators to:
*   **Architect Complex Logic:** Co-developing the C++ implementation for low-level Windows API hooking and anti-debugging.
*   **Documentation:** The 'Detailed Reports' included in these folders are curated exports of technical deep-dives, code explanations, and workflow logic generated during AI research and browsing sessions.


## 🛡️ Analyst Mindset
Every project in this lab includes a **Defensive Analysis** section, providing Indicators of Compromise (IOCs) and detection strategies for Blue Teams and EDR developers.

## ⚠️ Disclaimer
All content is for **educational and security research purposes only**. The code is intended to be executed in isolated sandbox environments.

---
**"To defeat a threat, you must first understand its architecture."**
