for /D %%i in (*) DO (
	rd /s /q %%i
)

for %%i in (*) DO (
	if not %%i==rm.bat (
		del /s /q %%i
	)
)
exit
