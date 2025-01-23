# Go Fish Game
FMI | Course project 

## Overview
This project implements a simple console-based version of the classic card game "Go Fish!" It consists of two stages: 
1. Collecting sets by asking for cards from the opponent.
2. Guessing sets already completed by the opponent.

Player competes against the computer to collect all card sets (four cards of the same rank).

## Features
- Card dealing and deck shuffling
- Turn-based gameplay with user input validation
- Basic AI for the computer opponent
- Two-stage gameplay with dynamic interactions
- Win/loss detection based on sets collected

## How to Play
1. The game starts by dealing 6 cards to the player and the computer.
2. Players take turns asking for cards of a specific rank from the opponent.
3. If the opponent has the requested cards, they are transferred; otherwise, the player draws from the deck.
4. Completed sets are automatically removed from hands.
5. Stage 2 begins when all sets are formed. Players take turns guessing the opponent's sets.
6. The first player to collect all sets wins!
