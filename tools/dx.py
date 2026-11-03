#!/usr/bin/env python3
"""Decompile explorer: index '/* Function: NAME */' markers, extract functions, find callers."""
import re, sys, bisect, os, pickle
DEC = "/mnt/user-data/uploads/fearonline/trash/decompiles/decompiled_gameclient.c"
ZN  = "/mnt/user-data/uploads/fearonline/trash/decompiles/decompiled_znetwork.c"
CACHE = os.path.join(os.path.dirname(__file__), "idx_%s.pkl")

def load(path):
    key = os.path.basename(path)
    cp = CACHE % key
    with open(path, "rb") as f:
        data = f.read().decode("utf-8", "replace")
    if os.path.exists(cp):
        with open(cp, "rb") as f:
            names, poss = pickle.load(f)
    else:
        names, poss = [], []
        for m in re.finditer(r"/\* Function: (\S+) \*/", data):
            names.append(m.group(1)); poss.append(m.start())
        with open(cp, "wb") as f:
            pickle.dump((names, poss), f)
    return data, names, poss

def func(data, names, poss, name):
    out = []
    for i, n in enumerate(names):
        if n == name:
            end = poss[i+1] if i+1 < len(poss) else len(data)
            out.append(data[poss[i]:end])
    return out

def enclosing(data, names, poss, charpos):
    i = bisect.bisect_right(poss, charpos) - 1
    return names[i] if i >= 0 else None

def callers(data, names, poss, name, limit=50):
    res = {}
    for m in re.finditer(re.escape(name) + r"\s*\(", data):
        fn = enclosing(data, names, poss, m.start())
        if fn and fn != name:
            res[fn] = res.get(fn, 0) + 1
    return res

if __name__ == "__main__":
    which = DEC
    args = sys.argv[1:]
    if args and args[0] == "-z":
        which = ZN; args = args[1:]
    data, names, poss = load(which)
    cmd = args[0]
    if cmd == "fn":
        for n in args[1:]:
            fs = func(data, names, poss, n)
            if not fs: print(f"=== {n}: NOT FOUND ===")
            for f in fs:
                print(f"=== {n} ({len(f)} bytes) ===")
                print(f)
    elif cmd == "callers":
        for n in args[1:]:
            c = callers(data, names, poss, n)
            print(f"=== callers of {n}: {len(c)} ===")
            for k, v in sorted(c.items(), key=lambda kv: -kv[1]):
                print(f"  {k} x{v}")
    elif cmd == "grep":
        pat = re.compile(args[1])
        hits = {}
        for m in pat.finditer(data):
            fn = enclosing(data, names, poss, m.start())
            hits.setdefault(fn, []).append(m.group(0)[:120])
        print(f"=== grep /{args[1]}/ : {sum(len(v) for v in hits.values())} hits in {len(hits)} functions ===")
        for k, v in list(hits.items())[:int(args[2]) if len(args) > 2 else 200]:
            print(f"  {k}: {len(v)}  e.g. {v[0]!r}")
    elif cmd == "grepl":
        # grep with line context
        pat = re.compile(args[1])
        n = 0
        for m in pat.finditer(data):
            ls = data.rfind("\n", 0, m.start()) + 1
            le = data.find("\n", m.end())
            fn = enclosing(data, names, poss, m.start())
            print(f"[{fn}] {data[ls:le].strip()[:200]}")
            n += 1
            if n >= (int(args[2]) if len(args) > 2 else 100): break
    elif cmd == "count":
        print(len(names), "functions")

def addrmap(data, names, poss):
    import re as _re
    m = {}
    for i, n in enumerate(names):
        seg = data[poss[i]:poss[i]+200]
        mm = _re.search(r"/\* Address: ([0-9a-fA-F]+) \*/", seg)
        if mm: m[mm.group(1).lower()] = n
    return m

if __name__ == "__main__" and len(sys.argv) > 1 and sys.argv[1] == "addr":
    data, names, poss = load(DEC)
    am = addrmap(data, names, poss)
    for a in sys.argv[2:]:
        print(a, "->", am.get(a.lower().replace("lab_","").replace("0x",""), "NOT FOUND"))
