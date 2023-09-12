cd C:\Daten\2015

$myinput = Get-Content .\12-1.txt -Raw | ConvertFrom-Json #I can feel this being usefull later
$myinput = Get-Content .\12-1.txt

$list = $myinput -replace "[^-|\d]+", ","

$sum = 0
$list.Split(",")|%{$sum += $_}

$sum