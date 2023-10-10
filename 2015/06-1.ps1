cd C:\Daten\2015

#0---> +X
#|
#|
#|
#V +Y

$myinput = Get-Content -Path .\6-1.txt

$lights = [bool[,]]::new(1000,1000) #$lights[x,y] This style is needed, as it otherwise would flatten immediatly

#$tinylights = [bool[,]]::new(10,10)

function Change_Lights{
param(
    [string]$action,#toggle, on , of
    [int]$xlow,
    [int]$xhigh,
    [int]$ylow,
    [int]$yhigh
    )
    #Write-Host "Turning $($action)"
    if($xlow -gt $xhigh){ $xlow, $xhigh = $xhigh, $xlow}
    if($ylow -gt $yhigh){ $ylow, $yhigh = $yhigh, $ylow}
    #Aligment is now the spot top left and bottom right of the affected area
    $changed
    for ($y = $ylow; $y -le $yhigh; $y += 1)
    {
        for ($x = $xlow; $x -le $xhigh; $x += 1){
            $changed += 1
            if($action -eq "on"){
                $lights[$x,$y] = $true}
            elseif($action -eq "off"){
                $lights[$x,$y] = $false}
            elseif($action -eq "toggle"){
                #Write-Host "Toggled $($x):$($y) from $($lights[$x,$y])"
                $lights[$x,$y] = -not $lights[$x,$y]}
        }
    }
    Write-Verbose "Affected $($changed) Lights"
}

$myinput|%{
    $Split = $_.Split()
    $action = If ($Split[0] -eq "toggle") {$Split[0]} Else {$Split[1]}

    Change_Lights -action $action -xlow $split[-3].split(',')[0] -ylow $split[-1].split(',')[1] -xhigh $split[-1].split(',')[0] -yhigh $split[-3].split(',')[1]
}
#$lights[$y][$xlow..$xhigh] -replace $true,$false #Honable mention, but doesn't work

Write-Host "Done flipping switches"

$Sum_lights = 0
for ($y = 0; $y -le 999; $y += 1)
    {
        for ($x = 0; $x -le 999; $x += 1){
            if($lights[$x,$y] -eq $true){
                $Sum_lights += 1}
        }
    }

Write-Host "$($Sum_lights) Lights are On"
