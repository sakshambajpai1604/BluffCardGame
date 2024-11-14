#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PLAYERS 4
#define DECK_SIZE 52

typedef struct {
    int rank;
    int suit;
} Card;

typedef struct CardNode {
    Card card;
    struct CardNode *next;
} CardNode;

const char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
const char *ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};

void initializeDeck(Card *deck) {
    for (int i = 0, suit = 0; suit < 4; suit++)
        for (int rank = 2; rank <= 14; rank++, i++) {
            deck[i].rank = rank;
            deck[i].suit = suit;
        }
    srand(time(NULL));
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void addCardToHand(CardNode **hand, Card card) {
    CardNode *newNode = (CardNode *)malloc(sizeof(CardNode));
    newNode->card = card;
    newNode->next = *hand;
    *hand = newNode;
}

Card removeCard(CardNode **hand) {
    CardNode *temp = *hand;
    Card card = temp->card;
    *hand = temp->next;
    free(temp);
    return card;
}

void addCardToPile(CardNode **pile, Card card) {
    CardNode *newNode = (CardNode *)malloc(sizeof(CardNode));
    newNode->card = card;
    newNode->next = NULL;
    if (*pile == NULL) {
        *pile = newNode;
    } else {
        CardNode *current = *pile;
        while (current->next) current = current->next;
        current->next = newNode;
    }
}

void displayCard(Card card) {
    printf("%s of %s", ranks[card.rank - 2], suits[card.suit]);
}

void displayHand(CardNode *hand) {
    while (hand) {
        displayCard(hand->card);
        printf(" | ");
        hand = hand->next;
    }
    printf("\n");
}

int isBluff(CardNode *hand, int declaredRank) {
    for (CardNode *current = hand; current; current = current->next)
        if (current->card.rank == declaredRank) return 0;
    return 1;
}

void collectPile(CardNode **hand, CardNode **pile) {
    printf("Collecting cards from the pile: ");
    while (*pile) {
        displayCard((*pile)->card);
        printf(" | ");
        CardNode *temp = *pile;
        *pile = (*pile)->next;
        addCardToHand(hand, temp->card);
        free(temp);
    }
    printf("\n");
}

int remainingPlayersWithCards(CardNode *playerHands[]) {
    int count = 0;
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (playerHands[i] != NULL) count++;
    }
    return count == 1;
}

int main() {
    Card deck[DECK_SIZE];
    CardNode *playerHands[NUM_PLAYERS] = {NULL};
    CardNode *pile = NULL;
    int playerTurn = 0;
    char callBluff;

    initializeDeck(deck);

    for (int i = 0; i < DECK_SIZE; i++)
        addCardToHand(&playerHands[i % NUM_PLAYERS], deck[i]);

    printf("Initial hands:\n");
    for (int i = 0; i < NUM_PLAYERS; i++) {
        printf("Player %d's hand: ", i);
        displayHand(playerHands[i]);
    }

    while (!remainingPlayersWithCards(playerHands)) {
        if (playerHands[playerTurn] == NULL) { 
            playerTurn = (playerTurn + 1) % NUM_PLAYERS;
            continue;
        }

        printf("\n--- Player %d's Turn ---\n", playerTurn);
        printf("Player %d's hand before turn: ", playerTurn);
        displayHand(playerHands[playerTurn]);

        Card playedCard = removeCard(&playerHands[playerTurn]);
        addCardToPile(&pile, playedCard);

        int declaredRank;
        printf("Player %d, enter the rank you declare (2-14): ", playerTurn);
        scanf("%d", &declaredRank);
        printf("Player %d declares a rank of %s.\n", playerTurn, ranks[declaredRank - 2]);

        printf("Player %d, do you want to call bluff? (y/n): ", (playerTurn + 1) % NUM_PLAYERS);
        scanf(" %c", &callBluff);

        if (callBluff == 'y' || callBluff == 'Y') {
            if (isBluff(playerHands[playerTurn], declaredRank)) {
                printf("Bluff detected! Player %d was lying and picks up all cards in the pile.\n", playerTurn);
                collectPile(&playerHands[playerTurn], &pile);
            } else {
                printf("No bluff detected. Player %d picks up all cards in the pile.\n", (playerTurn + 1) % NUM_PLAYERS);
                collectPile(&playerHands[(playerTurn + 1) % NUM_PLAYERS], &pile);
            }
        }

        printf("Player %d's hand after turn: ", playerTurn);
        displayHand(playerHands[playerTurn]);

        if (remainingPlayersWithCards(playerHands)) {
            for (int i = 0; i < NUM_PLAYERS; i++) {
                if (playerHands[i] != NULL) {
                    printf("Player %d wins the game!\n", i);
                    break;
                }
            }
            break;
        }

        playerTurn = (playerTurn + 1) % NUM_PLAYERS;
    }

    return 0;
}
