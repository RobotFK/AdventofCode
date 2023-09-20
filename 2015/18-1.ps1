cd C:\Daten\2015

$myinput = Get-Content -Path .\18-1.txt ;$maxsteps = 100
#$myinput = Get-Content -Path .\18-1-1.txt ;$maxsteps = 4
$le = $myinput[0].Length # maximum distance in one Dimension

$l_0 = [bool[,]]::new($le,$le) #Lights at t0 (now) (y,x)
$l_1 = [bool[,]]::new($le,$le) #Lights at t+1

$hash=@{
    [char]"#"=$true; #Char is not optional, as the key is not recognised if the type is wrong (char vs string)
    [char]"."=$false;
    $true=[char]"#";
    $false=[char]"."
    }



function Count-Neighbours{
param(
    [int]$x,
    [int]$y
    )
    $sum = 0
    if($x -ne 0){ #Left
        $sum += $l_0[$y,($x-1)]
        $left = $true
    }
    if($y -ne 0){ #up
        $sum += $l_0[($y-1),$x]
        $up = $true
    }
    if($x -ne ($le-1)){ #Right
        $sum += $l_0[$y,($x+1)]
        $right = $true
    }
    if($y -ne ($le-1)){ #down
        $sum += $l_0[($y+1),$x]
        $down = $true
    }
    if($left -and $up){  $sum += $l_0[($y-1),($x-1)]}
    if($left -and $down){$sum += $l_0[($y+1),($x-1)]}
    if($right-and $up){  $sum += $l_0[($y-1),($x+1)]}
    if($right-and $down){$sum += $l_0[($y+1),($x+1)]}
    return $sum
    }

function Increment-lights{#Only works in this script
    [array]::copy($l_0,$l_1,($l_0.length))
    for($y = 0;$y -lt $le;$y++){
        for($x = 0;$x -lt $le;$x++){
            $neighbours = Count-Neighbours -x $x -y $y
            if ($neighbours -eq 3 ){$l_1[$y,$x] = $true} #Turn on (+ stay on)
            elseif($neighbours -ne 2){$l_1[$y,$x] = $false} #Turn off (if not staying on)
        }
    }
    [array]::copy($l_1,$l_0,($l_1.length))
}

function Visualise-lights{
    for($y = 0;$y -lt $le;$y++){
        $line = ""
        for($x = 0;$x -lt $le;$x++){
            $line += $hash[$l_0[$y,$x]]
        }
        Write-host $line
    }
}

#Transform input into the inital board
for($y = 0;$y -lt $le;$y++){
    for($x = 0;$x -lt $le;$x++){
        $l_0[$y,$x]=$hash[$myinput[$y][$x]]
    }
}

Visualise-lights
Write-Host "Animating:"
$step = 1
while($step -le $maxsteps){
    Write-Host "`nStep $($step)"
    Increment-lights
    $step++
    #Visualise-lights
}

#Count lights
$sumlights = 0
for($y = 0;$y -lt $le;$y++){
    for($x = 0;$x -lt $le;$x++){
        if($l_0[$y,$x]){$sumlights++}
    }
}
Write-Host "Done `n $($sumlights) are on after $($step) steps"
#3898 too high