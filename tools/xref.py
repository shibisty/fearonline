#!/usr/bin/env python3
"""xref.py <pe> <string|0xVA> : find string VA(s) and code refs (imm32) to it."""
import sys, pefile, capstone, struct
pe = pefile.PE(sys.argv[1], fast_load=True)
IB = pe.OPTIONAL_HEADER.ImageBase
img = pe.get_memory_mapped_image()
def find_str(s, wide=False):
    b = s.encode("utf-16le") if wide else s.encode("latin1")
    b = b + (b"\x00\x00" if wide else b"\x00")
    res = []; i = 0
    while True:
        i = img.find(b, i)
        if i < 0: break
        # require preceding NUL (string start)
        if i == 0 or img[i-1] == 0:
            res.append(IB + i)
        i += 1
    return res
def find_refs(va):
    needle = struct.pack("<I", va)
    res = []; i = 0
    text = [s for s in pe.sections if s.Name.startswith(b".text")][0]
    lo, hi = text.VirtualAddress, text.VirtualAddress + text.Misc_VirtualSize
    while True:
        i = img.find(needle, i)
        if i < 0: break
        if lo <= i < hi: res.append(IB + i)
        i += 1
    return res
arg = sys.argv[2]
if arg.startswith("0x"):
    vas = [int(arg, 16)]
else:
    vas = find_str(arg) or find_str(arg, wide=True)
for va in vas:
    refs = find_refs(va)
    print(f"string/target 0x{va:08x}: {len(refs)} code refs: " + " ".join(f"0x{r:08x}" for r in refs[:40]))
