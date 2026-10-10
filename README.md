# Système de contrôle de climatiseur à distance

Système embarqué permettant de piloter un climatiseur à distance par Wi-Fi.

Projet de conception électronique et programmation embarquée, de septembre à novembre 2024.

## Le problème

Un climatiseur classique ne se commande que par sa télécommande, dans la pièce où il se trouve. L'objectif était de pouvoir l'allumer, l'éteindre et le régler à distance, sans remplacer l'appareil.

## Le principe

Un microcontrôleur ESP32, qui intègre le Wi-Fi, reçoit la commande et actionne un relais. Le relais joue le rôle d'interrupteur commandé électroniquement : il ouvre ou ferme le circuit d'alimentation du climatiseur sans contact direct entre la partie commande et la partie puissance.

## Ce que fait le système

- Réception d'une commande à distance par Wi-Fi
- Commande du climatiseur par relais
- Intégration prévue de capteurs de température, pour un déclenchement automatique selon un seuil

## Validation

Le montage a été simulé et testé sous **Proteus** avant toute réalisation matérielle. C'est la démarche normale en électronique : une erreur de câblage détruit un composant, alors qu'une erreur en simulation ne coûte rien.

## Stack

C++ pour le microcontrôleur, ESP32, relais, Proteus pour la simulation et les tests.

## Ce que ce projet m'a appris

Deux choses. La séparation entre commande et puissance, qui est une règle de sécurité avant d'être un choix technique. Et l'intérêt de la simulation : vérifier avant de brancher évite de remplacer du matériel.
