
# 💻 AUTOMATED NETWORK SCANNER
An autonomous, continuous threat intelligence and network footprinting daemon designed to mimic automated background reconnaissance operations.
![Console Demo](screenshot.png)

## 🚀 Core Technical Features
* **Persistent Daemon Architecture**: Runs endlessly in a low-resource process loop while the operator is away from the console.
* **Self-Purging Queue Ingestion**: Automatically detects targets inside `targets.txt`, loads them into internal memory, and instantly purges the inbound file to prevent redundant scanning loops.
* **Anti-Firewall Traffic Evasion**: Shuffles and completely randomizes target scanning order to break sequential traffic signatures. Dynamically morphs connection signatures by rotating legitimate desktop/mobile User-Agents and injecting spoofed tracing headers (`X-Forwarded-For`).
* **Long-Term State Memory**: Cross-references every scanned node against a dedicated local history log to minimize network noise and guarantee zero redundant host lookups over time.
* **Deep Content Pattern Mining**: Leverages advanced multi-pattern Regular Expressions (Regex) to extract structured text indicators (hidden emails, corporate phone numbers, tracking IPs, and href links) directly from raw web source blocks.
* **Mass Subnet Bit-Shift Scaling**: Features an integrated mathematical CIDR range calculator capable of instantly expanding networks from /16 down to /30 blocks into thousands of individual target nodes.

## 📂 Repository File Guide
* `net-keyword-scanner.exe`: The compiled, standalone independent binary desktop application.
* `net-keyword-scanner.ps1`: The complete production source code showing the underlying extraction logic.
* `targets.txt`: The active input queue file used to drop target nodes or subnets.
* `history.txt`: The long-term tracking database used by the script to remember previously scanned nodes and prevent duplicate network noise.
* `intelligence_report.csv`: The master spreadsheet log where all harvested data, matching keywords, extracted emails, and phone numbers are permanently stored.
* `reset.ps1`: A maintenance utility script used to clear the history logs, flush the queues, and completely wipe the environment back to a fresh starting state.
## 🛠️ Operational Guide
1. Launch the standalone application framework by double-clicking **`net-keyword-scanner.exe`**.
2. Type your primary target keyword tracking signatures into the interactive user interface console (e.g., `google`, `wikipedia`, or `beavertail`) and press **Enter**.
3. Open **`targets.txt`** at any time, paste your list of target domains or an entire subnet range (e.g., `10.0.0.1/22`), and save the file.
4. The background engine will automatically ingest the text layer, wipe the file, run the randomized evasion checks, and alert you when a match is found.
5. Review all structured metrics and harvested intelligence assets inside the dynamically updated **`intelligence_report.csv`** spreadsheet!
