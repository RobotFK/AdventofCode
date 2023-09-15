cd C:\Daten\2015

$myinput = Get-Content .\12-1.txt

$list =  $myinput -replace "(`"red`")", "x"

$red = @($false)*$list.Length
function Generate-redarr { #List that determines if an index is invalud due to an x (red)
    0..($list.Length-1)|%{
    if($list[$_] -eq 'x'){#Found a Seed of Red
        Write-Host "Found x"
        $loweri = $i
        $upperi = $i
        $parentobj = $false # Value if first parent is an Object
        $i = $_
        $inobj = 0 #How many objects we are deep
        $inarr = 0 #How many lists we are deep
        $parentnotfound = $true
        while($parentnotfound){ #Look before
                switch($list[$i]){
                ']'{$inarr++}
                '['{$inarr--}
                '}'{$inobj++}
                '{'{$inobj--}
                }
                if($inobj -lt 0){ #Meaning we have left our Encasing Object
                $loweri = $i
                $parentnotfound = $false
                $parentobj = $true
                }
                if($inarr -lt 0){ #Meaning we have left our Encasing Array
                $loweri = $i
                $parentnotfound = $false
                }
                $i--
        }
        if($parentobj){ # We asume to be in an Object an Cound the other direction
            $i = $_
            $inobj = 0 #How many objects we are deep
            $inarr = 0 #How many lists we are deep
            $parentnotfound = $true
            while($parentnotfound){ #Look before
                switch($list[$i]){
                ']'{$inarr--}
                '['{$inarr++}
                '}'{$inobj--}
                '{'{$inobj++}
                }
                if($inobj -lt 0){ #Meaning we have left our Encasing Object
                $upperi = $i
                $parentnotfound = $false
                }
                if($inarr -lt 0){ #Meaning we have left our Encasing Array
                $upperi = $i
                $parentnotfound = $false
                Write-Error "MISMATCHING PARENT! ERROR IN INPUT OR SCRIPT"
                }
                $i++
            }
            #Now we mark out the section
            $loweri..$upperi|%{$red[$_] = $true}
            Write-Host "Marked $($upperi-$loweri) Indexis"
        }
    }
    }
}
Generate-redarr

#Filter String
$clist = ""
0..($list.Length-1)|%{
    if(-not $red[$_]){$clist += $list[$_]}
}

$clist = $clist -replace "[^-|\d]+", ","
#Count
while($false){
$sum = 0
$clist.Split(",")|%{$sum += $_}
Write-Host "Finall sum: $($sum)"
}

#68466
#Too high