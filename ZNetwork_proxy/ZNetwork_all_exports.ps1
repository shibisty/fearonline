param(
    [string]$DllPath = "ZNetwork_orig.dll"
)

$bytes = [System.IO.File]::ReadAllBytes($DllPath)

function RvaToOffset($rva, $sections) {
    foreach ($s in $sections) {
        if ($rva -ge $s.VirtualAddress -and $rva -lt ($s.VirtualAddress + $s.VirtualSize)) {
            return $rva - $s.VirtualAddress + $s.PointerToRawData
        }
    }
    return -1
}

$peOffset = [BitConverter]::ToInt32($bytes, 0x3C)
$machine  = [BitConverter]::ToUInt16($bytes, $peOffset + 4)
$numSections = [BitConverter]::ToUInt16($bytes, $peOffset + 6)
$optHeaderSize = [BitConverter]::ToUInt16($bytes, $peOffset + 20)
$optHeaderOffset = $peOffset + 24
$magic = [BitConverter]::ToUInt16($bytes, $optHeaderOffset)

if ($magic -eq 0x10b) {
    $dataDirOffset = $optHeaderOffset + 96   # PE32
} else {
    $dataDirOffset = $optHeaderOffset + 112  # PE32+
}

$exportDirRVA  = [BitConverter]::ToInt32($bytes, $dataDirOffset)
$exportDirSize = [BitConverter]::ToInt32($bytes, $dataDirOffset + 4)

Write-Host "Export Directory RVA: $exportDirRVA  Size: $exportDirSize"

# --- секции ---
$sectionTableOffset = $optHeaderOffset + $optHeaderSize
$sections = @()
for ($i = 0; $i -lt $numSections; $i++) {
    $base = $sectionTableOffset + ($i * 40)
    $virtAddr = [BitConverter]::ToInt32($bytes, $base + 12)
    $virtSize = [BitConverter]::ToInt32($bytes, $base + 8)
    $rawPtr   = [BitConverter]::ToInt32($bytes, $base + 20)
    $sections += [PSCustomObject]@{ VirtualAddress = $virtAddr; VirtualSize = $virtSize; PointerToRawData = $rawPtr }
}

$exportOffset = RvaToOffset $exportDirRVA $sections
if ($exportOffset -lt 0) { Write-Host "Export directory RVA not mapped to any section!"; exit }

$numberOfNames    = [BitConverter]::ToInt32($bytes, $exportOffset + 24)
$addrOfNamesRVA   = [BitConverter]::ToInt32($bytes, $exportOffset + 32)
$addrOfNamesOff   = RvaToOffset $addrOfNamesRVA $sections

Write-Host "NumberOfNames: $numberOfNames"
Write-Host "----------------------------------------"

$names = New-Object System.Collections.Generic.List[string]
for ($i = 0; $i -lt $numberOfNames; $i++) {
    $nameRVA = [BitConverter]::ToInt32($bytes, $addrOfNamesOff + ($i * 4))
    $nameOff = RvaToOffset $nameRVA $sections
    $sb = New-Object System.Text.StringBuilder
    $p = $nameOff
    while ($bytes[$p] -ne 0) {
        [void]$sb.Append([char]$bytes[$p])
        $p++
    }
    $names.Add($sb.ToString())
}

$names | Sort-Object | Out-File -Encoding utf8 "all_exports.txt"
Write-Host "Saved $($names.Count) names to all_exports.txt"
