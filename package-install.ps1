
if (Test-Path ".build") {
    Remove-Item ".build" -Recurse -Force   
}

Copy-Item "package" ".build" -Recurse
R.exe CMD INSTALL .build
