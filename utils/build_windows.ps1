# This script builds and prepares IrrerrC for Windows hosts using QMake, Visual Studio 2013 and WinDeployQt.

. .\ext\Invoke-Environment.ps1
. .\ext\functions.ps1

$dirs = @{
  utildir  = (pwd);
  repodir  = [System.IO.Path]::GetFullPath((Join-Path (pwd) "..\"));  # path to the repo
  builddir = [System.IO.Path]::GetFullPath((Join-Path (pwd) "..\build\"));  # path to the build directory
  outdir   = [System.IO.Path]::GetFullPath((Join-Path (pwd) "..\build\output"));  # path to the output directory
  qtdir    = "C:\Qt\5.4\msvc2013\bin\";  # the Qt dir to use (must contain qtenv2.bat)
  vsdir    = [System.IO.Path]::GetFullPath((Join-Path $env:VS120COMNTOOLS "..\..\VC\"));  # path to Visual C++ (must contain vcvarsall.bat)
};

$flags = @{
  qmake       = @('"CONFIG+=windows"', '-config release');
  nmake       = @('/nologo');
  windeployqt = @('--release', '--no-compiler-runtime', '--no-webkit2', '--no-angle', '--no-system-d3d-compiler', '--no-quick-import',
                  '--no-translations',
                  '"--no-bluetooth"', '--no-clucene', '--no-concurrent', '--no-declarative', '--no-designer', '--no-designercomponents',
                  '--no-enginio', '--no-qthelp', '--no-multimedia', '--no-multimediawidgets', '--no-multimediaquick', '"--no-nfc"', '--no-opengl',
                  '--no-positioning', '--no-printsupport', '--no-qml', '--no-qmltooling', '--no-quick', '--no-quickparticles',
                  '--no-quickwidgets', '--no-script', '--no-scripttools', '--no-sensors', '--no-serialport', '--no-sql', '--no-test',
                  '--no-webkit', '--no-webkitwidgets', '--no-websockets', '--no-xml', '--no-xmlpatterns', '--no-webenginecore', '--no-webengine',
                  '--no-webenginewidgets');
}

$build_bin_path = [System.IO.Path]::GetFullPath((Join-Path $dirs['builddir'] ".\release\IrrerrC.exe"));
$output_bin_path = [System.IO.Path]::GetFullPath((Join-Path $dirs['outdir'] ".\IrrerrC.exe"));

Write-Host -ForegroundColor Green ">>> Applying Qt environment variables";
Invoke-Environment -output "`"$(Join-Path $dirs['qtdir'] "qtenv2.bat")`"";
Write-Host -ForegroundColor Green ">>> Applying Visual Studio environment variables";
Invoke-Environment -output "`"$(Join-Path $dirs['vsdir'] "vcvarsall.bat")`" x86";

Write-Host -ForegroundColor Green ">>> Preparing build directory";
if (Test-Path $dirs['builddir']) {
  Remove-Item -Recurse -Force $dirs['builddir'];
}
mkdir $dirs['builddir'];
Set-Location $dirs['builddir'];

Write-Host -ForegroundColor Green ">>> Configuring (qmake)";
& qmake (,$flags['qmake']) $dirs['repodir'];
Check-Success -ErrorMessage "Could not configure project!" "Configuration successful."

Write-Host -ForegroundColor Green ">>> Starting build (nmake)";
& nmake (,$flags['nmake']);
Check-Success -ErrorMessage "Build failed!" "Build successful."

Write-Host -ForegroundColor Green ">>> Preparing output directory";
if (Test-Path $dirs['outdir']) {
  Remove-Item -Recurse -Force $dirs['outdir'];
}
mkdir $dirs['outdir'];
Set-Location $dirs['outdir'];
Move-Item $build_bin_path $output_bin_path

Write-Host -ForegroundColor Green ">>> Deploying to $($dirs['outdir']) (windeployqt)";
& windeployqt $flags['windeployqt'] $output_bin_path;
Check-Success -ErrorMessage "Deployment failed!" "Deployment successful."
if (Test-Path ([System.IO.Path]::GetFullPath((Join-Path $dirs['outdir'] ".\bearer")))) {
  Remove-Item -Recurse -Force ([System.IO.Path]::GetFullPath((Join-Path $dirs['outdir'] ".\bearer")));
}
if (Test-Path ([System.IO.Path]::GetFullPath((Join-Path $dirs['outdir'] ".\opengl32sw.dll")))) {
  Remove-Item -Recurse -Force ([System.IO.Path]::GetFullPath((Join-Path $dirs['outdir'] ".\opengl32sw.dll")));
}

Write-Host -ForegroundColor Green ">>> Windows build complete!";
Write-Host -NoNewline "To create an installer, you may run ";
Write-Host -NoNewline -ForegroundColor Yellow ".\mknsis.ps1";
Write-Host " now.";

Set-Location $dirs['utildir'];
