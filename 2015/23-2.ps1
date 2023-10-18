cd C:\Daten\2015

$myinput = Get-Content -Path .\23-1.txt

$registers = @{
a = 1;
b = 0;
}

$instructions = $myinput
$instructionindex = 0

while($instructionindex -lt $instructions.Length){
    #Write-host $instructionindex
    $instruction = $instructions[$instructionindex].Replace(',','').split()
    switch ($instruction[0]){
    'hlf'{$registers.($instruction[1])/=2;$instructionindex++}
    'tpl'{$registers.($instruction[1])*=3;$instructionindex++}
    'inc'{$registers.($instruction[1])++;$instructionindex++}
    'jmp'{$instructionindex += $instruction[1]}
    'jie'{if($registers.($instruction[1])%2 -eq 0){$instructionindex += $instruction[2]}else{$instructionindex++}}
    'jio'{if($registers.($instruction[1]) -eq 1){$instructionindex += $instruction[2]}else{$instructionindex++}}
    }
}

Write-host "B: $($registers.b)"