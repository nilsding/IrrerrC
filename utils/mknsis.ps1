# This script creates a NSIS installer.

. .\ext\functions.ps1

$dirs = @{
  utildir  = (pwd);
  repodir  = [System.IO.Path]::GetFullPath((Join-Path (pwd) "..\"));  # path to the repo
  builddir = [System.IO.Path]::GetFullPath((Join-Path (pwd) "..\build\"));  # path to the build directory
  outdir   = [System.IO.Path]::GetFullPath((Join-Path (pwd) "..\build\output"));  # path to the output directory
};

$files = @{
  makensis      = "C:\Program Files (x86)\NSIS\makensis.exe";  # full path to makensis.exe
  installscript = [System.IO.Path]::GetFullPath((Join-Path $dirs['utildir'] ".\installer.nsi"));  # path to the NSIS install script
  filelist      = [System.IO.Path]::GetFullPath((Join-Path $dirs['builddir'] ".\nIRC_files.nsh"));  # path to the (to be created) file list
};

if (-not (Test-Path -PathType Container $dirs['outdir'])) {
  Write-Host -ForegroundColor Red "Please run .\build_windows.ps1 first.";
  exit 1
}

Write-Host -ForegroundColor Green ">>> Building file list";
$infiles = @();  # contains the lines of the install part of the script
$unfiles = @();  # contains the lines of the uninstall part of the script 
if (Test-Path -PathType Leaf $files['filelist']) {
  Remove-Item $files['filelist'];
}

$rmdirs = @();  # contains the directories to remove
$last_outpath_opt = "";  # contains the last SetOutPath operation
Get-ChildItem -Recurse $dirs['outdir'] | ForEach-Object {
  $filename = $_.FullName.Substring($dirs['outdir'].Length + 1);
  if ($_.PSIsContainer) {  # this object is a directory
    Write-Host "d $filename";
    $rmdirs += if ($filename.LastIndexOf('\') -ne -1) {
                 ($filename.Substring(0, $filename.LastIndexOf('\')));
               } else {
                 ($filename);
               };
  } else {  # this object is a file
    Write-Host "a $filename";
    $path = if ($filename.LastIndexOf('\') -ne -1) {
              "\$($filename.Substring(0, $filename.LastIndexOf('\')))";
            } else {
              "";
            };
    $file = if ($filename.LastIndexOf('\') -ne -1) {
              $filename.Substring($filename.LastIndexOf('\') + 1);
            } else {
              $filename;
            };
    
    # this will not repeat "SetOutPath" all the time
    $next_outpath_opt = "SetOutPath `$INSTDIR$path";
    if ($next_outpath_opt -ne $last_outpath_opt) {
      $infiles += ($next_outpath_opt);
      $last_outpath_opt = $next_outpath_opt;
    }

    $infiles += ("File $($_.FullName)");
    $unfiles += ("Delete `"`$INSTDIR\$filename`"");
  }
}
$rmdirs | ForEach-Object {
  $unfiles += ("RMDir `"`$INSTDIR\$_`"");
}

Write-Host -ForegroundColor Green ">>> Writing file list";
# install macro
"!macro nIRC_Install_Files" | Out-File -Append -Encoding ascii $files['filelist'];
$infiles | ForEach-Object {
  $_ | Out-File -Append -Encoding ascii $files['filelist'];
}
"!macroend" | Out-File -Append -Encoding ascii $files['filelist'];

# uninstall macro
"!macro nIRC_Uninstall_Files" | Out-File -Append -Encoding ascii $files['filelist'];
$unfiles | Sort-Object | ForEach-Object {
  $_ | Out-File -Append -Encoding ascii $files['filelist'];
}
"!macroend" | Out-File -Append -Encoding ascii $files['filelist'];


Write-Host -ForegroundColor Green ">>> Building NSIS installer"
Set-Location $dirs['outdir'];
& $files['makensis'] $files['installscript'];
Check-Success -ErrorMessage "Build failed!" "Build successful."

Write-Host -NoNewline "The installer is located at ";
Write-Host -ForegroundColor Yellow ([System.IO.Path]::GetFullPath((Join-Path $dirs['outdir'] ".\nIRC_Installer.exe")));
& explorer "/select,$([System.IO.Path]::GetFullPath((Join-Path $dirs['outdir'] ".\nIRC_Installer.exe")))"

Set-Location $dirs['utildir'];
