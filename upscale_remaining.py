"""
High-quality upscale remaining intro images using PIL with advanced enhancement.
For images where Gemini quota was exhausted.
"""
from PIL import Image, ImageEnhance, ImageFilter
import os

INPUT_DIR = r"c:\TURBOC3\Projects\pdf_output"
PROJ_DIR = r"c:\TURBOC3\Projects"
ASSETS_DIR = r"c:\TURBOC3\Projects\game_assets"
TARGET_W, TARGET_H = 1280, 720

# Remaining images that need upscaling (page5_img3 through page8_img3)
REMAINING = [
    ("page5_img3.jpeg", "page5_img3_rc.png", "scene_07.png"),  # Lab fire
    ("page6_img1.jpeg", "page6_img1_rc.png", "scene_08.png"),  # Supreme AI
    ("page6_img2.jpeg", "page6_img2_rc.png", "scene_09.png"),  # ICJ meeting
    ("page7_img1.jpeg", "page7_img1_rc.png", "scene_10.png"),  # James phone
    ("page7_img2.jpeg", "page7_img2_rc.png", "scene_11.png"),  # James planning
    ("page8_img1.jpeg", "page8_img1_rc.png", "scene_12.png"),  # Agent tea
    ("page8_img2.jpeg", "page8_img2_rc.png", "scene_13.png"),  # Red indicator
    ("page8_img3.jpeg", "page8_img3_rc.png", "scene_14.png"),  # Mission brief
]

def premium_upscale(src_path, dst_path, tw=TARGET_W, th=TARGET_H):
    """Premium upscaling with multi-pass enhancement for best quality."""
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
    
    img.save(dst_path, "PNG", optimize=True)
    return img.size

for src_name, rc_name, scene_name in REMAINING:
    src = os.path.join(INPUT_DIR, src_name)
    rc_dst = os.path.join(PROJ_DIR, rc_name)
    scene_dst = os.path.join(ASSETS_DIR, scene_name)
    
    if not os.path.exists(src):
        print(f"WARNING: Missing {src_name}")
        continue
    
    # Create enhanced rc version
    size = premium_upscale(src, rc_dst)
    print(f"Created {rc_name} ({size[0]}x{size[1]})")
    
    # Remove old scene and replace
    if os.path.exists(scene_dst):
        os.remove(scene_dst)
    premium_upscale(src, scene_dst)
    print(f"Replaced {scene_name}")

print("\nAll remaining images upscaled and replaced!")
