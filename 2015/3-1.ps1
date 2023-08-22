# Coordinate system used:
# ^+Y
# |
# 0->+X
# (X,Y)
$input = Get-Content -Path .\3-1.txt |%{[char[]]$_}
$dict = @{
    "^" = 0,1;
    "v" = 0,-1;
    ">" = 1,0;
    "<" = -1,0;
          }

class pos{
[int]$x = 0
[int]$y = 0
[bool]$first_visit = $true
}

$Santa = [pos]::new()
$visited = @([pos]::new())

#Translation from moves to positions
$input[0..($input.Length-1)]|%{$move = $dict[$_.tostring()];
$Santa.x += $move[0];
$Santa.y += $move[1];
#Write-Host "Moved to $($santa.x) | $($santa.y)";
$visited += [pos]@{x=$santa.x;y=$santa.y}
#$visited +=$Santa.PsObject.copy(); #Doesnt Work due to Copy being shallow
}

if((Read-Host -Prompt ("Output visited ? (y/n)")) -eq  "y"){ $visited}

#Checking if Position has been visited before
$index = @(0..($i.Length-1))
$index|%{$current_index = $_;$visited[$current_index]}|%{
    $testindex = 0;#Increments form 0 to the current index
    while($testindex -lt $current_index){
        
        if(($visited[$current_index].x -eq $visited[$testindex].x) -and ($visited[$current_index].y -eq $visited[$testindex].y)){
            $visited[$current_index].first_visit = $false;
            #Write-Host "Index $($current_index) is a revisit of $($testindex)"
            $testindex = $current_index;
        }else{
            $testindex += 1;
        }
    }
}

#Summing up Unique positions
$visited_houses = 0
$visited[0..($i.Length-1)]|%{if($_.first_visit){$visited_houses+=1}}

Write-Host "Santa visited $($visited_houses) different houses"
