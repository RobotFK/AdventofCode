cd C:\Daten\2015

#We only requrire one Chunk

$my_input = "iwrupvqb"

function Get-Hash{
     param (
        [string]$plain
    )

    $stringAsStream = [System.IO.MemoryStream]::new()
    $writer = [System.IO.StreamWriter]::new($stringAsStream)
    $writer.write($plain)
    $writer.Flush()
    $stringAsStream.Position = 0
    return (Get-FileHash -InputStream $stringAsStream -Algorithm MD5|%{$_.Hash})
}

function Get-Hash2 { #Faster and made by Chat gpt
    param (
        [string]$plain
    )

    $bytes = [System.Text.Encoding]::UTF8.GetBytes($plain)
    $md5 = [System.Security.Cryptography.MD5]::Create()
    $hashBytes = $md5.ComputeHash($bytes)
    $md5.Clear()

    $hashString = [BitConverter]::ToString($hashBytes) -replace '-', ''
    return $hashString.ToLower()
}

$goal = "00000"
$hsh = $null #Needed to clear
[int]$increment = 0
[int]$pulse = 1000
Write-Host "Running Loop with input $($my_input)"
while($goal -ne $hsh){
$test = $my_input + $increment.ToString()
$hsh = [string]::Join("",(Get-Hash2 $test)[0..4])
$increment += 1
if($increment -eq $pulse){
    Write-Host "Cleared $($pulse) options"
    $pulse *= 10
}
}

$increment-=1
Write-Host "The first macthing number for $($my_input) is $($increment)"