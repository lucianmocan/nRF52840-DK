# Internet des Objets

Ce dépôt contient les programmes et les sujets nécessaires pour réaliser les travaux pratiques de l'Unité d'Enseignement [Internet des Objets](https://mathinfo.unistra.fr/formations/master/master-informatique/odf-parcours-science-et-ingenierie-des-reseaux-de-linternet-et-des-systemes-siris-PR12-18105/odf-cours-EN1097-18105-PR12/) du Master 2 informatique, parcours SIRIS, de l'Université de Strasbourg.
Les travaux pratiques reposent sur le système d'exploitation [Contiki-NG](https://www.contiki-ng.org/) et des cartes embarquées [nRF52840](https://www.nordicsemi.com/Products/Development-hardware/nRF52840-DK).
La compilation et l'envoi de micrologiciels sur les cartes nécessitent des outils spécifiques qui sont détaillés ci-dessous.

Certains des exercices ont été proposés ou sont inspirés d'exercices rédigés par Maxime Bucher.

## Pré-requis

Dans un premier temps, installez les paquets suivants :
```sh
build-essential doxygen git git-lfs curl wireshark python3-serial srecord rlwrap
```

Ensuite, clonez ce dépôt puis installez sur votre poste le paquet `nrf-command-line-tools_10.24.2_amd64.deb` qui se trouve à la racine du dépôt.

Enfin, installez le compilateur pour ARM :
- décompressez l'archive `gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2` se trouvant à la racine du dépôt.
- ajoutez le cheming vers le répertoire `bin` de l'archive dans votre variable `PATH`.

## Test préliminaire

Vérifiez que votre environnement de développement est fonctionnel :
- connectez la carte à votre poste via USB (la prise se trouvant sur le bord court de la carte) et allumez cette dernière. Une diode (L5) devrait s'allumer à côté du MCU.
- compilez le programme `hello_world` et envoyez le micrologiciel correspondant sur la carte :
```sh
cd contiki-ng/examples/hello_world
make TARGET=nrf52840 hello-world.upload
```
- connectez-vous sur la carte pour vous assurez que le message `"Hello, world"` est affiché toutes les 10 secondes.
```sh
make login
```

Félicitation, votre environnement est fonctionnel !

## Dongle NRF52840

Pour flasher des programmes pour le dongle nrf52840 il faut :
- installer l'outil `nrfutil` disponible sur la [page du constructeur](https://www.nordicsemi.com/Products/Development-tools/nrf-util)
- placez l'exécutable `nrfutil `dans le répertoire `/usr/local/bin`
- installer les modules supplémentaires via
```sh
nrfutil install nrf5sdk-tools device
```

Pour compiler un programme pour le dongle, il faut depuis le répertoire source du programme :
```sh
make TARGET=nrf BOARD=nrf52840/dongle
```

Pour flasher le dongle, il faut le mettre en mode DFU (*Device Firmware Update*) en appuyant sur le boutton RESET jusqu'à ce que la led soit rouge. Vous pouvez vous référer à la [documentation du constructeur](https://academy.nordicsemi.com/flash-instructions-for-nrf52840-dongle) pour identifier le bouton sur la carte.

Ensuite, vous pouvez entrer la commande :
```sh
make TARGET=nrf BOARD=nrf52840/dongle nom-du-firmware.dfu-upload
```

Si vous rencontrez un problème de droits sur le périphérique, ajoutez simplement le droit d'écriture sur ce dernier pour l'utilisateur courant.
## Documentation

La documentation pour Contiki-ng est disponible [ici](https://docs.contiki-ng.org/en/develop/_api/index.html).

La documentation de la carte nRF52840 est disponible [ici](https://docs.nordicsemi.com/bundle/ug_nrf52840_dk/page/UG/dk/intro.html).
