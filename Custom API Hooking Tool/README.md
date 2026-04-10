

Click on the **[Detailed Report](./Detailed-Report/report.txt)** for a full reference and workflow analysis (13,000+ words).

# Windows API Research: Custom Hooking Framework (C++)

## 🔍 Overview
This project is an advanced research framework designed to intercept and monitor Windows System Calls in real-time. By hooking low-level APIs, this tool provides a "transparent" view into a sample's hidden behavior, such as file system manipulation, process hiding etc.

This framework was built to study **Dynamic Binary Instrumentation** and **Malware Evasion** techniques.


## 🚀 Key Research Areas
*   **System Call Interception:** Hooking `CreateFileW` to monitor I/O and `NtQuerySystemInformation` to detect potential rootkit behavior.
*   **Memory Manipulation:** Using `VirtualProtect` to modify function prologues for inline hooking.
*   **Stealth Analysis:** Researching how malware detects hooks (e.g., timing attacks or checking for JMP instructions) and how to counter-evade those checks.

## 📖 Deep Dive Analysis
For a comprehensive breakdown of the methodology, memory forensic results, and full workflow:
Click on the **[Detailed Report](./Detailed-Report/report.txt)** for a full reference and workflow analysis (13,000+ words).


## 🛡️ Defensive Perspective (Blue Team)
This tool demonstrates techniques used by both advanced malware and EDR solutions. In this repository, I analyze:
1. **Hook Detection:** How to scan the **Import Address Table (IAT)** for discrepancies.
2. **Persistence:** Identifying if hooking engines are being used to maintain a foothold in high-privilege processes.

## ⚠️ Safety & Ethics
**FOR EDUCATIONAL AND RESEARCH PURPOSES ONLY.** This tool is intended for use in a controlled sandbox environment by security professionals. Unauthorized use against systems you do not own is strictly prohibited.

