cd C:\Daten\2015

#This took me quite long, because the Task doesn't explicitly state that the replacements are CASE SENSITIVE.But they need to be
$myinput = Get-Content -Path .\19-1.txt
$molecule0 = $myinput[-1]

$replacements = @()
0..($myinput.Length-3)|%{
    $replacements += ,@($myinput[$_] -split " => ")
    }

function Substitute-substring{
    param(
    [int]$i, #index
    [string]$torep, #String to replace
    [string]$replacement,
    [string]$old
    )
    $new = ""
    if($i -ne 0){
        $new += $molecule0.Substring(0,$i)
    }
    $new += $replacement
    #Write-Host $new
    if($old.Length -ne $i+$torep.Length){
        $new += $old.Substring($i+($torep.Length))
    }
    return $new
}

$molecule1 = @()
for($r = 0;$r -lt $replacements.Length;$r++){
    $in = $replacements[$r][0] #Input to transform
    if($molecule0 -cnotmatch '.?'+$in+'.?'){ #C FOR CASESENTSITIVE . THIS TOOK ME HOUSRS TO FIND
        #Write-Host "Input $($in) not in string, skipping";
        continue}
    $out = $replacements[$r][1] #Output of transformation
    $replaceindexes = (Select-String -InputObject $molecule0 -Pattern $in -AllMatches -CaseSensitive).Matches.Index
    $added = 0
    $duplicate = 0
    #Write-Host "Found $($in) $($replaceindexes.Length)times"
    $replaceindexes|%{
        #Write-Host "Index $($_)"
        $testmol = Substitute-substring $_ $in $out $molecule0
        if($testmol -notin $molecule1){
            $molecule1+=$testmol;
            $added++;
        }else{
        $duplicate++
        }
    }
    #Write-Host "Added: $($added)"
}
Write-Host "$($duplicate) duplicates ignored, but"
Write-Host "$($molecule1.Count) Combinations found"
