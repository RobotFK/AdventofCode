cd C:\Daten\2015

$myinput = Get-Content -Path .\22-1.txt
$day = 2
$player = @{hp = 50;mana =500}
$boss = @{hp = $myinput[0].Split()[-1];dmg = $myinput[1].Split()[-1]}

if($spells -eq $null){
$mm =@{ 
    name = "Magic Missile"
    cost = 53
    dmg = 4
    }

$drain =@{
    name = "Drain"
    cost = 73
    dmg = 2
    heal = 2
    }

$Shield =@{
    name = "Shield"
    id = 0 #Index where in $activespells the ttl is stored
    cost = 113
    ttl = 6
    armor = 7
    }

$Poison  =@{
    name = "Poison"
    id = 1 #Index where in $activespells the ttl is stored
    cost = 173
    ttl = 6
    dmg = 3
    }

$Recharge  =@{
    name = "Recharge"
    id = 2 #Index where in $activespells the ttl is stored
    cost = 229
    ttl = 5
    mana = 101
    }
$spells = @($mm,$drain,$shield,$Poison,$Recharge)
}

#Note that spells with a Time are active twice between each boss attack
$minmana = 53

$activespells = @(0,0,0) #Shield,Poison,Recharge rounds left
$node = @($player.hp , $player.mana ,$boss.hp,$boss.dmg)+$activespells + 0 #Spent mana counter
#Yes, strings. This is the best for comparing complex data that i found

#$node =@(10,250,14,8)+$activespells + 0

$node = $node -join ' '
$tree=@() #Each new layer of the tree covers both a player and Boss turn
$tree+=,@($node)

function Test-Victory{
param([array]$state)
return [bool]($state[2] -le 0)
}

function Test-Defeat{
param([array]$state)
return [bool]($state[1] -le $minmana -or $state[0] -le 0)
}

function Apply-Effects{
param([array]$state) 
if($state[4] -ne 0){$state[4]--} #Shield
if($state[5] -ne 0){$state[2]-= $Poison.dmg;$state[5]--}#Poison
if($state[6] -ne 0){$state[1]+= $Recharge.mana;$state[6]--}#Recharge
return ,$state
}

function Apply-Bossattack{
param([array]$state)
if($state[4] -ne 0){$state[0]-= ($state[3]-$shield.armor)} #Shield
else{$state[0]-= $split[3]} #No shield
return ,$state
}

$minspentmana = [int]::MaxValue
while($true){
    $tree+=,@()
    #Write-Host "Testing $($tree[-2].Length) nodes"

    $count = 0
    foreach($node in $tree[-2]){
        #Progressblock , Displays a Progress bar
        $count ++
        $progress = [math]::Round($count/$tree[-2].Length * 1000)/10
        Write-Progress -Activity "Round $($tree.Length): $($tree[-2].Length)" -Status "$progress% Complete:" -PercentComplete $progress 
        #End of Progressblock

        $snode = $node.Split()|%{[int]$_} #Sliced node 
        if($snode[-1] -gt $minspentmana){continue}#Node is already worse than already submitted result
        #Base calculations
        if($tree.Length -gt 2){#Stops Calcuations before the first player action
        #Boss Attack Turn
        $snode = Apply-Effects $snode
        if(Test-Victory $snode){ 
            if($snode[-1] -lt $minspentmana){
                Write-host "New low: $($snode[-1])" 
                $minspentmana = $snode[-1]
            }
            continue
        }
        $snode = Apply-Bossattack $snode

        if(Test-Defeat $snode){continue}

        #Player Attack Turn
        }
        if($day -eq 2){$snode[0]--}
        if($snode[0] -le 0){continue}
        $snode = Apply-Effects $snode

        if(Test-Victory $snode){
            Write-Host "Win!";
            if($snode[-1] -lt $minspentmana){
                Write-host "New low: $($snode[-1])" 
                $minspentmana = $snode[-1]
            }
            continue
        }
        #Write-Host "Current (Pre spell): `n $($snode)"
        if($snode[1] -lt $minmana){continue}#Cant afford any spell
        
        :spells foreach ($spell in $spells){
            $ssnode= @()+$snode #Sliced Spell node
            #Write-Host "$($spell.name)"
            if($ssnode[1] -lt $spell.cost){continue spells}#Cant afford this spell
            #Applying new spell
            if($spell.ttl -ne $null){#Spell has a ttl
                if($ssnode[$spell.id +4]){continue spells}#Spell is already active
                $ssnode[$spell.id +4] += $spell.ttl

            }else{#Spell is instant
                $ssnode[0] += $spell.heal
                $ssnode[2] -= $spell.dmg
            }
            $ssnode[1] -= $spell.cost
            $ssnode[-1] += $spell.cost
            $node = $ssnode -join ' '
            if(($tree[-1] -match $node).Length -eq 0){
            #if($node -notin $tree[-1]){
                #Write-Host "+ $($spell.name)"
                $tree[-1]+=,$node
            }
        }
    }
    if($tree[-1].Length -lt 1){break}
    #if($tree.Length -ge 2){break}
}
Write-Host "Finallow: $($minspentmana)"
#900 to low