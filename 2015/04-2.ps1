cd C:\Daten\2015

#Warning, this is now efficent
#We only requrire one Chunk

$my_input = "iwrupvqb"


function Get-Hash { #Faster and made by Chat gpt
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

$goal = "000000"
$hsh = $null #Needed to clear
[int]$increment = 0
[int]$pulse = 10000 #Too keep the Dev sane and the console clear
Write-Host "Running Loop with input $($my_input)"
while($goal -ne $hsh){
$test = $my_input + $increment.ToString()
$hsh = [string]::Join("",(Get-Hash $test)[0..5])
$increment += 1
if($increment -eq $pulse){
    Write-Host "Cleared $($pulse) options"
    $pulse *= 10
}
}

$increment-=1
Write-Host "The first macthing number for $($my_input) is $($increment)"
