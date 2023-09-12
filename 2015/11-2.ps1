cd C:\Daten\2015

#a-z = 97-122
$myinput = "hxbxwxba"

$straights = @()
0..23|%{if($_ -notin 6..14){$straights += [char](([int][char]"a")+$_)+[char](([int][char]"b")+$_)+[char](([int][char]"c")+$_)}}

function Test-ValidPw{
param(
    [string]$pw
)
    $containsstraight = $false
    if(($pw -notmatch "([a-z])\1.*([a-z])\2") -or ($pw -match "(i|o|l)+")){return $false}
    $straights|%{if($pw -like ("*"+$_+"*")){$containsstraight = $true}}
    return $containsstraight
}

function Generate-NextValidPw{
param(
    [string]$oldpw
)    $rpw = $oldpw #runningpw
    Write-Host "Old Password is $($rpw)"
    while(-not (Test-ValidPw $rpw)){
        #Write-Host "Testing $($rpw)"
        #While Detection of Validity could be done string based, incrementing ist easier as an Chararray
        $pwarr = @()
        $rpw.ToCharArray()|%{$pwarr+=[int][char]$_}
        :carry for($i = 7;$i -ge 0;$i--){#Index of affected letter, going to the next loop means need to change the next letter aswell 
            #Write-Host "$($pwarr) pre $($i)" 
            $pwarr[$i] += 1

            if($pwarr[$i] -eq 123){# 1 past z
                #Write-Host "Index $($i) overflow"
                $i..7|%{$pwarr[$_] = 97}
            }else{
                if($pwarr[$i] -in 105,108,111){
                    $pwarr[$i]+= 1
                       #$i..7|%{$pwarr[$_] = 97}
                }
            break carry #While the lable is not needed it still might help with understanding
            }
            #Write-Host "$($pwarr) post" 
        }

        $rpw = ($pwarr|%{[char]$_}) -join "" # Chararray to String
    }
    Write-Host "Valid: $($rpw)"   
    return $rpw
}


$result1 = Generate-NextValidPw $myinput
$pwarr = @()
$result1.ToCharArray()|%{$pwarr+=[int][char]$_}
for($i = 7;$i -ge 0;$i--){ 
	$pwarr[$i] += 1
	if($pwarr[$i] -eq 123){# 1 past z
		$i..7|%{$pwarr[$_] = 97}
	}else{
		if($pwarr[$i] -in 105,108,111){
			$pwarr[$i]+= 1
		}
	break
	}
}
$result1_1 = ($pwarr|%{[char]$_}) -join "" # Chararray to String
Write-host "New +1 ist $($result1_1)"
Generate-NextValidPw $result1_1
