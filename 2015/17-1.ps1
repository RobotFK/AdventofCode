cd C:\Daten\2015

$myinput = Get-Content -Path .\17-1.txt;$volume = 150
#While the Sorting is not nessecary it makes the search a bit more predicatble
$con = $myinput |Sort-Object {[int]$_} -Descending
#$con = @(20, 15, 10, 5,5);$volume = 25 #Test case
function Calculate-sum{
    param(
     [array]$c #combination
    )
    $sum=0
    0..($c.Length-1)|%{
    #Write-host "$($con[$_]) * $($c[$_])"    
    $sum += $con[$_]*$c[$_]}
    return $sum
}

$running = @(0)* $con.Length 
#Inital finding
0..($running.Length-1)|%{
    if(((Calculate-sum -c $running) + $con[$_]) -le $volume){
        #Write-Host "Adding i$($_):$($con[$_])"
        $running[$_]= 1
    }
}

Write-Host "$($running -join`"`")"
$combinations = @(,[string]($running -join ""))

#Set the minimum Bits that can all be 0 and the Target volume is still achiveable
#This could be compounded with the inital running aswell, but as this is run once I just keep it more readable
$minstatic = 0
$sum = 0
0..($running.Length-1)|%{
    if(((Calculate-sum -c $running) + $con[$_]) -le $volume){
        #Write-Host "Adding i$($_):$($con[$_])"
        $running[$_]= 1
    }
}

Write-Host "Starting Main Run"
$exhaused = $false
#Static assign
$loop = 0
while(-not $exhaused){
    #Write-Host "`nNew loop:"

    #Progress report:
    if($loop++%1000 -eq 0){Write-Host "$($loop): $($running -join`"`")"}

    0..($running.Length-1)|%{if($running[$_] -eq 1){$static = $_}} #static is the last digit (smallest container) index we now ignore

    #If only the last bit is static we increase it
    if($static-eq ($running.Length-1)){
    $running[-1] = 0;continue
    }

    #Setting all bits after (and) static to 0
    $static..($running.Length-1)|%{$running[$_] = 0}
    $viusals = ""
    0..($static)|%{$viusals += $running[$_]}
    ($static+1)..($running.Length-1)|%{$viusals += "?"}
    #Write-Host $viusals

    #Exit Condition
    if(1 -notin $running -and $static -eq ($running.Length-1)){$exhaused = $true;continue}

    #Calculate the next solution with the given static
    ($static+1)..($running.Length-1)|%{
        if(((Calculate-sum -c $running) + $con[$_]) -le $volume){
            $running[$_]= 1
        }
    }

    #Add solution to to the collection, if found , decrease static if not
    if((Calculate-sum -c $running) -eq $volume){
        #Write-Host "$($running -join`"`") Found"
        $combinations += [string]($running -join "")
    }
    #Write-Host "Static:$($static)"
    #if($Static -lt 1){Write-Host "Final: $($running -join`"`")" ;$exhaused = $true}
}

Write-Host "Done:"
Write-Host $combinations.Length

# 2282 too low