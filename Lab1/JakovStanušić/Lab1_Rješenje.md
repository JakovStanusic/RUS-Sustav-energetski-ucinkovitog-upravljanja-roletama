# 1. Opis sustava
Sustav koristi prekide za obradu ulaznih signala s tipkala i potenciometra te generira vizualne indikacije pomoću LED dioda.

# 2. Funkcionalnosti
## 2.1. Rukovanje prekidima tipkala
### 2.1.1 Sustav mora detektirati pritiske triju tipkala (BUTTON0, BUTTON1, BUTTON2).
U prikazanome kodu se aktivira prekid kada se pritisne tipka, a vrsta prekida je "FALLING".

![image](https://github.com/user-attachments/assets/2ec19c91-23b0-45c1-85ff-7fad1552fa5a)

Kada se aktivira prekid pokreće se proces button0ISR u kojemu se provjerava prekid te se pripremaju varijable potrebne za ostale zadatke.
Prikazani kod je za button0, ali postoji za svaki button ovisno o broju.

![image](https://github.com/user-attachments/assets/ce246a56-fe61-4b90-b363-8117bdf7c7fc)

### 2.1.2 Svakom tipkalu mora biti dodijeljen prioritet:
### BUTTON0 → Visoki prioritet (INT0)
### BUTTON1 → Srednji prioritet (INT1)
### BUTTON2 → Niski prioritet (INT2)
Na prikazanome kodu određeno je koji button ima koji prioritet.
![image](https://github.com/user-attachments/assets/26875156-97af-4754-bc3f-b6bfbfd0e97c)


### 2.1.3 Kada se pritisne tipkalo, odgovarajuća LED (LED_INT0, LED_INT1, LED_INT2) mora titrati 1 sekundu. Debounce mehanizam mora spriječiti višestruko aktiviranje prekida u kratkom vremenu.
U if petlji na prvoj slici možemo vidjeti da se provjerava kada je zadnji prekid bio što mora biti više od 1 sekunde kako bi se spriječilo višestruko aktiviranje.
Varijabla ledtimer0 broji vrijeme koliko je upaljena dioda te nakon 1 sekunde gasi diodu pomoću if-a koji se nalazi na slici ispod.

![image](https://github.com/user-attachments/assets/114361dd-4f61-4dff-b41e-97933ab340c0)

![image](https://github.com/user-attachments/assets/4899ca33-25e8-4350-b1fc-fbe3320ccd1c)

## 2.2. Rukovanje prekidom tajmera
### Tajmer (TIMER1) mora generirati prekid svake 1 sekunde. Pri aktivaciji tajmera, LED_Timer mora svijetliti kratko kako bi signalizirala događaj.
![image](https://github.com/user-attachments/assets/d11d5250-753a-462b-9943-0087ed98dac7)
![image](https://github.com/user-attachments/assets/affbecee-4848-4780-a925-a5e4e5d38651)

Na lijevoj slici možemo vidjeti setup od tajmera koji sadrži svoj flag, varijabla koja zapisuje prije koliko se desila zadnja aktivacija prekida te interval kada želimo da se desi (svakih 1000 ms)
Na desnoj slici nalazi se kontrola za led diodu gdje svaki put kada se dogodi prekid se upali led dioda i ugasi nakon 50 ms.

![image](https://github.com/user-attachments/assets/24b28d26-fc8f-4878-bb35-702785aec2b5)

Ovaj dio koda služi za aktivaciju prekida. Kada se flag postavi na true zapravo se aktivira prekid.

## 2.3. Rukovanje potenciometrom
Na slici je prikazano rukovanje potenciometrom. Kada se na potenciometru prijeđe vrijednost od 500 led dioda se upali.

![image](https://github.com/user-attachments/assets/0a27a30a-e689-41d5-8e08-fce01947bcb6)

## 2.4. Izvršavanje prioriteta prekida
### Ako se istovremeno dogodi više prekida, mora se poštivati sljedeći prioritet:
Tajmer prekid (TIMER1) → INT0 (BUTTON0) → INT1 (BUTTON1) → Potenciometar →  INT2 (BUTTON2)
![image](https://github.com/user-attachments/assets/49530aef-6624-49c7-9553-c2e7c86a13d3)

Na slici se može vidjeti da kada se odradi prekid za button0 ne provjeravaju se ostali prekidi što znači da nije ih moguće izvesti.

## 2.5. Optimizacija performansi
Implementacija mora koristiti bezblokirajuće metode (millis()) umjesto delay(), gdje je moguće.

![image](https://github.com/user-attachments/assets/6ce10403-589a-43e5-857d-a221ff828806)

Kroz cijeli projekt se millis() koristi gdje god je moguće ga zamijeniti kako bi se smanjila potreba za korištenjem delay() funkcije kako bi se smanjilo procesorsko vrijeme koje program koristi.

## 2.6. Realizacija i ispitivanje rada sustava
### Cijeli sustav je napravljen i testiran u sustavu Wokwi. Led diode su glavne komponente koje prikazuju ispravni rad sustava na način da kada se prioritet višega stupnja odradi samo se ta led dioda upali dok druge diode nižeg prioriteta moraju čekati da završi taj prekid.

# Link na projekt: https://wokwi.com/projects/427139978681810945
