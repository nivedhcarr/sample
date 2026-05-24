# Configurations
$InputFile = "targets.txt"
$OutputFile = "intelligence_report.csv"
$HistoryFile = "history.txt"
$ScanIntervalSeconds = 15

# Global Evasion Array
$UserAgents = @(
    "Mozilla/5.0 (compatible; Googlebot/2.1; +http://google.com)",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (iPhone; CPU iPhone OS 17_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.2 Mobile/15E148 Safari/604.1"
)

# Deep Data Mining Regex Patterns
$RegexPatterns = @{
    "Emails"      = '\b[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.(?!(?:png|jpg|jpeg|gif|svg|webp|ico)\b)[a-zA-Z]{2,}\b'
    "PhoneNums"   = '\b(?:\+?\d{1,3}[-. ]?)?\(?\d{3}\)?[-. ]?\d{3}[-. ]?\d{4}\b'
    "IPAddresses" = '\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b'
    "WebLinks"    = 'https?://[a-zA-Z0-9./?=&_-]+'
}

# Advanced Helper Function: Computes and expands ANY CIDR range (/16 to /30) mathematically
function Expand-IPRange {
    param([string]$Cidr)
    
    # If it's a standard domain or single IP without CIDR notation, return it as-is
    # If it contains a slash but it's not followed by a subnet number, it's a URL. Pass it through safely.
    if ($Cidr -notmatch '/\d{1,2}$') { return @($Cidr) }

    
    try {
        $Parts = $Cidr.Split('/')
        $IpStr = $Parts[0]
        $MaskBits = [int]$Parts[1]
        
        # Validate boundary limits to prevent crashes on bad user inputs
        if ($MaskBits -lt 16 -or $MaskBits -gt 30) {
            Write-Host "[-] Warning: CIDR /$MaskBits out of bounds. Supported ranges are /16 to /30." -ForegroundColor Yellow
            return @()
        }

        # Convert the IP address string into a 32-bit unsigned integer
        $IpBytes = [System.Net.IPAddress]::Parse($IpStr).GetAddressBytes()
        if ([BitConverter]::IsLittleEndian) { [Array]::Reverse($IpBytes) }
        $IpInt = [BitConverter]::ToUInt32($IpBytes, 0)

        # Calculate total host capacity using bit-shifting math
        $TotalHosts = [Math]::Pow(2, (32 - $MaskBits))
        
        # Calculate the base Network Address mask
        $MaskInt = [uint32]([Math]::Pow(2, 32) - $TotalHosts)
        $NetworkInt = $IpInt -band $MaskInt

        $IpList = @()
        # Loop through all possible mathematical hosts in the calculated sub-network range
        for ($i = 1; $i -lt ($TotalHosts - 1); $i++) {
            $CurrentIpInt = $NetworkInt + $i
            $CurrentBytes = [BitConverter]::GetBytes($CurrentIpInt)
            if ([BitConverter]::IsLittleEndian) { [Array]::Reverse($CurrentBytes) }
            $IpList += ($CurrentBytes -join '.')
        }
        return $IpList
    }
    catch {
        Write-Host "[-] Parsing error on CIDR format: $Cidr" -ForegroundColor Red
        return @()
    }
}

# Ensure baseline database structures exist
if (-not (Test-Path $OutputFile)) {
    "Timestamp,Target_Host,Server_Banner,Matched_Keywords,Emails_Found,Phones_Found,IPs_Found,Links_Found" | Out-File -FilePath $OutputFile -Encoding utf8
}
if (-not (Test-Path $HistoryFile)) { "" | Out-File -FilePath $HistoryFile }

Write-Host "==========================================================" -ForegroundColor Red
Write-Host "[!] INITIALIZING MASS SUBNET SCALING AUTOMATION DAEMON    " -ForegroundColor Red
Write-Host "==========================================================" -ForegroundColor Red

$UserInput = Read-Host "Enter target keywords to track alongside Regex (e.g. google)"
$TargetKeywords = $UserInput.Split(',') | ForEach-Object { $_.Trim() }

Write-Host "[*] Mass Scaling Engine Active. Scanning loops armed." -ForegroundColor Yellow

while ($true) {
    if (-not (Test-Path $InputFile)) { "" | Out-File -FilePath $InputFile }
    $RawEntries = Get-Content $InputFile | Where-Object { -not [string]::IsNullOrWhiteSpace($_) }
    
    if ($RawEntries.Count -gt 0) {
        $Timestamp = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
        
        # 1. Mathematically expand all target lists
        $ExpandedQueue = @()
        foreach ($Entry in $RawEntries) { $ExpandedQueue += Expand-IPRange -Cidr $Entry.Trim() }
        $UniqueQueue = $ExpandedQueue | Select-Object -Unique
        
        # 2. Historical memory filter lookup
        $PastHistory = Get-Content $HistoryFile | Where-Object { -not [string]::IsNullOrWhiteSpace($_) }
        $FinalQueue = @()
        $SkippedCount = 0
        foreach ($Target in $UniqueQueue) {
            if ($PastHistory -contains $Target) { $SkippedCount++ } else { $FinalQueue += $Target }
        }

        # Clear inbound queue file immediately to avoid redundant processing cycles
        $null | Out-File -FilePath $InputFile -Force

        if ($FinalQueue.Count -eq 0) {
            Write-Host "`n[.] All targets skipped via historical memory logs." -ForegroundColor Yellow
            Start-Sleep -Seconds $ScanIntervalSeconds
            continue
        }

        # Randomize target distribution arrays across the entire expanded landscape
        $ShuffledQueue = $FinalQueue | Get-Random -Count $FinalQueue.Count
        Write-Host "`n[*] [$Timestamp] Ingested queue. Total Nodes: $($UniqueQueue.Count) | Skipped: $SkippedCount | Active Scanning Target Array: $($ShuffledQueue.Count)" -ForegroundColor Cyan

        # 3. Mass Processing Optimization Loop
        foreach ($Target in $ShuffledQueue) {
            $RandomAgent = $UserAgents | Get-Random
            $SpoofedIP = "$(Get-Random -Minimum 10 -Maximum 192).$(Get-Random -Minimum 0 -Maximum 254).$(Get-Random -Minimum 0 -Maximum 254).$(Get-Random -Minimum 1 -Maximum 254)"
            
            try {
                # Optimized for speed: 0.5 sec connection timeout, 1.5 sec max execute time to process thousands of nodes safely
                $Response = curl.exe -s -i -L -k "https://$Target" -A $RandomAgent -H "X-Forwarded-For: $SpoofedIP" -H "X-Real-IP: $SpoofedIP" --connect-timeout 0.5 --max-time 1.5
                $Target | Out-File -FilePath $HistoryFile -Append -Encoding utf8
                if ([string]::IsNullOrEmpty($Response)) { continue }

                $ServerBanner = "Unknown"
                foreach ($Line in ($Response -split "`n")) {
                    if ($Line -like "*Server:*") { $ServerBanner = ($Line -replace "Server:\s*", "").Trim() -replace ',', ';'; break }
                }

                $FoundKeywords = @()
                foreach ($Keyword in $TargetKeywords) {
                    if ($Response -match $Keyword) { $FoundKeywords += $Keyword }
                }

                $ExtractedData = @{"Emails"=@(); "PhoneNums"=@(); "IPAddresses"=@(); "WebLinks"=@()}
                foreach ($Key in $RegexPatterns.Keys) {
                    $Pattern = $RegexPatterns[$Key]
                    if ($Response -match $Pattern) {
                        $MatchesData = [regex]::Matches($Response, $Pattern)
                        foreach ($M in $MatchesData) {
                            if ($ExtractedData[$Key] -notcontains $M.Value) { $ExtractedData[$Key] += $M.Value }
                        }
                    }
                }

                $EmailStr = if ($ExtractedData["Emails"].Count -gt 0) { ($ExtractedData["Emails"] | Select-Object -First 5) -join "; " } else { "None" }
                $PhoneStr = if ($ExtractedData["PhoneNums"].Count -gt 0) { ($ExtractedData["PhoneNums"] | Select-Object -First 5) -join "; " } else { "None" }
                $IpStr    = if ($ExtractedData["IPAddresses"].Count -gt 0) { ($ExtractedData["IPAddresses"] | Select-Object -First 5) -join "; " } else { "None" }
                $LinkStr  = if ($ExtractedData["WebLinks"].Count -gt 0) { ($ExtractedData["WebLinks"] | Select-Object -First 5) -join "; " } else { "None" }
                $KeywordStr = if ($FoundKeywords.Count -gt 0) { $FoundKeywords -join "; " } else { "None" }

                if (($FoundKeywords.Count -gt 0) -or ($EmailStr -ne "None") -or ($PhoneStr -ne "None")) {
                    Write-Host "[!] EXTRACTION ALERT [$Target] -> Keywords:($KeywordStr) | Emails:($EmailStr)" -ForegroundColor Green
                                        # Load native Windows .NET speech synthesis
                    Add-Type -AssemblyName System.Speech | Out-Null
                    $Synth = New-Object System.Speech.Synthesis.SpeechSynthesizer
                    $Synth.Speak("Alert. Match found on host $Target")

                    
                    $CsvRow = "$Timestamp,$Target,$ServerBanner,$KeywordStr,$EmailStr,$PhoneStr,$IpStr,$LinkStr"
                    $CsvRow | Out-File -FilePath $OutputFile -Append -Encoding utf8
                }
            } catch {}
        }
        Write-Host "[*] Large-scale scan sweep complete. Engine resting." -ForegroundColor Cyan
    }
    Start-Sleep -Seconds $ScanIntervalSeconds
}
