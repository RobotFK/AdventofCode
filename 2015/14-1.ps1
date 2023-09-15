cd C:\Daten\2015

$myinput = Get-Content -Path .\14-1.txt

class Reindeer  {
    [string]$name
    [int]$speed
    [int]$duration
    [int]$rest
}

$reindeers = @()

$myinput|%{
    $split = $_.split()
    $reindeer = [Reindeer]::new()
    $reindeer.name = $split[0]
    $reindeer.speed = $split[3]
    $reindeer.duration = $split[6] #How long it can go fast
    $reindeer.rest = $split[-2]
    $reindeers += $reindeer
}

function Calculate-Distance{
    param(
     [reindeer]$reindeer,
     [int] $timelimit
    )
    $finaldistance = 0
    $cycletime = $reindeer.duration + $reindeer.rest
    $cycledistance = $reindeer.speed*$reindeer.duration
    $finaldistance += ([Math]::floor($timelimit/$cycletime))*$cycledistance
    $remainingtime = $timelimit%$cycletime
    if($remainingtime -ge $reindeer.duration){$finaldistance += $cycledistance}
    else{$finaldistance +=$remainingtime*$reindeer.speed}
    return $finaldistance
}

$distances = @()
0..($reindeers.Length-1)|%{
    $distances += Calculate-Distance -timelimit 2503 -reindeer $reindeers[$_]
}
$distances | measure -Maximum