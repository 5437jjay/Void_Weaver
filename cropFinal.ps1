Add-Type -AssemblyName System.Drawing
function CropAndSave($srcPath, $destPath, $x, $y, $w, $h) {
    $src = [System.Drawing.Image]::FromFile($srcPath)
    $bmp = New-Object System.Drawing.Bitmap($w, $h)
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
    $g.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
    $srcRect = New-Object System.Drawing.Rectangle($x, $y, $w, $h)
    $destRect = New-Object System.Drawing.Rectangle(0, 0, $w, $h)
    $g.DrawImage($src, $destRect, $srcRect, [System.Drawing.GraphicsUnit]::Pixel)
    $g.Dispose()
    $final = New-Object System.Drawing.Bitmap(1280, 720)
    $g2 = [System.Drawing.Graphics]::FromImage($final)
    $g2.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
    $g2.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
    $g2.DrawImage($bmp, 0, 0, 1280, 720)
    $g2.Dispose()
    $final.Save($destPath, [System.Drawing.Imaging.ImageFormat]::Png)
    $final.Dispose(); $bmp.Dispose(); $src.Dispose()
    Write-Output "OK: $destPath"
}
$b = "c:\TURBOC3\Projects\pdf_pages\Document1 (2)_page-"
$o = "c:\TURBOC3\Projects\assets"
# Scene01: cut off bottom text more aggressively
CropAndSave "${b}0002.jpg" "$o\scene01.png" 80 1118 678 340
# Scene02: cut top text
CropAndSave "${b}0003.jpg" "$o\scene02.png" 80 598 678 348
Write-Output "Final fixes done"
