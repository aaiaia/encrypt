#include <iostream>
#include <cstdio>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <string>
#include <cmath>

#define MAX_N 1000

using namespace std;

void eratosthenes(bool *isPrime)
{
    memset(isPrime, true, sizeof(isPrime));

    isPrime[0] = isPrime[1] = false;
    isPrime[2] = false; // except prime number 2

    int sqrtn = int(sqrt(MAX_N));

    for (int i = 3; i <= sqrtn; i++) {
        if (isPrime[i]) {
            for (int j = i*i; j <= MAX_N; j += i) {
                isPrime[j] = false;
            }
        }
    }
}

int main()
{
    cout << "A라는 사람이 B라는 사람에게 정보를 암호화해서 보내고 싶다." << endl;
    cout << "이때 RSA 암호화 알고리즘을 이용하고자 한다." << endl;

    cout << "B는 A에게 정보를 받기 위해 공개키(n,e), 개인키(n,d)를 형성한다." << endl;

    bool isPrime[MAX_N + 1];
    eratosthenes(isPrime); // 소수 생성

    srand((unsigned)time(NULL)); // 랜덤 테이블 형성

    // n, p, q값 구하기
    int n = 0;
    int p = 0, q = 0;

    cout << endl;
    cout << "n, p, q 값 구하는 중" << endl;
    while (!n)
    {
        //

        // p를 만들고
        while (!p)
        {
            int randVal = (rand() % MAX_N) + 1; // (rand() % MAX_N) + 1 => 1 ~ MAX_N
            if (isPrime[randVal])
                p = randVal;
        }

        // q를 만들고
        while (!q && q != p)
        {
            int randVal = (rand() % MAX_N) + 1;
            if (isPrime[randVal])
                q = randVal;
        }

        // pi가 2가 되면 안된다.
        if ((p - 1) * (q - 1) == 2)
        {
            p = 0, q = 0;
            continue;
        }
        // n을 만들어준다.
        n = p * q;
    }


    // e 구하기
    // Φ(n) = (p - 1) * (q - 1)
    // 1 < e < Φ(n), e와 Φ(n)은 서로소
    int pi = (p - 1) * (q - 1);
    int e = 0;

    cout << "n :: " << n << endl;
    cout << "p :: " << p << endl;
    cout << "q :: " << q << endl;
    cout << "pi :: " << pi << endl;

    cout << "e 값 구하는 중" << endl;

    while (!e)
    {
        //srand((unsigned)time(NULL)); // 랜덤 테이블 형성

        int randVal = rand() % pi;  // 0 ~ (pi - 1)

        // 1 < e < Φ(n)
        if (1 < randVal && randVal < pi)
        {
            e = randVal;

            // e와 Φ(n)이 서로소인지 검사
            int cnt = 0;
            for (int i = 2; i < e; i++) // 1을 제외한 공약수 X
            {
                if (e % i == 0 && pi % i == 0) {
                    e = 0;
                    break;
                }
            }
        }
    }

    cout << "e :: " << e << endl;

    // d 구하기
    // (e * d) mod Φ(n) = 1
    int d = 0;

    cout << "d 값 구하는 중" << endl;

    while (!d)
    {

        int randVal = (rand() % MAX_N) + 1;   // 1 ~ MAX_N
    //    cout << "randVal :: " << randVal << endl;
        if ((e * randVal) % pi == 1) {
            d = randVal;
        }
        else
        {
            int _tmp;
            for(_tmp = randVal; _tmp != (randVal - 1); _tmp = (_tmp + 1) % MAX_N)
            {
                //cout << "_tmp => " << _tmp << endl;
                if ((e * _tmp) % pi == 1) {
                    d = _tmp;
                }
            }
            //cout << "randVal = " << randVal << ", _tmp = " << _tmp << endl;
            if(d == 0) {
                cout << "d의 값을 찾을 수 없는 케이스 입니다." << endl;
            }
            break;
        }
    }

    cout << "<< 최종 >> " << endl;
    cout << "n :: " << n << endl;
    cout << "p :: " << p << endl;
    cout << "q :: " << q << endl;
    cout << "pi :: " << pi << endl;
    cout << "e :: " << e << endl;
    cout << "d :: " << d << endl;

    cout << endl;
    cout << "공개키(n, e) :: " << "(" << n << "," << e << ")" << endl;
    cout << "개인키(n, d) :: " << "(" << n << "," << d << ")" << endl;
    cout << endl;

    cout << "공개키를 A에게 주고 B는 개인키만 가지고 있는다." << endl;

    long long M = 127 % n;
    cout << "이제 A가 '" << M << "'이라는 정보를 암호화해서 보내고자 한다." << endl;

    cout << "A가 보낼 정보를 C로 암호화 한다." << endl;

    long long C = ((long long)pow(M, e) % n);

    cout << "암호화 된 값 :: " << C << endl;
    cout << "암호화된 C정보를 받은 B는 이제 해독하고자 한다." << endl;

    M = ((long long)pow(C, d) % n);

    cout << "B가 받은 해독 결과 :: " << M << endl;

    cout << endl;


    unsigned int _r;
    unsigned int _y;

    unsigned int _M = 1004 % n;  // set Origin message
    cout << "_M = " << _M << endl;

    // init values
    _r = 1;
    _y = _M % n;
    //start do power
    for(unsigned int _exp = e; _exp != 0; _exp = _exp >> 1) {
        if( _exp & 0x1 ) {
            _r = (_r * _y) % n;
        }
        else
        {
            // Do nothing
        }
        _y = (_y * _y) % n;
    }
    cout << "_r = " << _r << endl;

    unsigned int _C = _r;    // set Encrypt message

    // init values
    _r = 1;
    _y = _C % n;
    //start do power
    for(unsigned int _exp = d; _exp != 0; _exp = _exp >> 1) {
        if( _exp & 0x1 ) {
            _r = (_r * _y) % n;
        }
        else
        {
            // Do nothing
        }
        _y = (_y * _y) % n;
    }
    cout << "_r = " << _r << endl;

    return 0;
}

//                                                       This source code Copyright belongs to Crocus
//                                                        If you want to see more? click here >>
