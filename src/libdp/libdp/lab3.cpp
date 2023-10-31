#include <libdp/lab3.hpp>

std::array<uint8_t, 32> sha256_to_bytes(std::string hash)
{
	std::array<uint8_t, 32> Bytes{};
	uint8_t high;
	uint8_t low;

	for(uint8_t i = 0, j = 0; i < 32; ++i, j += 2) {
		high = (hash[j] < 'a') ? (uint8_t)hash[j] << 4 : (uint8_t)(hash[j] - 87) << 4;
		low = (hash[j + 1] < 'a') ? (uint8_t)hash[j + 1] & 0b00001111 : (uint8_t)hash[j + 1] - 87;

		Bytes[i] = high | low;
	}

	return Bytes;
}

int64_t mod(int64_t n, int64_t d)
{
	int64_t res = n % d;
	if(res * d < 0) res += d;

	return res;
}

void write_sign(std::string &filename, std::array<int64_t, 32> S)
{
	std::ofstream file;
	file.open(filename, std::ios::app);

	file << std::endl;
	for(uint8_t i = 0; i < 32; i++) {
		file << S[i] << " ";
	}

	file.close();
}

void write_sign_elgamal(std::string &filename, int64_t &r, std::array<int64_t,32> S)
{
	std::ofstream file;
	file.open(filename, std::ios::app);

	file << std::endl << r << " ";

	for(uint8_t i = 0; i < 32; i++) {
		file << S[i] << " ";
	}

	file.close();
}

std::string read_file(std::string &filename)
{
	std::ifstream file;
	file.open(filename, std::ios::binary);
	std::stringstream buffer;
	buffer << file.rdbuf();

	file.close();
	return buffer.str();
}

std::string read_file(std::string &filename, uint64_t &size)
{
	std::ifstream file;
	file.open(filename, std::ios::binary);
	std::stringstream buffer;

	buffer << file.rdbuf();

	file.close();

	return (buffer.str()).substr(0, size);
}

std::array<int64_t, 32> read_sign(std::string &filename, uint64_t &size)
{
	std::ifstream file;
	file.open(filename);
	file.seekg(size, std::ios::beg);

	std::array<int64_t, 32> sign;
	for(int64_t i = 0; i < 32; ++i) {
		file >> sign[i];
	}

	file.close();

	return sign;
}

std::array<int64_t, 33> read_sign_elgamal(std::string &filename, uint64_t &size)
{
	std::ifstream file;
	file.open(filename);
	file.seekg(size, std::ios::beg);

	std::array<int64_t, 33> sign;
	for(int64_t i = 0; i < 33; i++) {
		file >> sign[i];
	}

	file.close();

	return sign;
}

std::array<int64_t, 2> RSA_sign(std::string &filename, std::string &data)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int64_t> distr(1e3, 1e4);

	int64_t P, Q, d, c;

	do {
		P = distr(gen);
		Q = distr(gen);
	} while((!MillerRabinTest(P,128) || !MillerRabinTest(Q,128)) || P == Q);

	int64_t N = P*Q;
	int64_t phi = (P-1) * (Q-1);

	do {
		d = distr(gen);
		c = Evklid(phi, d)[2];
	} while(Evklid(d, phi)[0] != 1 || c*d%phi != 1);

	std::string hash;
	picosha2::hash256_hex_string(data, hash);

	auto h = sha256_to_bytes(hash);

	std::array<int64_t, 32> s{};

	for(size_t i = 0; i < 32; i++) {
		s[i] = powmod(h[i], c, N);
	}

	write_sign(filename, s);

	return std::array<int64_t, 2> {d, N};
}

bool RSA_valid(std::string &filename, int64_t &d, int64_t &N, uint64_t &size)
{
	std::array<int64_t, 32> S = read_sign(filename,size);
	int64_t e;

	std::string hash;
	picosha2::hash256_hex_string(read_file(filename, size), hash);
	std::array<uint8_t, 32> h = sha256_to_bytes(hash);

	for(uint8_t i = 0; i < 32; i++) {
		e = powmod(S[i], d, N);

		if(e!= h[i]) return false;
	}

	return true;
}

std::array<int64_t, 3> Elgamal_sign(std::string &filename, std::string &data)
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

	int64_t x = distr(gen);
	int64_t y = powmod(g, x, P);
	int64_t k;
	do {
		k = distr(gen);
	} while (Evklid(k, (P-1))[0] != 1);

	int64_t r = powmod(g, k, P);
	int64_t inv_k = Evklid(k, P - 1)[2];

	std::string hash;
	picosha2::hash256_hex_string(data, hash);
	std::array<uint8_t, 32> h = sha256_to_bytes(hash);


	int64_t u;
	std::array<int64_t, 32> S;

	for(uint8_t i = 0; i < 32; i++) {
		u = mod(h[i] - x*r, P-1);
		S[i] = mod(inv_k*u, P-1);
	}

	write_sign_elgamal(filename, r, S);

	return std::array<int64_t, 3> {P, g, y};

}

bool Elgamal_valid(std::string &filename, std::array<int64_t, 3> Pgy, uint64_t &size)
{
	std::string hash;
	picosha2::hash256_hex_string(read_file(filename, size), hash);
	std::array<uint8_t, 32> h = sha256_to_bytes(hash);
	std::array<int64_t, 33> S = read_sign_elgamal(filename, size);
	auto P = Pgy[0];
	auto g = Pgy[1];
	auto y = Pgy[2];
	int64_t a, b;
	for (int i = 0; i < 32; i++)
	{
		a = (powmod(y,S[0],P)*powmod(S[0],S[i+1],P)) % P;
		b = powmod(g, h[i], P);

		if(a!=b) return false;
	}
	return true;
}

std::array<int64_t, 4> GOST_sign(std::string &filename, std::string &data)
{
	const int64_t Pmin = 1073741824;
	const int64_t Pmax = 2147483647;
	const int64_t Qmin = 32768;
	const int64_t Qmax = 65535;
	const int64_t Bmin = 16384;
	const int64_t Bmax = 65535;

	int64_t p, q, b, a;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int64_t> distr(Qmin, Qmax);

	do {
		q = distr(gen);
	} while (!MillerRabinTest(q, 256));

	distr.param(std::uniform_int_distribution<int64_t>(Bmin,Bmax).param());

	do {
		b = distr(gen);
		p = b*q + 1;
	} while (!MillerRabinTest(p, 256) || p > Pmax || p < Pmin);

	distr.param(std::uniform_int_distribution<int64_t>(2, p - 2).param());

	do {
		a = powmod(distr(gen), b, p);
	} while (a <= 1);

	std::string hash;
	picosha2::hash256_hex_string(data, hash);
	auto h = sha256_to_bytes(hash);

	std::array<int64_t, 32> S;
	int64_t k, r;

	distr.param(std::uniform_int_distribution<int64_t>(1, q - 1).param());

	int64_t x = distr(gen);
	int64_t y = powmod(a,x,p);
	while(true) {
		k = distr(gen);
		r = powmod(a, k, p) % q;
		if(r == 0) continue;
		int i = 0;
		for(; i < 32; i++) {
			S[i] = (k*h[i] + x*r) % q;
			if(S[i] == 0) {
				break;
			}
		}
		if(i < 32)
			continue;
		else
			break;
	}

	write_sign_elgamal(filename, r, S);

	return std::array<int64_t, 4> {p, q, a, y};
}

bool GOST_valid(std::string &filename, std::array<int64_t, 4> pqay, uint64_t &size)
{
	std::string hash;
	picosha2::hash256_hex_string(read_file(filename,size), hash);
	auto h = sha256_to_bytes(hash);
	auto S = read_sign_elgamal(filename, size);

	auto p = pqay[0];
	auto q = pqay[1];
	auto a = pqay[2];
	auto y = pqay[3];

	int64_t inv_h, u1, u2, v;

	for(int i = 0; i < 32; i++) {
		inv_h = Evklid(h[i], q)[2];
		if(inv_h < 0) inv_h += q;
		u1 = mod(S[i+1] * inv_h, q);
		u2 = mod((-S[0]) * inv_h, q);
		v = powmod(a, u1,p)*powmod(y,u2,p)%p%q;
		if(v != S[0]) {
			return false;}
	} 
	return true;
}