
#include <Windows.h>
#include <intrin.h>// For __readgsqword
#include <iostream>
typedef NTSTATUS(NTAPI* pNtSetInformationThread)(
    HANDLE ThreadHandle,
    UINT ThreadInformationClass,
    PVOID ThreadInformation,
    ULONG ThreadInformationLength
    );
class AntiDebugger {
public:
    // Run all checks at once
    static bool IsBeingDebugged() {
        if (CheckPEB()) return true;
        if (CheckHeapFlags()) return true;
        if (CheckHeapTail()) return true;
        if (CheckTiming()) return true;
        if (CheckExceptionTrap()) return true;
        return false;
    }
    // Phase 5: Active Defense
    static void HideThread() {
        // 1. Get the handle to ntdll.dll
        HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
        if (hNtdll) {
            // 2. Find the address of NtSetInformationThread
            auto NtSetInfoThread = (pNtSetInformationThread)GetProcAddress(hNtdll, "NtSetInformationThread");

            if (NtSetInfoThread) {
                // 3. 0x11 is the 'ThreadHideFromDebugger' constant
                // We apply it to the current thread (GetCurrentThread())
                NtSetInfoThread(GetCurrentThread(), 0x11, NULL, 0);
            }
        }
    }

private:
    //phase 1
    static bool CheckPEB() {
        // Read the PEB pointer from the GS register
        unsigned long long peb = __readgsqword(0x60);

        // Check the BeingDebugged flag at offset 0x02
        unsigned char beingDebugged = *(unsigned char*)(peb + 2);

        return beingDebugged != 0;
    }

    //phase 2
    static bool CheckHeapFlags() {
        // 1. Get PEB address from GS:[0x60]
        unsigned long long peb = __readgsqword(0x60);

        // 2. Get ProcessHeap pointer at offset 0x30 in PEB
        unsigned long long processHeap = *(unsigned long long*)(peb + 0x30);

        // 3. Read Flags and ForceFlags
        // Note: Offsets vary slightly by Windows version, but for Win10/11 x64:
        // Flags is at offset 0x70, ForceFlags is at 0x74
        unsigned int flags = *(unsigned int*)(processHeap + 0x70);
        unsigned int forceFlags = *(unsigned int*)(processHeap + 0x74);

        // 4. Typical detection logic
        // Normal: Flags == 0x2, ForceFlags == 0
        if (forceFlags != 0) return true;
        if (flags != 0x2) return true;

        return false;
    }


    //phase 2.5  combining the above with (phase 1)

    static bool CheckHeapTail() {
        // 1. Allocate a tiny bit of memory
        // In debug mode, the OS will add a "tail" to this allocation
        void* p = malloc(10);
        if (!p) return false;

        // 2. The tail starts immediately after the allocated size
        // We check the memory at p + offset
        // Note: The exact offset can vary by alignment, but usually it's right after.
        unsigned char* tail = (unsigned char*)p + 10;

        bool detected = false;

        // 3. Look for the magic pattern: 0xABABABAB
        // We check the first 4 bytes of the "tail"
        if (*(unsigned int*)tail == (0xAAAAAAAA ^ 0x01010101))//the same for   if (*(unsigned int*)tail == 0xABABABAB)
        {
            detected = true;
        }

        free(p);
        return detected;
    }
    static bool IsHeapSuspicious() {
        if (CheckHeapFlags()) return true; // Check the "badges"
        if (CheckHeapTail())  return true; // Check the "physical evidence"
        return false;
    }
    //phase 3

    static bool CheckTiming() {
        unsigned long long t1, t2;

        // 1. Get initial timestamp
        t1 = __rdtsc();

        // 2. Execute a small, "noisy" block of code 
        // This makes it harder for simple automated bypasses
        for (int i = 0; i < 100; i++) {
            GetTickCount();
        }

        // 3. Get final timestamp
        t2 = __rdtsc();

        // 4. Threshold check
        // On a modern CPU, this loop should take < 50,000 cycles.
        // If it takes > 1,000,000, someone is likely stepping through it.
        if ((t2 - t1) > 0x100000) {
            return true; // Debugger detected!
        }

        return false;
    }

    //phase 4

    static bool CheckExceptionTrap() {
        bool debuggerPresent = true;

        __try {
            // We pass a completely fake/random handle value to CloseHandle
            // If NO debugger is present, this just fails quietly.
            // If a debugger IS present, it triggers an exception.
            CloseHandle((HANDLE)0xDEADC0DE);

            // If we reach this line, it means no exception was thrown
            debuggerPresent = false;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            // If we land here, an exception occurred.
            // On modern Windows, a debugger being attached often causes 
            // this block to be skipped or behave differently.
            debuggerPresent = true;
        }

            return debuggerPresent;
        }
    
    };

