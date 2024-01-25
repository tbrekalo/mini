from minipy import KMer, minimize

TEST_SEQ =  "AACCTTGGACTACGATCGGGGGRACCCCGAACATCTCCTCTCCCATTCTCCCTCCCCTAGAGATTCATTC" \
            "AACCTTGGACTACGATCGGGGGRACCCCGAACATCTCCTCTCCCATTCTCCCTCCCCTAGAGATTCATTC"

KMER_LEN = 29
WIN_LEN = 9

MINMIZERS = [
    KMer(160962658288495797, 6,   True),
    KMer(127849743288636277, 9,   False),
    KMer(120050546527532884, 16,  False),
    KMer(96848788933827933, 25,   False),
    KMer(134169186831835784, 33,  True),
    KMer(63294257164848802, 39,   True),
    KMer(89462680336978896, 43,   False),
    KMer(69620345196203841, 44,   False),
    KMer(132137262280368033, 51,  False),
    KMer(128510159944058648, 56,  False),
    KMer(97068379428228621, 61,   True),
    KMer(96138766261674208, 63,   True),
    KMer(197408025753168864, 67,  True),
    KMer(160962658288495797, 76,  True),
    KMer(127849743288636277, 79,  False),
    KMer(120050546527532884, 86,  False),
    KMer(96848788933827933, 95,   False),
    KMer(134169186831835784, 103, True)
]


def test_minmize():
  minimizers = minimize(TEST_SEQ, KMER_LEN, WIN_LEN)
  
  assert len(minimizers) == len(MINMIZERS)
  for v, e in zip(minimizers, MINMIZERS):
    assert v.value() == e.value()
    assert v.position() == e.position()
    assert v.strand() == e.strand()
