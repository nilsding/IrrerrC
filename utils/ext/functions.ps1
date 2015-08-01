<#
.Synopsis
   checks if the previous operation succeeded and write it to the console
.EXAMPLE
   Check-Success "Everything went fine."
.EXAMPLE
   Check-Success -Optional -ErrorMessage "Something happened" "Success!"
#>
function Check-Success
{
  Param
  (
    [Parameter(Mandatory=1)][string]$Message,
    [string] $ErrorMessage,
    [switch] $Optional
  )

  $emsg = if ($ErrorMessage) { $ErrorMessage } else { "An error occurred." };
  
  if ($LASTEXITCODE -eq 0) {
    Write-Host -ForegroundColor Green ">>> $Message"
  } else {
    Write-Host -ForegroundColor Red ">>> $emsg"
    if (-not $Optional) {
      exit 1;
    }
  }
}
