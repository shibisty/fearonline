# CSV table stubs for FEAR Online (Game/CSV folder)

Reconstructed from decompiled loader functions in `GameClient.dll` (Ghidra).
Every loader follows the same pattern: open `./CSV/<name>.csv`, register column
names via `thunk_FUN_10349c80("ColumnName", index)`, then read rows by
**name-based** lookup (`thunk_FUN_10349d60("ColumnName")`) until EOF. Because
lookups are name-based (not positional), exact column *order* in the file is
not required to match the registration order — only the *names* need to be
present as headers for rows that use them. All files here are **headers-only
(zero data rows)**, which is a confirmed-safe path: the reading loop simply
never executes when there are no data rows (same pattern already verified
for the in-game clan-member list).

## Status per file

| File | Schema source | Notes |
|---|---|---|
| `EtcInfo.csv` | **Confirmed**, `FUN_1035ac00` | Matches our earlier independent guess exactly. Includes one test data row pointing at `127.0.0.1:30003`. |
| `OptionPatchList.csv` | **Confirmed**, `FUN_10367220` | |
| `Challenges.csv` | **Confirmed**, `FUN_10351360` | One column (`UnknownColumn7`) has no resolved name — the code references an unnamed string constant (`&DAT_104638e4`) whose text wasn't read. |
| `GameItem.csv` | **Confirmed**, `FUN_1007c710` | Loaded from `./csv/GameItem.csv` (lowercase `csv` — Windows paths are case-insensitive so this shouldn't matter in practice). |
| `CapsuleItem.csv` | **Confirmed**, `FUN_10352e20` | 3 unnamed columns (unresolved `DAT_` string constants). |
| `CapsuleReward.csv` | **Confirmed**, `FUN_10354260` | |
| `ClassInfo.csv` | **Confirmed**, `FUN_10355610` | 1 unnamed column. |
| `Curse.csv` | **Confirmed**, `FUN_10356ef0` | Likely a chat profanity-filter word list. |
| `PartLinkTable.csv` | **Confirmed**, `FUN_10358210` | |
| `CustomPartItem.csv` | **Confirmed**, `FUN_103597c0` | |
| `EventAttendance.csv` | **Confirmed**, `FUN_1035c510` | Column index 1 is skipped in the source (no name registered for it) — omitted here. |
| `FunctionItem.csv` | **Confirmed**, `FUN_1035d9a0` | |
| `MapInfo.csv` | **Confirmed**, `FUN_10361540` | Several unnamed columns (unresolved `DAT_` constants); two index slots (0x16/0x17, 0x19) are skipped entirely in the source. |
| `Material.csv` | **Confirmed**, `FUN_10362eb0` | 2 unnamed columns. |
| `Mission.csv` | **Confirmed**, `FUN_10364550` | |
| `Nickname.csv` | **Confirmed**, `FUN_10365cc0` | Likely a forbidden-nickname word list (same record layout as `Curse.csv`). |
| `Perk.csv` | **Confirmed**, `FUN_103687b0` | |
| `PveAiInfo.csv` | **Confirmed**, `FUN_10369f20` | Column index 0 is skipped in the source — omitted here. |
| `Recipe.csv` | **Confirmed**, `FUN_1036b4c0` | 1 unnamed column. |
| `RewardItem.csv` | **Confirmed**, `FUN_1036cdd0` | 2 unnamed columns. |

**Every file's schema in this pack is now sourced directly from decompiled
code** — none are guesses. `UnknownColumnN` placeholders mark columns whose
name string wasn't captured during decompilation (referenced via an
unresolved `DAT_xxxxxxxx` pointer); they are not needed for these
headers-only files since no data rows reference them.

## Full list of CSV paths referenced in GameClient.dll

```
./CSV/Challenges.csv
./CSV/CapsuleItem.csv
./CSV/CapsuleReward.csv
./CSV/ClassInfo.csv
./CSV/Curse.csv
./CSV/PartLinkTable.csv
./CSV/CustomPartItem.csv
./CSV/EtcInfo.csv
./CSV/EventAttendance.csv
./CSV/FunctionItem.csv
./csv/GameItem.csv          (note: lowercase "csv" folder in this one string)
./CSV/MapInfo.csv
./CSV/Material.csv
./CSV/Mission.csv
./CSV/Nickname.csv
./CSV/OptionPatchList.csv
./CSV/Perk.csv
./CSV/PveAiInfo.csv
./CSV/Recipe.csv
./CSV/RewardItem.csv
```

## Where to put these

```
FEAR_Online\CSV\<filename>.csv
```

## How to test

1. Copy all files from this pack into `CSV\` (overwrite existing `EtcInfo.csv`
   / `OptionPatchList.csv` if you already customized them, or merge manually).
2. Launch the game, reproduce the previous crash (after pressing Login).
3. If the crash disappears or moves to a new location, one (or more) of
   these missing files was part of the cause. Narrow it down by adding files
   back one at a time if needed.
4. If behavior is unchanged, these tables are likely unrelated to the
   current crash, and direct tracing via x32dbg/Ghidra remains the next step.
