@echo off

  cd %APPVEYOR_BUILD_FOLDER%

  echo Compiler: %COMPILER%
  echo Architecture: %MSYS2_ARCH%
  echo Platform: %PLATFORM%
  echo MSYS2 directory: %MSYS2_DIR%
  echo MSYS2 system: %MSYSTEM%
  echo Bits: %BIT%

  IF %COMPILER%==msys2 (
    @echo on
    SET "PATH=C:\%MSYS2_DIR%\%MSYSTEM%\bin;C:\%MSYS2_DIR%\usr\bin;%PATH%"
    rem Appveyor CI script for Remix
    rem Created by Devopsralf
    rem PATH C:\msys64\%MSYSTEM%\bin;C:\msys64\usr\bin;%PATH%
    rem Upgrade the MSYS2 platform
    bash -lc "pacman --noconfirm --sync --refresh --refresh pacman"
    bash -lc "pacman --noconfirm --sync --refresh --refresh --sysupgrade --sysupgrade"
    rem Install required tools
    bash -xlc "pacman --noconfirm -S --needed base-devel"
    rem Install the relevant native dependencies
    bash -xlc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-toolchain"
    bash -xlc "pacman --noconfirm -S --needed make"
    bash -xlc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-cmake"
    bash -xlc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-boost"
    bash -xlc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-openssl"
    bash -xlc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-zeromq"
    bash -xlc "pacman --noconfirm -S --needed mingw-w64-%MSYS2_ARCH%-libsodium"
    rem Invoke subsequent bash in the build tree
    cd %APPVEYOR_BUILD_FOLDER%
    set CHERE_INVOKING=yes
    rem Build/test scripting
    
    IF %BIT%==64 (
        bash -xlc "make release-static-win64" 
    )
    IF %BIT%==32 (
        bash -xlc "make release-static-win32" 
    )
  )