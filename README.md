project-wakker
==============

Hoe werken we met git? We werken met een bepaalde workflow genaamd **git flow**.

Het idee is dat je een master branch hebt die altijd gedeployed kan worden. 
Elke keer dat je aan een feature gaat werken maak je een nieuwe branch aan, doe dit dan ook consistent, anders komen we 
in de problemen.
Tijdens het maken van je feature maak je een aantal commits, bijvoorbeeld als je een paar bijbehorende functies hebt geschreven.
Doe dit een keer of vier tijdens het schrijven van een feature. Dit maakt het later makkelijk om te vinden waar bugs vandaan komen.

Quickstart guide:

### Pullen van de repo (locale kopie maken)
```
git pull https://github.com/Azeirah/project-wakker.git project-wekker
```

Hierna kan je het project importeren in lpcexpresso, **let op, sla je locale kopie NIET op in je workspace, sla hem bijvoorbeeld tijdelijk
op in je downloads of op je bureaublad.** Importeer je map hierna in lpcexpresso. Doe je dit niet dan verkracht je van alles.

Om dan de git bash te starten in de goede map: rechtermuis op de project-wekker map in lpcexpresso -> utilities -> open in directory

Vanuit daar kan je je git bash starten.

### branch maken
```
// dit maakt een nieuwe branch aan en switcht er automatisch naar
git checkout -b nieuwe_branch_naam
```

### Switchen van branch
```
git checkout branch_naam
```

Hier staat nog wat diepgaandere info
https://guides.github.com/introduction/flow/index.html
