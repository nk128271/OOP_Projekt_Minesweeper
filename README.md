# Opis
Ovo je klasična igra Minesweeper implementirana u C++ koristeći Qt framework. Cilj igre je otkriti sva sigurna polja na mreži, izbjegavajući mine.

Igra nudi tri razine težine:

- Lako: 10 mina na mreži 10x10
- Teško: 25 mina na mreži 10x10
- Nemoguće: 35 mina na mreži 10x10
# Funkcionalnosti
- Interaktivno sučelje: Dugmad koja reagiraju na lijevi i desni klik.
- Lijevi klik: Otkriva sadržaj ćelije.
- Desni klik: Označava ćeliju zastavicom (🚩).
- Zvukovi:
    - Zvuk eksplozije pri gubitku igre.
    - Zvuk pobjede kada uspješno otkrijete sva sigurna polja.
- Izbor težine: Igrač bira razinu težine prije početka igre.
# Kako igrati?
1. Pri pokretanju igre odaberite razinu težine: Lako, Srednje, ili Teško.
2. Kliknite na ćeliju kako biste je otkrili:
    - Ako je ćelija mina 💣, igra završava.
    - Ako je ćelija sigurna, prikazuje broj mina u susjedstvu ili se automatski otkriva više polja.
3. Desnim klikom označite ćelije za koje mislite da sadrže mine (🚩).
4. Pobjedite kada otkrijete sva sigurna polja.
# Instalacija i pokretanje
1. Preuzmite ili klonirajte repozitorij:
    git clone <URL>
2. Otvorite projekt u Qt Creatoru.
3. Kompajlirajte projekt i pokrenite igru.
