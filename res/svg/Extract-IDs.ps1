# -------------------------------------------------------
# Author: Arthur Damböck
# 2025/2026
# -------------------------------------------------------

# define input and output files
$InputFile  = ".\frames_architecture.svg"
$OutputFile = ".\frames_architecture_ids.txt"

# extract all id's, which are not needed
$ids = Get-Content $InputFile |
Select-String 'id="([^"]+)"' |
ForEach-Object {
    if ($_ -match 'id="([^"]+)"') {
        $id = $matches[1]
        if (
        $id -notmatch '^g\d+$' -and
        $id -notmatch '^path\d+.*$' -and
        $id -notmatch '^svg\d+$' -and
        $id -notmatch '^true$' -and
        $id -notmatch '^false$' -and
        $id -notmatch '^defs\d+$' -and
        $id -notmatch '^namedview\d+$' -and
        $id -notmatch '^grid\d+$' -and
        $id -notmatch '^swatch\d+$' -and
        $id -notmatch '^stop\d+$' -and
        $id -notmatch '^linearGradient\d+$' -and
        $id -notmatch '^layer\d+$'
        ) {
            $id
        }
    }
}

# remove NULL values and empty lines
$ids = $ids | Where-Object { $_ -and $_.Trim() -ne "" }

# write into output file
$ids | Set-Content $OutputFile -Encoding UTF8

Write-Host "IDs saved:  $OutputFile ."
