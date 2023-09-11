cd C:\Daten\2015
#This aint gonna cut if for 50, so wee need to improve it slightly
#I think the important part 
$myinput = "1113122113" #This is Conways Element 87 ('FR')

$state = @(0)*92
$state[86] = 1 # Element 87 is index 86

$elements = Get-Content -Path .\10-2-1.txt #A list of Atoms of this Sequence and Their lenght and Decays, taken from Wikipedia :)

$elementsindex = @{} #Index <-> Elementname
$elements|%{
    $split = $_.split()
    $elementsindex[$split[1]] = [int]($split[0]-1)
    $elementsindex[$split[0]-1] = ($split[1])
    }

$elementsdecay = @{} # Elementname -> Decayproducts
$elements|%{
    $split = $_.split()
    $elementsdecay[$split[1]] = @($split[3].split('.'))
    }

$elementlength = @{} # Index -> length
$elements|%{
    $split = $_.split()
    $elementlength[[int]($split[0]-1)] = $split[2].length
    }

function Expand-Lookandsay{
param(
    [string]$prev
) 
 $running_digit = $prev[0]
 $running_amount = 0
 0..($prev.Length-1)|%{
    $digit = $prev[$_]
    #Write-Host "$($digit) -eq $($running_digit) : $($digit -eq $running_digit)"
    if($digit -eq $running_digit){
        $running_amount++
    }else{ 
        $next += "$($running_amount)$($running_digit)"
        $running_digit = $digit
        $running_amount = 1
    }
 }
 $next += "$($running_amount)$($running_digit)"
 return $next
}

function Decay-Lookandsay{
    $oldstate = @() + $script:state
    0..($script:state.Length-1)|%{
        if($oldstate[$_] -ne 0){
            $element = $elementsindex[$_]
            #Write-Host "Decaying $($element)"
            foreach($decay in $elementshash[$element]){
                $script:state[$elementsindex[$decay]] += $oldstate[$_]
            }
            $script:state[$elementsindex[$element]] -= $oldstate[$_]
        }
    }
}

function Get-StateLenght{
    $result = 0
    0..($script:state.Length-1)|%{
        if($script:state[$_] -ne 0){
        $result += $elementlength[$_] * $script:state[$_]
        }
    }
    return $result
}

1..50|%{
    Decay-Lookandsay;
    #Write-Host "$($_) Rounds = $(Get-StateLenght)"
}

Write-Host "50 Rounds = $(Get-StateLenght)"

while($false){#Debugging Statements

$state0..($state.Length-1)|%{
        if($state[$_] -ne 0){ Write-Host "State $($_)x$($state[$_]) `tName: $($elementsindex[$_]) ($($elementlength[$_]))"

    }
}


}