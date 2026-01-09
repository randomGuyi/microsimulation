# build_msim.ps1 - configure and build microsimulation (Release)
param(
    [string]$QtDir = $env:QT_DIR,
    [string]$QScintillaDir = $env:QSCINTILLA_DIR,
    [string]$Generator = "Ninja",
    [switch]$Clean
)

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$buildDir = Join-Path $scriptDir "cmake-build-release"

if ($Clean) {
    if (Test-Path $buildDir) { Remove-Item -Recurse -Force $buildDir }
}

if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir | Out-Null }
Push-Location $buildDir

$cmakeArgs = @(
    "-DCMAKE_BUILD_TYPE=Release",
    "-G", $Generator
)
if ($QtDir) { $cmakeArgs += "-DQTDIR=$QtDir" }
if ($QScintillaDir) { $cmakeArgs += "-DQSCINTILLA_DIR=$QScintillaDir" }

# Configure
& cmake @cmakeArgs "$scriptDir"
# Build
& cmake --build . --config Release

Pop-Location
Write-Host "Build complete: $buildDir"
