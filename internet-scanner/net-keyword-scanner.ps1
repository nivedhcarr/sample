# Configurations
$InputFile = "targets.txt"
$OutputFile = "intelligence_report.csv"
$HistoryFile = "history.txt"
$ScanIntervalSeconds = 15

# Global Evasion Array
$UserAgents = @(
    "Mozilla/5.0 (compatible; Googlebot/2.1; +http://google.com)",
    "Mozilla/5.0 (compatible; Bingbot/2.0; +http://bing.com)",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (iPhone; CPU iPhone OS 17_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.2 Mobile/15E148 Safari/604.1",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36"
)

# Helper Function: Expands /24 blocks into individual sequential IPs
function Expand-IPRange {
    param([string]$Cidr)
    if ($Cidr -like "*/24") {
        $Base = $Cidr -replace "\d{1,3}/24$", ""
        $IpList = @()
        for ($i = 1; $i -le 254; $i++) { $IpList += "$Base$i" }
        return $IpList
    }
    return @($Cidr)
}

# Ensure baseline structures exist
if (-not (Test-Path $OutputFile)) {
    "Timestamp,Target_Host,Impersonated_Agent,Server_Banner,Matched_Keywords" | Out-File -FilePath $OutputFile -Encoding utf8
}
if (-not (Test-Path $HistoryFile)) { "" | Out-File -FilePath $HistoryFile }

Write-Host "==========================================================" -ForegroundColor Red
Write-Host "[!] INITIALIZING SELF-TRACKING APT ENGINE WITH MEMORY    " -ForegroundColor Red
Write-Host "==========================================================" -ForegroundColor Red

$UserInput = Read-Host "Enter the target keywords to hunt for"
$TargetKeywords = $UserInput.Split(',') | ForEach-Object { $_.Trim() }

Write-Host "[*] Engine Active. Memory logs loaded from $HistoryFile" -ForegroundColor Yellow

while ($true) {
    if (-not (Test-Path $InputFile)) { "" | Out-File -FilePath $InputFile }

    # Ingest targets dynamically
    $RawEntries = Get-Content $InputFile | Where-Object { -not [string]::IsNullOrWhiteSpace($_) }
    
    if ($RawEntries.Count -gt 0) {
        $Timestamp = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
        
        # 1. Expand input ranges
        $ExpandedQueue = @()
        foreach ($Entry in $RawEntries) { $ExpandedQueue += Expand-IPRange -Cidr $Entry.Trim() }
        
        # Deduplicate the current queue array
        $UniqueQueue = $ExpandedQueue | Select-Object -Unique
        
        # 2. Historical Filtering (Memory check)
        $PastHistory = Get-Content $HistoryFile
        $FinalQueue = @()
        $SkippedCount = 0

        foreach ($Target in $UniqueQueue) {
            if ($PastHistory -contains $Target) {
                $SkippedCount++
            } else {
                $FinalQueue += $Target
            }
        }

        # Clear queue file immediately to unlock it for fresh user drops
        $null | Out-File -FilePath $InputFile -Force

        if ($FinalQueue.Count -eq 0) {
            Write-Host "`n[.] Ingested file, but all targets skipped (Already processed historically)." -ForegroundColor Yellow
            Start-Sleep -Seconds $ScanIntervalSeconds
            continue
        }

        # Randomize the remaining, un-scanned targets to evade detection signatures
        $ShuffledQueue = $FinalQueue | Get-Random -Count $FinalQueue.Count
        Write-Host ""
        Write-Host "[*] [$Timestamp] Ingested queue. Total: $($UniqueQueue.Count) | Skipped: $SkippedCount | Active Targets: $($ShuffledQueue.Count)" -ForegroundColor Cyan
        Write-Host "[*] Local queue file wiped. Launching evasion processing loops..." -ForegroundColor DarkGray

        # 3. Processing Loop
        foreach ($Target in $ShuffledQueue) {
            $RandomAgent = $UserAgents | Get-Random
            $SpoofedIP = "$(Get-Random -Minimum 10 -Maximum 192).$(Get-Random -Minimum 0 -Maximum 254).$(Get-Random -Minimum 0 -Maximum 254).$(Get-Random -Minimum 1 -Maximum 254)"
            
            try {
                $Response = curl.exe -s -i -L -k "https://$Target" -A $RandomAgent -H "X-Forwarded-For: $SpoofedIP" -H "X-Real-IP: $SpoofedIP" --connect-timeout 1 --max-time 2
                
                # Log to history file IMMEDIATELY after scanning so it's remembered next time
                $Target | Out-File -FilePath $HistoryFile -Append -Encoding utf8

                if ([string]::IsNullOrEmpty($Response)) { continue }

                # Extract Server info
                $ServerBanner = "Unknown Signature"
                foreach ($Line in ($Response -split "`n")) {
                    if ($Line -like "*Server:*") {
                        $ServerBanner = $Line -replace "Server:\s*", ""
                        $ServerBanner = $ServerBanner.Trim() -replace ",", ";"
                        break
                    }
                }

                # Evaluate watchlists
                $FoundKeywords = @()
                foreach ($Keyword in $TargetKeywords) {
                    if ($Response -match $Keyword) { $FoundKeywords += $Keyword }
                }

                if ($FoundKeywords.Count -gt 0) {
                    $KeywordString = $FoundKeywords -join "; "
                    $CsvRow = "$Timestamp,$Target,$RandomAgent,$ServerBanner,$KeywordString"
                    
                    Write-Host "[!] INTRUSION ALERT [$Target] -> Match: ($KeywordString)" -ForegroundColor Green
                    Write-Host "    └─ Evasion Profile: Spoofed IP ($SpoofedIP)" -ForegroundColor DarkGreen
                                              # Load native Windows .NET speech synthesis
                    Add-Type -AssemblyName System.Speech | Out-Null
                    $Synth = New-Object System.Speech.Synthesis.SpeechSynthesizer
                    $Synth.Speak("Alert. Match found on host $Target")
              
                    $CsvRow | Out-File -FilePath $OutputFile -Append -Encoding utf8
                }
            } catch {}
        }
        Write-Host "[*] Operational sweep complete. Dropping into standby state." -ForegroundColor Cyan
    }
    Start-Sleep -Seconds $ScanIntervalSeconds
}
