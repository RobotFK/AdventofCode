$turn = 0
$height = 0
do{
Get-Content -Path .\1-1.txt |%{[char[]]$_}|%{$height += @{"(" = 1;")" = -1}[$_.ToString()];$turn +=1;if($height -lt  0){break}}
}while($false)
Write-host $turn
