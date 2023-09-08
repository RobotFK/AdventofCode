cd C:\Daten\2015
#Not Terrebly fast, but does it's job
$myinput = "1113122113"

function Expand-Lookandsay{
param(
    [string]$prev
) 
 $running_digit = $prev[0]
 $running_amount = 0
 0..($prev.Length-1)|%{
    $digit = $prev[$_]
    #Write-Host "$($digit) -eq $($running_digit) : $($digit -eq $running_digit)"
    if($digit -eq $running_digit){
        $running_amount++
    }else{ 
        $next += "$($running_amount)$($running_digit)"
        $running_digit = $digit
        $running_amount = 1
    }
 }
 $next += "$($running_amount)$($running_digit)"
 return $next
}

$result = $myinput
1..40|%{$result = Expand-Lookandsay $result;Write-Host $_}

Write-Host $result.length
