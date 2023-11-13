$myinput = Get-Content -Path .\19-1.txt
#After a lot of work I undestand that spending my time on this is not ideal here.This has already tought me new theoretical stuff.For now I will just use the specif proof from somebody else
#www.reddit.com/r/adventofcode/comments/3xflz8/day_19_solutions/cy4etju did the math, this is just the automated solution :/

$elements=(($myinput[-1].substring(0,1).toupper() + $myinput[-1].substring(1) -creplace '[A-Z]', ' $&').Trim())
$elements = -split $elements

$Rncount = 0
$Ycount = 0
$elements|%{
    if($_ -eq 'Rn'){$Rncount++}
    if($_ -eq 'Y'){$Ycount++}
}

$solution = $elements.Length -( ($Rncount*2) + ($Ycount*2) +1)

Write-Host "Solution is $($solution)"
