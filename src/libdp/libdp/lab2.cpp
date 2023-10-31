

#include <libdp/lab1.hpp>
#include <libdp/lab2.hpp>

int64_t PrimeNum() {
    std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int64_t> distr(1, 1e9);

    int64_t number = 0;
    do {
        number = distr(gen);
    } while (!MillerRabinTest(number,128));

    return number;
}


void Shamir(std::vector<char>& message, std::string &filename)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int64_t> distr(1, 1e9);

	int64_t P = PrimeNum();
	int64_t Ca = 0, Da = 0, Cb = 0, Db = 0;
	
	std::array<int64_t,3> EvkRes{};

	do {
		do {
			Ca = distr(gen);
			EvkRes = Evklid(Ca,P-1);
		} while (EvkRes[0] != 1);
		Da = EvkRes[1] + (P-1);
	} while (Ca*Da%(P-1) != 1);

	do {
		do {
			Cb = distr(gen);
			EvkRes = Evklid(Cb,P-1);
		} while (EvkRes[0] != 1);
		Db = EvkRes[1] + (P-1);
	} while (Cb*Db%(P-1) != 1);

	std::vector<int64_t> cripted(message.size());

	for(size_t i = 0; i < message.size(); i++) {
		auto ch = (unsigned char)message[i];
		cripted[i] = powmod(ch, Ca, P);
	}

	for(auto &i : cripted) {
		auto ch = cripted[i];
		cripted[i] = powmod(ch, Cb, P);
	}

	writeBytesAsFile<int64_t>("output/Shamir_enc" + filename, cripted);

	for(auto &i : cripted) {
		auto ch = cripted[i];
		cripted[i] = powmod(ch, Da, P);
	}

	for(size_t i = 0; i < message.size(); i++) {
		auto ch = cripted[i];
		message[i] = (char)powmod(ch, Db, P);
	}

	writeBytesAsFile<char>("output/Shamir_dec" + filename, message);
	
}

void Elgamal(std::vector<char>& message, std::string& filename) 
{
	int64_t P = BigPrimeNum();
	int64_t g = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int64_t> distr(2,P-2);
	for(int64_t i = 1; i == 1; i = powmod(g, (P-1)/2, P) ) {
		g = distr(gen);
	}

	distr.param(std::uniform_int_distribution<int64_t>(2,P-1).param());

	int64_t x = distr(gen);      //секрет боба
	int64_t y = powmod(g, x, P); //не секрет боба

	distr.param(std::uniform_int_distribution<int64_t>(2,P-2).param());

	int64_t k = distr(gen);     //сессионный ключ
	int64_t a = powmod(g, k, P);
	std::vector<int64_t> b(message.size());

	int64_t pieceY = powmod(y, k, P);

	for(size_t i = 0; i < message.size(); i++) {
		b[i] = powmod((unsigned char)message[i]*pieceY, 1, P);
	}

	int64_t pieceA = powmod(a, P-1-x,P);


	b.insert(b.begin(), a);

	writeBytesAsFile<int64_t>("output/Elgamal_enc" + filename, b);

	std::vector<int64_t> newb(b.size() - 1);

	for(size_t i = 1; i < b.size(); i++) {
		newb[i-1] = b[i];
	}

	std::vector<char> messagebob(b.size());
	for(size_t i = 0; i < newb.size(); i++) {
		messagebob[i] = (char)(powmod(newb[i]*pieceA, 1, P));
	}

	writeBytesAsFile<char>("output/Elgamal_dec" + filename, messagebob);
}

void Vernam(std::vector<char> &message, std::string& filename)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<char> distr(-128, 127);

	std::vector<char> key(message.size());

	for(size_t i = 0; i < message.size(); i++) {
		key[i] = distr(gen);
		message[i] = message[i] ^ key[i];
	}

	writeBytesAsFile<char>("output/Vernam_enc" + filename, message);	

	for(size_t i = 0; i < message.size(); i++) {
		message[i] = message[i]^key[i];
	}

	for(auto &keyy: key) {
		std::cout << (int)keyy << " ";
	}
	std::cout << std::endl;
	writeBytesAsFile<char>("output/Vernam_dec" + filename, message);
}

void RSA(std::vector<char>& message, std::string filename)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int64_t> distr((int64_t) pow(10,3), (int64_t) pow(10,4));

	int64_t P = 0,Q = 0;

	do {
		P = distr(gen);
		Q = distr(gen);
	} while(!MillerRabinTest(P,128) || !MillerRabinTest(Q,128));

	int64_t N = P * Q;
	int64_t phi = (P - 1) * (Q - 1);

	distr.param(std::uniform_int_distribution<int64_t>(1,phi-1).param());
	int64_t d,c;

	do {
		d = distr(gen);
		c = Evklid(phi, d)[2];
	} while (Evklid(d, phi)[0] != 1 || c*d%phi !=1);
	
	// c = c < 0 ? c + phi : c;
	std::vector<int64_t> e(message.size());
	for(size_t i = 0; i < message.size(); i++) {
		e[i] = powmod((unsigned char)message[i], d, N);
	}

	writeBytesAsFile<int64_t>("output/RSA_enc" + filename, e);

	std::vector<char> messagebob(e.size());

	for(size_t i = 0; i < e.size(); i++) {
		auto tmp = powmod(e[i], c, N);		
		messagebob[i] = (char)tmp;
	}

	writeBytesAsFile<char>("output/RSA_dec" + filename, messagebob);
}
