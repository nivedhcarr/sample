# Zero‑CRT Runtime Architecture
### *Building a Native, Deterministic C++ Runtime from First Principles*

---


> An educational systems-engineering project exploring low-level runtime construction, PE internals, compiler behavior, and binary analysis—completely free from the traditional C Runtime (CRT).


The purpose of this repository is to understand the complete execution path of a native executable, from the moment the operating system transfers control to the image, through initialization, dependency resolution, runtime construction, and execution.
A program is not:

Plain Text
main()
└─ logic
 

A program is:

Plain Text
Executable Image
│
▼
Loader Initialization
│
▼
Runtime Construction
│
▼
Dependency Resolution
│
▼
Memory Layout
│
▼
Execution Model
│
▼
Application Logic

The runtime is organized into independent subsystems.

┌──────────────────────────────┐
│        Entry Layer           │
└──────────────┬───────────────┘
               │
               ▼
┌──────────────────────────────┐
│    Initialization Layer      │
└──────────────┬───────────────┘
               │
               ▼
┌──────────────────────────────┐
│       PEB Subsystem          │
└──────────────┬───────────────┘
               │
               ▼
┌──────────────────────────────┐
│     Loader Subsystem         │
└──────────────┬───────────────┘
               │
               ▼
┌──────────────────────────────┐
│    Export Analysis Layer     │
└──────────────┬───────────────┘
               │
               ▼
┌──────────────────────────────┐
│    Interface Abstraction     │
└──────────────┬───────────────┘
               │
               ▼
┌──────────────────────────────┐
│  Memory/String Facilities    │
└──────────────┬───────────────┘
               │
               ▼
┌──────────────────────────────┐
│   Runtime State Machine      │
└──────────────────────────────┘
