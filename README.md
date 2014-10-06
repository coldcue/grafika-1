# A házi feladat leírása

Az alábbiakban olvashatod a Számítógépes Grafika tárgy kiadott házi feladatait.

## Első feladat

Készítsen „Síkon táncoló kontrollpontok” programot. A felhasználó az egér balgomb egyszeri lenyomással/elengedéssel veszi fel a kontrollpontokat *(max 10-et)*, amelyekhez 2cm sugarú kisebb fekete köröket rendelünk. Ha a kontrollpontok száma legalább kettő, azokra egy türkiszkék színű, kitöltött konvex burkot, piros **Bézier görbét**, nulla kezdő és végsebességű, a kontrollpont lehelyezésének idejét paraméterként használó zöld **Catmull-Rom spline**-t és kék **Catmull-Clark görbét** illeszt.

A háttér világosszürke. Legnagyobb prioritása a kontrollpontoknak van, majd a görbék jönnek, végül jön a konvex burok.

Space lenyomására a kontrollpontok egy-egy 5 cm-es kör tetejéről elindulva, 5 másodpercenként egy teljes fordulatot téve, elkezdenek keringeni, mégpedig a páros indexűek az óramutató járásával megegyező, a páratlan indexűek pedig azzal ellentétes irányban. A konvex burok és a görbék követik a kontrollpontokat. Mindezt a felhasználó egy 58cm x 68 cm-es kameraablakon keresztül látja. Ha a felhasználó az egér jobb gombbal rábök egy kontrollpontra, akkor a kameraablakot ehhez köti, a kontrollpont elmozdulása automatikusan a kameraablakot is arrébb viszi.

*Beadási határidő: 2014. 10. 19. 23:59*

## A megoldás módja

A feladatot **C++** nyelven kell megvalósítani **OpenGL** és **GLUT** felhasználásával az alábbi sablon módosításával. A feladat megoldása során implementálni kell az `onInitialization()`, `onDisplay()`, `onKeyboard()`, `onMouse()`, `onMouseMotion()` és `onIdle()` függvényeket. Amennyiben a feladat megköveteli természetesen létrehozhatsz segédfüggvényeket is. Fontos azonban, hogy csak a jelzett részen belül található programrészeket értékeljük.

## A forráskód feltöltése

Az elkészült forráskód feltöltésére a Kódfeltöltés menüpont alatt van lehetőséged. A házi feladat határidejének lejártáig tetszőleges alkalommal tölthetsz fel megoldás, értékelni az utolsó változatot fogjuk.

A fordítási és futási eredmények ellenőrzése

A fordítás sikerességét az Eredmények menüpont alatt tudod ellenőrizni.

## Néhány tanács

A programokat GNU g++ 4.4.1 verziószámú fordítóprogrammal fordítjuk, mely szigorúbban követi a C++ szabványt mint a Visual Studio különböző változatai, ezért előfordulhat, hogy sikertelen lesz a fordítás annak ellenére, hogy a Visual Studio fordítójával lefordul. A pontos hibáról az Eredmények menüpont alatt tájékozódhatsz.

A feladatokat **ISO8859-1** vagy **ISO8859-2** formátumban várjuk, az **UTF-16** és **UNICODE** formátumú programok hibás fordítást okoznak.

A sablonban szereplő elválasztó sorokat (`//~~~~~~`...) célszerű a beküldött programban is megőrizni. Ellenkező esetben előfeldolgozási hiba lehet, bár a gyakoribb esetekre a parsert felkészítettük.

### Gyakori hibák

* Gyakori hiba konstans argumentumot átadni referenciát váró függvénynek. Pl. `void fv(int& a)` függvényt `void fv(2)`-vel hívni. Ez nem okoz hibát Visual Studio alatt az alap beállításokkal, de a szabvány szerint hibás.
* A tipikus **C++** hibákról jó összefoglalót találhatsz ezen az oldalon.
* Az **OpenGL** specifikáció nem rendelkezik a **ModelView** és **Projection** mátrixok alapértelmezett értékeiről. Ezek a program indulásakor tetszőlegesek lehetnek. Több beküldött feladatnál tapasztaltuk, hogy hibásan feltételezte az egység mátrixot. A kiadott forráskód-sablonban ezért inicializáljuk a mátrixokat a futás kezdetén.
* Több beküldött megoldásban tapasztaltuk az előfordítói `#define` direktívával definiált makrók hibás használatát. Semmi sem garantálja, hogy más rendszereken vagy akár csak más **GLUT** verzióban a numerikus konstansok megegyeznek, ezért hibás programozói gyakorlat ezen numerikus konstansok használata. Helyettük az előfordítói makrókat kell használni.
* Az `onIdle()` függvény akkor hívódik amikor semilyen esemény nem érkezik az rendszertől. Ennek megfelelően semmi sem garantálja, hogy mikor fog lefutni. Ebből következően itt olyan műveleteket végezni, melyek nélkül a renderelés hibás lesz (pl. a mátrixok beállítása) nem érdemes.
* Nehány hasznos tanács a **GLUT** használatához.
* Csak a sablonban regisztralt callback fuggvenyeket erdemes hasznalni, mivel a teszt rendszerben a tobbi glut fuggveny meghivasa sikertelen lesz.