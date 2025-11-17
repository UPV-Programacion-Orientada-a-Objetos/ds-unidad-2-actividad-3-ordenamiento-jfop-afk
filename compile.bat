@echo off
echo ========================================
echo Compilando Sistema E-Sort
echo ========================================
echo.

REM Compilar cada archivo .cpp
echo Compilando SerialSource.cpp...
g++ -Wall -std=c++11 -c SerialSource.cpp

echo Compilando FileSource.cpp...
g++ -Wall -std=c++11 -c FileSource.cpp

echo Compilando CircularBuffer.cpp...
g++ -Wall -std=c++11 -c CircularBuffer.cpp

echo Compilando ExternalSort.cpp...
g++ -Wall -std=c++11 -c ExternalSort.cpp

echo Compilando main.cpp...
g++ -Wall -std=c++11 -c main.cpp

echo.
echo Enlazando...
g++ -Wall -std=c++11 -o esort.exe main.o SerialSource.o FileSource.o CircularBuffer.o ExternalSort.o

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Compilacion exitosa!
    echo Ejecutable creado: esort.exe
    echo ========================================
    echo.
    echo Para ejecutar: esort.exe
) else (
    echo.
    echo ========================================
    echo ERROR en la compilacion
    echo ========================================
)

pause