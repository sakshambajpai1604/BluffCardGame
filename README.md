# Bluff Card Game

This is a simple card game called **Bluff** implemented in C. It allows multiple players to play a game of Bluff, where players try to deceive each other by declaring false card ranks.

## Game Rules
- Each player is dealt a hand of cards.
- Players take turns to play cards from their hand and declare a rank.
- The next player can choose to call a bluff if they believe the rank is false.
- If a bluff is called correctly, the bluffing player picks up all cards in the pile. If not, the accuser picks up the cards.

## Features
- Linked list data structure to manage player hands and pile.
- Functions to shuffle and deal cards, display hands, check for bluffs, and handle turns.
- Display of cards in a human-readable format.

## Getting Started

### Prerequisites
- C compiler (e.g., GCC)

### Compilation
Compile the code using a C compiler:

```bash
gcc bluff_card_game.c -o bluff_card_game
