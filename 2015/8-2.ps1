cd C:\Daten\2015
# ` is the Escape Character to Put Quotes in strings

$myinput = @(
"`"`"",
"`"abc`"",
"`"aaa\`"aaa`"",
"`"\x27`"")
$myinput = Get-Content -Path .\8-1.txt

function Get-encoded{ #Returns the number of characters for encoding of the values of the strings
param(
    [string]$string
    )

    $string = $string -replace "\\", "\\" #backslashes
    $string = $string -replace "`"", "\`"" #quotes
    #$string = $string -replace "((?<!\\)\\x[a-z|0-9][a-z|0-9])", "_" #hex (just replaced by underscores)

    $string = -join("`"",$string,"`"")

    Write-Host $string

    #$string = -join("`"",$string,"`"") #Add Front and Trailing Quotes
    return ($string.Length)
}

$string_literals = 0
$memory_values = 0
$myinput|%{$string_literals += $_.Length;$memory_values += Get-encoded $_}
#$myinput|%{Write-Host $_.Length " ->" $(Get-Characters $_)}


Write-Host "Final Calc is $($memory_values) - $($string_literals) = $($memory_values-$string_literals)"


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