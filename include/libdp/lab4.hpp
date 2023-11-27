#include <libdp/lab3.hpp>

class Player {
private:
	const int64_t P_;
	int64_t C;
	int64_t D;
	std::vector<int64_t> hand_;

	

public:
	Player(const int64_t& P) : P_(P) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<uint64_t> distr(2, P_-2);

		std::array<int64_t,3> EvkRes{};
		do {
			do {
				C = distr(gen);
				EvkRes = Evklid(C,P_-1);
			} while (EvkRes[0] != 1);
			D = EvkRes[2];
			D > 0 ? D = D : D += P-1;
		} while (C*D%(P_-1) != 1);
	}

	void shuffle(std::vector<int64_t>& deck) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int64_t> distr(0,deck.size()-1);
		for(std::size_t i = 0; i < deck.size(); i++) {
			std::swap(deck[distr(gen)] , deck[distr(gen)]);
		}
	}
	void encode_deck(std::vector<int64_t>& deck) {
		for(auto &card : deck) {
			card = powmod(card, C, P_);
		}
	}

	void decode_hand(std::vector<int64_t>& hand) {
		for(auto &card : hand) {
			card = powmod(card, D, P_);
		}
	}

	void decode_own_hand() {
		for(auto &card : hand_) {
			card = powmod(card, D, P_);
		}
	}

	void take_card(int card) {
		hand_.push_back(card);
	}

	std::vector<int64_t>& give_hand()
	{
		return hand_;
	}

	bool operator==(Player& rhs) {
		return C == rhs.C && D == rhs.D && hand_ == rhs.hand_;
	}

	bool operator!=(Player& rhs) {
		return !(*this == rhs);
	}
};

class Poker {
private:
	int64_t P;
	std::vector<int64_t> deck;
	std::vector<Player> players;
	std::vector<int64_t> table;

public:

	Poker(const int player_n, const int card_n) {
		if(player_n * 2 + 5 > card_n) {
			throw "ЧЕ ТО ДОФИГА ИГРОКОВ!!";
		}

		P = BigPrimeNum();
		for(int i = 0; i < player_n; i++) {
			players.push_back(Player(P));
		}
		for(int i = 0; i < card_n; i++) {
			deck.push_back(i+2);
		}
		
		std::cout << "[✔] ПОКЕР НА-А-АЧИНАЕТСЯ!!!" << std::endl;
	}

	void print_cards(std::vector<int64_t>& cards) {
		for(auto &card : cards) {
			std::cout << card << " ";
		}
		std::cout << std::endl;
	}

	void Game() {
		std::cout << "[✔] Игроков: " << players.size() << ",\tКарт в колоде: " << deck.size() << std::endl;

		for(auto &player : players) {
			player.encode_deck(deck);
			player.shuffle(deck);
		}

		for(int i = deck.size()-1, j = 0; i > (int)(deck.size() - players.size()*2); i -= 2, j++) {
			players[j].take_card(deck[i]);
			players[j].take_card(deck[i-1]);
		}
		deck.erase(deck.begin() + (deck.size() - players.size()*2), deck.end());


		std::cout << "[✔] Карты розданы!! В колоде осталось: " << deck.size() << " карты" << std::endl;
		std::cout << "\nЗашифрованные карты игроков:\n\tЗашифровали все:" << std::endl;
		size_t num = 1;
		for(auto &player : players) {
			std::cout << "\t Игрок " << num << ": ";
			print_cards(player.give_hand());
			num++;
		}
		std::cout << "\tРасшифровали все, кроме владельца:\n";
		num = 1;


		for(auto &player1 : players) {
			for(auto &player2 : players) {
				if(player1 != player2) {
					player2.decode_hand(player1.give_hand());
				}
			}
			std::cout << "\t Игрок " << num << ": ";
			print_cards(player1.give_hand());
			num++;
			player1.decode_own_hand();
		}

		std::cout << "\n[✔] Игроки расшифровали свои карты" << std::endl;

		for(std::size_t i = 0; i < 5; i++) {
			table.push_back(deck.back());
			deck.pop_back();
		}

		for(auto &player : players) {
			player.decode_hand(table);
		}


		
		std::cout << "\nКарты на столе:\t";
		print_cards(table);

		std::cout << "\nВСКРЫВА-А-А-АЕМСЯ\n\n";
		num = 1;
		for(auto &player : players) {
			std::cout << "Игрок " << num << ": ";
			print_cards(player.give_hand());
			num++;
		}

	}
};
