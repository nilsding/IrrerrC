; NSIS installer for IrrerrC
; (c) 2015 nilsding

!include "MUI2.nsh"
!include "..\build\IrrerrC_files.nsh"

Name "IrrerrC"
OutFile "..\build\output\IrrerrC_Installer.exe"
SetCompressor /SOLID lzma

; Installation folders and registration keys
InstallDir "$PROGRAMFILES\IrrerrC"
InstallDirRegKey HKLM "Software\nilsding\IrrerrC" "Install_Dir"
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
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\nilsding\IrrerrC"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!insertmacro MUI_PAGE_STARTMENU IrrerrC $StartMenuFolder
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Languages
!insertmacro MUI_LANGUAGE "English"

; Installer Sections
Section ""
  !insertmacro IrrerrC_Install_Files

  WriteRegStr HKLM "Software\nilsding\IrrerrC" "" $INSTDIR
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN IrrerrC
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\IrrerrC.lnk" "$INSTDIR\IrrerrC.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section "Uninstall"
  !insertmacro IrrerrC_Uninstall_Files

  !insertmacro MUI_STARTMENU_GETFOLDER IrrerrC $StartMenuFolder
  Delete "$SMPROGRAMS\$StartMenuFolder\IrrerrC.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey /ifempty HKCU "Software\nilsding\IrrerrC"
SectionEnd

