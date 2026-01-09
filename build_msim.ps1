# build_msim.ps1 - configure and build microsimulation (Release)
param(
    [string]$QtDir = $env:QT_DIR,
    [string]$QScintillaDir = $env:QSCINTILLA_DIR,
    [string]$Generator = "Ninja",
    [bool]$Clean = $false
)

# ============= EDIT THESE DEFAULTS IF YOU WANT =============
$DefaultQtDir = 'C:\Qt\6.9.1\mingw_64'
$DefaultQScintillaDir = 'C:\Qt\QScintilla'
# ==========================================================

if (-not $QtDir) { $QtDir = $DefaultQtDir }
if (-not $QScintillaDir) { $QScintillaDir = $DefaultQScintillaDir }

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$buildDir = Join-Path $scriptDir "cmake-build-release"

if ($Clean -and (Test-Path $buildDir)) { Remove-Item -Recurse -Force $buildDir }
if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir | Out-Null }
Push-Location $buildDir

# Try to detect QScintilla include/lib if QScintillaDir is set
$QScInclude = $env:QSCINTILLA_INCLUDE_DIR
$QScLib = $env:QSCINTILLA_LIBRARY

if (-not $QScInclude -and $QScintillaDir) {
    $cand1 = Join-Path $QScintillaDir 'include\Qsci'
    $cand2 = Join-Path $QScintillaDir 'include'
    if (Test-Path $cand1) { $QScInclude = Split-Path $cand1 -Parent }
    elseif (Test-Path $cand2) { $QScInclude = $cand2 }
}

if (-not $QScLib -and $QScintillaDir) {
    $libDir = Join-Path $QScintillaDir 'lib'
    if (Test-Path $libDir) {
        $found = Get-ChildItem -Path $libDir -Filter "*qscintilla*" -File -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($found) { $QScLib = $found.FullName }
    }
}

$cmakeArgs = @(
    "-DCMAKE_BUILD_TYPE=Release",
    "-G", $Generator
)
if ($QtDir) { $cmakeArgs += "-DCMAKE_PREFIX_PATH=$QtDir" }
if ($QScInclude) { $cmakeArgs += "-DQSCINTILLA_INCLUDE_DIR=$QScInclude" }
if ($QScLib) { $cmakeArgs += "-DQSCINTILLA_LIBRARY=$QScLib" }

Write-Host "Configuring with: $($cmakeArgs -join ' ')"
& cmake @cmakeArgs "$scriptDir"

Write-Host "Building (Release)..."
& cmake --build . --config Release

Pop-Location
Write-Host "Build complete: $buildDir"
