Write-Host "==========================================================" -ForegroundColor Yellow
Write-Host "[!] RUNNING THREAT INTELLIGENCE FRAMEWORK MASTER RESET     " -ForegroundColor Yellow
Write-Host "==========================================================" -ForegroundColor Yellow

$Confirm = Read-Host "Are you sure you want to completely wipe your logs and history? (Y/N)"

if ($Confirm -eq "Y" -or $Confirm -eq "y") {
    # Delete database and memory logs completely
    if (Test-Path "history.txt") { Remove-Item "history.txt" -Force }
    if (Test-Path "intelligence_report.csv") { Remove-Item "intelligence_report.csv" -Force }
    if (Test-Path "targets.txt") { $null | Out-File -FilePath "targets.txt" -Force }

    # Reinitialize clean structures instantly
    "" | Out-File -FilePath "history.txt" -Encoding utf8
    "Timestamp,Target_Host,Impersonated_Agent,Server_Banner,Matched_Keywords" | Out-File -FilePath "intelligence_report.csv" -Encoding utf8

    Write-Host "[+] Master reset complete! System memory and spreadsheets are completely clean." -ForegroundColor Green
} else {
    Write-Host "[.] Reset aborted. Your current database files are safe." -ForegroundColor Gray
}
