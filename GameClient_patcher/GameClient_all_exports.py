# read_exports.py
import pefile
import sys

def read_exports(dll_path):
    pe = pefile.PE(dll_path)
    
    if hasattr(pe, 'DIRECTORY_ENTRY_EXPORT'):
        exports = pe.DIRECTORY_ENTRY_EXPORT.symbols
        print(f"Found {len(exports)} exports in {dll_path}")
        print("=" * 60)
        for exp in exports:
            if exp.name:
                print(f"{exp.name.decode()}")
        print("=" * 60)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python read_exports.py <dll_path>")
        sys.exit(1)
    read_exports(sys.argv[1])
    