# 📖 STEP-BY-STEP OPERATIONAL GUIDE

This guide details exactly how to initialize, configure, and execute the **Automated Network Scanner** tool.

## 📥 Prerequisites
Ensure the following files sit inside the exact same directory folder on your local machine:
1. `net-keyword-scanner.exe` (The compiled desktop binary application)
2. `net-keyword-scanner.ps1` (The underlying source logic engine)
3. `targets.txt` (The targets to be scanned)
4. `intelligence_report.csv` (Data collected)
5. `history.txt` (The list of scanned targets)
6. `reset.ps1` (To clear all the actions and logs)

## 🕹️ Step-by-Step Execution Tutorial

### Step 1: Launch the Application Engine
* Double-click **`net-keyword-scanner.exe`** from your Windows File Explorer.
* A dedicated console terminal interface will pop open on your screen and display the initialization banner.

### Step 2: Configure Your Watchlist Strings
* The screen will prompt: `Enter target keywords to track alongside Regex (e.g. google):`
* In targets.txt , Type a word or target phrase you want to discover (e.g., `google.com`, `wikipedia.com`,`10.0.0.1/22`) and press **Enter**.


### Step 3: Populate Your Targets Queue
* Open the empty **`targets.txt`** file in Notepad or any text editor.
* Paste your target domains or network ranges (one entry per line). You can use standard web domains or massive subnet notations:
  ```text
  google.com
  wikipedia.org
  10.0.0.1/22
  ```
* Save the text document (**Ctrl + S**) and close it.

  
### Step 4: The Scan
* The tool scans the targets for the input and also pulls out other metadata.
* If a target matches your keywords or contains hidden contact info, an alert will flash on your screen and play an audio chime.
* The target list is wiped once the cycle is completed.
* The application will arm the watchlists and enter its persistent, background monitoring standby state .
### Step 4: Monitor Autonomous Background Activity
** When the user writes another target name to the file , the system wakes up and begins the scan.
* Within 15 seconds, the application will automatically ingest your entries, mathematically expand the subnets, scramble the target order, and completely wipe `targets.txt` clean so it is ready for your next batch of links.
  
### Step 5: Extract Your Reports
* Open the freshly generated **`intelligence_report.csv`** file using Microsoft Excel or Google Sheets.
* Review your collected network metrics, server software signatures, and extracted email contact strings neatly sorted into columns!

---

## 🧹 Maintenance & System Reset
If you want to clear out your previous scan histories, wipe your spreadsheet data, and restart your testing environment from scratch, double-click your **`reset.ps1`** script or run it from the console to instantly flush the logs.
