cd C:\Daten\2015

$myinput = Get-Content -Path .\19-1.txt

#This could be Solvable by using the theory of Context-free grammar

#This code Works in 3 steps:
#1: Convert rules to CNF
#2 Apply CKY Algorithm
#3 Reverse results to generate a parse tree 

$mmolecule = $myinput[-1]

$replacements = @() #List of Replacements that were given as input
0..($myinput.Length-3)|%{$replacements += ,@($myinput[$_] -split " => ")}

function Display-Rules{
param(
    [array]$rules
    )
    $ouput= ""
    foreach($rule in $rules){
    $ouput += -join $rule[0],"->",($rule[1] -join "-")
    $ouput +="`n"
    }
    Write-Host $ouput
}


#Beginning of Converting to Chomsky normal form
#An asumption made here is that Each replacements resulst in atleast 2 Elements

$replacements_CNF=@()
$replacements|%{$replacements_CNF+= ,@($_[0],($_[1]-creplace '[A-Z]', ' $&').trim().Split())}
$arc= 0 #Artifical rule counter, increment AFTER each use !
$out_to_in = @{} #Hashlist of reversed direction. Meaning that it tells the Input for the Given output (of artifical rules)

#START
 #Not Needed, as e is never on the right

#TERM
 #A bit more difficult, as Symbols can be both Terminal and Nonterminal.
 #But by adding C(non terminal) -> C(terminal) we can adhere to this
$nonterminals =@()
$replacements_CNF|%{if($_[0] -cnotin $nonterminals){$nonterminals +=$_[0]}}

$terminals =@()
($mmolecule-creplace '[A-Z]', ' $&').trim().Split()|%{if($_ -cnotin $terminals -and $_ -cnotin $nonterminals){$terminals +=$_}}

#Adding the new rule
    foreach($terminal in $terminals){ 
    $out_to_in[$terminal]=$arc
    $replacements_CNF+= ,@($arc++,@($terminal))
    }

#Updating all old ones:
for($i=0;$i-lt ($replacements_CNF.Length-$arc);$i++){
    for($j=0;$j -lt $replacements_CNF[$i][1].Length;$j++){
        if($replacements_CNF[$i][1][$j] -cin $terminals){
            #Write-Host "Replaced in Rule $($i): $($replacements_CNF[$i][1][$j]) -> $($out_to_in[$replacements_CNF[$i][1][$j]])"
            $replacements_CNF[$i][1][$j]= $out_to_in[$replacements_CNF[$i][1][$j]]
        }
    }
}
#Read-Host "X"
#BIN
 #The big one:
:replacement for($i=0;$i-lt ($replacements_CNF.Length-$arc);$i++){
    if($replacements_CNF[$i][1].Length -le 2){continue replacement}
    #Write-Host "Expanding $($replacements_CNF[$i][0]) -> $($replacements_CNF[$i][1])"
    $old_ouput = @()
    $replacements_CNF[$i][1]|%{$old_ouput += $_}
    #Each one but the last ist dynamically created
    for($j=0;$j -lt ($old_ouput.length-2);$j++){
        if($j -eq 0){
            $replacements_CNF[$i][1]= @($old_ouput[$j],$arc++)
            #Write-Host "Add1. $($replacements_CNF[$i][0]) -> $($replacements_CNF[$i][1])"
        }else{
            $replacements_CNF+= ,@(($arc-1),@($old_ouput[$j],$arc++))
            #Write-Host "Added $($replacements_CNF[-1][0]) -> $($replacements_CNF[-1][1])"
        }
    }
    #Write-Host "New last rule for $($old_ouput[-2],$old_ouput[-1])"
    $replacements_CNF+= ,@(($arc-1),@($old_ouput[-2],$old_ouput[-1]))
    #Write-Host "Addn. $($replacements_CNF[-1][0]) -> $($replacements_CNF[-1][1])"
}
#DEL
 #There are no Epsilon Rules
#UNIT 
 #There are no unit rules

Write-Host "Expanded $($replacements.Length) Rules to $($replacements_CNF.Length)"

#Additional Optimisation Run
#Read-host "XX"
$O_replacements_CNF = $replacements_CNF#Preserves the old Value, for debugging
$optimised = 1
while($optimised -ne 0){
    $optimised = 0
    $old_rules = $O_replacements_CNF
    $O_replacements_CNF = @()
    $substitute = @{}
    $skip =0
    #We test for each Rule if there is a rule before it that has the same input and ouput
    :oldrules foreach($rule in $old_rules){
        if($skip++ -le ($replacements.Length +3)){$O_replacements_CNF+=,$rule;continue oldrules}
        #Write-Host "Testing Rule $($rule[0]) -> $($rule[1] -join '-')"
        $duplicate = -1
        if($O_replacements_CNF.length -eq 0){$O_replacements_CNF+= ,$rule ;continue}
        0..($O_replacements_CNF.length-1)|%{#True Algo
            if(($O_replacements_CNF[$_][1] -join '-') -eq ($rule[1] -join '-')){#Output matches
                $duplicate= $_
                #Write-Host "$($rule[0]) is a duplicate"
            }
        }
        if($duplicate -eq -1){ # Has there already been a rule with the same output ?
            $O_replacements_CNF+=,$rule
        }else{#Rule is a duplicate. The substitute is noted, and replaced in bulk at the end of the (while) Cycle
            $substitute.Add($rule[0],$O_replacements_CNF[$duplicate][0])
            $optimised++
        }
    }
    #Read-Host "Removing $($substitute.keys)"
    0..($O_replacements_CNF.Length -1)|%{#For every Rule
        for($out = 0;$out -lt ($O_replacements_CNF[$_][1].length);$out++){#For its every output
            #Write-Host "Testing $($O_replacements_CNF[$_][1][$out])"
            if($substitute[$O_replacements_CNF[$_][1][$out]] -ne $null){#This is an Output that has a replacement
                #Write-Host "Substituted $($O_replacements_CNF[$_][1][$out]) with $($substitute[$O_replacements_CNF[$_][1][$out]])"
                $O_replacements_CNF[$_][1][$out] =$substitute[$O_replacements_CNF[$_][1][$out]]
            }
        }
    }
    Write-Host "Optimised (eliminated) $($optimised) Rules"
    #break #Placeholder
}

Write-host "CNF:"
Display-Rules $O_replacements_CNF

#Creation of $rules hash to speed up usage (Outputs joined with hypens to avert confucsion 11 2 and 1 12)
$rules = @{}
$O_replacements_CNF|%{
    if($_[1].Length -eq 2){#Ignores Terminal substitution rules
    $rules[$_[1] -join '-']=$_[0]}
}

#-1 is a placeholder again, I use -1 instead of $null because it has a lenght of 1
$marr = (($mmolecule-creplace '[A-Z]', ' $&').trim().Split()) #Medicine array
$words=@()
$words+=,(@(-1)*$marr.Length)
0..($marr.Length-1)|%{$words[0][$_]=@($marr[$_])}#Each Field is an array of possible values

0..($words[0].Length-1)|%{#Sets up the First Line of the CYK Algo
    if($out_to_in[$marr[$_]] -eq $null){#Cheeky throwback, contains only the Terminals
    $words[0][$_]= @($marr[$_])
    }else{
    $words[0][$_]= @($out_to_in[$marr[$_]])
    }
}

function Return-Parents{
param(
    [array]$rules = $Script:O_replacements_CNF
    ,[array]$symbol1
    ,[array]$symbol2
    )
    $ouput= @()
    foreach($s1 in $symbol1){
        foreach($s2 in $symbol2){
            foreach($rule in $rules){
            if($rule[1][0]-eq $s1 -and $rule[1][1]-eq $s2){$ouput+=$rule[0]}
            }
        }
    }
    return $ouput
}

function Display-Word{
param(
    [array]$Word
    )
    $output= ""
    foreach($Symbol in $Word){
    if($Symbol.length -gt 1){
        Write-Host ($Symbol -join '/')
        $output+= ($Symbol -join '/')
        }
    else{$output+= $Symbol[0]}
    $output +='x'
    }
    Write-Host $output
}

#Each Space that is not null is the x-y Coordiante of what Symbols are the parents of this reduction 
#x is the index from the Bottom (long word)
#Y is the index from the left
$ancestors = @($null)*$marr.Length
0..($marr.Length-1)|%{$ancestors[$_]=@($null)*($marr.Length-$_)}

#Read-Host "Beginning Main Reversal"
#Now, as opposed to a simple Redaction to ensure a solution we need to allow for Positions having multiple temporary solutions
#Worst Case runtime is (295^3 * 64)
while($words[-1].Length -gt 1){
    $len = $words[-1].Length -1
    Write-Host "Calculting for Len $($len+1)"
    #Display-Word $words[-1]
    $words+= ,(@(-1)*$len)#New word is has one slot less for possible Elements
    for($i = 0;$i -lt $words[-2].Length-1;$i++){#Iterates to all but the last index
        for($fdi=$words.Length-2;$fdi -ge 0;$fdi--){#Firstoutput-Depth-Iterator, loop iterates depth. -2 is composed due to index and that the last element is the one currently calculating
        #Iterates from one lower to the Current(words[-1]) to 0
            $first = $words[$fdi][$i]
            $last = $words[$words.Length-2-$fdi][$i+1+$fdi]
            #if($fdi -ge 1){Read-host "$($first)- $($last)"}
            #Read-host "$($first)-$($last)"
            if($first -eq -1 -or $last -eq -1){continue}
            #$first and last need to be arrays from here on out
            $parents = Return-Parents -symbol1 $first -symbol2 $last
            if($parents.length -eq 0){
                #No Matching Rule
                continue
            }else{
                $words[-1][$i]=$parents
                $ancestors[$words.Length-1][$i]= @($fdi,$i,($words.Length-2-$fdi),($i+1+$fdi))
                #if($parents.length -gt 1){Read-host "Match-$($i): $($first -join '/')-$($last -join '/') -> $($words[-1][$i])"}
            }
            #Fourth loop Not Needed
        }
    }
    if($words.Length -ge 2000){break}
}
if($words[-1] -eq 'e'){Write-Host "Startingpoint e reached Sucsessfully"}else{Write-Host "Invalid Result";break}

#As the last big step we need to Generate a parse tree
while($debugging){
    $htree = @($null)*$words.Length
    :word for($wordi = $words.Length-1;$wordi -gt 0;$wordi--){
        :symbol foreach($symbol in $words[$wordi]){
            if($symbol -eq -1){continue symbol}
            #Write-Host "Adding $($symbol-join '/') to index $($wordi)"
            0..($symbol.Length-1)|%{$htree[$wordi]+=$symbol[$_];$htree[$wordi]+=" "}
        }
    }
    $htree| Export-Clixml -Path .\htree.xml
}

#$ancestors| Export-Clixml -Path .\ancestors.xml
Write-Host "Pruning ancestors"
$tpath = @()#List of Active Knots 
$tpath+= ,(@(($words.Length-1),0) -join '-') #Format is in 'X-Y' to allow for -in as searching this space
#$tpath+= ($ancestors[-1][0][0]),($ancestors[-1][0][1])
#$tpath+= ($ancestors[-1][0][2]),($ancestors[-1][0][3])
$truerules = 0
:word for($wordi = $words.Length-1;$wordi -gt 0;$wordi--){#Go through each Word, starts at the last index 
    :symbol for($symboli = 0;$symboli -le $words[$wordi].Length;$symboli++){#Go through each Symbol
    if($words[$wordi][$symboli] -eq -1){continue symbol}
    #Write-Host (@($wordi,$symboli) -join '-')
    if((@($wordi,$symboli) -join '-') -in $tpath){#Position is Needed as a derivative from The Root
        #Write-Host "$(@($wordi,$symboli) -join '-')"
        $tpath+= ,(($ancestors[$wordi][$symboli][0]),($ancestors[$wordi][$symboli][1]) -join '-')
        if($words[$wordi][$symboli] -notmatch '\d+' -or $words[$wordi][$symboli] -le ($terminals.Length-1)){
            #Write-Host "$($words[$wordi][$symboli])->$($words[($ancestors[$wordi][$symboli][0])][($ancestors[$wordi][$symboli][1])]) $($words[($ancestors[$wordi][$symboli][2])][($ancestors[$wordi][$symboli][3])])"
            $truerules++
        }
        $tpath+= ,(($ancestors[$wordi][$symboli][2]),($ancestors[$wordi][$symboli][3]) -join '-')
    }
    }
}
Write-Host "Completed"
Write-Host "The original Rules habe been applied $($truerules) times"
