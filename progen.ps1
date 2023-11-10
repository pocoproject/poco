#
# POCO progen automation script
#
# Usage:
# ------
# progen.ps1   [-poco_base    dir]
#              [-vs           140 | 150 | 160| 170]
#              [-omit         "Lib1X,LibY,LibZ,..."]
#              [-samples]
#              [-tests]

[CmdletBinding()]
Param
(
	[Parameter()]
	[string] $poco_base,

	[Parameter()]
	[ValidateSet(140, 150, 160, 170)]
	[int] $vs = 140,

	[string] $omit,
	[switch] $samples = $false,
	[switch] $tests = $false,

	[switch] $help
)

function Process-Input
{
	if ($help -eq $true)
	{
		Write-Host 'Usage:'
		Write-Host '------'
		Write-Host 'progen.ps1 [-poco_base <dir>]'
		Write-Host '    [-vs           140 | 150 | 160 | 170]'
		Write-Host '    [-omit         "Lib1X,LibY,LibZ,..."]'
		Write-Host '    [-samples]'
		Write-Host '    [-tests]'
		Exit
	}
	else
	{
		Write-Host ""
		Write-Host "--------------------"
		Write-Host "Progen configuration:"
		Write-Host "--------------------"
		Write-Host "Poco Base:     $poco_base"
		Write-Host "Version:       $vs"
		Write-Host "Samples:       $samples"
		Write-Host "Tests:         $tests"

		if ($omit -ne '')
		{
			Write-Host "Omit:          $omit"
		}

		Write-Host "----------------------------------------"
		Write-Host ""

		# NB: this won't work in PowerShell ISE
		#Write-Host "Press Ctrl-C to exit or any other key to continue ..."
		#$x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyUp")
	}
}

function Run-Progen-Samples
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
		Invoke-Expression "$progenPath /tool=vs$vs $sampleProgenPath" 
	}
}

function Run-Progen-Components([string] $type)
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
	Get-Content "$poco_base\components" | Foreach-Object {

		$component = $_
		$componentDir = $_.Replace("/", "\")
		$componentArr = $_.split('/')
		$componentName = $componentArr[$componentArr.Length - 1]

		$omitArray = @()
		$omit.Split(',') | ForEach-Object {
				$omitArray += $_.Trim()
		}

		if ($omitArray -NotContains $component)
		{

			Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
			Write-Host "| Running ProGen for $componentDir"
			Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

			if($type -eq "lib") {
				$componentProgenPath = "$poco_base\$componentDir\$componentName.Progen"
				Invoke-Expression "$progenPath /tool=vs$vs $componentProgenPath" 
			}
			ElseIf ($tests -and ($type -eq "test")) {
				$componentTestProgenPath = "$poco_base\$componentDir\testsuite\TestSuite.Progen"
				Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
				Write-Host "| Running Progen for $componentDir\testsuite"
				Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
				Invoke-Expression "$progenPath /tool=vs$vs $componentTestProgenPath" 
			}
			ElseIf ($samples -and ($type -eq "sample")) {
				Get-Childitem "$poco_base\$($componentDir)" -Recurse |`
				Where-Object {$_.Extension -Match ".progen" -And $_.DirectoryName -Like "*samples*" } `
				| Run-Progen-Samples "$_"
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

function Run
{
	Process-Input

	Run-Progen-Components "lib"
	Run-Progen-Components "test"
	Run-Progen-Components "sample"
}


Run