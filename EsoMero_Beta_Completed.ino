#include <SPI.h>  
#include <SD.h>  // itt könyvtárat hívok be, ami kezeli az sd kártyára/kártyáról történő írást olvasást!


#define HALL_SENSOR_PIN_START 2 // Ebben a két sorban azt mondjuk meg, hogy hol kezdi az alaplapon a tüskéket olvasni, az alaplapon ezek a D2-D7-ig terjedve viselik a neveket.
#define HALL_SENSOR_PIN_END 7
#define FILE_NAME "magnetic_data.txt" // itt a fájlt hozzuk létre az SD kártyán. Ebbe fogjuk rögzíteni az érkező adatokat.

int previousHallValue = LOW; // itt állítunk egy alapértéket, amihez viszonyítani fogjuk az érkező jeleket lentebb.

File dataFile;    // Na ez nem tudom mi lehet, de tippre Format és változó nevet jelent, gondolom egy nem definiált értékű változóként kezeljük hozzuk létre, 
                  // aminek később adunk értéket.

void setup() {
  Serial.begin(9600);     // itt inicializáljuk a soros portot, 9600 bps sebességgel.


  for (int pin = HALL_SENSOR_PIN_START; pin <= HALL_SENSOR_PIN_END; pin++) // ezzel a ciklussal végig megyünk a tüskéken és mindegyiket beállítjuk inputra,
                                                                          // hogy adatot fogadjon az alaplap, a megadott tüskéken.
    {
      pinMode(pin, INPUT);
    }
  
  if (!SD.begin(10)) {                                                // ebben az elágazásban azt mondjuk, hogy kezdje meg a kommunikációt a 10. tüskére helyezett
    Serial.println("Hiba a SD kártya inicializálásakor!");            // SD kártyával, illetve adjon hibaüzenetet ha ez nem megy.
    return;
  }

  dataFile = SD.open(FILE_NAME, FILE_WRITE);                        // itt a változóban FILE_NAME változóban tárolt, adott nevű fájl-t hozzuk létre és írunk fájlba
  if (!dataFile) {                                                   // ha ez nem sikerül, hibát ad vissza a soros monitorra.
    Serial.println("Hiba a fájl megnyitásakor!");
    return;
  }

  Serial.println("Hall-érzékelő és SD kártya inicializálva.");      // ha a fentebbi ciklusnál és elágazásoknál nem lép fel hiba, akkor sikerrel jártunk.
}

void loop() {
  int hallValue = digitalRead(HALL_SENSOR_PIN);                     // itt a hallValue változóban egy beolvasó függvényből tároljuk a kinyert adatokat
                                                                    // egy előre definiált tüskéről
  if (hallValue != previousHallValue) {                             // ebben a feltételben azt vizsgáljuk hogy a mért érték eltér e a kiindulásitól.
    Serial.print("Jel értéke: ");
    Serial.println(hallValue);                                      // És ha igen, mennyivel?
    dataFile = SD.open(FILE_NAME, FILE_WRITE);
    if (dataFile) {                                                 // ebben az elágazásban beleírjuk a fájlunkba és bezárjuk a fájlt az adatok rögzítése után.
      dataFile.println("Mágneses mező változás észlelve!");
      dataFile.print("Jel értéke: ");
      dataFile.println(hallValue);
      dataFile.close();
    } else {                                                        // vagy kimegyünk hibába.
      Serial.println("Hiba a fájl írása közben!");
    }
    previousHallValue = hallValue;                                  // itt egyenlővé tesszük a kiindulási értéket a mért értékkel.
  }                                                                 // A KÉRDÉS: hogy ha az érték nulláról 1-re változik, akkor a következő mérés értéke 1 lesz ugyanúgy
}                                                                   // és nem történik semmi? vagy a jel visszafelé billenéskor -1 lesz, és a különbség így 2-re változik e?
