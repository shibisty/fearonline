#!/usr/bin/env python3
"""Tiny PE disassembler helper: dis.py <pe> <va> [count] | thunk <pe> <va>... | info <pe>"""
import sys, pefile, capstone
def load(path):
    pe = pefile.PE(path, fast_load=True)
    return pe
def va2off(pe, va):
    rva = va - pe.OPTIONAL_HEADER.ImageBase
    return pe.get_offset_from_rva(rva)
def read(pe, va, n):
    return pe.get_data(va - pe.OPTIONAL_HEADER.ImageBase, n)
def dis(pe, va, count=30):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    code = read(pe, va, count*8)
    out = []
    for i, ins in enumerate(md.disasm(code, va)):
        out.append(f"{ins.address:08x}  {ins.bytes.hex():<16} {ins.mnemonic} {ins.op_str}")
        if i+1 >= count: break
    return out
def thunk(pe, va):
    b = read(pe, va, 5)
    if b[0] == 0xE9:
        rel = int.from_bytes(b[1:5], "little", signed=True)
        return va + 5 + rel
    return None
if __name__ == "__main__":
    cmd = sys.argv[1]
    pe = load(sys.argv[2])
    if cmd == "info":
        print("ImageBase 0x%x" % pe.OPTIONAL_HEADER.ImageBase, "EntryPoint 0x%x" % pe.OPTIONAL_HEADER.AddressOfEntryPoint)
        print("DllCharacteristics 0x%x" % pe.OPTIONAL_HEADER.DllCharacteristics, "(DYNAMIC_BASE=%s)" % bool(pe.OPTIONAL_HEADER.DllCharacteristics & 0x40))
        for s in pe.sections: print(s.Name.decode(errors='replace').strip('\x00'), "VA 0x%x" % (pe.OPTIONAL_HEADER.ImageBase+s.VirtualAddress), "size 0x%x" % s.Misc_VirtualSize)
    elif cmd == "thunk":
        for a in sys.argv[3:]:
            va = int(a, 16); t = thunk(pe, va)
            print(f"{va:08x} -> {t:08x}" if t else f"{va:08x} -> (not a JMP rel32: {read(pe,va,8).hex()})")
    elif cmd == "dis":
        va = int(sys.argv[3], 16); n = int(sys.argv[4]) if len(sys.argv) > 4 else 30
        print("\n".join(dis(pe, va, n)))
