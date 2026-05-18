"""
Prepare game assets: Resize all PDF-extracted images to uniform 1280x720 game resolution.
Maps PDF page images to sequential scene IDs for the game.
"""
from PIL import Image, ImageEnhance, ImageFilter
import os

INPUT_DIR = r"c:\TURBOC3\Projects\pdf_output"
OUTPUT_DIR = r"c:\TURBOC3\Projects\game_assets"
os.makedirs(OUTPUT_DIR, exist_ok=True)

TARGET_W, TARGET_H = 1280, 720

# Mapping: scene_id -> (pdf_image_filename, description)
SCENE_MAP = {
    0:  ("page2_img1.jpeg",   "Title screen VOID WEAVER"),
    1:  ("page3_img1.jpeg",   "Intro: President thinking about AI"),
    2:  ("page3_img2.jpeg",   "Intro: President and Ethan handshake"),
    3:  ("page4_img1.jpeg",   "Intro: Ethan typing on keyboard"),
    4:  ("page4_img2.jpeg",   "Intro: Ethan celebrates victory"),
    5:  ("page5_img1.jpeg",   "Intro: Ethan realizes AI uncontrollable"),
    6:  ("page5_img2.jpeg",   "Intro: Ethan typing to erase code"),
    7:  ("page5_img3.jpeg",   "Intro: Lab fire explosion"),
    8:  ("page6_img1.jpeg",   "Intro: Supreme AI visual"),
    9:  ("page6_img2.jpeg",   "Intro: ICJ meeting"),
    10: ("page7_img1.jpeg",   "Intro: James phone call"),
    11: ("page7_img2.jpeg",   "Intro: James plans to send agent"),
    12: ("page8_img1.jpeg",   "Intro: Agent Zero tea"),
    13: ("page8_img2.jpeg",   "Intro: Red indicator appears"),
    14: ("page8_img3.jpeg",   "Intro: Blue screen mission brief"),
    # Module 1
    15: ("page9_img1.jpeg",   "M1: House exterior"),
    16: ("page10_img1.jpeg",  "M1: Room interior base1"),
    17: ("page11_img1.jpeg",  "M1: Computer passcode screen"),
    18: ("page11_img2.jpeg",  "M1: Room with computer ON base2"),
    19: ("page12_img1.jpeg",  "M1: Book Machine Intelligence"),
    20: ("page12_img2.jpeg",  "M1: Desk passcode roller"),
    21: ("page13_img1.jpeg",  "M1: Desk contents floating"),
    22: ("page13_img2.jpeg",  "M1: Photo fallen key revealed"),
    23: ("page14_img1.jpeg",  "M1: Mirror close-up"),
    24: ("page14_img2.jpeg",  "M1: Mirror open shelf"),
    25: ("page15_img1.jpeg",  "M1: Letter from shelf"),
    26: ("page15_img2.jpeg",  "M1: Computer desktop logged in"),
    27: ("page16_img1.jpeg",  "M1: Map with red dot"),
    # Travel scenes
    28: ("page16_img2.jpeg",  "Travel: Flight"),
    29: ("page17_img1.jpeg",  "Travel: Boat"),
    30: ("page17_img2.jpeg",  "Travel: Harbour"),
    31: ("page18_img1.jpeg",  "Travel: Garage door"),
    # Module 2
    32: ("page18_img2.jpeg",  "M2: Inside garage mirror"),
    33: ("page19_img1.jpeg",  "M2: Mirror reflection"),
    34: ("page19_img2.jpeg",  "M2: Vortex mirror text"),
    35: ("page19_img3.jpeg",  "M2: TTT intro screen"),
    36: ("page20_img1.jpeg",  "M2: Ultimate TTT board"),
    37: ("page21_img1.jpeg",  "M2: TTT selected state ref"),
    38: ("page21_img2.jpeg",  "M2: After win vehicle"),
    # Module 3
    39: ("page22_img1.jpeg",  "M3: Vehicle cockpit wall"),
    40: ("page22_img2.jpeg",  "M3: Wall door sliding up"),
    41: ("page22_img3.jpeg",  "M3: Vehicle floating up"),
    42: ("page23_img1.jpeg",  "M3: Levitation vehicle"),
    43: ("page23_img2.jpeg",  "M3: Red line route"),
    44: ("page23_img3.jpeg",  "M3: Lab door approach"),
    45: ("page24_img1.jpeg",  "M3: Lab entrance"),
    46: ("page24_img2.jpeg",  "M3: Agent near wall"),
    47: ("page24_img3.jpeg",  "M3: Wall zoomed"),
    48: ("page25_img1.jpeg",  "M3: Chess intro words"),
    49: ("page25_img2.jpeg",  "M3: Chess board partial"),
    # Lift
    50: ("page26_img1.png",   "Lift: Floor display number"),
    51: ("page26_img2.jpeg",  "Lift: Inside lift view"),
    52: ("page26_img3.jpeg",  "Lift: Door closing"),
    # Module 4
    53: ("page27_img1.jpeg",  "M4: Submarine passlock room"),
    54: ("page27_img2.jpeg",  "M4: Passlock zoomed"),
    55: ("page27_img3.jpeg",  "M4: AI machine room"),
    56: ("page28_img1.jpeg",  "M4: Breaking glass"),
    57: ("page28_img2.jpeg",  "M4: Machine explosion"),
    58: ("page28_img3.jpeg",  "M4: President ending"),
}

def resize_and_enhance(img_path, out_path, target_w=TARGET_W, target_h=TARGET_H):
    """Resize image to target dimensions with high-quality Lanczos resampling."""
    img = Image.open(img_path).convert("RGB")
    
    # Calculate aspect-ratio-preserving resize, then crop to fit
    src_ratio = img.width / img.height
    tgt_ratio = target_w / target_h
    
    if src_ratio > tgt_ratio:
        # Source is wider - fit height, crop width
        new_h = target_h
        new_w = int(img.width * (target_h / img.height))
    else:
        # Source is taller - fit width, crop height
        new_w = target_w
        new_h = int(img.height * (target_w / img.width))
    
    img = img.resize((new_w, new_h), Image.LANCZOS)
    
    # Center crop to exact target
    left = (new_w - target_w) // 2
    top = (new_h - target_h) // 2
    img = img.crop((left, top, left + target_w, top + target_h))
    
    # Slight sharpening for clarity
    enhancer = ImageEnhance.Sharpness(img)
    img = enhancer.enhance(1.2)
    
    # Uniform brightness/contrast adjustment
    enhancer = ImageEnhance.Brightness(img)
    img = enhancer.enhance(1.05)
    
    enhancer = ImageEnhance.Contrast(img)
    img = enhancer.enhance(1.08)
    
    img.save(out_path, "PNG", quality=95)
    return True

# Process all scenes
for scene_id, (filename, desc) in sorted(SCENE_MAP.items()):
    src = os.path.join(INPUT_DIR, filename)
    dst = os.path.join(OUTPUT_DIR, f"scene_{scene_id:02d}.png")
    
    if not os.path.exists(src):
        print(f"WARNING: Missing source: {filename} for scene {scene_id} ({desc})")
        continue
    
    resize_and_enhance(src, dst)
    print(f"Scene {scene_id:02d}: {desc} -> {dst}")

print(f"\nAll {len(SCENE_MAP)} scenes processed into {OUTPUT_DIR}")
