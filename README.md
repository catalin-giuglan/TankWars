TankWars - Joc 2D cu tancuri

Descriere

TankWars este un joc 2D de luptă între tancuri, implementat în C++ folosind OpenGL. Jocul implică doi jucători care controlează tancuri poziționate pe un teren generat procedural și încearcă să-și doboare adversarul prin tragerea de proiectile. Terenul este dinamic și se poate modifica la impactul proiectilelor, creând cratere care pot influența strategia jocului. Fiecare tanc are o bară de viață care scade la impact, iar jucătorul care își pierde toată viața este eliminat. Jocul oferă o experiență competitivă și necesită o combinație de precizie și strategie pentru a învinge adversarul.

Gameplay și controale

Tancul Jucătorului 1:

A/D - Deplasare

W/S - Reglarea unghiului tunului

SPACE - Tragere cu proiectil

R - Resetare viață

Tancul Jucătorului 2:

Săgeți Stânga/Dreapta - Deplasare

Săgeți Sus/Jos - Reglarea unghiului tunului

ENTER - Tragere cu proiectil

CTRL Dreapta - Resetare viață

Arhitectura codului

lab3.cpp și lab3.h: Implementarea principală a jocului, inclusiv terenul, logica proiectilelor și coliziunile.

lab3_vis2D.cpp și lab3_vis2D.h: Funcții pentru randare 2D și viewport.

object2D.h: Funcții pentru generarea obiectelor grafice (tancuri, proiectile, teren etc.).

Funcționalități implementate

✅ Generarea terenului cu funcții sinusoidale pentru un mediu dinamic
✅ Simulare gravitațională pentru proiectile, influențând traiectoria acestora
✅ Coliziuni între proiectile și tancuri, determinând scăderea vieții
✅ Modificarea terenului la impactul proiectilelor, creând cratere
✅ Bară de viață pentru fiecare tanc, vizibilă în timpul jocului
✅ Control simultan pentru doi jucători, oferind o experiență competitivă
✅ Traiectorie previzibilă a proiectilelor pentru un gameplay strategic
✅ Posibilitatea de resetare a vieții pentru fiecare jucător
