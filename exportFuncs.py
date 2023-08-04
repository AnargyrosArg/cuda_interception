import sys


UUIDS = [
    "0x4ae7f45b6cfbd56b 0xf99dfd1239d98789",
    "0x742e742e8c7994a0 0x660a0c200008f293",
    "0x47cbf623815ad842 0xdcec3a8ae7f69882",
    "0x11df21116e3393c6 0x9395d855f368c3a8",
    "0x4b70e6bd552008d4 0xf2e1663c12ba348d",
]

N_Vals = [
    1,
    1,
    1,
    0,    
    1
]

N_functs = [9,
            6,
            2,
            3,
            2
]


file = open("./include/exportfuncs.h","w");

for export_table in UUIDS:
    
    for function in N_functs:
        file.write("""  
                   
                   
                   
                   """)
        