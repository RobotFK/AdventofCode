cd C:\Daten\2015

$myinput = Get-Content -Path .\13-1.txt

#We can reuse A LOT of code from Day 9

$permuatations = New-Object 'object[]' 40320
$permuatations = @()
function Get-Permutations{
#Core Algorithm by B. R. Heap (1963)
param(
    [array]$elements,
    [int]$ecount = $elements.Length #How many Elements we have to Rearange
    )
    $state = @(0)*$ecount #Keeps programm at linear runtime by simulating an additional Loop
    $script:permuatations += $null
    $script:permuatations[-1] = @($elements)
    $i = 1
    while($i -lt $ecount){
        if($state[$i] -lt $i){
            if(-not($i%2)){
                $elements[0],$elements[$i] = $elements[$i],$elements[0]
                #Write-Host "1 <> $($i+1) (0 switch)"
            }else{
                $elements[$state[$i]],$elements[$i] = $elements[$i],$elements[$state[$i]]
                #Write-Host "$($state[$i]+1) <> $($i+1)"
            }
            $script:permuatations += $null
            $script:permuatations[-1] = @($elements)
            $state[$i] +=1
            $i = 1
        }else{
            $state[$i] = 0
            $i += 1
        }
    }
    Write-Host "Permutations complete"
}
Get-Permutations -elements @(1..8)

$names = @() #Names of attendees
$hppy = [int[,]]::new(8,8) #happiness
$sign = @{"gain"=1;"lose"=-1}

#$hppy filling
$myinput|%{
    $split = $_.split()
    $n1 = $split[0]
    if($split[0] -notin $names){
        $names += $n1
    }
    $n2 = $split[-1].TrimEnd('.')
    if($split[-1].TrimEnd('.') -notin $names){
        $names += $n2
    }
    $ni1 = $names.IndexOf($n1) #Name index 1
    $ni2 = $names.IndexOf($n2) #Name index 2
    Write-Host "$($ni1) -> $($ni2)"
    $hppy[$ni1,$ni2] = $sign[$split[2]] * $split[3]
}

function Get-Permutationhappyness{
param(
    [array]$permutation
    )
    $sum = 0
    0..($permutation.Length-2)|%{#All but the last
    $sum += $script:hppy[($permutation[$_]-1),($permutation[$_+1]-1)]#-1 as Permutaions starts at 1 but $tt at 0
    $sum += $script:hppy[($permutation[$_+1]-1),($permutation[$_]-1)]#For Day 13 relations are now bidirektional
    #Write-Host $sum
    }
    #Adding first to last interaction:
    $sum += $script:hppy[($permutation[0]-1),($permutation[-1]-1)]#-1 as Permutaions starts at 1 but $tt at 0
    $sum += $script:hppy[($permutation[-1]-1),($permutation[0]-1)]#For Day 13 relations are now bidirektional
    return $sum
}

$highscore = 0
1..(($permuatations.Length)-1)|%{
$sum =  Get-Permutationhappyness $permuatations[$_]
#Write-Host "Sum is $($sum)"
if($sum -gt $highscore){$highscore = $sum;Write-Host "Permutaion $($_) is new fastest $($highscore)"}
}
Write-Host "Best Order gives $($highscore) happyness"

Write-Host "Done"