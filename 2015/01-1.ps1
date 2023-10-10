$len = 0
Get-Content -Path .\1-1.txt |%{[char[]]$_}|%{$len += @{"(" = 1;")" = -1}[$_.ToString()]}
$len
