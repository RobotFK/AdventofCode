cd C:\Daten\2015

$myinput = Get-Content -Path .\14-1.txt

#2503 seconds are Relevant

#For day to keep it simple we will just give each Reindeer a Table of [time]->Distance and the interate all seconds
class Reindeer  {
    [string]$name
    [int]$speed
    [int]$duration
    [int]$rest
    [array]$pos
    $points
}

$reindeers = @()

$myinput|%{
    $split = $_.split()
    $reindeer = [Reindeer]::new()
    $reindeer.name = $split[0]
    $reindeer.speed = $split[3]
    $reindeer.duration = $split[6] #How long it can go fast
    $reindeer.rest = $split[-2] #How long it needs to rest
    $reindeers += $reindeer
}

function Calculate-position{
    param(
     [reindeer]$reindeer,
     [int] $timelimit
    )
    $time= 0
    $reindeer.pos = @() +$distance
    $stamina = 0 #To simplify everything we will treat the timing of running with a stamia value, wher positive values are the already run seconds and negative ones are the time still needed to rest
    $distance = 0
    while($time -lt $timelimit){
        if($stamina -eq $reindeer.duration){
            #Write-Host "Resting"
            $stamina = 0 - $reindeer.rest
        }elseif($stamina -ge 0){ #Currently running
            #Write-Host "Running"
            $distance += $reindeer.speed
        }
        $stamina++
        $time++
        $reindeer.pos +=$distance
    }
    return $distance
}

$distances = @()
0..($reindeers.Length-1)|%{
    $distances += Calculate-position -timelimit 2503 -reindeer $reindeers[$_]}
#$distances | measure -Maximum #Usefull for testing if the new system doesn't have any of by ones

#We need to do a couple loops as comparing a set index of arrays of objects isn't solved already
for($i = 0;$i -lt 2504;$i++){
    $maxdistance = 0
    0..($reindeers.Length-1)|%{$pos =$reindeers[$_].pos[$i];if($pos -gt $maxdistance){$maxdistance = $pos}} #Find furthest ahead
    0..($reindeers.Length-1)|%{$pos =$reindeers[$_].pos[$i];if($pos -eq $maxdistance){$reindeers[$_].points++}} #Award point to the leading ones
    #Write-Host "Lead is $($maxdistance) far"
}
$points = @()
$reindeers|%{$points += $_.points}
$points | measure -Maximum