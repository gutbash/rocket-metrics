param(
    [string]$BakkesModSdkPath,

    [Parameter(Mandatory = $true)]
    [string]$BakkesModFolder,

    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Release',

    [switch]$CloneSdkIfMissing
)

$ErrorActionPreference = 'Stop'

$RepoRoot = Resolve-Path (Join-Path $PSScriptRoot '..\..')
$BuildDir = Join-Path $RepoRoot 'build'
$PluginsDir = Join-Path $BakkesModFolder 'bakkesmod\plugins'

if ([string]::IsNullOrWhiteSpace($BakkesModSdkPath)) {
    $BakkesModSdkPath = Join-Path $RepoRoot 'external\BakkesModSDK'
}

if ($CloneSdkIfMissing -and -not (Test-Path $BakkesModSdkPath)) {
    git clone https://github.com/bakkesmodorg/BakkesModSDK.git $BakkesModSdkPath
}

if (-not (Test-Path (Join-Path $BakkesModSdkPath 'include'))) {
    throw "Invalid BakkesMod SDK path. Missing include folder: $BakkesModSdkPath. Clone with: git clone https://github.com/bakkesmodorg/BakkesModSDK.git $BakkesModSdkPath"
}

if (-not (Test-Path (Join-Path $BakkesModSdkPath 'lib'))) {
    throw "Invalid BakkesMod SDK path. Missing lib folder: $BakkesModSdkPath"
}

if (-not (Test-Path $PluginsDir)) {
    throw "Could not find BakkesMod plugins folder: $PluginsDir"
}

cmake -S $RepoRoot -B $BuildDir -G "Visual Studio 17 2022" -A x64 -DBAKKESMOD_SDK_PATH="$BakkesModSdkPath" -DROCKETMETRICS_REQUIRE_SDK=ON
cmake --build $BuildDir --config $Configuration

$BuiltDll = Join-Path $BuildDir "$Configuration\RocketMetrics.dll"
if (-not (Test-Path $BuiltDll)) {
    throw "Build succeeded but DLL not found: $BuiltDll"
}

Copy-Item $BuiltDll -Destination (Join-Path $PluginsDir 'RocketMetrics.dll') -Force
Write-Host "Installed RocketMetrics.dll to $PluginsDir"
Write-Host "Restart Rocket League/BakkesMod (or run plugin reload in F6 console) to test."
