# This script will download and build OpenSSL in debug, release
# or both configurations for Win32 or x64.
#
# Requires PowerShell version 3.0 or newer
#
# Usage:
# ------
# build.ps1 [-vs_version 120 | 110 | 100 | 90]
#           [-config     release | debug | both]
#           [-platform   Win32 | x64]
#           [-library    shared | static]
#
# Modified and adapted for POCO from https://github.com/hadouken/openssl
# (see https://github.com/hadouken/openssl/blob/master/LICENSE for license)
#

[CmdletBinding()]
Param
(
  [Parameter()]
  [ValidateSet(90, 100, 110, 120, 140)]
  [int] $vs_version = 120,

  [Parameter()]
  [ValidateSet('release', 'debug', 'both')]
  [string] $config = 'release',

  [Parameter()]
  [ValidateSet('Win32', 'x64')]
  [string] $platform = 'x64',

  [Parameter()]
  [ValidateSet('shared', 'static', 'both')]
  [string] $library = 'shared'
)

$PACKAGES_DIRECTORY   = Join-Path $PSScriptRoot "packages"
$OUTPUT_BIN_DIRECTORY = $PSScriptRoot
$OUTPUT_INC_DIRECTORY = Join-Path $PSScriptRoot "include"
$VERSION              = "0.0.0"

if (Test-Path Env:\APPVEYOR_BUILD_VERSION) {
    $VERSION = $env:APPVEYOR_BUILD_VERSION
}

# 7zip configuration section
$7ZIP_VERSION      = "9.20"
$7ZIP_DIRECTORY    = Join-Path $PACKAGES_DIRECTORY "7zip-$7ZIP_VERSION"
$7ZIP_TOOL         = Join-Path $7ZIP_DIRECTORY "7za.exe"
$7ZIP_PACKAGE_FILE = "7za$($7ZIP_VERSION.replace('.', '')).zip"
$7ZIP_DOWNLOAD_URL = "http://downloads.sourceforge.net/project/sevenzip/7-Zip/$7ZIP_VERSION/$7ZIP_PACKAGE_FILE"

# NASM configuration section
$NASM_VERSION      = "2.11.06"
$NASM_DIRECTORY    = Join-Path $PACKAGES_DIRECTORY "nasm-$NASM_VERSION"
$NASM_PACKAGE_FILE = "nasm-$NASM_VERSION-win32.zip"
$NASM_DOWNLOAD_URL = "http://www.nasm.us/pub/nasm/releasebuilds/$NASM_VERSION/win32/$NASM_PACKAGE_FILE"

# Strawberry Perl configuration section
$PERL_VERSION      = "5.20.1.1"
$PERL_DIRECTORY    = Join-Path $PACKAGES_DIRECTORY "strawberry-perl-$PERL_VERSION"
$PERL_PACKAGE_FILE = "strawberry-perl-$PERL_VERSION-32bit-portable.zip"
$PERL_DOWNLOAD_URL = "http://strawberryperl.com/download/5.20.1.1/$PERL_PACKAGE_FILE"

# OpenSSL configuration section
$OPENSSL_VERSION         = "1.0.2e"
$OPENSSL_DIRECTORY       = Join-Path $PACKAGES_DIRECTORY "openssl-$OPENSSL_VERSION"
$OPENSSL_CLEAN_DIRECTORY = Join-Path $PACKAGES_DIRECTORY "openssl-$OPENSSL_VERSION.clean"
$OPENSSL_PACKAGE_FILE    = "openssl-$OPENSSL_VERSION.tar.gz"
$OPENSSL_DOWNLOAD_URL    = "https://www.openssl.org/source/$OPENSSL_PACKAGE_FILE"


function Download-File {
    param (
        [string]$url,
        [string]$target
    )

    $webClient = new-object System.Net.WebClient
    $webClient.DownloadFile($url, $target)
}

function Extract-File {
    param (
        [string]$file,
        [string]$target
    )

    [System.Reflection.Assembly]::LoadWithPartialName('System.IO.Compression.FileSystem') | Out-Null
    [System.IO.Compression.ZipFile]::ExtractToDirectory($file, $target)
}

function Load-DevelopmentTools {
    # Set environment variables for Visual Studio Command Prompt
    
    if ($vs_version -eq 0)
    {
      if     ($Env:VS140COMNTOOLS -ne '') { $script:vs_version = 140 }
      elseif ($Env:VS120COMNTOOLS -ne '') { $script:vs_version = 120 }
      elseif ($Env:VS110COMNTOOLS -ne '') { $script:vs_version = 110 }
      elseif ($Env:VS100COMNTOOLS -ne '') { $script:vs_version = 100 }
      elseif ($Env:VS90COMNTOOLS  -ne '') { $script:vs_version = 90 }
      else
      {
        Write-Host 'Visual Studio not found, exiting.'
        Exit
      }
    }

    $vsct = "VS$($vs_version)COMNTOOLS"
    $vsdir = (Get-Item Env:$vsct).Value
    $Command = ''
    if ($platform -eq 'x64')
    {
      $Command = "$($vsdir)..\..\VC\bin\x86_amd64\vcvarsx86_amd64.bat"
    }
    else
    {
      $Command = "$($vsdir)vsvars32.bat"
    }

    $tempFile = [IO.Path]::GetTempFileName()
    cmd /c " `"$Command`" && set > `"$tempFile`" "
    Get-Content $tempFile | Foreach-Object {
      if($_ -match "^(.*?)=(.*)$")
      {
        Set-Content "Env:$($matches[1])" $matches[2]
      }
    }
    Remove-Item $tempFile
}

# Get our dev tools
Load-DevelopmentTools

# Create packages directory if it does not exist
if (!(Test-Path $PACKAGES_DIRECTORY)) {
    New-Item -ItemType Directory -Path $PACKAGES_DIRECTORY | Out-Null
}

# Download 7zip
if (!(Test-Path (Join-Path $PACKAGES_DIRECTORY $7ZIP_PACKAGE_FILE))) {
    Write-Host "Downloading $7ZIP_PACKAGE_FILE"
    Download-File $7ZIP_DOWNLOAD_URL (Join-Path $PACKAGES_DIRECTORY $7ZIP_PACKAGE_FILE)
}

# Download NASM
if (!(Test-Path (Join-Path $PACKAGES_DIRECTORY $NASM_PACKAGE_FILE))) {
    Write-Host "Downloading $NASM_PACKAGE_FILE"
    Download-File $NASM_DOWNLOAD_URL (Join-Path $PACKAGES_DIRECTORY $NASM_PACKAGE_FILE)
}

# Download Strawberry-Perl
if (!(Test-Path (Join-Path $PACKAGES_DIRECTORY $PERL_PACKAGE_FILE))) {
    Write-Host "Downloading $PERL_PACKAGE_FILE"
    Download-File $PERL_DOWNLOAD_URL (Join-Path $PACKAGES_DIRECTORY $PERL_PACKAGE_FILE)
}

# Download OpenSSL
if (!(Test-Path (Join-Path $PACKAGES_DIRECTORY $OPENSSL_PACKAGE_FILE))) {
    Write-Host "Downloading $OPENSSL_PACKAGE_FILE"
    Download-File $OPENSSL_DOWNLOAD_URL (Join-Path $PACKAGES_DIRECTORY $OPENSSL_PACKAGE_FILE)
}

# Unpack 7zip
if (!(Test-Path $7ZIP_DIRECTORY)) {
    Write-Host "Unpacking $7ZIP_PACKAGE_FILE"
    Extract-File (Join-Path $PACKAGES_DIRECTORY $7ZIP_PACKAGE_FILE) $7ZIP_DIRECTORY
}

# Unpack NASM
if (!(Test-Path $NASM_DIRECTORY)) {
    Write-Host "Unpacking $NASM_PACKAGE_FILE"
    Extract-File (Join-Path $PACKAGES_DIRECTORY $NASM_PACKAGE_FILE) $PACKAGES_DIRECTORY
}

# Unpack Strawberry-Perl
if (!(Test-Path $PERL_DIRECTORY)) {
    Write-Host "Unpacking $PERL_PACKAGE_FILE"
    Extract-File (Join-Path $PACKAGES_DIRECTORY $PERL_PACKAGE_FILE) $PERL_DIRECTORY
}

function Unpack-OpenSSL {

    if (!(Test-Path -Path $OPENSSL_CLEAN_DIRECTORY -ErrorAction SilentlyContinue)) {

        # Unpack OpenSSL
        Write-Host "Unpacking $OPENSSL_PACKAGE_FILE"
        $tmp = Join-Path $PACKAGES_DIRECTORY $OPENSSL_PACKAGE_FILE
        
        & "$7ZIP_TOOL" x $tmp -o"$PACKAGES_DIRECTORY" -y
        if ($LastExitCode -ne 0) {
            Write-Host "Error extracting files"
            Exit
        }
        $tmp = $tmp.replace('.gz', '')
        & "$7ZIP_TOOL" x $tmp -o"$PACKAGES_DIRECTORY" -y
        if ($LastExitCode -ne 0) {
            Write-Host "Error extracting files"
            Exit
        }

        Rename-Item -path $OPENSSL_DIRECTORY -newname "$OPENSSL_CLEAN_DIRECTORY" -force
        if ($? -eq $false) {
            Write-Host "Error renaming OpenSSL directory [$OPENSSL_DIRECTORY] to [$OPENSSL_CLEAN_DIRECTORY]"
            Exit
        }
    }

    # Clean up stale files from the previous build and copy clean directory tree for this build
    # Note:
    # For no apparent reason, sometimes Test-Path throws
    # UnauthorizedAccessException. Since we must own this directory,
    # the exception is ignored on Test-Path and the Remove-Item may work
    # as expected. If the exception is for real, Remove-Item will fail
    # anyway, so this is a safe thing to do.
    if (Test-Path -Path $OPENSSL_DIRECTORY -ErrorAction SilentlyContinue) {
        Remove-Item $OPENSSL_DIRECTORY -Recurse -Force
        if ($? -eq $false) {
            Write-Host "Error removing old OpenSSL directory: $OPENSSL_DIRECTORY"
            Exit
        }
    }

    Copy-Item -Path "$OPENSSL_CLEAN_DIRECTORY" -Destination "$OPENSSL_DIRECTORY" -Recurse -Force
    if ($? -eq $false) {
        Write-Host "Error copying clean OpenSSL directory [$OPENSSL_CLEAN_DIRECTORY] to [$OPENSSL_DIRECTORY]"
        Exit
    }
}

function Replace-String {
    param (
        [string]$file,
        [string]$original,
        [string]$replacement
    )
    (gc $file).replace($original, $replacement) | sc $file
}

function Compile-OpenSSL {
    param (
        [string]$winplatform,
        [string]$configuration,
        [string]$target,
        [string]$library
    )

    # start every build with clean directory
    Unpack-OpenSSL

    pushd $OPENSSL_DIRECTORY

    # Set up portable Strawberry Perl
    $env:Path = "$(Join-Path $PERL_DIRECTORY perl\site\bin);" + $env:Path
    $env:Path = "$(Join-Path $PERL_DIRECTORY perl\bin);" + $env:Path
    $env:Path = "$(Join-Path $PERL_DIRECTORY c\bin);" + $env:Path

    $d = ""
    if ($configuration -eq "debug") { $d = "d" }

    $l = ""
    if ($library -eq "shared") { $l = "MD" }
    else                       { $l = "MT" }

    $b = "32"
    if ($winplatform -eq "win64") { $b = "64" }


    # Set up nasm
    $env:Path = "$NASM_DIRECTORY;" + $env:Path

    perl Configure $target --prefix="bin/$winplatform/$configuration"

    # Run nasm
    cmd /c ms\do_nasm.bat

    if ($winplatform -eq "win64") {
        Replace-String "util\pl\VC-32.pl" "`$ssl=	`"ssleay32`"" `
                                          "`$ssl=	`"ssleay$b$l$d`""
        
        Replace-String "util\pl\VC-32.pl" "`$crypto=`"libeay32`"" `
                                          "`$crypto=`"libeay$b$l$d`""
        
        Replace-String "util\pl\VC-32.pl" "`$crypto=`"libeayfips32`"" `
                                          "`$crypto=`"libeayfips$b$l$d`""
        
        Replace-String "ms\do_win64a.bat" "perl util\mkdef.pl 32 libeay > ms\libeay32.def" `
                                          "perl util\mkdef.pl $b libeay > ms\libeay$b$l$d.def"
        
        Replace-String "ms\do_win64a.bat" "perl util\mkdef.pl 32 ssleay > ms\ssleay32.def" `
                                          "perl util\mkdef.pl $b ssleay > ms\ssleay$b$l$d.def"
        
        cmd /c ms\do_win64a

        Replace-String "ms\libeay$b$l$d.def" "LIBEAY32" "LIBEAY$b$l$d"
        Replace-String "ms\ssleay$b$l$d.def" "SSLEAY32" "SSLEAY$b$l$d"
    }
    else {
        Replace-String "ms\libeay32.def" "LIBEAY32" "LIBEAY$b$l$d"
        Replace-String "ms\ssleay32.def" "SSLEAY32" "SSLEAY$b$l$d"
    }

    $lib = ""
    if ($library -eq "shared") { $lib = "dll" }
    if ($configuration -eq "debug") {
        Replace-String "ms\nt$lib.mak" "`$(TMP_D)/lib" "`$(TMP_D)/openssl$b$l$d"
    }

    # Run nmake
    nmake -f ms\nt$lib.mak

    # Run nmake install
    nmake -f ms\nt$lib.mak install

    popd
}

function Output-OpenSSL {
    param (
        [string]$winplatform,
        [string]$configuration,
        [string]$library
    )

    pushd $OPENSSL_DIRECTORY
    
    $t = Join-Path $OUTPUT_BIN_DIRECTORY "$winplatform"
    $lib = "lib"

    $d = ""
    if ($configuration -eq "debug") { $d = "d" }

    $l = ""
    if ($library -eq "shared") { $l = "MD" }
    else                       { $l = "MT" }

    $b = "32"
    if ($winplatform -eq "win64") { $b = "64" }

    # Copy output files
    if ($library -eq "shared") {
        xcopy /y bin\$winplatform\$configuration\bin\*.dll "$t\bin\$configuration\*"

        if ($winplatform -eq "win32") {
            if (Test-Path "$t\bin\$configuration\libeay$b$l$d.dll") {
                Remove-Item "$t\bin\$configuration\libeay$b$l$d.dll" -force
            }
            Rename-Item -path "$t\bin\$configuration\libeay32.dll" -newname "libeay$b$l$d.dll" -force

            if (Test-Path "$t\bin\$configuration\ssleay$b$l$d.dll") {
                Remove-Item "$t\bin\$configuration\ssleay$b$l$d.dll" -force
            }
            Rename-Item -path "$t\bin\$configuration\ssleay32.dll" -newname "ssleay$b$l$d.dll" -force
        }

        $lib = "bin"
    }

    xcopy /y bin\$winplatform\$configuration\lib\*.lib "$t\$lib\$configuration\*" 
    
    if (($configuration -eq "debug") -and ($library -eq "static")) {
        $tmp = Join-Path $OPENSSL_DIRECTORY "tmp32.dbg"
        xcopy /y $tmp\openssl*.pdb "$t\$lib\$configuration\*" /E
    }

    if ($winplatform -eq "win32") {
        if (Test-Path "$t\$lib\$configuration\libeay$b$l$d.lib") {
             Remove-Item "$t\$lib\$configuration\libeay$b$l$d.lib" -force
        }
        Rename-Item -path "$t\$lib\$configuration\libeay32.lib" -newname "libeay$b$l$d.lib" -force

        if (Test-Path "$t\$lib\$configuration\ssleay$b$l$d.lib") {
             Remove-Item "$t\$lib\$configuration\ssleay$b$l$d.lib" -force
        }
        Rename-Item -path "$t\$lib\$configuration\ssleay32.lib" -newname "ssleay$b$l$d.lib" -force
    }

    if (!(Test-Path -Path "$OUTPUT_INC_DIRECTORY")) {
        xcopy /y bin\$winplatform\$configuration\include\* "$OUTPUT_INC_DIRECTORY\*" /E
    }

    popd
}



if ($platform -eq "Win32") {
    
    if (($config -eq "debug") -or ($config -eq "both")) {
        if (($library -eq "shared") -or ($library -eq "both")) {
            Compile-OpenSSL "win32" "debug" "debug-VC-WIN32" "shared"
            Output-OpenSSL  "win32" "debug" "shared"
        }
        
        if (($library -eq "static") -or ($library -eq "both")) {
            Compile-OpenSSL "win32" "debug" "debug-VC-WIN32" "static"
            Output-OpenSSL  "win32" "debug" "static"
        }
    }

    if (($config -eq "release") -or ($config -eq "both")) {
        if (($library -eq "shared") -or ($library -eq "both")) {
            Compile-OpenSSL "win32" "release" "VC-WIN32" "shared"
            Output-OpenSSL  "win32" "release" "shared"
        }
        
        if (($library -eq "static") -or ($library -eq "both")) {
            Compile-OpenSSL "win32" "release" "VC-WIN32" "static"
            Output-OpenSSL  "win32" "release" "static"
        }
    }
}
elseif ($platform -eq "x64") {

    if (($config -eq "debug") -or ($config -eq "both")) {
        if (($library -eq "shared") -or ($library -eq "both")) {
            Compile-OpenSSL "win64" "debug" "debug-VC-WIN64A" "shared"
            Output-OpenSSL  "win64" "debug" "shared"
        }
        
        if (($library -eq "static") -or ($library -eq "both")) {
            Compile-OpenSSL "win64" "debug" "debug-VC-WIN64A" "static"
            Output-OpenSSL  "win64" "debug" "static"
        }
    }

    if (($config -eq "release") -or ($config -eq "both")) {
        if (($library -eq "shared") -or ($library -eq "both")) {
            Compile-OpenSSL "win64" "release" "VC-WIN64A" "shared"
            Output-OpenSSL  "win64" "release" "shared"
        }
        
        if (($library -eq "static") -or ($library -eq "both")) {
            Compile-OpenSSL "win64" "release" "VC-WIN64A" "static"
            Output-OpenSSL  "win64" "release" "static"
        }
    }
}
else {
    Write-Error "Unknown platform: $platform"
    Exit
}
