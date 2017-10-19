del /Q /s *.pdb
del /Q /s /f *.suo
del /Q /s *.ncb
del /Q /s *.VC.db
del /Q /s *.pch
del /Q /s *.ipch
del /Q /s *.tlog
del /Q /s *.ilk
del /Q /s *.exp
del /Q /s *.map
del /Q /s *.log
del /Q /s *.obj
del /Q /s *.lastbuildstate
del /Q /s *.obj


for /r %%d in (.) do if exist "%%d\ipch" rd /s /q "%%d\ipch"
