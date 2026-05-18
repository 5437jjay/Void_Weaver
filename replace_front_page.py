"""
Replace Title/Front Page image (scene_00.png) with the new 'front page.jpeg'
Applies premium upscaling and high-quality enhancement to match the game quality.
"""
from PIL import Image, ImageEnhance, ImageFilter
import os

# Define relative paths based on the directory containing this script
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
SRC_IMAGE = os.path.join(BASE_DIR, "front page.jpeg")

# Output files to replace
DST_SCENE = os.path.join(BASE_DIR, "game_assets", "scene_00.png")
DST_RC = os.path.join(BASE_DIR, "page2_img1_rc.png")
DST_PDF = os.path.join(BASE_DIR, "pdf_page2_img1.png")

TARGET_W, TARGET_H = 1280, 720

def premium_upscale(src_path, dst_path, tw=TARGET_W, th=TARGET_H):
    """Premium upscaling with multi-pass enhancement for best quality."""
    if not os.path.exists(src_path):
        print(f"ERROR: Source image not found at: {src_path}")
        return False
        
    print(f"Processing: {src_path}")
    img = Image.open(src_path).convert("RGB")
    
    # Step 1: Calculate aspect-preserving resize
    src_ratio = img.width / img.height
    tgt_ratio = tw / th
    if src_ratio > tgt_ratio:
        new_h = th
        new_w = int(img.width * (th / img.height))
    else:
        new_w = tw
        new_h = int(img.height * (tw / img.width))
    
    # Step 2: Upscale with LANCZOS (best quality resampling)
    img = img.resize((new_w, new_h), Image.LANCZOS)
    
    # Step 3: Center crop to exact target
    left = (new_w - tw) // 2
    top = (new_h - th) // 2
    img = img.crop((left, top, left + tw, top + th))
    
    # Step 4: Multi-pass enhancement for clarity
    # Sharpen to recover detail lost in upscaling
    enhancer = ImageEnhance.Sharpness(img)
    img = enhancer.enhance(1.4)
    
    # Slight contrast boost for clarity
    enhancer = ImageEnhance.Contrast(img)
    img = enhancer.enhance(1.1)
    
    # Slight color saturation boost
    enhancer = ImageEnhance.Color(img)
    img = enhancer.enhance(1.08)
    
    # Brightness normalization
    enhancer = ImageEnhance.Brightness(img)
    img = enhancer.enhance(1.03)
    
    # Unsharp mask for fine detail enhancement
    img = img.filter(ImageFilter.UnsharpMask(radius=2, percent=80, threshold=3))
    
    # Save directory existence check
    dst_dir = os.path.dirname(dst_path)
    if dst_dir and not os.path.exists(dst_dir):
        os.makedirs(dst_dir, exist_ok=True)
        
    img.save(dst_path, "PNG", optimize=True)
    print(f"Saved high-quality asset to: {dst_path}")
    return True

if __name__ == "__main__":
    print("=" * 60)
    print("  VOID WEAVER - Title Image Replacement Script")
    print("=" * 60)
    
    # Process replacements
    success = True
    for dst in [DST_SCENE, DST_RC, DST_PDF]:
        if premium_upscale(SRC_IMAGE, dst):
            print(f"Successfully replaced/created: {os.path.basename(dst)}")
        else:
            success = False
            
    print("-" * 60)
    if success:
        print("SUCCESS: All title images replaced with new high-quality versions!")
    else:
        print("WARNING: Some files could not be replaced. Check if paths are correct.")
    print("=" * 60)
