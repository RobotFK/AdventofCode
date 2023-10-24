cd C:\Daten\2015

$myinput = Get-Content -Path .\25-1.txt

function Coord-to-index{
    param(
     [int]$r
     ,[int]$c
    )
    $tri = (($c+$r -1)*(($c+$r-1)+1))/2 #Triangular number for c+r-1
    return $tri-($r-1)
    }

#Coord-to-index -c 2 -r 4 #12
#Coord-to-index -c 5 -r 1 #15

$code1 = 20151125
$code = $code1
$split = $myinput.Replace(',','').Replace('.','').Split()
$index = (Coord-to-index -r $split[-3] -c $split[-1] )-1 #Starts at 0, gives us how many times we need to apply the connection
$index
for($i= 1;$i -le $index ;$i++){
    if($i%1000000 -eq 0){Write-host "Loop: $($i)"}
    $code*=252533
    $code%=33554393
}
$code
#*252533
#%33554393