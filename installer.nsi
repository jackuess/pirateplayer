Name "Pirateplayer"

OutFile "build\target\ppinstaller.exe"
InstallDir $PROGRAMFILES\Pirateplayer

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Pirateplayer" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

Section "Pirateplayer (obligatorisk)"

  SectionIn RO
  
  SetOutPath $INSTDIR
  File "build\target\pirateplayer.exe"
  File "build\target\ffmpeg.exe"
  File "build\target\ffplay.exe"
  File "build\target\*.dll"
  File "LICENSE"

  SetOutPath $INSTDIR\QtDesktop
  File /r "build\target\QtDesktop\*"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\Pirateplayer "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Pirateplayer" "DisplayName" "Pirateplayer"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Pirateplayer" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Pirateplayer" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Pirateplayer" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Startmenygenvägar"

  CreateDirectory "$SMPROGRAMS\Pirateplayer"
  CreateShortCut "$SMPROGRAMS\Pirateplayer\Avinstallera.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Pirateplayer\Pirateplayer.lnk" "$INSTDIR\pirateplayer.exe" "" "$INSTDIR\pirateplayer.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Pirateplayer"
  DeleteRegKey HKLM SOFTWARE\Pirateplayer

  ; Remove files and uninstaller
  Delete $INSTDIR\*.exe
  Delete $INSTDIR\LICENSE
  Delete "$INSTDIR\*.dll"
  RMDir /r "$INSTDIR\QtDesktop"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Pirateplayer\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Pirateplayer"
  RMDir "$INSTDIR"

SectionEnd
