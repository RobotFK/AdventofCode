cd C:\Daten\2015

$myinput = Get-Content -Path .\15-1.txt
#To shorten some expressions we will shorten the propertynames of the ingredients
class ingredient  {
    [string]$name
    [int]$c #capacity
    [int]$d #durability
    [int]$f #flavor
    [int]$t #texture
    [int]$cal #calories
}

$is = @() #ingridients

$myinput|%{
    $split = $_.split().trim(',')
    $ingredient = [ingredient]::new()
    $ingredient.name = $split[0]
    $ingredient.c= $split[2] 
    $ingredient.d = $split[4] 
    $ingredient.f = $split[6] 
    $ingredient.t = $split[8] 
    $ingredient.cal = $split[10] 
    $is += $ingredient 
}

function Calculate-score{
    param(
     [array]$r #ration
    )
    $mc =0
    $md =0
    $mf =0
    $mt =0
    0..($r.Length-1)|%{$mc += $r[$_]*$is[$_].c}
    0..($r.Length-1)|%{$md += $r[$_]*$is[$_].d}
    0..($r.Length-1)|%{$mf += $r[$_]*$is[$_].f}
    0..($r.Length-1)|%{$mt += $r[$_]*$is[$_].t}
    $mc *=($mc -gt 0)
    $md *=($md -gt 0)
    $mf *=($mf -gt 0)
    $mt *=($mt -gt 0)
    $score = $mc * $md * $mf * $mt
    return $score
}

function Calculate-Cal{
    param(
     [array]$r #ration
    )
    $cal =0
    0..($r.Length-1)|%{$cal += $r[$_]*$is[$_].cal}
    return $cal
}

$ratio = @(1,1,1,97)
$highscore = 0
#Limits are 70,80,80 and 71
for(($i0 = 1),($i1 = 1), ($i2 = 1);$i2 -lt 81;){
$ratio = @($i0,$i1,$i2,(100-($i0+$i1+$i2)))
if($ratio[-1] -lt 1){Write-Host "Error at r: $($ratio)"}else{
    if((Calculate-Cal -r $ratio)-eq 500){
        $scoretry = Calculate-score -r $ratio
        if($scoretry -gt $highscore){$highscore = $scoretry;Write-Host "$($ratio) -> $($scoretry)"}
    }
}

if(($i0 -gt 70)-or($i0+$i1+$i2 -gt 97)){ $i0 = 1;$i1++
}else{$i0++}
if($i1 -gt 80 -or(($i0-eq 1)-and($i1+$i2 -gt 96))){ $i1 = 1;$i2++}
#if($i2 -gt 80){ $i2 = 1;$i3++}
}
Write-Host "Done, highscore: $($highscore)"
#the function we are maximising is f(x,y,z,a)=(4x-z)*(5y-2x)*(5z-2a-y)*2a
#This allows a couple simplifications

#9482720 too low