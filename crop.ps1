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
    # Now upscale to 1280x720 for game resolution
    $final = New-Object System.Drawing.Bitmap(1280, 720)
    $g2 = [System.Drawing.Graphics]::FromImage($final)
    $g2.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
    $g2.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
    $g2.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
    $g2.DrawImage($bmp, 0, 0, 1280, 720)
    $g2.Dispose()
    $final.Save($destPath, [System.Drawing.Imaging.ImageFormat]::Png)
    $final.Dispose()
    $bmp.Dispose()
    $src.Dispose()
    Write-Output "OK: $destPath"
}

$b = "c:\TURBOC3\Projects\pdf_pages\Document1 (2)_page-"
$o = "c:\TURBOC3\Projects\assets"

# Page 2: Image 1 - President James at desk (approx y=1020, h~470)
CropAndSave "${b}0002.jpg" "$o\scene01.png" 68 1020 690 470

# Page 3: Image 2 - President meets Ethan shaking hands (approx y=440, h~470)
CropAndSave "${b}0003.jpg" "$o\scene02.png" 68 440 690 470

# Page 4: Image 3 - Ethan coding in lab (approx y=120, h~470)
CropAndSave "${b}0004.jpg" "$o\scene03.png" 68 120 690 510

# Page 4: Image 4 - Ethan success celebration (approx y=1020, h~440)
CropAndSave "${b}0004.jpg" "$o\scene04.png" 55 1020 700 440

Write-Output "Batch 1 complete"
