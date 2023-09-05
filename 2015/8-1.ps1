cd C:\Daten\2015
# ` is the Escape Character to Put Quotes in strings

$myinput = @(
"`"`"",
"`"abc`"",
"`"aaa\`"aaa`"",
"`"\x27`"")
#$myinput = @(,"`"bidsptalmoicyorbv\\`"")
$myinput = Get-Content -Path .\8-1.txt

function Get-Characters{ #Returns the number of characters in memory for the values of the strings
param(
    [string]$string# and,or,lshift,rshift
    )
    #$string = -join($string[1..($string.Length-2)]) #Remove Front and Trailing Quotes
    
    $string = $string -replace "(\\\\)", "/" #backslashes
    $string = $string -replace "((?<!\\)\\x[a-z|0-9][a-z|0-9])", "_" #hex (just replaced by underscores)
    $string = $string -replace "(\\`"(?=[^\t\n\r]))", "`"" #quotes
    #$string = $string -replace "(\\\\)+", "\" #backslashes

    #Write-Host $string

    #$string = -join("`"",$string,"`"") #Add Front and Trailing Quotes
    return ($string.Length-2)
}

$string_literals = 0
$memory_values = 0
$myinput|%{$string_literals += $_.Length;$memory_values += Get-Characters $_}
#$myinput|%{Write-Host $_.Length " ->" $(Get-Characters $_)}


Write-Host "Final Calc is $($string_literals) - $($memory_values) = $($string_literals-$memory_values)"

#1375 Too high
#1364 Too high
#1332 Wrong
while($false){
$test = $myinput[-3]
$test = -join($test[1..($test.Length-2)])
$test = -join("`"",$test,"`"")

Get-Characters "`"\x66`""

Get-Characters "`"\\x66`""

Get-Characters "`"\\\x66`""

Get-Characters "`"\\\\x66`""

#1 4 2 5
}