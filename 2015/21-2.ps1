cd C:\Daten\2015

$myinput = Get-Content -Path .\21-1.txt

$playerhp = 100

$wshop = @()
$wshop+= ,@(8,4,0) #Dagger
$wshop+= ,@(10,5,0) #Shortsword
$wshop+= ,@(25,6,0) #Warhammer
$wshop+= ,@(40,7,0) #Longsword
$wshop+= ,@(74,8,0) #Greataxe

$ashop = @()
$ashop+= ,@(0,0,0) #Naked
$ashop+= ,@(13,0,1) #Leather
$ashop+= ,@(31,0,2) #Chainmail
$ashop+= ,@(53,0,3) #Splintmail
$ashop+= ,@(75,0,4) #Bandedmail
$ashop+= ,@(102,0,5) #Platemail

$rshop = @()
$rshop+= ,@(0,0,0) #Naked
$rshop+= ,@(0,0,0) #Naked
$rshop+= ,@(25,1,0) #Damage1
$rshop+= ,@(50,2,0) #Damage2
$rshop+= ,@(100,3,0) #Damage3
$rshop+= ,@(20,0,1) #Defense1
$rshop+= ,@(40,0,2) #Defense2
$rshop+= ,@(80,0,3) #Defense3

function Evaluate-Fight{
    param(
        [array]$player, #HP, Damage Armor
        [array]$boss = @($myinput[0].Split()[-1],$myinput[1].Split()[-1],$myinput[2].Split()[-1]),
        [bool]$v = $false #Visualise
    )
    #ttd is the Turns/Attacks until the player dies
    switch ($boss[1]-$player[2]){
        {$_-lt 1}{$ttd = $player[0]; Break} #Full armor Block
        {$_ -gt $player[0]}{$ttd = 1; Break} #One shot
        Default {$ttd = [math]::Ceiling($player[0]/($boss[1]-$player[2]))}
    }
    #ttk is the Turns/Attacks until the Boss dies
    switch ($player[1]-$boss[2]){
        {$_-lt 1}{$ttk = $boss[0]; Break} #Full armor Block
        {$_ -gt $boss[0]}{$ttk = 1; Break} #One shot
        Default {$ttk = [math]::Ceiling($boss[0]/($player[1]-$boss[2]))}
    }
    if($v){Write-Host "TTK: $($ttk) vs TTD: $($ttd)"}
    return ($ttk -le $ttd)
}

$maxgold = $null
foreach($weapon in $wshop){
    foreach($armor in $ashop){
        :rings for($rings = 0; $rings -lt 64;$rings++){#This ist just a simpler for of iteration
        if([math]::floor($rings/8) -eq $rings%8){continue rings}
            $r1 = $rshop[[math]::Floor($rings/8)]
            $r2 = $rshop[($rings%8)]

            $player = @(
                $playerhp,
                ($weapon[1]+$armor[1]+$r1[1]+$r2[1]),
                ($weapon[2]+$armor[2]+$r1[2]+$r2[2])
                )
            if(-not (Evaluate-Fight -player $player)){
                $cost = $weapon[0]+$armor[0]+$r1[0]+$r2[0]
                if($cost -gt $maxgold -or $maxgold -eq $null){
                    Write-Host "Costplit: $($weapon[0])-$($armor[0])-$($r1[0])-$($r2[0])"
                    Write-Host "newmax= $($cost)"
                    $maxgold = $cost
                }
            }
        }
    }
}
#115 too low
#131 too high
Write-Host "Final Minimum: $($maxgold)" 

#Evaluate-Fight @(8,5,5) @(12,7,2) $true
#100/7