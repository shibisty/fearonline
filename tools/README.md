# Static-analysis helpers (2026-11-03)

Python 3 scripts used for the offline analysis of GameClient.dll / Engine.exe
(no Ghidra session needed). Require `pip install capstone pefile`.

- `dx.py` -- explorer for the bulk Ghidra exports in `trash/decompiles/`:
  `python dx.py fn FUN_10151060` (dump a function), `callers FUN_x`,
  `grepl REGEX [max]` (grep with enclosing-function names), `-z` = ZNetwork decompile.
- `pedis.py` -- `info <pe>`, `thunk <pe> <va>...` (resolve JMP thunks such as
  the LAB_xxxx callback thunks), `dis <pe> <va> [n]` (capstone disassembly).
- `xref.py` -- `<pe> <string|0xVA>`: find a string and the code that references it.
- `findbase.py` -- `<GameClient.dll> <znetwork_proxy_log.txt>`: derives the
  runtime base of GameClient.dll from the `StoreCallback:` pointers in the log
  and prints every callback -> real handler address (Ghidra addresses).
- `callback_map_2026-10-run.txt` -- output of findbase.py for the October run
  (runtime base 0x0B610000).
