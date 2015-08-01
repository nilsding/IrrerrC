<#
.Synopsis
	Invokes a command and imports its environment variables.

.Link
	https://github.com/nightroman/PowerShelf
#>
<#
.Synopsis
   Short description
.DESCRIPTION
   Long description
.EXAMPLE
   Example of how to use this cmdlet
.EXAMPLE
   Another example of how to use this cmdlet
#>
function Invoke-Environment
{
param
(
	[Parameter(Mandatory=1)][string]$Command,
	[switch]$Output,
	[switch]$Force
)

$stream = if ($Output) { ($temp = [IO.Path]::GetTempFileName()) } else { 'nul' }
$operator = if ($Force) {'&'} else {'&&'}

foreach($_ in cmd /c " $Command > `"$stream`" 2>&1 $operator SET") {
	if ($_ -match '^([^=]+)=(.*)') {
		[System.Environment]::SetEnvironmentVariable($matches[1], $matches[2])
	}
}

if ($Output) {
	Get-Content -LiteralPath $temp
	Remove-Item -LiteralPath $temp
}

}