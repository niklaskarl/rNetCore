
If (-Not (Test-Path ".\.tests-win-msbuild"))
{
    New-Item ".tests-win-msbuild" -ItemType "directory"
}

Push-Location ".tests-win-msbuild"
cmake.exe -G "Visual Studio 15 2017 Win64" ".."
msbuild.exe "rNetCore.sln"
Pop-Location
