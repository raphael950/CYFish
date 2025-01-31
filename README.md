# CY-Fish
Un jeu de stratégie sur une grille hexagonale, où des pingouins se livrent une bataille acharnée pour collecter des poissons avant que la banquise ne fonde.

Rendu final du jeu:


![previw-gif](https://github.com/user-attachments/assets/1e07fe6c-2bf4-4fb2-aeed-2961a93c5de7)

## Installation

> [!IMPORTANT]
> Le jeu fonctionne uniquement sous Linux et MacOS.
> La librairie `ncurses` est nécessaire.

1. Cloner le dépôt :
```
git clone https://github.com/raphael950/CYFish.git
```
2. Se rendre dans le répertoire du jeu :
```
cd CYFish
```
3. Il ne reste plus qu'à le compiler et le lancer :
```
make && ./exec
```

## Règles du jeu

- 2 à 6 joueurs
- Manger un poisson rapporte 1 point
- Un seul déplacement par tour

La partie se termine lorsque plus aucun pingouin ne peut se déplacer.
Le joueur ayant le plus de points l'emporte !




*Jeu réalisé en 2024 par un groupe de 3 étudiants de CY Tech dans le cadre d'un projet de PreING 1*
