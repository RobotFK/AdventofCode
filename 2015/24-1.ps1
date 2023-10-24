cd C:\Daten\2015

$myinput = Get-Content -Path .\24-1.txt #28 Elements
#$myinput = 1..5 + 7..11
#This is a Subset Sum problem + More 
#Or just a Rucksack problem
#Their Value is just ln($_) inverted
function Calculate-sum{
    param(
     [array]$c #combination
    )
    $sum=0
    0..($c.Length-1)|%{  
    $sum += $packages[$_]*$c[$_]}
    return $sum
}

function Calculate-minpackageamount{ #Only solves, does not handle errors
    param(
     [array]$p #$packages
    ,[int]$Groupsize #Sum that we need to achive excatly by the selected Packages
    ,[bool]$v = $false #Visualise
    )
    #Setting Bounds of how many Items are atleast in the first group
    $lowerbound = 0
    $running = @(0)* $packages.Length #Combination of Packages we iterate through
    0..($running.Length-1)|%{$running[$_]= [int](((Calculate-sum -c $running) ) -le $Groupsize)}
    $lowerbound =($running|Measure-Object -Sum).Sum

    $upperbound = 0
    $running = @(0)* $packages.Length #Combination of Packages we iterate through
    0..($running.Length-1)|%{$running[$_]= [int](((Calculate-sum -c $running) + $packages[$_]) -le $Groupsize)}
    if((Calculate-sum -c $running) -eq $Groupsize){
        $upperbound = ($running|Measure-Object -Sum).Sum
        }else{$upperbound = $packages.Length-1}
    if($v){Write-Host "Smallest group has $($lowerbound )-$($upperbound) packages"}

    :lowersearch while($lowerbound -lt $upperbound){
        $opackages = @() #We need a subarray of all but the first $lowerbound packages to stop solutions where a large package is used twice
        ($lowerbound)..($packages.Length-1)|%{$opackages += $packages[$_]}
        for($i = 0;$i -lt $lowerbound;$i++){#$i is the  Package index that we exclude to test for a valid Lowerbound solution
            #This could be written smaller but less readable, but is not done due to obvious reasons
            $running = @(0)* $packages.Length
            0..($lowerbound-1)|%{$running[$_]= 1}
            $running[$i]= 1
            if($Groupsize -( Calculate-sum -c $running) -in $opackages) {
                if($v){Write-Host "Solution with Current lowerbund $($lowerbound) found !"}
                $upperbound = $lowerbound
                continue lowersearch
            }
        }
        if($v){Write-Host "No Solution with Current lowerbund $($lowerbound) found ,increasing loweround"}
        $lowerbound++
    }
    if($v){Write-Host "Group 1 has to be at least of size $($lowerbound)"}
    return $lowerbound
}

#Get Groupsize
$Groupsize = ($myinput|Measure-Object  -Sum).sum/3
#508

#Sort Packages descending
$packages = $myinput |%{[int]$_}|Sort-Object -Descending

$hp =  @{}#hash of Package Size mapped to index
0..($packages.Length-1)|%{$hp[[int]$packages[$_]] = $_}

#Creation of RemainingSum, an array that contains the sum of n+1..omega at index n 
$RemainingSum = @(0)* $packages.Length
0..($RemainingSum.Length-1)|%{$RemainingSum[$_]=Calculate-sum -c (@(0)*($_+1) + @(1)*($RemainingSum.Length-1 -$_))}

$minpackageamount = Calculate-minpackageamount -p $packages -Groupsize $Groupsize -v $false

Write-Host "Group 1 has to be of size $($minpackageamount)"

#Add the ln() quantum value:
$qpackages = @()
$packages|%{$qpackages += ,@($_,[Math]::Log($_))}
$qpackages = $qpackages|Sort-Object -Property {$_[0]}

$lowestqe = [double]::MaxValue #Placeholder value
$running = @(0)* $packages.Length
0..($running.Length-1)|%{$running[$_]= [int](((Calculate-sum -c $running) + $packages[$_]) -le $Groupsize)}
$picked = @(0)* ($minpackageamount-1)
1..$picked.Length|%{$picked[$_-1]=$_-1}

$loop = 0
:pick while($true){
    :tally for($i=0;$i -lt $picked.Length;$i++){
        if($i -eq ($picked.Length-1) -and $picked[$i] -eq ($packages.Length-1)){Write-Host "Exhausted"; break pick}
        elseif($i -eq ($picked.Length-1)){$picked[$i]++}
        elseif($picked[$i] -eq $picked[($i+1)]){
            $picked[$i]=0
        }else{
            $picked[$i]++
            break tally
        }
    } #Counter incrementation
    
    #Duplicate Skip
    if(($picked|Sort-Object|Get-Unique).length -ne $picked.Length){continue pick}

    if($loop++%10000 -eq 0){Write-Host "$($loop):`t $($picked)"}
    #if($loop -eq 2000){break}

    #Test if Final Pick is real
    $sum = 0
    $picked|%{$sum+=$packages[$_]}
    if($hp[[int]($Groupsize-$sum)] -eq $null){continue pick} #Needed last Package not real
    if($hp[[int]($Groupsize-$sum)] -in $picked){continue pick} #Needed last Package already in $picked
    #Write-Host "Solution index $($picked) + $($hp[[int]($Groupsize-$sum)])"
    $soli = $picked + $hp[[int]($Groupsize-$sum)]#Solution in index form
    $QE = 1
    $sol =@()
    $soli|%{$sol+=$packages[$_]}
    $sol|%{$QE*=$_}
    if($QE -lt $lowestqe){
        $lowestqe =$QE
        Write-Host "New low: $($QE)"
        Write-Host "Solution index $($picked) + $($hp[[int]($Groupsize-$sum)])"
        $values = @()
        $picked|%{$values+= $packages[$_]}
        $values+=$hp[[int]($Groupsize-$sum)]
        Write-Host "Values: $($values)"
    }
    #break

}

Write-Host "$($lowestqe)"

#10439961859