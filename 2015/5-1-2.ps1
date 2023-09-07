cd C:\Daten\2015

$myinput = Get-Content -Path .\5-1.txt
#$myinput = "jchzalrnumimnmhp"
$sum = 0 

$myinput |%{if(($_ -match '((a|e|i|o|u)\S*){3}') -and ($_ -match '([a-z])\1')-and ($_ -notmatch '(ab)|(cd)|(pq)|(xy)')){$sum +=1}}

$sum

#$test = "jchzalrnumimnmhp"
#if(($test -match '((a|e|i|o|u)\S*){3}') -and ($test -match '([a-z])\1')-and ($test -notmatch '(ab)|(cd)|(pq)|(xy)')){"True"}
