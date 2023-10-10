cd C:\Daten\2015

$myinput = Get-Content -Path .\5-1.txt

function Test-Nice-Regex { 
 param (
        [string]$string
        )
        return (($string -match '([a-z])\S\1+') -and ($string -match '([a-z]{2})\S*\1'))
        }
$sum_nice = 0

$myinput|%{if(Test-Nice-Regex $_){$sum_nice+=1}}

Write-Host "Of $($myinput.length) strings $($sum_nice) were nice"

#"Examples"
#Test-Nice "qjhvhtzxzqqjkmpb"
#Test-Nice "xxyxx"
#Test-Nice "uurcxstgmygtbstg"
#Test-Nice "ieodomkazucvgmuy"
