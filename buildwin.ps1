#
# POCO build script
# 
# Usage:
# ------
# buildwin.ps1 [-poco_base    dir]
#              [-vs_version   140 | 120 | 110 | 100 | 90]
#              [-action       build | rebuild | clean]
#              [-linkmode     shared | static_mt | static_md | all]
#              [-config       release | debug | both]
#              [-platform     Win32 | x64 | WinCE | WEC2013]
#              [-samples]
#              [-tests]
#              [-omit         "Lib1X;LibY;LibZ;..."]
#              [-tool         msbuild | devenv | vcexpress | wdexpress]
#              [-openssl_base dir]
#              [-mysql_base   dir]

[CmdletBinding()]
Param
(
  [Parameter()]
  [string] $poco_base,

  [Parameter()]
  [ValidateSet(90, 100, 110, 120, 140)]
  [int] $vs_version,

  [Parameter()]
  [ValidateSet('build', 'rebuild', 'clean')]
  [string] $action = 'build',

  [Parameter()]
  [ValidateSet('shared', 'static_mt', 'static_md', 'all')]
  [string] $linkmode = 'shared',

  [Parameter()]
  [ValidateSet('release', 'debug', 'both')]
  [string] $config = 'release',

  [Parameter()]
  [ValidateSet('Win32', 'x64', 'WinCE', 'WEC2013')]
  [string] $platform = 'x64',
  
  [switch] $tests = $false,
  [switch] $samples = $false,
  [string] $omit,
  
  [Parameter()]
  [ValidateSet('msbuild', 'devenv', 'vcexpress', 'wdexpress')]
  [string] $tool = 'msbuild',

  [Parameter()]
  [string] $openssl_base,

  [Parameter()]
  [string] $mysql_base,

  [switch] $help
)


function Add-Env-Var([string] $lib, [string] $var)
{
  if ((${Env:$var} -eq $null) -or (-not ${Env:$var}.Contains(${Env:$lib_$var"})))
  {
    $libvar = "$lib" + "_" + "$var"
    $envvar = [Environment]::GetEnvironmentVariable($libvar, "Process")
    [Environment]::SetEnvironmentVariable($var, $envvar, "Process")
  }
  
}


function Set-Environment
{
  if ($poco_base -eq '') { $script:poco_base = Get-Location }

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

  if (-Not $Env:PATH.Contains("$Env:POCO_BASE\bin64;$Env:POCO_BASE\bin;")) 
  { $Env:PATH = "$Env:POCO_BASE\bin64;$Env:POCO_BASE\bin;$Env:PATH" }

  if ($openssl_base -eq '')
  {
    if ($platform -eq 'x64') { $script:openssl_base = 'C:\OpenSSL-Win64' }
    else                     { $script:openssl_base = 'C:\OpenSSL-Win32' }
  }
  
  $Env:OPENSSL_DIR     = "$openssl_base"
  $Env:OPENSSL_INCLUDE = "$Env:OPENSSL_DIR\include"
  $Env:OPENSSL_LIB     = "$Env:OPENSSL_DIR\lib;$Env:OPENSSL_DIR\lib\VC"
  Add-Env-Var "OPENSSL" "INCLUDE"
  Add-Env-Var "OPENSSL" "LIB"

  if ($mysql_base -ne '')
  {
    $Env:MYSQL_DIR     = "$mysql_base"
    $Env:MYSQL_INCLUDE = "$Env:MYSQL_DIR\include"
    $Env:MYSQL_LIB     = "$Env:MYSQL_DIR\lib"
    Add-Env-Var "MYSQL" "INCLUDE"
    Add-Env-Var "MYSQL" "LIB"
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


function Process-Input
{
  if ($help -eq $true)
  {
    Write-Host 'Usage:'
    Write-Host '------'
    Write-Host 'buildwin.ps1 [-poco_base    dir]'
    Write-Host '             [-vs_version   140 | 120 | 110 | 100 | 90]'
    Write-Host '             [-action       build | rebuild | clean]'
    Write-Host '             [-linkmode     shared | static_mt | static_md | all]'
    Write-Host '             [-config       release | debug | both]'
    Write-Host '             [-platform     Win32 | x64 | WinCE | WEC2013]'
    Write-Host '             [-samples]'
    Write-Host '             [-tests]'
    Write-Host '             [-omit         "Lib1X;LibY;LibZ;..."]'
    Write-Host '             [-tool         msbuild | devenv | vcexpress | wdexpress]'
    Write-Host '             [-openssl_base dir]'
    Write-Host '             [-mysql_base   dir]'

    Exit
  }
  else
  { 
    Set-Environment

    Write-Host "Build configuration:"
    Write-Host "--------------------"
    Write-Host "Poco Base:     $poco_base"
    Write-Host "Version:       $vs_version"
    Write-Host "Action:        $action"
    Write-Host "Link Mode:     $linkmode"
    Write-Host "Configuration: $config"
    Write-Host "Platform:      $platform"
    Write-Host "Tests:         $tests"
    Write-Host "Samples:       $samples"
    Write-Host "Build Tool:    $tool"

    if ($omit -ne '')
    {
      Write-Host "Omit:          $omit"
    }

    if ($openssl_base -ne '')
    {
      Write-Host "OpenSSL:       $openssl_base"
    }
  
    if ($mysql_base -ne '')
    {
      Write-Host "MySQL:         $mysql_base"
    }

    # NB: this won't work in PowerShell ISE
    Write-Host "Press Ctrl-C to exit or any other key to continue ..."
    $x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyUp")
  }
}


function Build-MSBuild([string] $vsProject)
{
  Write-Host "Build-MSBuild ==> $vsProject"
  if ($linkmode -eq 'all')
  {
    $linkModeArr = 'shared', 'static_mt', 'static_md'

    foreach ($mode in $linkModeArr)
    {
      if ($config -eq 'both')
      {
        $configArr = 'release', 'debug'
        foreach ($cfg in $configArr)
        {
          $projectConfig = "$cfg"
          $projectConfig += "_$mode"
          Invoke-Expression "msbuild $vsProject /t:$action /p:Configuration=$projectConfig /p:Platform=$platform /p:useenv=true"
        }
      }
      else #config
      {
        $projectConfig = "$config"
        $projectConfig += "_$mode"
        Invoke-Expression "msbuild $vsProject /t:$action /p:Configuration=$projectConfig /p:Platform=$platform /p:useenv=true"
      }
    }
  }
  else #linkmode
  {
    if ($config -eq 'both')
    {
      $configArr = 'release', 'debug'
      foreach ($cfg in $configArr)
      {
        $projectConfig = "$cfg"
        $projectConfig += "_$linkmode"
        Invoke-Expression "msbuild $vsProject /t:$action /p:Configuration=$projectConfig /p:Platform=$platform /p:useenv=true"
      }
    }
    else #config
    {
      $projectConfig = "$config"
      $projectConfig += "_$linkmode"
      Invoke-Expression "msbuild $vsProject /t:$action /p:Configuration=$projectConfig /p:Platform=$platform /p:useenv=true"
    }
  }
}


function Build-Devenv([string] $vsProject)
{
  if ($linkmode -eq 'all')
  {
    $linkModeArr = 'shared', 'static_mt', 'static_md'

    foreach ($mode in $linkModeArr)
    {
      if ($config -eq 'both')
      {
        $configArr = 'release', 'debug'
        foreach ($cfg in $configArr)
        {
          $projectConfig = "$cfg"
          $projectConfig += "_$mode"
          Invoke-Expression "devenv /useenv /$action $projectConfig $vsProject"
        }
      }
      else #config
      {
        $projectConfig = "$config"
        $projectConfig += "_$mode"
        Invoke-Expression "devenv /useenv /$action $projectConfig $vsProject"
      }
    }
  }
  else #linkmode
  {
    if ($config -eq 'both')
    {
      $configArr = 'release', 'debug'
      foreach ($cfg in $configArr)
      {
        $projectConfig = "$cfg"
        $projectConfig += "_$linkmode"
        Invoke-Expression "devenv /useenv /$action $projectConfig $vsProject"
      }
    }
    else #config
    {
      $projectConfig = "$config"
      $projectConfig += "_$linkmode"
      Invoke-Expression "devenv /useenv /$action $projectConfig $vsProject"
    }
  }
  $projectConfig = "$config"
  $projectConfig += "_$linkmode"
  Invoke-Expression "devenv /useenv /$action $projectConfig $vsProject"
}


function Build-samples
{
  process { 
    $sampleName = $_.BaseName.split("_")[0]
    $sampleProjName = "$($poco_base)\$($componentDir)\samples\$($sampleName)\$($_)"
    if ($tool -eq 'devenv') { Build-Devenv $sampleProjName }
    elseif ($tool -eq 'msbuild') { Build-MSBuild $sampleProjName }
    else{ Write-Host "Tool not supported: $tool" }
  }
}


function Build
{
  Process-Input

  if ($vs_version -lt 100) { $extension = 'vcproj'  }
  else                     { $extension = 'vcxproj' }

  $platformName = ''
  if ($platform -eq 'x64')       { $platformName = '_x64' }
  elseif ($platform -eq 'WinCE') { $platformName = '_CE' }

  Get-Content "$poco_base\components" | Foreach-Object {

    $component = $_
    $componentDir = $_.Replace("/", "\")
    $componentArr = $_.split('/')
    $componentName = $componentArr[$componentArr.Length - 1]
    $suffix = "_vs$vs_version"
    
    $omitArray = @()
    $omit.Split(',;') | ForEach {
        $omitArray += "$_"
    }

    if ($omitArray -NotContains $component)
    {
      $vsProject = "$poco_base\$componentDir\$componentName$($platformName)$($suffix).$($extension)"
      
      if (!(Test-Path -Path $vsProject)) # when VS project name is not same as directory name
      {
        $vsProject = "$poco_base\$componentDir$($platformName)$($suffix).$($extension)"
        if (!(Test-Path -Path $vsProject)) # not found
        {
          Write-Host "+------------------------------------------------------------------"
          Write-Host "| VS project $vsProject not found, skipping."
          Write-Host "+------------------------------------------------------------------"
          Return # since Foreach-Object is a function, this is actually loop "continue"
        }
      }
      
      Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
      Write-Host "| Building $vsProject"
      Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

      if ($tool -eq 'devenv')      { Build-Devenv $vsProject }
      elseif ($tool -eq 'msbuild') { Build-MSBuild $vsProject }
      elseif ($tool -ne '')        { Write-Host "Build tool not supported: $tool" }
      else 
      {
        Write-Host "Build tool not specified. Exiting."
        Exit
      } 

      if ($tests)
      {
        $vsTestProject = "$poco_base\$componentDir\testsuite\TestSuite$($platformName)$($suffix).$($extension)"
        Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        Write-Host "| Building $vsTestProject"
        Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

        if ($tool -eq 'devenv') { Build-Devenv $vsTestProject }
        elseif ($tool -eq 'msbuild') { Build-MSBuild $vsTestProject }
        else{ Write-Host "Tool not supported: $tool" }
      }

      if ($samples)
      {
        Get-Childitem "$poco_base\$($componentDir)" -Recurse |`
          Where {$_.Extension -Match $extension -And $_.DirectoryName -Like "*samples*" -And $_.BaseName -Like "*$platformName$($suffix)" } `
          | Build-samples "$_"
      }
    }
    else
    {
      Write-Host "-------------------------------"
      Write-Host "# Skipping $componentDir"
      Write-Host "-------------------------------"
    }
  }
}


Build
