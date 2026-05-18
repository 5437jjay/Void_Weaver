import fitz
import os

doc = fitz.open("OOP Project-1.pdf")
print(f"Total pages: {len(doc)}")
print("="*80)

for i, page in enumerate(doc):
    print(f"\n{'='*80}")
    print(f"--- PAGE {i+1} ---")
    print(f"{'='*80}")
    text = page.get_text()
    print(text)
    
    # Extract images from each page
    images = page.get_images(full=True)
    print(f"\n[Images on this page: {len(images)}]")
    for img_idx, img in enumerate(images):
        xref = img[0]
        pix = fitz.Pixmap(doc, xref)
        img_filename = f"pdf_page{i+1}_img{img_idx+1}.png"
        if pix.n < 5:
            pix.save(img_filename)
        else:
            pix2 = fitz.Pixmap(fitz.csRGB, pix)
            pix2.save(img_filename)
        print(f"  Saved: {img_filename} ({pix.width}x{pix.height})")

doc.close()
