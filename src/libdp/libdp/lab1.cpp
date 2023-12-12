#include <libdp/lab1.hpp>


// int64_t powmod(int64_t a, int64_t x, int64_t p)
// {
// 	int64_t y = 1;
// 	int64_t s = a;

// 	while(x > 0) {
// 		if(x % 2 == 1) {
// 			y = y*s%p;
// 		}
// 		s = s*s%p;
// 		x /= 2;
// 	}

// 	return y;
// }

int64_t powmod(int64_t a, int64_t x, int64_t p)
{
	int64_t remainder;
    int64_t y = 1;
    while (x > 0) {
        //Работа со степенью
        remainder = x % 2;
        x = x / 2;
        if (remainder == 1) {
            y = (y * a) % p;
        }
        a = a * a % p;
    }
    return y;
}

template <typename T>
T powmod(T a, T x, T p)
{
	T remainder;
    T y = 1;
    while (x > 0) {
        //Работа со степенью
        remainder = x % 2;
        x = x / 2;
        if (remainder == 1) {
            y = (y * a) % p;
        }
        a = a * a % p;
    }
    return y;
}


bigint mod_pow(bigint num, bigint degree, bigint p)
{
	bigint result = 1;
	for(; degree; num = num * num % p, degree >>= 1) {
		if(degree & 1) {
			result = result * num % p;
		}
	}

	return result;
}

std::array<int64_t,3> Evklid(int64_t a, int64_t b) 
{
	if(b > a) {
		std::swap(a,b);
	}
	int q;

	std::array<int64_t, 3> U = {a, 1, 0};
	std::array<int64_t, 3> V = {b, 0, 1};
	std::array<int64_t, 3> T;
	while (V[0] != 0) {
		q = U[0]/V[0];
		T[0] = U[0]%V[0];
		T[1] = U[1] - q*V[1];
		T[2] = U[2] - q*V[2];
		for(int i = 0; i < 3; i++) {
			U[i] = V[i];
			V[i] = T[i];
		}
	}
	return U;
}

std::array<bigint,3> Evklid(bigint a, bigint b) 
{
	if(b > a) {
		std::swap(a,b);
	}
	bigint q;

	std::array<bigint, 3> U = {a, 1, 0};
	std::array<bigint, 3> V = {b, 0, 1};
	std::array<bigint, 3> T;
	while (V[0] != 0) {
		q = U[0]/V[0];
		T[0] = U[0]%V[0];
		T[1] = U[1] - q*V[1];
		T[2] = U[2] - q*V[2];
		for(int i = 0; i < 3; i++) {
			U[i] = V[i];
			V[i] = T[i];
		}
	}
	return U;
}


template <typename T>
std::array<T,3> Evklid(T a, T b) 
{
	if(b > a) {
		std::swap(a,b);
	}
	int q;

	std::array<T, 3> U = {a, 1, 0};
	std::array<T, 3> V = {b, 0, 1};
	std::array<T, 3> t;
	while (V[0] != 0) {
		q = U[0]/V[0];
		t[0] = U[0]%V[0];
		t[1] = U[1] - q*V[1];
		t[2] = U[2] - q*V[2];
		for(int i = 0; i < 3; i++) {
			U[i] = V[i];
			V[i] = t[i];
		}
	}
	return U;
}

template <typename T>
bool MillerRabinTest(T n, int k)
{
	if (n == 2 || n == 3) return true;
	if (n < 2 || n % 2 == 0) return false;

	T t = n - 1;
	int s = 0;
	while (t % 2 == 0) {
		t /= 2;
		s += 1;
	}

	for (int i = 0; i < k; i++) {
		std::random_device rd;
		std::mt19937 gen(rd());
		boost::random::uniform_int_distribution<T> distr(2, n-3);
		T a = distr(gen);

		T x = powmod(a, t, n);

		if(x == 1 || x == n - 1) continue;

		for (int r = 1; r < s; r++) {
			x = powmod(x,2,n);
			if(x == 1) return false;
			if(x == n - 1) break;
		}
		if(x!=n-1) return false;
	}
	return true;

}

int64_t BigPrimeNum()
{
	int64_t Q;
	int64_t P;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int64_t> distr((int64_t) pow(10,7), (int64_t) pow(10,9));
	while(true) {
		Q = distr(gen);
		if(MillerRabinTest(Q, 128)) {
			P = 2*Q+1;
			if(MillerRabinTest(P, 128)) {
				break;
			}
		}
	}
	return P;
}

void DiffHellm()
{
	// int64_t Q;
	int64_t P = BigPrimeNum();
	int64_t g;
	std::random_device rd;
	std::mt19937 gen(rd());
	// std::uniform_int_distribution<int64_t> distr((int64_t) pow(10,9), (int64_t) pow(10,12));
	// while(true) {
	// 	Q = distr(gen);
	// 	if(MillerRabinTest(Q, 128)) {
	// 		P = 2*Q+1;
	// 		if(MillerRabinTest(P, 128)) {
	// 			break;
	// 		}
	// 	}
	// }
	std::uniform_int_distribution<int64_t> distr(2,P-2);
	for(int64_t i = 1; i == 1; i = powmod(g, (P-1)/2, P) ) {
		g = distr(gen);
	}
	
	distr.param(std::uniform_int_distribution<int64_t>(1,P-1).param());
	int64_t Xa, Xb, Ya, Yb, Zab, Zba;

	Xa = distr(gen);
	Xb = distr(gen);
	Ya = powmod(g, Xa, P);
	Yb = powmod(g, Xb, P);
	Zab = powmod(Yb, Xa, P);
	Zba = powmod(Ya, Xb, P);

	std::cout <<"3) P = " << P << ",\tg = " << g << "\n   Xa = " << Xa << ",\tXb = " << Xb << "\n   Ya = " <<
	Ya << ",\tYb = " << Yb << "\n   Zab = " << Zab << ",\tZba = " << Zba << "\n\n";

}

int64_t BSGS()
{
	int64_t Q;
	int64_t P;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int64_t> distr((int64_t) pow(10,9), (int64_t) pow(10,12));
	while(true) {
		Q = distr(gen);
		if(MillerRabinTest(Q, 256)) {
			P = 2*Q+1;
			if(MillerRabinTest(P, 256)) {
				break;
			}
		}
	}

	int64_t m = (int64_t) ceil(sqrt(P + .0));
	int64_t a, y;
	do {
		a = distr(gen);
		y = distr(gen);
	} while ((a>P)||(y>P));
	int64_t x = 0;
	std::map<int64_t, int64_t> container;

	for(int64_t i = 0; i < m; i++) {
		container.emplace(powmod(a, i, P)*y%P, i);
	}

	for(int64_t i = 0; i < m; i++) {
		int64_t tmp = powmod(a,i*m, P);
		if(auto search = container.find(tmp); search != container.end()) {
			x = i*m - search->second;
			break;
		}
	}

	std::cout << "4) x = " << x << ",\t\ta = " << a << ",\tP = " << P << 
			"\n   modpow(a,x,P) = " << powmod(a,x,P) << ",\ty = " << y << std::endl;

	return x;
}