import re, sys, pefile
pe = pefile.PE(sys.argv[1], fast_load=True)
log = open(sys.argv[2], encoding="utf-8", errors="replace").read()
cbs = {m.group(1): int(m.group(2), 16) for m in re.finditer(r"StoreCallback: ([A-Z_0-9]+) -> 0x([0-9A-Fa-f]+)", log)}
ptrs = sorted(set(cbs.values()))
IB = pe.OPTIONAL_HEADER.ImageBase
text = [s for s in pe.sections if s.Name.startswith(b".text")][0]
tlo, thi = IB + text.VirtualAddress, IB + text.VirtualAddress + text.Misc_VirtualSize
def is_thunk(va):
    try:
        b = pe.get_data(va - IB, 5)
    except Exception:
        return False
    return b[0] == 0xE9
best = None
for base in range(0x00010000, 0x7FFF0000, 0x10000):
    ok = 0
    for p in ptrs:
        va = p - base + IB
        if tlo <= va < thi and is_thunk(va): ok += 1
    if ok == len(ptrs):
        print("base candidate 0x%08x : all %d callbacks are E9 thunks" % (base, ok))
        best = base
print("callbacks:", len(ptrs))
if best:
    for n, p in sorted(cbs.items()):
        va = p - best + IB
        b = pe.get_data(va - IB, 5)
        rel = int.from_bytes(b[1:5], "little", signed=True)
        print("  %-40s runtime 0x%08X ghidra 0x%08X -> target 0x%08X" % (n, p, va, va + 5 + rel))
