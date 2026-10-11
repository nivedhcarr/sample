extern "C" void __declspec(noreturn) _start() {
	//custom entry point
	run();//your call logic
	for (;;) {}//never return

}



struct PEB;
inline PEB* get_peb() noexcept {
	return reinterpret_cast<PEB*>(__readgsqword(0x60));
}

template<typename T>
constexpr void my_memcpy(T* dst, const T* src, size_t n) {
    for (size_t i = 0; i < n; ++i)
        dst[i] = src[i];
}

constexpr size_t my_strlen(const char* s) {
    size_t n = 0;
    while (s[n]) ++n;
    return n;
}
void run() {
    //step1:get PEB
    PEb* peb = get_peb();
    if (!peb) panic();
    char buf[32];
    // Zero‑CRT memory ops
    my_memset(buf, 0, sizeof(buf));

    // Zero‑CRT string ops
    constexpr size_t n = my_strlen("test");
    //step2:identify ntdll.dll
    void* ntdll_base = find_ntdll(peb);
    if (!ntdll_base) panic();
    //step3:Build Nt API table
    NtApiTable api{};
    if (!bind_nt_api(api, ntdll_base))
        panic();
    // Step 4: Bind NT functions (conceptual)
    bind_nt_functions(api, ntdll_base);
    // Step 5: Use api.NtAllocateVirtualMemory etc. (conceptual only)

}
constexpr uint32_t hash_str(const char* s, uint32_t h = 0x811C9DC5) {
    return (*s) ? hash_str(s + 1, (h ^ uint32_t(*s)) * 0x01000193) : h;
}
constexpr uint32_t HASH_NTDLL = hash_str("ntdll.dll");
constexpr uint32_t HASH_KERNEL32 = hash_str("kernel32.dll");
uint32_t hash_unicode(const UNICODE_STRING& u) {
    // Conceptual: convert wchar_t → char and hash
    // Educational placeholder only
    return 0;
}



//2.1
struct LIST_ENTRY {
    LIST_ENTRY* Flink;
    LIST_ENTRY* Blink;
};

struct UNICODE_STRING {
    unsigned short Length;
    unsigned short MaximumLength;
    wchar_t* Buffer;
};

struct LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InMemoryOrderLinks;
    void* DllBase;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
};

struct PEB_LDR_DATA {
    unsigned long Length;
    unsigned long Initialized;
    void* SsHandle;
    LIST_ENTRY    InMemoryOrderModuleList;
};

struct PEB {
    unsigned char Reserved1[0x18];
    PEB_LDR_DATA* Ldr;
};
constexpr uint32_t hash_str(const char* s, uint32_t h = 0x811C9DC5) {
    return (*s) ? hash_str(s + 1, (h ^ uint32_t(*s)) * 0x01000193) : h;
}
constexpr uint32_t HASH_NTDLL = hash_str("ntdll.dll");
constexpr uint32_t HASH_KERNEL32 = hash_str("kernel32.dll");


uin32_t hash_unicode(const UNICODE_STRING& u){
    //convert wchar_t to char and hash
    return 0;
}
void identify_modules() {
    PEB* peb = get_peb();
    PEB_LDR_DATA* ldr = peb->Ldr;

    LIST_ENTRY* head = &ldr->InMemoryOrderModuleList;
    LIST_ENTRY* curr = head->Flink;

    void* ntdll_base = nullptr;
    void* kernel32_base = nullptr;

    while (curr != head) {
        auto entry = reinterpret_cast<LDR_DATA_TABLE_ENTRY*>(
            reinterpret_cast<unsigned char*>(curr) -
            offsetof(LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks)
            );

        uint32_t name_hash = hash_unicode(entry->BaseDllName);

        if (name_hash == HASH_NTDLL) {
            ntdll_base = entry->DllBase;
        }
        else if (name_hash == HASH_KERNEL32) {
            kernel32_base = entry->DllBase;
        }

        curr = curr->Flink;
    }

    // At this point you conceptually have:
    // ntdll_base
    // kernel32_base
}


//step 3
IMAGE_EXPORT_DIRECTORY* get_export_directory(void* module_base) {
    auto dos = reinterpret_cast<IMAGE_DOS_HEADER*>(module_base);
    auto nt = reinterpret_cast<IMAGE_NT_HEADERS*>(
        reinterpret_cast<unsigned_char*>(module_base) + dos->e_lfanew);

}


auto& dir = nt->OptionalHeader.DataDirectory[0];
if (dir.VirtualAddress == 0) {
    return nullptr;
}
return reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(reinterpret_cast<unsigned char*>(module_base) + dir.VirtualAddress);
}

void enumerate_exports(void* module_base) {
    auto exp = get_export_directory(module_base);
    if (!exp) return;

    auto names = reinterpret_cast<uint32_t*>(
        reinterpret_cast<unsigned char*>(module_base) + exp->AddressOfNames
        );

    auto ordinals = reinterpret_cast<uint16_t*>(
        reinterpret_cast<unsigned char*>(module_base) + exp->AddressOfNameOrdinals
        );

    auto functions = reinterpret_cast<uint32_t*>(
        reinterpret_cast<unsigned char*>(module_base) + exp->AddressOfFunctions
        );

    for (uint32_t i = 0; i < exp->NumberOfNames; ++i) {
        const char* func_name = reinterpret_cast<const char*>(
            reinterpret_cast<unsigned char*>(module_base) + names[i]
            );

        uint16_t ordinal = ordinals[i];
        uint32_t func_rva = functions[ordinal];

        // Conceptual: hash func_name, compare to target hash
        // Educational only
    }
}

uint32_t h = hash_str(func_name);
if (h == TARGET_HASH) {
    // Conceptual: matched function
}
void* bind_function(void* module_base, uint32_t target_hash) {
    // Conceptual: walk exports, match hash, compute address
    return nullptr; // placeholder
}


//step4

using NtExample_t = long(*)(int, int);//this makes the NtExample_t's type a pointer to a function which accepts 2 integers and returns a long integer

using NtAllocateVirtualMemory_t = long(*)(
    void* ProcessHandle,
    void** BaseAddress,
    unsigned long ZeroBits,
    unsigned long* RegionSize,
    unsigned long AllocationType,
    unsigned long Protect
    );
constexpr uint32_t HASH_NT_ALLOC = hash_str("NtAllocateVirtualMemory");
constexpr uint32_t HASH_NT_QUERY = hash_str("NtQueryInformationProcess");
struct NtApiTable {
    NtAllocateVirtualMemory_t NtAllocateVirtualMemory;
    NtQueryInformationProcess_t NtQueryInformationProcess;
    // Add more as needed
};
void bind_nt_functions(NtApiTable& api, void* ntdll_base) {
    // Conceptual: walk export table of ntdll
    // Compare hashed names
    // Assign function pointers

    // Example (placeholder):
    // if (hash == HASH_NT_ALLOC)
    //     api.NtAllocateVirtualMemory = reinterpret_cast<NtAllocateVirtualMemory_t>(addr);

    // Educational only — no operational resolution
}
//step 5
template<typename T>
constexpr void my_memcpy(T*src,T*dst,size_t n){
    for (size_t i = 0; i < n; ++i) {
        src[i] = dst[i];
    }
    

template<typename T>
constexpr void my_memset(T * src, T value, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        src[i] = value;
    }
}

template<typename T>
constexpr int my_memcmp(const T* a, const T* b, size_t n) {

    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            return 1;
        }
     
    }
    return 0;
}

constexpr int my_strlen(const char* s) {
   
    int n = 0;
    while (*s!=0) {
        ++s;
        ++n;
       
    }
    return n;
}


template<typename T>
constexpr int  my_strcmp(const T* a, const T* b) {
    size_t i = 0;
    while (a[i] != '\0' && b[i] != '\0'){
        if (a[i] != b[i]) {
            return 1;
        }
        ++i;
}
    return(a[i] == b[i]) ? 0 : 1;


}

constexpr int my_strcpy(const char* src, char* dst) {
    size_t i = 0;
    while (src[i]) {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
    return 0;
}



//step 6
enum class Error {
    Ok,
    InvalidArgument,
    NotFound,
    Failed,
    Panic
};


Error do_something() {
    if (/* bad */)
        return Error::InvalidArgument;//failed to validate

    return Error::Ok;//passed validation check
}
enum class ValueType { Int, Float, String };

struct Value {
    ValueType type;
    void* data;
};
//minimal panic subsystem
[[noreturn]]
void panic( ){
    // In a real runtime: log, break, halt
    for (;;) {} // safe infinite halt
}

void process_systemstate(Error state{
    if (state == Error::Panic){

        panic();//Execution halts permanently right here

        }
    }

    //module resolution
    void* find_module(uint32_t hash) {
    void* base = enumerate_and_match(hash);
    if (!base)
        return nullptr; // fallback
    return base;
}

     //export resolution

void resolve_export(void* module, uint32_t hash) {
    void addr = walk_exports(module, hash);
    if (!addr)
        return nullptr;//fallback
    return addr;
}

enum class State {
    Init,
    Resolve,
    Bind,
    Ready,
    Error
};
State run_state_machine() {
    State s = State::Init;
    //step 1
    if (!init()) return State::Error;
    s = State::Resolve;
    //step2
    if (!resolve()) return State::Error;
    s = State::Bind;
    //step3
    if (!bind()) return State::Error;
    s = State::Ready;

    return s;
}


//step 7
struct Runtime {
    bool peb_ok;
    bool modules_ok;
    bool ntapi_ok;
    bool ready;
};

Runtime rt{};

void init_runtime() {
    rt.peb_ok = (get_peb() != nullptr);
    rt.modules_ok = rt.peb_ok && identify_modules();
    rt.ntapi_ok = rt.modules_ok && bind_nt_api();
    rt.ready = rt.ntapi_ok;
}
extern "C" __declspec(noreturn) void _start() {
    init_runtime();

    if (!rt.ready)
        panic();

    run();
}
/* ALL INITIALIZATION MUST HAPPEN INSIDE START*/

//step 8


/*Zero‑CRT Build Configuration & Linker Strategy*/

//step 9
/*Even if your code and build flags are perfect, the compiler or linker can still inject:

CRT helpers

exception metadata

RTTI metadata

TLS callbacks

hidden imports

stack cookies

.CRT$XCU initialization stubs

.pdata / .xdata unwind tables

Step 9 teaches you how to inspect your binary and confirm that none of these exist*/

//step10

//Putting It All Together


//step11
//Advanced Enhancements
constexpr size_t dos_offset() { return offsetof(IMAGE_DOS_HEADER, e_lfanew); }
constexpr size_t export_dir_offset() { return offsetof(IMAGE_OPTIONAL_HEADER, DataDirectory); }


struct ApiHash {
    const char* name;
    uint32_t hash;
};
constexpr ApiHash api_table[] = {
    { "NtAllocateVirtualMemory",hash_str("NtAllocateVirtualMemory")},
    {"NtQueryInformationProcess",hash_str("NtQueryInformationProcess")},
    {"NtClose",hash_str("NtClose")}
};

template<uint32_t Hash, typename Fn>
Fn bind(void* module_base);

constexpr auto NtAlloc = bind<HASH_NT_ALLOC, NtAllocateVirtualMemory_t>(ntdll_base);
template<State S>
struct RuntimeState;

template<>
struct RuntimeState<State::Init> {
    static constexpr State next() { return State::Resolve; }
};

//Step 12  Forensic Validation & Binary Audit 
