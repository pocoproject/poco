#
# POCO progen automation script
#
# Usage:
# ------
# progen.ps1   [-poco_base    dir]
#              [-vs           160| 170]
#              [-omit         "Lib1X,LibY,LibZ,..."]
#              [-components   "Lib1X,LibY,LibZ,..."]
#              [-platform     Win32 | x64 | ARM64 | WEC2013]
#              [-samples]
#              [-tests]
#              [-nobuild]

[CmdletBinding()]
Param
(
	[Parameter()]
	[string] $poco_base = $([System.Environment]::GetEnvironmentVariable('POCO_BASE')),

	[Parameter()]
	[ValidateSet(160, 170)]
	[int] $vs = 170,

	[string] $omit,
	[string] $components,

	[Parameter()]
	[ValidateSet('Win32', 'x64', 'ARM64', 'WEC2013')]
	[string] $platform = 'x64',

	[switch] $samples = $false,
	[switch] $tests = $false,
	[switch] $nobuild = $false,


	[switch] $help
)

function ProcessInput
{
	if ($help -eq $true)
	{
		Write-Host 'Usage:'
		Write-Host '------'
		Write-Host 'progen.ps1 [-poco_base <dir>]'
		Write-Host '    [-vs           160 | 170]'
		Write-Host '    [-omit         "Lib1X,LibY,LibZ,..."]'
		Write-Host '    [-components   "Lib1X,LibY,LibZ,..."]'
		Write-Host '    [-samples]'
		Write-Host '    [-tests]'
		Write-Host '    [-nobuild]'
		Exit
	}
	else
	{
		if($components -ne '' -and $omit -ne '') {
			Write-Host "-components and -omit cannot be used simultaneously, exiting..."
			Exit
		}
		Write-Host ""
		Write-Host "--------------------"
		Write-Host "Progen configuration:"
		Write-Host "--------------------"
		Write-Host "Poco Base:     $poco_base"
		Write-Host "Version:       $vs"
		Write-Host "Samples:       $samples"
		Write-Host "Tests:         $tests"
		Write-Host "No Build:      $nobuild"

		if ($omit -ne '')
		{
			Write-Host "Omit:          $omit"
		}

		if ($components -ne '')
		{
			Write-Host "Components:          $components"
		}

		Write-Host "----------------------------------------"
		Write-Host ""

		# NB: this won't work in PowerShell ISE
		#Write-Host "Press Ctrl-C to exit or any other key to continue ..."
		#$x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyUp")
	}
}

function InvokeProcess([string] $exe, [string] $arguments)
{
	$proc = Start-Process -NoNewWindow  -FilePath $exe -ArgumentList $arguments -PassThru
	$handle = $proc.Handle # cache proc.Handle, necessary to get exit code
	$proc.WaitForExit();
	if ($proc.ExitCode -ne 0) {
	    Write-Warning "$_ exited with status code $($proc.ExitCode)"
	}
}

function InvokeProgenSamples
{
	process {
		$sampleName = $_.BaseName.split(".")[0]
		if($_.Name -eq "samples.progen") {
			Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
			Write-Host "| Running Progen for $componentDir\$sampleName"
			Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
			$sampleProgenPath = "$($poco_base)\$($componentDir)\$($sampleName)\$($_)"
		}
		else {
			Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
			Write-Host "| Running Progen for $componentDir\samples\$sampleName"
			Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
			$sampleProgenPath = "$($poco_base)\$($componentDir)\samples\$($sampleName)\$($_)"
		}
		if (Test-Path -Path $sampleProgenPath) {
			InvokeProcess $progenPath "/tool=vs$vs $sampleProgenPath"
		}
		else {
			Write-Host "NOTICE: No .progen file for $sampleName"
		}
	}
}

function InvokeProgenComponents([string] $type)
{
	if(Test-Path "$poco_base\ProGen\bin64\static_mt\progen.exe") {
		$progenPath = Resolve-Path "$poco_base\ProGen\bin64\static_mt\progen.exe"
	}
	elseif(Test-Path "$poco_base\ProGen\bin64\static_md\progen.exe") {
		$progenPath = Resolve-Path "$poco_base\ProGen\bin64\static_md\progen.exe"
	}
	else {
		$progenPath = Resolve-Path "$poco_base\ProGen\bin64\progen.exe"
	}
	$exists = Test-Path "$poco_base\ProGen\bin64\static_mt\progen.exe"
	if (-not $exists) {
		Write-Error "Progen not found, exiting..."
		Exit -1
	}

	Get-Content "$poco_base\components" | Foreach-Object {

		$component = $_
		$componentDir = $_.Replace("/", "\")
		$componentArr = $_.split('/')
		$componentName = $componentArr[$componentArr.Length - 1]

		$omitArray = @()
		$omit.Split(',') | ForEach-Object {
				$omitArray += $_.Trim()
		}

		$componentsArray = @()
		$components.Split(',') | ForEach-Object {
				$componentsArray += $_.Trim()
		}

		if ($omitArray -NotContains $component -and ((-not ($component -Contains "Foundation")) -or ($type -eq "sample")) -and (($componentsArray -Contains $component) -or ($components -eq '')))
		{
			if($type -eq "lib") {
				Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
				Write-Host "| Running ProGen for $componentDir"
				Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
				$componentProgenPath = "$poco_base\$componentDir\$componentName.progen"
				InvokeProcess $progenPath "/tool=vs$vs $componentProgenPath"
			}
			ElseIf ($tests -and ($type -eq "test")) {
				if (Test-Path -Path "$poco_base\$componentDir\testsuite") {
					$componentTestProgenPath = "$poco_base\$componentDir\testsuite\TestSuite.Progen"
					if (Test-Path -Path $componentTestProgenPath) {
						Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
						Write-Host "| Running Progen for $componentDir\testsuite"
						Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
						InvokeProcess $progenPath "/tool=vs$vs $componentTestProgenPath"

						if ($component -eq "Data") # special case for Data
						{
							$componentTestProgenPath = "$poco_base\$componentDir\DataTest\DataTest.progen"
							Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
							Write-Host "| Running Progen for $componentDir\DataTest"
							Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
							InvokeProcess $progenPath "/tool=vs$vs $componentTestProgenPath"

						}
					}
					Else {
						Write-Host "NOTICE: Missing .progen file for $componentDir\testsuite"
					}
				}
			}
			ElseIf ($samples -and ($type -eq "sample")) {
				Get-Childitem "$poco_base\$($componentDir)" -Recurse |`
				Where-Object {$_.Extension -Match ".progen" -And $_.DirectoryName -Like "*samples*" } `
				| InvokeProgenSamples "$_"
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

function InvokeBuildWin {
	Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	Write-Host "| Building Foundation,XML,JSON,Util,Progen"
	Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	Invoke-Expression "$poco_base\buildwin.ps1 -poco_base $poco_base -platform $platform -linkmode static_mt -vs $vs -action build -components `"Foundation,XML,JSON,Util,Progen`" "
	Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	Write-Host "| Build finished."
	Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
}

function Run
{
	ProcessInput

	if($nobuild -eq $false) {
		InvokeBuildWin
	}

	InvokeProgenComponents "lib"
	InvokeProgenComponents "test"
	InvokeProgenComponents "sample"
}


Run