# mini
Minimizer extraction in python with c++ implementation

## Requirements
- c++14 compliant compiler
- python >= 3.6 

### Dependencies
- [pybind11](https://github.com/pybind/pybind11)

### Instalation
```bash
pip install git+git://github.com/tbrekalo/mini.git@master
```

### Usage
```python
from minipy import KMer, minimize

EG_SEQ =  "AACCTTGGACTACGATCGGGGGRACCCCGAACATCTCCTCTCCCATTCTCCCTCCCCTAGAGATTCATTC" \
          "AACCTTGGACTACGATCGGGGGRACCCCGAACATCTCCTCTCCCATTCTCCCTCCCCTAGAGATTCATTC"

KMER_LEN = 29
WIN_LEN = 9

minimizers = minimize(EG_SEQ, KMER_LEN, WIN_LEN)
for m in minimizers:
  print(f'{m.value()}, {m.position()}, {m.strand()}')
```

