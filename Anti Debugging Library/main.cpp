#include "Antidebug.h"
#include<iostream>
#include <Windows.h>
#include <intrin.h>  // Required for __readgsqword

int main(){
	AntiDebugger::HideThread(); // Phase 5: Make the main thread invisible to debuggers
    // Check all phases at once
    if (AntiDebugger::IsBeingDebugged()) {
        // Exit silently to confuse the analyst
        return 0;
    }
    std::cout << "Secure execution started..." << std::endl;
    // ... your sensitive code here ...(We could not detect any debuggers , so you can write your sensitive code now)
    

    return 0;

}

