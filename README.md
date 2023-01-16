# mini
Minimizer extraction in python with c++ implementation

## Requirements
- c++17 compliant compiler (clang 7+, gcc 8+)
- python >= 3.8

### Dependencies
- [nanobind](https://github.com/wjakob/nanobind)

### Instalation

Via ssh (recommended):
```bash
pip install git+git://github.com/tbrekalo/mini.git@master
```

Via https (if you do not have GitHub ssh configured):
```
pip install git+https://github.com/tbrekalo/mini.git
```

### Usage
```python
from minipy import KMer, minimize, decode_kmer

EG_SEQ =  "AACCTTGGACTACGATCGGGGGRACCCCGAACATCTCCTCTCCCATTCTCCCTCCCCTAGAGATTCATTC" \
          "AACCTTGGACTACGATCGGGGGRACCCCGAACATCTCCTCTCCCATTCTCCCTCCCCTAGAGATTCATTC"

KMER_LEN = 29
WIN_LEN = 9

minimizers = minimize(EG_SEQ, KMER_LEN, WIN_LEN)
for m in minimizers:
  print(f'{decode_kmer(m, KMER_LEN)}, {m.value()}, {m.position()}, {m.strand()}')
```

