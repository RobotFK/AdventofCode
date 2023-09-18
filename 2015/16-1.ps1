cd C:\Daten\2015

$myinput = Get-Content -Path .\16-1.txt

class aunt  {
    [int]$Sue #Id
    [int]$children= -1
    [int]$cats= -1
    [int]$samoyeds= -1
    [int]$pomeranians= -1
    [int]$akitas= -1
    [int]$vizslas= -1
    [int]$goldfish= -1
    [int]$trees= -1
    [int]$cars= -1
    [int]$perfumes= -1
}

$aunts = @()

$myinput|%{
    $split = $_.split().trim(',:')
    $aunt = [aunt]::new()
    for($i= 0;$i -lt $split.Length;$i+=2){
        $aunt.$($split[$i]) = $split[$i+1]  
    }
    $aunts += $aunt
}

$aunts|%{
    if( $_.children -in 3,-1 -and
        $_.cats -in 7,-1 -and
        $_.samoyeds -in 2,-1 -and
        $_.pomeranians -in 3,-1 -and
        $_.akitas -in 0,-1 -and
        $_.vizslas -in 0,-1 -and
        $_.goldfish -in 5,-1 -and
        $_.trees -in 3,-1 -and
        $_.cars -in 2,-1 -and
        $_.perfumes -in 1,-1){
        Write-Host "Match with Aunt $($_.Sue)"
    }
}