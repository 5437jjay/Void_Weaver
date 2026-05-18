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

# Page9: Img13 Agent cafe indicator - image starts at ~y=330, ends ~y=720
CropAndSave "${b}0009.jpg" "$o\scene13.png" 55 335 590 385

# Page9: Img14 Briefing screen - image starts at ~y=935, ends ~y=1340
CropAndSave "${b}0009.jpg" "$o\scene14.png" 55 940 700 400

# Page10: Img15 Submarine corridor - image starts at ~y=345, ends ~y=790
CropAndSave "${b}0010.jpg" "$o\scene15.png" 130 350 570 440

# Page11: Img16 Passlock system - image starts at ~y=50, ends ~y=430
CropAndSave "${b}0011.jpg" "$o\scene16.png" 200 55 440 380

# Page11: Img17 Vault open AI machine - image starts at ~y=790, ends ~y=1210
CropAndSave "${b}0011.jpg" "$o\scene17.png" 130 795 570 415

# Page12: Img18 AI machine glass - image starts at ~y=80, ends ~y=500
CropAndSave "${b}0012.jpg" "$o\scene18.png" 130 85 570 415

# Page12: Img19 Chip extraction - image starts at ~y=815, ends ~y=1230
CropAndSave "${b}0012.jpg" "$o\scene19.png" 130 820 570 410

# Page13: Img20 Mission successful - image starts at ~y=60, ends ~y=490
CropAndSave "${b}0013.jpg" "$o\scene20.png" 55 65 570 425

Write-Output "Batch B done"
