preprocessor gcc -O2 main.c -o solverDepth10.exe||exit
solver.exe output.txt
echo "%~dp0exe\.exe" has exited with code %errorlevel% (0x%=ExitCode%).