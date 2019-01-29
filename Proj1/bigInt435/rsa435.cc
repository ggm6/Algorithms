/*
***********************************************************************************************
* NOTE: (2^512)-1 is the largest 512 bit number, so I start there and increment to find a prime.
* 
* Code for modular multiplicative inverse retrieved from: https://rosettacode.org/wiki/Modular_inverse#C.2B.2B
* 
* *********************************************************************************************
*/
// You need to complete this program for a part of your first project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <thread>
#include <ctime>
#include <cstdlib>
#include <fstream>


// 'BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

using namespace std;

BigInteger operator^(BigInteger lhs, BigInteger rhs);
BigInteger fermat(BigInteger lhs, BigInteger rhs, BigInteger mod);
BigInteger modInverse(BigInteger a, BigInteger b);

int main(){
	/* The library throws 'const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a 'try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {
		      
			//int p1=11,p3=3,p7=7,p9=9;
			BigInteger p{0};
			BigInteger q{0};
			//vector<BigUint> vecPrimes;
			//vecPrimes.push_back(2);
			//while(vecPrimes[vecPrimes.size()-1]<2^(512)-1)
			//{
				
				//thread threes { [&]() 
					//{
						BigInteger temp=(((BigInteger) 2)^((BigInteger) 512))-1;  // Biggest possible 512 bit number, good starting point
						//cout << temp << endl << endl << endl;
						
						srand(time(0));
						while ((temp%10!=1 && temp%10!=3 && temp%10!=7 && temp%10!=9))
							temp+=rand();  // I arbitrarily add rand() until the number ends in 1,3,7 or 9; this allows for a more randomly selected prime
						while (p==0)
						{
							BigInteger fermatNum=fermat(((BigInteger) 2), temp-1, temp);
							if (fermatNum % temp==1)
							{
								p=temp;
								cout << "p: " << p << endl << endl;
								break;
							}
			
							if (temp%10==1 || temp%10==7 || temp%10==9)
								temp+=2;
							else if (temp%10==3)
								temp+=4;
						}
						
						temp=p-1;
						while (temp%10!=1 && temp%10!=3 && temp%10!=7 && temp%10!=9)
							temp+=rand();
						while (q==0)
						{
							BigInteger fermatNum=fermat(((BigInteger) 2), temp-1, temp);
							if (fermatNum % temp==1)
							{
								q=temp;
								cout << "q: " << q << endl;
								break;
							}
			
							if (temp%10==1 || temp%10==7 || temp%10==9)
								temp+=2;
							else if (temp%10==3)
								temp+=4;
						}
						ofstream file1;
						file1.open("p_q.txt");
						file1 << p << '\n' << q;
						file1.close();
						
						BigInteger n{p*q};
						BigInteger phi{(p-1)*(q-1)};
						cout << endl << endl << "phi(n): " << phi << endl;
						BigInteger e{rand()%1000};
						for (BigInteger i=2; i<e; ++i)
						{
							if (e%i==0 || phi%e==0)
							{
								i=2;
								++e;
							}
						}
						cout << endl << endl << "e: " << e << endl;
						
						BigInteger d{modInverse(e,phi)};
						cout << endl << endl << "d: " << d << endl;
						pair<BigInteger,BigInteger> PA{e,n};
						pair<BigInteger,BigInteger> SA{d,n};
						
						ofstream file2;
						file2.open("e_n.txt");
						file2 << PA.first << '\n' << PA.second;
						file2.close();
						ofstream file3;
						file3.open("d_n.txt");
						file3 << SA.first << '\n' << SA.second;
						file3.close();
						
						
						

      
	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}

BigInteger operator^(BigInteger lhs,BigInteger rhs)
{
	BigInteger result(lhs);
	//vector<BigInteger> expons;
	int size=0;
	BigInteger temp{rhs};
	while (temp!=2)
	{
		if (temp%2!=0)
		{
			++size;
			--temp;
		}
		else
		{
			++size;
			temp/=2;
		}
	}
	//cout << size << endl << endl << endl;
	BigInteger expons[size];
	
	for (int i=0; i<size; ++i)
	{
		if (rhs%2!=0)
		{
			expons[i]=1;
			--rhs;
		}
		else
		{
			expons[i]=2;
			rhs/=2;
		}
	}
	
	result*=result;
	for (int i=0; i<size; ++i)
	{
		//cout << "i: " << i << endl << result << endl;
		if (expons[i]==2)
			result*=result;
		else
			result*=lhs;
	}
	
	return result;
}

BigInteger fermat(BigInteger lhs, BigInteger rhs, BigInteger mod)
{
	
	BigInteger result(lhs);
	//vector<BigInteger> expons;
	int size=1;
	BigInteger temp{rhs};
	while (temp!=2)
	{
		if (temp%2!=0)
		{
			++size;
			--temp;
		}
		else
		{
			++size;
			temp/=2;
		}
		++size;
	}
	//cout << size << endl << endl << endl;
	BigInteger expons[size];
	expons[0]=3;
	for (int i=1; i<size; ++i)
	{
		if (rhs%2!=0)
		{
			expons[i]=1;
			--rhs;
		}
		else
		{
			expons[i]=2;
			rhs/=2;
		}
		++i;
		expons[i]=3;
	}
	//cout << "size: " << size << endl << endl;
	//cout << "base: " << result << endl << endl;
	result*=result;
	for (int i=size-1; i>=0; --i)
	{
		//cout << "i: " << i << "  " << expons[i] << endl << result << endl;
		//cout << result;
		if (expons[i]==2)
		{
			//cout << "^2 = ";
			result*=result;
			//cout << result << endl << endl;
		}
			
		else if (expons[i]==1)
		{
			//cout << "*" << lhs << " = ";
			result*=lhs;
			//cout << result << endl << endl;
			
		}
		else
		{
			//cout << "%" << mod << " = ";
			result=result%mod;
			//cout << result << endl << endl;
		}
	}
	
	return result;
}

BigInteger modInverse(BigInteger a, BigInteger b) 
{ 
    BigInteger b0 = b, t, q;
	BigInteger x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
} 
