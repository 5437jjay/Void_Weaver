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

# All 20 images - precise coordinates (images have ~5px border in document)
# Page2: Img1 - President James. Image top edge at ~1115, bottom at ~1475
CropAndSave "${b}0002.jpg" "$o\scene01.png" 75 1115 685 360

# Page3: Img2 - Handshake. Image at ~590 to ~955
CropAndSave "${b}0003.jpg" "$o\scene02.png" 75 590 685 365

# Page4: Img3 - Ethan coding. At ~170 to ~550
CropAndSave "${b}0004.jpg" "$o\scene03.png" 63 175 690 375

# Page4: Img4 - Ethan success. At ~1050 to ~1460
CropAndSave "${b}0004.jpg" "$o\scene04.png" 55 1060 700 400

# Page5: Img5 - Ethan regret. At ~175 to ~590
CropAndSave "${b}0005.jpg" "$o\scene05.png" 55 185 700 405

# Page5: Img6 - Ethan desperate. At ~850 to ~1270
CropAndSave "${b}0005.jpg" "$o\scene06.png" 55 855 700 410

# Page6: Img7 - Fire/explosion. At ~200 to ~555
CropAndSave "${b}0006.jpg" "$o\scene07.png" 62 205 680 350

# Page6: Img8 - News Supreme AI. At ~855 to ~1225
CropAndSave "${b}0006.jpg" "$o\scene08.png" 62 862 680 363

# Page7: Img9 - ICJ. At ~70 to ~478
CropAndSave "${b}0007.jpg" "$o\scene09.png" 62 75 690 403

# Page7: Img10 - Phone call. At ~775 to ~1195
CropAndSave "${b}0007.jpg" "$o\scene10.png" 62 780 690 415

# Page8: Img11 - Classified. At ~195 to ~655
CropAndSave "${b}0008.jpg" "$o\scene11.png" 62 200 700 455

# Page8: Img12 - Agent cafe. At ~1015 to ~1370
CropAndSave "${b}0008.jpg" "$o\scene12.png" 62 1020 580 350

# Page9: Img13 - Indicator. At ~345 to ~710
CropAndSave "${b}0009.jpg" "$o\scene13.png" 62 350 568 360

# Page9: Img14 - Briefing. At ~950 to ~1335
CropAndSave "${b}0009.jpg" "$o\scene14.png" 62 955 690 380

# Page10: Img15 - Submarine. At ~360 to ~785
CropAndSave "${b}0010.jpg" "$o\scene15.png" 140 365 555 420

# Page11: Img16 - Passlock. At ~65 to ~430
CropAndSave "${b}0011.jpg" "$o\scene16.png" 220 65 415 365

# Page11: Img17 - Vault. At ~805 to ~1200
CropAndSave "${b}0011.jpg" "$o\scene17.png" 140 810 555 390

# Page12: Img18 - AI machine glass. At ~100 to ~495
CropAndSave "${b}0012.jpg" "$o\scene18.png" 140 105 555 390

# Page12: Img19 - Extraction. At ~835 to ~1225
CropAndSave "${b}0012.jpg" "$o\scene19.png" 140 840 555 385

# Page13: Img20 - Mission. At ~175 to ~560
CropAndSave "${b}0013.jpg" "$o\scene20.png" 140 180 540 380

Write-Output "ALL 20 images cropped!"
