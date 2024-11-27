#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#define TOTAL_CARDS 52
#define MAX_PLAYERS 5
int n_players = 0;
typedef struct Card {
    int id;
    int index;
    const char* myTYPE;
    const char* mylet;
    struct Card* link;
} Card;
Card* TOP;
typedef struct Player {
    bool isturn;
    int id;
    
    char name[100];
    Card *MyCards;  // Array to store the player's cards
    int cardCount;              // To store how many cards the player has
} Player;

typedef struct Track
{
    int playerIndex;
    Card playercard;
}Track;


const char* suitNames[] = { "HEARTS", "CLUBS", "DIAMONDS", "SPADES" };
const char* rankNames[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

Card allCards[TOTAL_CARDS];
//Card tableCards[];
int move=0;
void create_cards() {
    for (size_t i = 0; i < TOTAL_CARDS; i++) {
        allCards[i].id = i;
        allCards[i].myTYPE = suitNames[i / 13]; // Assign suit
        allCards[i].mylet = rankNames[i % 13];  // Assign rank
    }
}

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

void show_cards(Card* cards, int size) {
    for (int i = 0; i < size; i++) {
        printf("Card ID: %d, Suit: %s, Rank: %s\n", cards[i].id, cards[i].myTYPE, cards[i].mylet);
    }
}

void distribute_cards(Card* deck, int deckSize, Player *players, int numPlayers) {
    // Initialize card counts for each player
    for (int i = 0; i < numPlayers; i++) {
        players[i].cardCount = 0; // Start with 0 cards
        players[i].MyCards = (Card*)malloc((deckSize/numPlayers)*sizeof(Card));
    }

    // Distribute cards among players in a round-robin manner
    for (int i = 0; i < deckSize; i++) {
        int playerIndex = i % numPlayers; // Cycle through players
        int cardIndex = players[playerIndex].cardCount; // Current card index for the player
        
        players[playerIndex].MyCards[cardIndex] = deck[i]; // Assign card to player
        players[playerIndex].cardCount++; // Increment player's card count
    }
}



void show_player_cards(Player players[], int numPlayers) {
    for (int player = 0; player < numPlayers; player++) {
        printf("Player %d's Cards (%d cards):\n", player + 1, players[player].cardCount);
        for (int card = 0; card < players[player].cardCount; card++) {
            printf("  Suit: %s, Rank: %s\n", players[player].MyCards[card].myTYPE, players[player].MyCards[card].mylet);
        }
        printf("\n");
    }
}

Card movePlayer(Player* player) {
    char rank[3], suit[10];
    printf("%s, it's your turn to play!\n", player->name);
    player->isturn=false;
    // Show the player's cards
    printf("Your cards:\n");
    for (int i = 0; i < player->cardCount; i++) {
        printf("  %d. Suit: %s, Rank: %s\n", i + 1, player->MyCards[i].myTYPE, player->MyCards[i].mylet);
    }
    bool played = false;
    while (played!=true)
    {
        /* code */
    
        printf("Enter the rank and suit of the card you want to play (e.g., 5 HEARTS): ");
        scanf("%s %s", rank, suit);

        // Search for the card with the given rank and suit
        for (int i = 0; i < player->cardCount; i++) {
            if (strcmp(player->MyCards[i].mylet, rank) == 0 && strcmp(player->MyCards[i].myTYPE, suit) == 0) {
                printf("You played: %s of %s\n", rank, suit);
                player->MyCards[i].index = i;
                played = true;
                return player->MyCards[i]; // Return the card id
            }
        }

        printf("Card not found! Please try again.\n");
    }
 // Invalid move
}

int firstmove(Player *player, int tn) {
    for (size_t i = 0; i < tn; i++) {
        Player *p = player+i;
        for (size_t j = 0; j < p->cardCount; j++) {
            if (strcmp(p->MyCards[j].myTYPE, "SPADES") == 0 && strcmp(p->MyCards[j].mylet, "A") == 0) {
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


void AddToTable(Card card)
{
    Card* temp = (Card*)malloc(sizeof(Card));
    temp->id = card.id;
    temp->mylet = card.mylet;
    temp->myTYPE = card.myTYPE;
    temp->link = TOP;
    TOP = temp;
    printf("Sended Card To Table\n");
}


Card RemoveFromTable()
{
    if (TOP == NULL)
    {
        printf("Overflow\n");
    }
    else
    {
        Card* temp = TOP;
        TOP = TOP->link;
        printf("RemovedFromTAble\n");
        Card k = *temp;
        free(temp);
        return k;
    }
    
}

void peep()
{
    if (TOP == NULL)
    {
        printf("Overflow\n");
    }
    else
    { 
        printf("id:%d Peep:%s %s\n",TOP->id,TOP->mylet,TOP->myTYPE);
    }
}

void display()
{
    if (TOP == NULL)
    {
        printf("Overflow\n");
    }
    else
    { 
        Card * ptr = TOP;
        printf("Table:");
        while (ptr!= NULL)
        {

            printf("id:%d Peep:%s %s\n",ptr->id,ptr->mylet,ptr->myTYPE);
            ptr = ptr->link;
        }
        printf("\n");
        
    }
}

void AddPlayerCard(Card card,Player *player)
{
    Card* temp = (Card*)malloc(sizeof(Card));
    temp->id = card.id;
    temp->mylet = card.mylet;
    temp->myTYPE = card.myTYPE;
    player->MyCards=temp;
    player->cardCount++;
    printf("Sended Card To Player\n");
}


void removePlayerCard(Card *arr, int n, int index) {
    // Check for valid index
    if (index < 0 || index >= n) {
        printf("Invalid index\n");
        return;
    }

    // Shift elements to the left
    for (int i = index; i < n - 1; i++) {
        arr[i] = arr[i + 1];
    }

    // Reduce the size of the array
    (n)--;

    // Optionally, reallocate memory (if you want to shrink the array size)
    arr = realloc(arr, n * sizeof(int));

    // You should check if realloc succeeded
    if (arr == NULL && n > 0) {
        printf("Memory reallocation failed!\n");
        exit(1);
    }

    // Print the updated array
    printf("Updated array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}



void main()
{
    //start
    printf("Enter Number of players You want:");
    scanf("%d",&n_players);
    create_cards();
    shuffle_deck(allCards,52);
    Player* players = (Player*)malloc(n_players*sizeof(Player));

    for (int i = 0; i < n_players; i++) {
        printf("Enter the name of Player %d: ", i + 1);
        scanf("%s", &players[i].name);
        
        players[i].id = i + 1;  // Set player ID
    }
    for (int i = 0; i < n_players; i++) {
        printf("%s %d\n",players[i].name,players[i].id);
    }
    distribute_cards(allCards, TOTAL_CARDS, players, n_players);
    printf("card dirstribution dont\n");
    int first = firstmove(players, n_players);
    int largestplayer = first;
    bool firstRound = true;
// Loop until game ends (you can change this condition as per game logic)
    Card large;  // Keeps track of the largest card played

    Track playerTrack[MAX_PLAYERS];
    int tracknum=0;
    while (1)
    {
        if (firstRound)
        {
            if (players[first].isturn)
            {
                printf("YOur Turn\n");
                // Show cards to the player and let them choose a card to play
                //show_cards(players[first].MyCards, players[first].cardCount);
                Card card= large = movePlayer(&players[first]);

                // Remove the card from the player's hand
                removePlayerCard(players[first].MyCards, players[first].cardCount, card.index);
                players[first].isturn = false;
                playerTrack[tracknum].playercard=card;
                playerTrack[tracknum].playerIndex=first;
                tracknum++;
                // Add the card to the table
                AddToTable(card);
                largestplayer = first;
                firstRound=false;
                first++;
                // Initially, set the card played by this player as the largest
            }

            
        }
        printf("second while loop\n");
        if (TOP->link)
        {
            /* code */
        
            
            Card *ptr = TOP;
            
            // Loop through the cards on the table to find the largest
            while (ptr != NULL)
            {
                if (rankToValue(ptr->mylet) > rankToValue(large.mylet))
                {
                    // If the rank is higher, check if the type is the same
                    if (ptr->myTYPE == large.myTYPE)
                    {
                        large = *ptr;  // Update the largest card
                        for (size_t i = 0; i < tracknum; i++)
                        {
                            if (large.id == playerTrack[tracknum].playercard.id)
                            {
                                largestplayer=playerTrack[tracknum].playerIndex;
                            }
                            
                        }
                        
                    }
                }

                // Move to the next card in the table
                ptr = ptr->link;
            }
        }
        
        // Player's turn logic
        display();
        for (size_t i = 0; i < MAX_PLAYERS; i++)
        {
            if (players[first].isturn)
            {
                printf("YOur Turn\n");
                // Show cards to the player and let them choose a card to play
                show_cards(players[first].MyCards, players[first].cardCount);
                Card card= movePlayer(&players[first]);

                // Remove the card from the player's hand
                removePlayerCard(players[first].MyCards, players[first].cardCount, card.index);
                players[first].isturn = false;
                playerTrack[tracknum].playercard=card;
                playerTrack[tracknum].playerIndex=first;
                tracknum++;
                // Add the card to the table
                AddToTable(card);

                if (large.myTYPE == card.myTYPE)
                {
                    if (rankToValue(card.mylet) > rankToValue(large.mylet))
                    {
                        large=card;
                        largestplayer = first;
                    }
                }
                else
                {
                    while (TOP!=NULL)
                    {
                        AddPlayerCard(RemoveFromTable(),&players[largestplayer]);
                    }
                    players[largestplayer].isturn = true;
                    firstRound=true;
                    break;
                    
                }
                
                
                // Initially, set the card played by this player as the largest
                first=(first+1)%MAX_PLAYERS;
                players[first].isturn = true;
            }


            
        }
        
        
        // If more than one card is on the table, find the largest card
        // After checking the table, determine if there are more than two cards
    }

    
}