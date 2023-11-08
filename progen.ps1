#
# POCO progen automation script
#
# Usage:
# ------
# progen.ps1   [-poco_base    dir]
#              [-vs           140 | 150 | 160| 170]
#              [-omit         "Lib1X,LibY,LibZ,..."]

[CmdletBinding()]
Param
(
	[Parameter()]
	[string] $poco_base = ".",

	[Parameter()]
	[ValidateSet(140, 150, 160, 170)]
	[int] $vs = 140,

	[string] $omit,

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

function Run-Progen-Components
{
	$progenPath = Resolve-Path "$poco_base\ProGen\bin64\static_mt\progen.exe"
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
			$componentProgenPath = "$poco_base\$componentDir\$componentName.Progen"

			Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
			Write-Host "| Running ProGen for $componentDir"
			Write-Host "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

			Invoke-Expression "$progenPath /tool=vs$vs $componentProgenPath" 
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

	Run-Progen-Components
}


Run