Add-Type -AssemblyName System.Drawing

function CropAndSave($srcPath, $destPath, $x, $y, $w, $h) {
    $src = [System.Drawing.Image]::FromFile($srcPath)
    $bmp = New-Object System.Drawing.Bitmap($w, $h)
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
    $g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
    $g.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
    $srcRect = New-Object System.Drawing.Rectangle($x, $y, $w, $h)
    $destRect = New-Object System.Drawing.Rectangle(0, 0, $w, $h)
    $g.DrawImage($src, $destRect, $srcRect, [System.Drawing.GraphicsUnit]::Pixel)
    $g.Dispose()
    $final = New-Object System.Drawing.Bitmap(1280, 720)
    $g2 = [System.Drawing.Graphics]::FromImage($final)
    $g2.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
    $g2.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
    $g2.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
    $g2.DrawImage($bmp, 0, 0, 1280, 720)
    $g2.Dispose()
    $final.Save($destPath, [System.Drawing.Imaging.ImageFormat]::Png)
    $final.Dispose(); $bmp.Dispose(); $src.Dispose()
    Write-Output "OK: $destPath"
}

$b = "c:\TURBOC3\Projects\pdf_pages\Document1 (2)_page-"
$o = "c:\TURBOC3\Projects\assets"

# Fix scene01: image only (cut more text at bottom, more at top)
CropAndSave "${b}0002.jpg" "$o\scene01.png" 68 1100 700 380

# Fix scene02: image starts lower (cut "2." and text above)
CropAndSave "${b}0003.jpg" "$o\scene02.png" 68 570 700 390

# Fix scene16: passlock - needs bottom portion too (CLEAR/0/ENTER row)
CropAndSave "${b}0011.jpg" "$o\scene16.png" 195 55 450 400

# Fix scene20: mission successful - cut the "20." text at top
CropAndSave "${b}0013.jpg" "$o\scene20.png" 130 130 570 390

Write-Output "Fixes done"
