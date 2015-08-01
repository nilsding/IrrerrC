; NSIS installer for nIRC
; (c) 2015 nilsding

!include "MUI2.nsh"
!include "..\build\nIRC_files.nsh"

Name "nIRC"
OutFile "..\build\output\nIRC_Installer.exe"
SetCompressor /SOLID lzma

; Installation folders and registration keys
InstallDir "$PROGRAMFILES\nIRC"
InstallDirRegKey HKLM "Software\nilsding\nIRC" "Install_Dir"
; Application privileges
RequestExecutionLevel admin

; Variables
Var StartMenuFolder

; Interface settings
!define MUI_ABORTWARNING

; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\nilsding\nIRC"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!insertmacro MUI_PAGE_STARTMENU nIRC $StartMenuFolder
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Languages
!insertmacro MUI_LANGUAGE "English"

; Installer Sections
Section ""
  !insertmacro nIRC_Install_Files

  WriteRegStr HKLM "Software\nilsding\nIRC" "" $INSTDIR
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN nIRC
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\nIRC.lnk" "$INSTDIR\nIRC.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section "Uninstall"
  !insertmacro nIRC_Uninstall_Files

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  Delete "$SMPROGRAMS\$StartMenuFolder\nIRC.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey /ifempty HKCU "Software\nilsding\nIRC"
SectionEnd

