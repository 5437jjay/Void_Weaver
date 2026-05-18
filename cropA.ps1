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

# Page2: Img1 President James - image starts at ~y=1080, ends ~y=1490
CropAndSave "${b}0002.jpg" "$o\scene01.png" 68 1085 700 410

# Page3: Img2 Meeting handshake - image starts at ~y=530, ends ~y=940
CropAndSave "${b}0003.jpg" "$o\scene02.png" 68 535 700 410

# Page4: Img3 Ethan coding - image starts at ~y=155, ends ~y=565
CropAndSave "${b}0004.jpg" "$o\scene03.png" 55 155 700 415

# Page4: Img4 Ethan success - image starts at ~y=1040, ends ~y=1470
CropAndSave "${b}0004.jpg" "$o\scene04.png" 55 1045 700 420

# Page5: Img5 Ethan regret/evolved - image starts at ~y=165, ends ~y=600
CropAndSave "${b}0005.jpg" "$o\scene05.png" 55 165 700 435

# Page5: Img6 Ethan desperate typing - image starts at ~y=830, ends ~y=1280
CropAndSave "${b}0005.jpg" "$o\scene06.png" 55 835 700 445

# Page6: Img7 Lab fire/explosion - image starts at ~y=175, ends ~y=560
CropAndSave "${b}0006.jpg" "$o\scene07.png" 55 180 700 380

# Page6: Img8 News/Supreme AI revolution - image starts at ~y=840, ends ~y=1230
CropAndSave "${b}0006.jpg" "$o\scene08.png" 55 845 700 385

# Page7: Img9 ICJ meeting - image starts at ~y=60, ends ~y=480
CropAndSave "${b}0007.jpg" "$o\scene09.png" 55 62 700 420

# Page7: Img10 President phone call - image starts at ~y=760, ends ~y=1200
CropAndSave "${b}0007.jpg" "$o\scene10.png" 55 765 700 435

# Page8: Img11 Classified/Agent Zero - image starts at ~y=175, ends ~y=660
CropAndSave "${b}0008.jpg" "$o\scene11.png" 55 180 700 480

# Page8: Img12 Agent cafe - image starts at ~y=1000, ends ~y=1380
CropAndSave "${b}0008.jpg" "$o\scene12.png" 55 1005 700 375

Write-Output "Batch A done"
