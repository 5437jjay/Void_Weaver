Add-Type -AssemblyName System.Drawing

# Crop function: extracts a region and saves as PNG with quality improvement
function CropImage($srcPath, $destPath, $x, $y, $w, $h) {
    $src = [System.Drawing.Image]::FromFile($srcPath)
    $bmp = New-Object System.Drawing.Bitmap($w, $h)
    $bmp.SetResolution(150, 150)
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
    $g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
    $g.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
    $g.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
    $srcRect = New-Object System.Drawing.Rectangle($x, $y, $w, $h)
    $destRect = New-Object System.Drawing.Rectangle(0, 0, $w, $h)
    $g.DrawImage($src, $destRect, $srcRect, [System.Drawing.GraphicsUnit]::Pixel)
    $g.Dispose()
    $bmp.Save($destPath, [System.Drawing.Imaging.ImageFormat]::Png)
    $bmp.Dispose()
    $src.Dispose()
    Write-Output "Saved: $destPath"
}

# Upscale+sharpen function: takes cropped image and upscales 2x with sharpening
function UpscaleImage($srcPath, $destPath, $targetW, $targetH) {
    $src = [System.Drawing.Image]::FromFile($srcPath)
    $bmp = New-Object System.Drawing.Bitmap($targetW, $targetH)
    $bmp.SetResolution(150, 150)
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
    $g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
    $g.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
    $g.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
    $destRect = New-Object System.Drawing.Rectangle(0, 0, $targetW, $targetH)
    $g.DrawImage($src, $destRect)
    $g.Dispose()
    $bmp.Save($destPath, [System.Drawing.Imaging.ImageFormat]::Png)
    $bmp.Dispose()
    $src.Dispose()
}

$base = "c:\TURBOC3\Projects\pdf_pages"
$out = "c:\TURBOC3\Projects\assets"
if (!(Test-Path $out)) { New-Item -ItemType Directory -Path $out -Force }

# Image locations (page, x, y, w, h) - measured from document layout
# Page 2: Image 1 - President James (large image, bottom half of page)
CropImage "$base\Document1 (2)_page-0002.jpg" "$out\scene01.png" 55 560 700 520

# Page 3: Image 2 - President meets Ethan (middle of page)
CropImage "$base\Document1 (2)_page-0003.jpg" "$out\scene02.png" 55 370 700 520

# Page 4: Image 3 - Ethan coding (upper half)
CropImage "$base\Document1 (2)_page-0004.jpg" "$out\scene03.png" 55 110 700 520

# Page 4: Image 4 - Ethan success (lower half)
CropImage "$base\Document1 (2)_page-0004.jpg" "$out\scene04.png" 55 940 700 520

Write-Output "Batch 1 done"
