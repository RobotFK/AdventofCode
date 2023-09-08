cd C:\Daten\2015

#We have 28 paths and 8 Cityies
#So we are looking at 8! or 40320 paths
#But we can already halve, as The direciton of travel is unimportant

$myinput =@(
"London to Dublin = 464",
"London to Belfast = 518",
"Dublin to Belfast = 141"
)
$myinput = Get-Content -Path .\9-1.txt

#Section that generates all City Permutaions via the Heap's Algorithm
$permuatations = New-Object 'object[]' 40320
$permuatations = @()
function Get-Permutations{
#Core Algorithm by B. R. Heap (1963)
param(
    [array]$elements,
    [int]$ecount = $elements.Length #How many Elements we have to Reaarange
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
    #returns would need more work to  
}
Get-Permutations -elements @(1..8)
#End of Permutations generation

$cities = @() #Names, while not needed for the solution it is quite usefull for debugging
$tt = [int[,]]::new(8,8) #Matrix of Traveltime

$myinput|%{
    $split = $_.split()
    if($split[0] -notin $cities){
        $cities += $split[0]
    }
    if($split[2] -notin $cities){
        $cities += $split[2]
    }
    $ci1 = $cities.IndexOf($split[0]) #City index 1
    $ci2 = $cities.IndexOf($split[2]) #City index 2
    #Write-Host "$($ci1) -> $($ci2)"
    $tt[$ci1,$ci2] = $split[-1]
    $tt[$ci2,$ci1] = $split[-1]
}

function Get-PermutationsTime{
param(
    [array]$permutation
    )
    $sum = 0
    0..($permutation.Length-2)|%{
    #Write-Host "TT: $(($permutation[$_]-1))-$(($permutation[$_+1]-1)) "
    $sum += $script:tt[($permutation[$_]-1),($permutation[$_+1]-1)]#-1 as Permutaions starts at 1 but $tt at 0
    #Write-Host $sum
    }
    return $sum
}

Write-Host "Generation Complete running Time solver"
$rt = Get-PermutationsTime $permuatations[0]#Shortest Route time of a pair
1..(($permuatations.Length/2)-1)|%{ # We only need halve, as the last place in $permutaiontn is due to the algorithm just incrementing
$sum =  Get-PermutationsTime $permuatations[$_]
if($sum -lt $rt){$rt = $sum;Write-Host "Permutaion $($_) is new fastest $($rt)"}
}
Write-Host "Fastest Roundtrip takes $($rt) Time"

while($false){
"
8*7+
7*6+
6*5+
5*4+
4*3+
3*2+
2*1+
"
$test[0],$test[1] = $test[1],$test[0]
}