.global a
.global b
.global c
.global d

.section .data
# Vrednost simbola, definisanog pomocu labele u okviru asemblerske naredbe,
# jednaka je adresi memorijske lokacije za koju je vezan dati simbol.
#
# Vrednost simbola treba jasno razlikovati od 
# vrednosti koja se nalazi u memoriji na adresi ukazanoj vrednoscu samog simbola.
#
# Svaki od naredna cetiri *simbola* (a, b, c, d) ima identicnu vrednost
# zato sto se izmedju njihovih definicija ne nalaze
# asemblerske direktive niti asemblerske naredbe koje *generisu sadrzaj*
# (usled cega su ova cetiri simbola vezana za istu memorijsku lokaciju).
a:
b:
c:
d:
.4byte 9    # 00 00 00 09
.int   8    # 00 00 00 08
.long  7
.4byte 6
.int   5
.long  4
.4byte 3
.int   2
.long  1
.4byte 0

.end
