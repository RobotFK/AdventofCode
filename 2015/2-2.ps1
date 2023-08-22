$boxes = Get-Content -Path .\2-1.txt |%{$_.split("`r`n")}
0..($boxes.Length -1)|%{$boxes[$_] = $boxes[$_].split('x')}
0..($boxes.Length -1)|%{$boxes[$_] = [int]$boxes[$_][0],[int]$boxes[$_][1],[int]$boxes[$_][2]}
0..($boxes.Length -1)|%{$boxes[$_] = $boxes[$_]|sort}
$sum = 0
$boxes|%{$sum += 2*($_[0]+$_[1])+$_[0]*$_[1]*$_[2]}
$sum