# GitHub Upload Script for OOP-Project
# Usage: .\upload_to_github.ps1 -Token "ghp_yourtoken"
param(
    [Parameter(Mandatory=$true)]
    [string]$Token
)

$USERNAME = "5437jjay"
$REPO = "OOP-Project"
$REMOTE_URL = "https://$TOKEN@github.com/$USERNAME/$REPO.git"

Write-Host "Setting up Git config..." -ForegroundColor Cyan
& "C:\Program Files\Git\bin\git.exe" config --global user.email "5437jjay@github.com"
& "C:\Program Files\Git\bin\git.exe" config --global user.name "5437jjay"

Write-Host "Initializing repository..." -ForegroundColor Cyan
& "C:\Program Files\Git\bin\git.exe" init

Write-Host "Creating .gitignore..." -ForegroundColor Cyan
@"
# Compiled binaries
*.exe
*.a
*.dll

# Large raw extraction folders
pdf_output/
pdf_pages/
"@ | Out-File -Encoding utf8 .gitignore

Write-Host "Adding remote..." -ForegroundColor Cyan
& "C:\Program Files\Git\bin\git.exe" remote remove origin 2>$null
& "C:\Program Files\Git\bin\git.exe" remote add origin $REMOTE_URL

Write-Host "Staging all files..." -ForegroundColor Cyan
& "C:\Program Files\Git\bin\git.exe" add .

Write-Host "Committing..." -ForegroundColor Cyan
& "C:\Program Files\Git\bin\git.exe" commit -m "Initial commit: OOP Project game code and assets"

Write-Host "Pushing to GitHub..." -ForegroundColor Cyan
& "C:\Program Files\Git\bin\git.exe" branch -M main
& "C:\Program Files\Git\bin\git.exe" push -u origin main --force

Write-Host "Done! Check https://github.com/$USERNAME/$REPO" -ForegroundColor Green
