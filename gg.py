import random
from collections import deque

# Define card deck and order
suits = ['Spades', 'Hearts', 'Diamonds', 'Clubs']
values = list(range(1, 15))  # 1 is Ace, 14 is King
cards = [(value, suit) for suit in suits for value in values]

# Card comparison utility
def card_value(card):
    return card[0]  # Value is the first element

def card_name(card):
    value_names = {1: "A", 11: "J", 12: "Q", 13: "K", 14: "1"}  # Handle special names
    value = value_names.get(card[0], str(card[0]))
    return f"{value} of {card[1]}"

# Distribute cards equally among players
def distribute_cards(num_players):
    random.shuffle(cards)
    hands = [deque() for _ in range(num_players)]
    for i, card in enumerate(cards):
        hands[i % num_players].append(card)
    return hands

# Game logic
def play_game(num_players):
    hands = distribute_cards(num_players)
    current_suit = None
    current_round = []
    loser = None  # Tracks who will start the game

    # Determine initial starter: player with Ace of Spades
    for i, hand in enumerate(hands):
        if (1, 'Spades') in hand:
            loser = i
            break

    while True:
        print("\nStarting new round...")
        print(f"Player {loser + 1} starts the round.")

        current_suit = None
        current_round = []
        largest_card = (-1, None)  # Value, player_index

        for i in range(num_players):
            player_index = (loser + i) % num_players

            if not hands[player_index]:
                continue  # Skip if player has no cards

            # Player plays a card
            playable_cards = (
                [card for card in hands[player_index] if card[1] == current_suit]
                if current_suit else hands[player_index]
            )
            if playable_cards:
                chosen_card = max(playable_cards, key=card_value)
            else:
                chosen_card = hands[player_index][0]

            hands[player_index].remove(chosen_card)
            current_round.append((chosen_card, player_index))

            if current_suit is None:
                current_suit = chosen_card[1]  # Set current suit to the first played card's suit

            print(f"Player {player_index + 1} plays {card_name(chosen_card)}")

            if chosen_card[1] == current_suit and card_value(chosen_card) > largest_card[0]:
                largest_card = (card_value(chosen_card), player_index)

        # Handle players who cannot follow the suit
        for card, player_index in current_round:
            if card[1] != current_suit:
                print(
                    f"Player {largest_card[1] + 1} collects the pile due to Player {player_index + 1}"
                )
                hands[largest_card[1]].extend([c[0] for c in current_round])
                loser = largest_card[1]
                break
        else:
            # If everyone followed the suit, the largest card loses the round
            loser = largest_card[1]

        # Check for a winner
        for i, hand in enumerate(hands):
            if not hand:
                print(f"Player {i + 1} is empty-handed! They win!")
                return

        print(f"Player {loser + 1} will start the next round.")

# Run the game for 4 players
play_game(4)
