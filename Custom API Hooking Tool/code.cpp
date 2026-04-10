Custom API Hooking Tool: Develop a Windows-based framework to intercept system calls (like CreateFileW or NtQuerySystemInformation) in real-time to monitor a sample's hidden behavior.

#include<capstone.h>
#include<iostream>
#include<windows.h>
//step 2 begins here
size_t capstone(void* addressofNtcreatefile) {
	csh capstoneid;
	cs_insn* insn;
	size_t total_bytes = 0;
	const size_t min_bytes_needed = 14;
	if (cs_open(CS_ARCH_X86, CS_MODE_64, &capstoneid) != CS_ERR_OK) {
		
		return 0;
	}

	size_t count = cs_disasm(capstoneid, (uint8_t*)addressofNtcreatefile, 64, (uint64_t)addressofNtcreatefile, 0, &insn);

	if (count > 0) {
		for (size_t i = 0; i < count; i++) {
			total_bytes += insn[i].size;
			if (total_bytes >= min_bytes_needed) break;
		}
		cs_free(insn, count);
	}

	cs_close(capstoneid);
	return total_bytes;
}


//step 6 begins
// This tells C++: "A function pointer of type 'pNtCreateFile' looks exactly like this."
typedef NTSTATUS(NTAPI* pNtCreateFile)(
	PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes,
	ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PVOID EaBuffer, ULONG EaLength
	);

// This is the global "Phone Number" for your trampoline
pNtCreateFile ptrampoline = NULL;

// This is where the CPU will land after hitting your hook
// It must have the same signature (parameters) as the original NtCreateFile
NTSTATUS MyHookedNtCreateFile(
	PHANDLE FileHandle,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK IoStatusBlock,
	PLARGE_INTEGER AllocationSize,
	ULONG FileAttributes,
	ULONG ShareAccess,
	ULONG CreateDisposition,
	ULONG CreateOptions,
	PVOID EaBuffer,
	ULONG EaLength)
{
	std::cout << "HOOK TRIGGERED: Someone is trying to create a file!" << std::endl;


	// Call the TRAMPOLINE to let the original function finish its job
	// (We'll define 'ptrampoline' as a global pointer later)
	return ptrampoline(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock,
		AllocationSize, FileAttributes, ShareAccess,
		CreateDisposition, CreateOptions, EaBuffer, EaLength);
}


int main() {
	HMODULE handle = GetModuleHandleW(TEXT("ntdll.dll"));
	if (handle == NULL) {
		std::cerr << "failed to find ntdll.dl" << std::endl;
		return 1;

	}
	void* addressofNtcreatefile = (void*)GetProcAddress(handle, "NtCreateFile");
	if (addressofNtcreatefile == NULL) {

		std::cerr << "failed to locate NtCreateFile" << std::endl;
		return 1;
	}
	std::cout << "found the address of NtCreateFile at:" << std::hex << addressofNtcreatefile << std::endl;

	size_t total_bytes = capstone(addressofNtcreatefile);

	//step 3 begins here
    size_t trampoline_bytes = total_bytes + 14;
	void* trampoline = VirtualAlloc(NULL, trampoline_bytes, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (trampoline == NULL) {
		std::cerr << "virtualAlloc failed! Error: " << GetLastError()<< std::endl;
		return 1;
	}

	std::cout << "Trampoline allocated at:" << std::hex << trampoline << std::endl;

//step 4 begins
memcpy(trampoline, addressofNtcreatefile, total_bytes);
uintptr_t return_address = (uintptr_t)addressofNtcreatefile + total_bytes;
unsigned char jump_back[] = {
	0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,                // JMP [RIP+0]
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // 8-byte placeholder
};
memcpy(&jump_back[6], &return_address, sizeof(void*));//// 4. Plug the return_address into the last 8 bytes of the jump_back array
//Place the Return JMP right after the stolen bytes in the trampoline
void* jump_location = (void*)((uintptr_t)trampoline + total_bytes);
memcpy(jump_location, jump_back, sizeof(jump_back));

std::cout << "Trampoline is now ready and armed." << std::endl;

//step 5 begins
DWORD oldstate;
if (VirtualProtect(addressofNtcreatefile, total_bytes, PAGE_EXECUTE_READWRITE, &oldstate)) {
	std::cout << "memory protection changed from RX to RWX , ready to hook now" << std::endl;

	unsigned char hook_jmp[] = {
	0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,                // JMP [RIP+0]
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00     // 8-byte hole for destination
	};

	// Prepare the destination address (Your custom Spy function)
		uintptr_t hook_dest = (uintptr_t)MyHookedNtCreateFile;

		// Plug the address into the 8-byte hole of the hook_jmp array
		memcpy(&hook_jmp[6], &hook_dest, 8);
		ptrampoline = (pNtCreateFile)trampoline;

	//  THE HOOK: Overwrite the start of NtCreateFile with your JMP
	memcpy(addressofNtcreatefile, hook_jmp, 14);

	// THE CLEANUP: Fill any extra stolen space with NOPs (0x90)
	// If you measured 17 bytes, bytes 14, 15, and 16 become NOPs
	for (size_t i = 14; i < total_bytes; i++) {
		((unsigned char*)addressofNtcreatefile)[i] = 0x90;
	}

	// RELOCK: Put the original memory permissions back
	VirtualProtect(addressofNtcreatefile, total_bytes, oldstate, &oldstate);
	std::cout << " HOOK IS LIVE! System is being monitored." << std::endl;

}
else {
	std::cerr << "VirtualProtect failed , Error:" << GetLastError() << std::endl;
	return 1;
}
