
If (-Not (Test-Path ".\.tests-win-mingw"))
{
    New-Item ".tests-win-mingw" -ItemType "directory"
}

Push-Location ".tests-win-mingw"
$path_old = $env:Path
$env:Path = "C:\Windows\System32;C:\Program Files\CMake\bin;C:\Rtools\mingw_64\bin"
cmake.exe -G "MinGW Makefiles" ".."
$env:Path = $path_old
mingw32-make.exe
Pop-Location
