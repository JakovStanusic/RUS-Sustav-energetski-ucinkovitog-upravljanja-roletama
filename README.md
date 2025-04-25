# RUS--Sustav-energetski-ucinkovitog-upravljanja-roletama
## Opis sustava
Sustav razvijen na Arduino MEGA platformi omogućuje inteligentnu regulaciju roleta temeljenu na stvarnim okolišnim uvjetima:<br>
 - Intenzitetu dnevnog svjetla (mjerenu putem LDR senzora) <br>
 - Temperaturi prostorije (DHT22 senzor) <br>
 - Vremenu dana (RTC modul) <br>
 
Upotrebom se automatizira upravljanje roletama na temelju trenutnih uvjeta okoline  i korisničkih preferencija, s ciljem: <br>
 ✔ Optimizacije potrošnje energije (smanjenje troškova grijanja/hlađenja) <br>
 ✔ Povećanja udobnosti (kontrola prirodnog svjetla i topline)<br>
 ✔ Pametne integracije s kućnom automatizacijom<br>

## Cilj projekta
Cilj je razviti automatizirani, energetski efikasan sustav za upravljanje roletama koji:<br>
 ✔ Smanjuje potrošnju energije kroz optimalnu kontrolu prirodnog svjetla i topline<br>
 ✔ Integrira senzorske podatke (svjetlina, temperatura, vrijeme) za autonomno upravljanje<br>
 ✔ Omogućuje ručnu nadogradnju putem tipki<br>
 ✔ Povećava udobnost u stambenim i poslovnim prostorima<br>

##  Potencijalna korist projekta
- Energetske uštede (smanjenje troškova grijanja/hlađenja) <br>
- Pametna integracija u kućne/uredske sustave <br>
- Ekološki doprinos smanjenjem ugljičnog otiska <br>

##  Postojeća slična rješenja
Na tržištu postoje: <br>
 - Timerom upravljani sustavi (fiksno vrijeme podizanja/spuštanja) <br>
 - Ručni daljinski upravljači (bez adaptacije na okolinu) <br>
 - Skupi pametni sustavi (npr. Somfy, Lutron) <br>
 
 Prednost našeg rješenja: <br> 
  ✔ Jeftinija implementacija (Arduino + osnovni senzori) <br>
  ✔ Pametnija regulacija (trenutni uvjeti > unaprijed programirano vrijeme) <br>
  ✔ Open-source pristup (mogućnost prilagodbe) <br>


##  Skup korisnika
  - Kućanstva – za smanjenje računa za energiju  <br>
  - Poslovni prostori – uredi, škole, bolnice  <br>
  - Gradnja pametnih zgrada – integriranje u sustave pametnih gradova  <br>


##  Mogućnost prilagodbe rješenja
Sustav se može nadograditi:  <br>
  - WiFi/Bluetooth modulom za daljinsko upravljanje preko mobilne aplikacije  <br>
  - Solarnim panelima za autonomno napajanje  <br>
  - Integracijom s Home Assistantom ili drugim pametnim kućnim sustavima  <br>
  - Dodatkom vremenske prognoze (API) za anticipacijsko upravljanje  <br>

## Opseg projektnog zadatka
Uključuje:

Hardversku implementaciju:
 - Arduino MEGA
 - LDR senzor svjetline
 - DHT22 senzor temperature
 - RTC sat (DS1307)
 - Servo motor za pokretanje roleta
 - Tipke za ručno upravljanje

Softverski razvoj:
 - Algoritam za donošenje odluka (svjetlina + temperatura + vrijeme)
 - Upravljanje servo motorom
 - Serijska komunikacija za debugiranje

Testiranje:
 - Reakcija na promjene svjetline/temperature
 - Provjera rada u ručnom i automatskom načinu
 - Analiza uštede energije

## Dijagram aktivnosti rada sustava (UML)
```mermaid
flowchart TD
    A[Inicijaliziranje sustava] --> C{Način rada?}
    
    %% Automatski način
    C -->|Automatski| J[Provjeri tipkalo 3]
    J --> |Stisnuto -> promjena načina rada| C
    J -->|Nije stisnuto tipkalo| D[Očitaj senzore]
    D --> E[Temperature]
    D --> F[Svjetlina]
    D --> G[RTC vrijeme]
    E & F & G --> H[Odredi poziciju]
    H --> I[Pomakni motor]
    I -.-> C
    
    %% Ručni način - lijevo poravnanje
    C -->|Ručni| CC[Čekaj tipkala]
    CC --> DD{Tipkalo 1?}
    CC --> FF{Tipkalo 2?}
    CC --> GG{Tipkalo 3?}
    DD -->|Da| EE[Podigni roletu]
    DD -->|Ne| CC[Čekaj tipkala]
    FF -->|Da| HH[Spusti roletu]
    FF -->|Ne| CC[Čekaj tipkala]
    GG -->|Da, promjena načina rada| C
    GG -->|Ne| CC
    
    %% Povratne veze
    EE & HH --> CC
```
  

## Shema

## Dokumentirani kod
