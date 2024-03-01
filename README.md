1.Hvordan du leser master file table-filen (som beskrivet på forelesningsfoilene) fra disk og laster inodene inn i minnet.

- Strukturen på master file table var slik at alle nodene var ordnet etter kronologisk rekkefølge. Dermed kunne vi lese en inode om gangen, se på antall barn og bruke fread til å lese neste node som blir barn av det siste foreldre direktoriet. Fread fungerer på den måten at den husker hvor mange bytes den har lest, slik at neste gang fread kalles kan vi bestemme hvor mange bytes vi leser. I oppgaveteksten var det spesifisert hvor mange bytes som skulle leses for hver av variablene til inode, id var 4 bytes osv. Dersom en node var en mappe og hadde barn så kalte vi fread og leste inn neste barn. I vår load inodes var brukes det også en løkke som sjekker om en inode faktisk er barn av forelder mappen før den legges til under den, men dette var egentlig ikke nødvendig. Altså for antall barn i node kalte vi rekursivt make inode og leste inn de nødvendige bytes fra master table.
 
2.Eventuelle implementasjonskrav som ikke er oppfylt.

- Alle implementasjonskrav er oppfylt.
  

3.Eventuelle deler av implementasjonen som avviker fra prekoden. Dersom du for eksempel oppretter egne filer, forklar

- Vi har kun hjelpemetoder som avviker fra pre koden som f.eks makeIndode.
  

4.Eventuelle tester som feiler og hva du tror årsaken kan være.

- Alle deler passerer testene. 		
