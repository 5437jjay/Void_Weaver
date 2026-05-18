import fitz  # PyMuPDF
import os

pdf_path = r"c:\TURBOC3\Projects\OOP Project-1.pdf"
output_dir = r"c:\TURBOC3\Projects\pdf_output"
os.makedirs(output_dir, exist_ok=True)

doc = fitz.open(pdf_path)

# Extract all text
with open(os.path.join(output_dir, "full_text.txt"), "w", encoding="utf-8") as f:
    for page_num in range(len(doc)):
        page = doc[page_num]
        text = page.get_text("text")
        f.write(f"\n{'='*80}\n")
        f.write(f"PAGE {page_num + 1}\n")
        f.write(f"{'='*80}\n")
        f.write(text)

# Extract all images
img_count = 0
for page_num in range(len(doc)):
    page = doc[page_num]
    images = page.get_images(full=True)
    for img_idx, img in enumerate(images):
        xref = img[0]
        base_image = doc.extract_image(xref)
        image_bytes = base_image["image"]
        ext = base_image["ext"]
        img_count += 1
        img_filename = f"page{page_num+1}_img{img_idx+1}.{ext}"
        with open(os.path.join(output_dir, img_filename), "wb") as f:
            f.write(image_bytes)
        print(f"Saved: {img_filename} ({base_image['width']}x{base_image['height']})")

# Also render each page as an image for reference
for page_num in range(len(doc)):
    page = doc[page_num]
    pix = page.get_pixmap(dpi=200)
    pix.save(os.path.join(output_dir, f"page_{page_num+1}_render.png"))
    print(f"Rendered page {page_num+1}")

print(f"\nTotal images extracted: {img_count}")
print(f"Total pages: {len(doc)}")
doc.close()
