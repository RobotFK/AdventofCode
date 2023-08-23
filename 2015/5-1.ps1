cd C:\Daten\2015

$myinput = Get-Content -Path .\5-1.txt
#Static lists
$vowels = "a","e","i","o","u"
$blacklisted = "ab", "cd", "pq", "xy"

function Test-Nice { 
 param (
        [string]$string
        )
        $vowelsamount = 0 #Amount
        $double = $false #Contains
        $blacklisted_found = $false
        
        0..($string.Length-2)|%{
            $char = $string[$_]
            $pair = $string[$_]+$string[($_+1)]
            #Write-Host $pair
            if($char -in $vowels){$vowelsamount +=1};
            if($pair[0] -eq $pair[1]){$double = $true;Write-Verbose "Found Double"};
            if($pair -in $blacklisted){$blacklisted_found = $true}
            }

        if($string[$string.Length-1] -in $vowels){$vowelsamount +=1}

        if($blacklisted_found){return $false}

        if(-not $double){return $false}

        return ($vowelsamount -ge 3)
}

$sum_nice = 0

$myinput|%{if(Test-Nice $_){$sum_nice+=1}}

Write-Host "Of $($myinput.length) strings $($sum_nice) were nice"

"Examples"
Test-Nice "ugknbfddgicrmopn"
Test-Nice "aaa"
Test-Nice "jchzalrnumimnmhp"
Test-Nice "haegwjzuvuyypxyu"
Test-Nice "dvszwmarrgswjxmb"