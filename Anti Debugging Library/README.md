
# Windows Anti-Analysis & Anti-Debugging Research Library (C++)

## 🔍 Overview
This is an advanced, header-only C++ research library designed to detect, signal, and respond to the presence of debuggers and analysis environments. It implements a multi-layered defense strategy, covering User-mode APIs, Kernel-mode flags, and hardware-level timing discrepancies.

In modern malware development, these techniques (MITRE ATT&CK T1497) are essential for protecting payloads from automated sandboxes and manual reverse engineering.

## 🛠️ Implementation Details
To make it truly advanced,this library goes beyond simple Windows API calls like IsDebuggerPresent() and instead use low-level system interactions that are harder for analysts to hook or patch. 
Core Anti-Debugging Categories implemented are
 * Environmental Artifact Checks: Search for "telltale signs" of a debugger's presence.
 * PEB Flags: Directly read the Process Environment Block (PEB) to check flags like BeingDebugged or NtGlobalFlag without using official APIs.
 * Exception-Based Detection: Leverage how debuggers handle errors differently than the OS.
 * Breakpoint Detection: Scan your own code in memory for 0xCC (INT 3) instructions, which indicate software breakpoints.
 * Structured Exception Handling (SEH): Intentionally trigger exceptions (like CloseHandle with an invalid handle) and check if a debugger "swallows" the error before your own SEH          handler can catch it.
 * Hardware-Level Checks: Monitor CPU-specific features used by advanced analysts.
   Direct Register Access (`readgsqword`): Implements x64-specific assembly intrinsics to access the **GS register**. By reading `gs:[0x60]`, the library retrieves the PEB                  address directly from the Thread Environment Block (TEB), bypassing `GetModuleHandle` or other hookable Win32 APIs.
 * RDTSC Fingerprinting:** Measuring execution delta between instructions to detect the "single-stepping" latency of a researcher.
 * System Call Evasion: Instead of calling ntdll.dll functions that analysts often hook, use Direct Syscalls. This allows the library to talk directly to the kernel, bypassing user-mode    monitoring tools.
   
## 🛠️ Advanced Detection Vectors (5-Path Analysis)
This library implements five distinct paths to identify and neutralize analysis environments:

1. **PEB Internal Inspection:** Direct parsing of the Process Environment Block to monitor the `BeingDebugged` flag, bypassing standard API hooks.
2. **Heap Flag Analysis:** Monitoring `ForceFlags` and `Flags` within the process heap, which exhibit specific patterns when a user-mode debugger is attached.
3. **CPU Timing Gap (RDTSC):** Utilizing the Read Time-Step Counter to detect the microscopic execution delays caused by debugger single-stepping or breakpoint hits.
4. **Exception Traps (CloseHandle):** Utilizing "Fake Handle" traps where calling `CloseHandle` on an invalid pointer triggers an `EXCEPTION_INVALID_HANDLE`—an event typically intercepted and revealed by an active debugger.
5. **Thread Hiding (Active Defense):** Implements `NtSetInformationThread` with the `ThreadHideFromDebugger` (0x11) flag. This actively detaches the thread from the debugger’s debug port, causing the debugger to lose control of the thread execution.



### 1. Environment & API Checks
*   **Static Win32 Detection:** Standard checks via `IsDebuggerPresent` and `CheckRemoteDebuggerPresent`.
*   **PEB (Process Environment Block) Parsing:** Direct memory inspection of the `BeingDebugged` and `NtGlobalFlag` members to bypass simple API hooks.

### 2. Exception & Fault Handling
*   **Invalid Handle Exceptions:** Triggering `CloseHandle` exceptions that only occur when a debugger is attached.

### 3. Hardware & Timing Analysis
*   **RDTSC Fingerprinting:** Measuring execution delta between instructions to detect the "single-stepping" latency of a researcher.

### 4. Advanced Stealth Techniques
*   **Parent Process Validation:** Checking if the parent process is `explorer.exe` or a known debugger like `x64dbg`.
*   **FindWindow Obfuscation:** Scanning for active class names associated with analysis tools (Ghidra, IDA Pro, Wireshark).

## 📂 Project Structure
- **`Antidebug.h`**: The core library containing all detection logic and macros.
- **`main.cpp`**: A Proof-of-Concept (PoC) demonstrating integration and conditional execution flow.
## 📚 Industry References & Benchmarks
This library was developed by researching industry-standard protection frameworks and advanced evasion techniques. 

*   **[antidbg (Windows)](https://github.com/NotRequiem/antidbg)**: A specialized x64 user-mode library utilized for its implementation of **direct syscalls** and multi-vector detection       logic.
*   **[Hades-AntiDebug](https://github.com/Giannis101/Hades-AntiDebug)**: An advanced reference for bypassing user-mode hooks through the use of **undocumented kernel functions**.
*   **[NativeShield (Android)](https://github.com/PhuongDoZz/NativeShield)**: A primary reference for **C++ hardening** and protection strategies against runtime instrumentation.

## 🚀 Usage Example
```cpp
#include "Antidebug.h"

int main() {
    // Perform a silent multi-vector check
    if (AntiDebug::IsEnvironmentTainted()) {
        // Implement evasion or self-deletion logic
        return 0; 
    }
    // Real payload/logic goes here
}
