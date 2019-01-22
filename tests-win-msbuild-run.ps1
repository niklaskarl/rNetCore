Push-Location .\.tests-win-msbuild\test\stringtranscoding\
.\Debug\stringtranscoding_test.exe
Pop-Location

Push-Location .\.tests-win-msbuild\test\semver\
.\Debug\semver_test.exe
Pop-Location

Push-Location .\.tests-win-msbuild\test\clrhost\
.\Debug\clrhost_test.exe
Pop-Location
