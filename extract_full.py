import fitz

doc = fitz.open("OOP Project-1.pdf")
with open("pdf_full_text.txt", "w", encoding="utf-8") as f:
    f.write(f"Total pages: {len(doc)}\n")
    for i, page in enumerate(doc):
        f.write(f"\n{'='*80}\n")
        f.write(f"--- PAGE {i+1} ---\n")
        f.write(f"{'='*80}\n")
        text = page.get_text()
        f.write(text)
        images = page.get_images(full=True)
        f.write(f"\n[Images on this page: {len(images)}]\n")
        for img_idx, img in enumerate(images):
            xref = img[0]
            pix = fitz.Pixmap(doc, xref)
            f.write(f"  Image {img_idx+1}: pdf_page{i+1}_img{img_idx+1}.png ({pix.width}x{pix.height})\n")
doc.close()
print("Done - saved to pdf_full_text.txt")
