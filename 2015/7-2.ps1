cd C:\Daten\2015


$myinput = $myinput = @("123 -> x",
"456 -> y",
"x AND y -> d",
"x OR y -> e",
"x LSHIFT 2 -> f",
"y RSHIFT 2 -> g",
"NOT x -> h",
"NOT y -> i")
$myinput = Get-Content -Path .\7-1.txt
#$myinput = $myinput = @("123 -> x","456 -> y")
#d: 72
#Max value is 65535
#Always %65536  or [int][char]

$wires_id = @() #Allows faster iteration over critical stuff, this array only contains the Id's of all Solved wires
$wires = @()
class wire {
    [string]$id
    [UInt16]$value #16 Bits
}

function Solve-Wire{ #Returns True if the Wire is created,false if creation is aborted
param(
    [string]$gate,# and,or,lshift,rshift
    [string]$id,
    [string]$key1,
    [string]$key2
    )
    #Write-Host "Called to Solve $($id)"
    switch ($gate)
        {#Test to see if We can get the needed values
            "AND" {if((($key1 -notin $wires_id) -and ($key1 -notmatch "^\d+$")) -or ($key2 -notin $wires_id)){return $false}}
            "OR"{if(($key1 -notin $wires_id) -or ($key2 -notin $wires_id)){return $false}}
            "LSHIFT" {if($key1 -notin $wires_id){return $false}}
            "RSHIFT" {if($key1 -notin $wires_id){return $false}}
            "NOT" {if($key1 -notin $wires_id){return $false}}
        }
    foreach ($iwire in $wires){
    if ($iwire.id -eq $key1){$input1 = $iwire.value}
    if ($iwire.id -eq $key2){$input2 = $iwire.value}
    }
    if($key1 -match "^\d+$"){$input1 = $key1}#Fallback for Being given Numbers for keys
    if($key2 -match "^\d+$"){$input2 = $key2}

    $wire = [wire]::new()
    $wire.id = $id
    switch ($gate)
        {
            "AND" {$value = $input1 -band $input2}
            "OR"{$value = $input1 -bor $input2}
            "LSHIFT" {$value = $input1 -shl [int]$input2}
            "RSHIFT" {$value = $input1 -shr [int]$input2}
            "NOT" {$value = [UInt16](65535-$input1)}
            "Assignment"{$value = $input1}
        }
    $wire.value= $value
    
    $script:wires += $wire
    $script:wires_id += $wire.id
    #Write-Host "Solved $($id)"
    return $true
}

$firstrun  = $true
$changed = -1
while($changed -ne 0){
    $changed = 0
    $myinput|%{
     $split = $_.split()
     if($split[-1] -in $wires_id){}#Already added
     elseif($split.Length -eq 3){#Pure Assignment
        $wire = [wire]::new()
        $wirevaluegiven = $false #Invalid Value to checm if a refence has been found later
        if($split[0] -match "^\d+$"){ # If Refernce is a number
        $wire.value = [UInt16]$split[0]
        $wirevaluegiven = $true
        }
        else {
            foreach ($iwire in $wires){
                if ($iwire.id -eq $split[0]){
                    $wire.value = [UInt16]$iwire.value
                    $wirevaluegiven = $true
                }}
        }
        if($wirevaluegiven ){
            $wire.id = $split[-1]
            $wires += $wire

            #Write-Host "Assigned $($wire.id)"
            $wires_id += $wire.id
            $changed += 1
        }
     }elseif($split.Length -eq 4){#Not Assignment
        # -bnot Does not work, due to the result being always put out in int32 even if the fckn input is in int16
        $changed += Solve-Wire -gate $split[0] -key1 $split[1] -id $split[-1]
     }else{#Gate Assignment
        $changed += Solve-Wire -gate $split[1] -key1 $split[0] -key2 $split[2] -id $split[-1]
     }
    }
    Write-Host "Completed $($changed) Wires"

    if(("a" -in $wires_id) -and ($firstrun)){
    $firstrun = $false
    Write-Host "'A' complete, activating second run"
    $avalue = $wires|%{if($_.id -eq "a"){$_.value}}
    0..($myinput.Length-1)|%{if($myinput[$_].split()[-1] -eq "b"){$myinput[$_] = "$($avalue) -> b"}}
    
    $wires_id = @()
    $wires = @()
    $changed = -1
    }
}


Write-Host "Wiring Complete"

$wires|%{if($_.id -eq "a"){$_}}