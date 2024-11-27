#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#define TOTAL_CARDS 52   // Total number of cards in the deck
int MAX_PLAYERS;        // Number of players

typedef struct Card {
    int id;
    const char* myTYPE;
    const char* mylet;
} Card;

typedef struct Player {
    bool isturn;
    int id;
    char name[100];
    Card mycards[TOTAL_CARDS];  // Array to store the player's cards
    int cardCount;              // To store how many cards the player has
} Player;

const char* suitNames[] = { "HEARTS", "CLUBS", "DIAMONDS", "SPADES" };
const char* rankNames[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

Card allCards[TOTAL_CARDS];
Card tableCards[];
int move=0;
// Function to create a standard deck of cards
void create_cards() {
    for (size_t i = 0; i < TOTAL_CARDS; i++) {
        allCards[i].id = i;
        allCards[i].myTYPE = suitNames[i / 13]; // Assign suit
        allCards[i].mylet = rankNames[i % 13];  // Assign rank
    }
}

// Fisher-Yates shuffle algorithm
void shuffle_deck(Card* deck, int size) {
    srand(time(NULL)); // Seed the random number generator

    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Random index between 0 and i
        // Swap cards
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// Function to display the deck of cards
void show_cards(Card* cards, int size) {
    for (int i = 0; i < size; i++) {
        printf("Card ID: %d, Suit: %s, Rank: %s\n", cards[i].id, cards[i].myTYPE, cards[i].mylet);
    }
}

// Function to distribute cards among players in a round-robin fashion
void distribute_cards(Card* deck, int deckSize, Player players[], int numPlayers) {
    // Initialize card counts for each player
    for (int i = 0; i < numPlayers; i++) {
        players[i].cardCount = 0; // Start with 0 cards
    }

    // Distribute cards among players in a round-robin manner
    for (int i = 0; i < deckSize; i++) {
        int playerIndex = i % numPlayers; // Cycle through players
        int cardIndex = players[playerIndex].cardCount; // Current card index for the player

        players[playerIndex].mycards[cardIndex] = deck[i]; // Assign card to player
        players[playerIndex].cardCount++; // Increment player's card count
    }
}

// Function to show each player's cards
void show_player_cards(Player players[], int numPlayers) {
    for (int player = 0; player < numPlayers; player++) {
        printf("Player %d's Cards (%d cards):\n", player + 1, players[player].cardCount);
        for (int card = 0; card < players[player].cardCount; card++) {
            printf("  Suit: %s, Rank: %s\n", players[player].mycards[card].myTYPE, players[player].mycards[card].mylet);
        }
        printf("\n");
    }
}

// Function to get a move from the player (i.e., which card they want to play)
int movePlayer(Player* player) {
    char rank[3], suit[10];
    printf("%s, it's your turn to play!\n", player->name);

    // Show the player's cards
    printf("Your cards:\n");
    for (int i = 0; i < player->cardCount; i++) {
        printf("  %d. Suit: %s, Rank: %s\n", i + 1, player->mycards[i].myTYPE, player->mycards[i].mylet);
    }

    printf("Enter the rank and suit of the card you want to play (e.g., 5 HEARTS): ");
    scanf("%s %s", rank, suit);

    // Search for the card with the given rank and suit
    for (int i = 0; i < player->cardCount; i++) {
        if (strcmp(player->mycards[i].mylet, rank) == 0 && strcmp(player->mycards[i].myTYPE, suit) == 0) {
            printf("You played: %s of %s\n", rank, suit);
            return player->mycards[i].id; // Return the card id
        }
    }

    printf("Card not found! Please try again.\n");
    return -1; // Invalid move
}

int firstmove(Player *player[], int tn) {
    for (size_t i = 0; i < tn; i++) {
        Player *p = player[i];
        for (size_t j = 0; j < p->cardCount; j++) {
            if (strcmp(p->mycards[j].myTYPE, "SPADES") == 0 && strcmp(p->mycards[j].mylet, "A") == 0) {
                p->isturn = true;
                return i;
            }
        }
    }
}

int rankToValue(const char* rank) {
    if (strcmp(rank, "A") == 0) return 14;
    if (strcmp(rank, "K") == 0) return 13;
    if (strcmp(rank, "Q") == 0) return 12;
    if (strcmp(rank, "J") == 0) return 11;
    if (strcmp(rank, "10") == 0) return 10;
    if (strcmp(rank, "9") == 0) return 9;
    if (strcmp(rank, "8") == 0) return 8;
    if (strcmp(rank, "7") == 0) return 7;
    if (strcmp(rank, "6") == 0) return 6;
    if (strcmp(rank, "5") == 0) return 5;
    if (strcmp(rank, "4") == 0) return 4;
    if (strcmp(rank, "3") == 0) return 3;
    if (strcmp(rank, "2") == 0) return 2;
    return 1;  // "1" for Ace in case it's used in your deck
}

int checkPower() {
    int large = 0;  // Variable to store the id of the largest card

    // Compare each card with the next one
    for (size_t i = 0; i < move - 1; i++) {  // Loop until the second last card
        int rank1 = rankToValue(tableCards[i].mylet);
        int rank2 = rankToValue(tableCards[i + 1].mylet);

        if (rank1 > rank2) {
            large = tableCards[i].id;  // If the current card is greater, store its id
        } else {
            large = tableCards[i + 1].id;  // Else store the next card's id
        }
    }

    printf("The largest card id is: %d\n", large);
    return large;
}

void checkwin()
{
    while (move!=0)
    {
        if(tableCards[move].myTYPE != tableCards[0].myTYPE)
        {
            int largecard = checkPower();
        }
    }
    
}



void playRound(Player *players, int numPlayers, int *currentPlayerIndex) {
    Card playedCards[MAX_PLAYERS];
    int highestValue = -1;
    int losingPlayerIndex = -1;

    printf("\nRound\n");
    for (int i = 0; i < numPlayers; i++) {
        Player *currentPlayer = &players[*currentPlayerIndex];
        printf("%s's turn: ", currentPlayer->name);

        int playedCardIndex = movePlayer(currentPlayer);;
        
            

        Card playedCard = currentPlayer->mycards[playedCardIndex];
        printf("Plays %s of %s\n", playedCard.mylet, playedCard.myTYPE);

        // Remove card from player's hand
        for (int j = playedCardIndex; j < currentPlayer->mycards - 1; j++) {
            currentPlayer->mycards[j] = currentPlayer->mycards[j + 1];
        }
        currentPlayer->cardCount--;

        playedCards[i] = playedCard;

        // Track highest card in the current suit
        if (strcmp(playedCard.) == 0 && playedCard.value > highestValue) {
            highestggValue = playedCard.value;
            losingPlayerIndex = *currentPlayerIndex;
        }

        // Move to next player
        *currentPlayerIndex = (*currentPlayerIndex + 1) % numPlayers;
    }

    // Losing player collects all cards
    printf("\n%s collects the cards!\n", players[losingPlayerIndex].name);
    for (int i = 0; i < numPlayers; i++) {
        players[losingPlayerIndex].mycards[players[losingPlayerIndex].cardCount++] = playedCards[i];
    }

    // Losing player starts the next round
    *currentPlayerIndex = losingPlayerIndex;
}

int main() {
    printf("Enter number of players: ");
    scanf("%d", &MAX_PLAYERS);
    Card delCards[52];
    tableCards[MAX_PLAYERS];
    // Validate MAX_PLAYERS
    if (MAX_PLAYERS < 1 || MAX_PLAYERS > TOTAL_CARDS) {
        printf("Invalid number of players! Please enter a valid number (1 - %d players).\n", TOTAL_CARDS);
        return 1;
    }

    // Create deck of cards
    create_cards();

    // Create an array of players
    Player players[MAX_PLAYERS];
    for (int i = 0; i < MAX_PLAYERS; i++) {
        printf("Enter the name of Player %d: ", i + 1);
        scanf("%s", players[i].name);
        players[i].id = i + 1;  // Set player ID
    }

    printf("Original Deck:\n");
    //show_cards(allCards, TOTAL_CARDS);

    // Shuffle deck
    shuffle_deck(allCards, TOTAL_CARDS);

    printf("\nShuffled Deck:\n");
    //show_cards(allCards, TOTAL_CARDS);

    // Distribute cards to players
    distribute_cards(allCards, TOTAL_CARDS, players, MAX_PLAYERS);

    // Show each player's cards
    printf("\nDistributed Cards:\n");
    //show_player_cards(players, MAX_PLAYERS);

    // Example move (can be repeated for each player)
    // for (int i = 0; i < MAX_PLAYERS; i++) {
    //     int cardId = movePlayer(&players[i]);
    //     if (cardId != -1) {
    //         printf("Player %s played card ID: %d\n", players[i].name, cardId);
    //     }
    // }
    //logic(players,MAX_PLAYERS);
    
    int currentPlayerIndex = firstmove(players,MAX_PLAYERS);
    while (1) {
        playRound(players, MAX_PLAYERS, &currentPlayerIndex, roundSuit);

        if (checkWinner(players, MAX_PLAYERS)) {
            printf("\nGame Over! Kazhutha: %s\n", players[currentPlayerIndex].name);
            break;
        }

        // Example of switching the round suit (you can implement better logic)
        strcpy(roundSuit, (strcmp(roundSuit, "Spades") == 0) ? "Diamonds" : "Spades");
    }

    return 0;
}
