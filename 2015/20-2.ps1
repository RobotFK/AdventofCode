
$myinput = 29000000 # 29,000,000

$minsum = $myinput/11

$upperbound = $minsum

function Sum-Presents{
    param(
        [int]$index,
        [bool]$v = $false #Visualise
    )
    $i = 1
    $sum = 0
    $ls = @()
    $sqr = [math]::Sqrt($index)
    while($i -le $sqr){
        if($index%$i -eq 0){
        $ls += $i
        if($index/$i -le 50){$sum += $i}
        }
        $i++;
    }
    if($v){Write-Host $ls}
    $ls|%{if($_ -lt $sqr){if($_ -le 50){$sum += $index/$_}}}
    return $sum
}

$test = 660000 #Approximated by trial and error
$presents = 0
$maxpoints = 0
while($presents -le $minsum){
    $test += 1
    if($test%100000 -eq 0){Write-Host $test}
    $presents = Sum-Presents $test
    if($presents -gt $maxpoints){
        $maxpoints = $presents
        Write-Host "Highscore: $($maxpoints) `tIndex $($test)"
    }
}

Write-Host $test
