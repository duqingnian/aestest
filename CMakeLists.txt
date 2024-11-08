name: Static Build for aestest with CMake

on:
  push:
    branches:
      - main
  pull_request:
    branches:
      - main

jobs:
  build:
    runs-on: windows-latest

    steps:
    - name: Checkout repository
      uses: actions/checkout@v3

    - name: Cache Chocolatey Packages
      uses: actions/cache@v3
      with:
        path: C:\ProgramData\chocolatey\lib
        key: ${{ runner.os }}-choco-cache

    - name: Set up Chocolatey
      if: steps.cache.outputs.cache-hit != 'true'
      shell: powershell
      run: |
        Set-ExecutionPolicy Bypass -Scope Process -Force
        [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.SecurityProtocolType]::Tls12
        Invoke-Expression ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))

    - name: Install Dependencies with Chocolatey
      shell: powershell
      run: |
        choco install cmake -y
        choco install ninja -y  # Ninja for efficient builds

    - name: Cache Qt 6.5.0
      uses: actions/cache@v3
      with:
        path: C:\Qt\qt-everywhere-src-6.5.0
        key: static-qt650-cache

    - name: Download and Extract Qt 6.5.0
      if: steps.qt-cache.outputs.cache-hit != 'true'
      run: |
        Invoke-WebRequest -Uri https://download.qt.io/official_releases/qt/6.5/6.5.0/single/qt-everywhere-src-6.5.0.zip -OutFile qt.zip
        Expand-Archive -Path qt.zip -DestinationPath C:\Qt
        echo "Qt 6.5.0 extracted to C:\Qt"

    - name: Set up Visual Studio environment
      shell: cmd
      run: |
        REM Use vswhere to locate and set up Visual Studio environment for MSVC
        for /f "tokens=*" %%i in ('"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -find VC\Auxiliary\Build\vcvars64.bat') do set VCVARSPATH=%%i
        call "%VCVARSPATH%"

    - name: Configure CMake
      run: |
        cmake -B build -G "Ninja" -DCMAKE_PREFIX_PATH="C:\Qt\qt-everywhere-src-6.5.0" .

    - name: Build Project
      run: |
        cmake --build build

    - name: Upload Artifact
      uses: actions/upload-artifact@v3
      with:
        name: aestest-static-build
        path: build/Release/aestest.exe
