/***
   prepared for CS435 Project 1 part 2
**/

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "sha256.h"
#include "BigIntegerLibrary.hh"

BigUnsigned bigModComp(BigUnsigned lhs, BigUnsigned rhs, BigUnsigned mod);

using namespace std;
 
int main(int argc, char *argv[])
{
   //Second part of your project starts here
   if (argc != 3 || (argv[1][0]!='s' && argv[1][0]!='v')) 
      std::cout << "wrong format! should be \"a.exe s filename\"" << endl;
   else {
      std::string filename = argv[2];
      
            
      //read the file
      std::streampos begin,end;
      std::ifstream myfile (filename.c_str(), std::ios::binary);
      begin = myfile.tellg();
      myfile.seekg (0, std::ios::end);
      end = myfile.tellg();
      std::streampos size = end-begin;
      
      myfile.seekg (0, std::ios::beg);
      char * memblock = new char[size];
      myfile.read (memblock, size); //read file; it's saved in the char array memblock
      myfile.close();
    
      string M{sha256(memblock)};
      BigUnsignedInABase M2{M,16};
      BigUnsigned M3{M2};
      ifstream pFile;
      string path;
      string arr[2];
        
      if (argv[1][0]=='s') {
          std::cout << "\n"<<"Need to sign the doc.\n\n";
		  cout << "Please enter full path of d_n file: ";
		  getline(cin,path);
		  cout << endl;
		  pFile.open(path);
		 
		  if (pFile.is_open())
		  {
			  string dStr{""};
			  int z=0;
			  while(getline(pFile,dStr))
			  {
				arr[z]=dStr;
				++z;
			  }
		  }
		  else {
			  cout << "Sorry, unable to open file." << endl;
		  }
		  pFile.close();
		  
		  BigUnsigned PAd{stringToBigUnsigned(arr[0])};
		  BigUnsigned PAn{stringToBigUnsigned(arr[1])};
		  BigUnsigned signature{bigModComp(M3,PAd,PAn)};
		  ofstream sigFile;
		  sigFile.open(filename + ".signature");
		  sigFile << signature << "\n\n";
		  sigFile.close();	 
      }
      else {
          std::cout << "\n"<<"Need to verify the doc.\n\n";
		  string Magain{sha256(memblock)};
		  BigUnsignedInABase M2again{M,16};
		  BigUnsigned M3again{M2};
		  
		  pFile.open(filename+".signature");
		  string Pstr;
		  getline(pFile,Pstr);
		  pFile.close();
		  BigUnsigned pEnc{stringToBigUnsigned(Pstr)};
		  

		  path="";
		  arr[0]="",arr[1]="";
		  cout << "Please enter full path of e_n file: ";
		  getline(cin,path);
		  cout << endl;
		  pFile.open(path);
		  
		  if (pFile.is_open())
		  {
			  string dStr{""};
			  int z=0;
			  while(getline(pFile,dStr))
			  {
				arr[z]=dStr;
				++z;
			  }
		  }
		  else {
			  cout << "Sorry, unable to open file." << endl;
		  }
		  pFile.close();
			
		  BigUnsigned SAe{stringToBigUnsigned(arr[0])};
		  BigUnsigned SAn{stringToBigUnsigned(arr[1])};
		  BigUnsigned signature2{bigModComp(pEnc,SAe,SAn)};
		  if (M3again==signature2)
			cout << "The encrypted hash matches the original sha256 hash of \"" << filename << "\", it is authentic." << endl;
		  else
			cout << "The original document has been modified!" << endl;
			 
		  }
      
      
      delete[] memblock;
    }
    return 0;
}

BigUnsigned bigModComp(BigUnsigned lhs, BigUnsigned rhs, BigUnsigned mod)
{
	BigUnsigned result(lhs);
	//vector<BigUnsigned> expons;
	int size=1;
	BigUnsigned temp{rhs};
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
	BigUnsigned expons[size];
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
		//cout << result << endl;;
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
