# Encryption/Decryption Application

Name: Garrett McDonnell

Algorithms Project 1

Instructions: 

	In order to run the executable for part 1, unzip Project1Part1, cd into the containing folder using linux command line, run "make", then enter "./rsa435".  I display each calculation to standard out for your ease.

	In order to run the executable for part 2, unzip Project1Part2, cd into the containing folder, run "make", then enter "./messageDigest435 s file.txt" to sign or "./messageDigest435 v file.txt" to verify.  When prompted to enter the pull path of d_n.txt and e_n.txt, this is the full path of the directory for rsa435.  For example, for me, I input "/home/garrett/Documents/AlgorithmsProj1/bigInt435/  d_n.txt" for where I have those files saved in my linux environment.


Methods - Part1:

	The program begins by generating a temporary BigInteger variable initialized with "2^512 - 1".  This is the largest 512 bit number, and this is an easy starting point for generating prime numbers of at least 512 bits.  I then arbitrarily add "rand()" to temp until it lands on a number that ends in 1, 3, 7, or 9, as all prime numbers end in one of those digits.  From there, I run the fermat calculation until I find a prime for variable p.  After each fail, I add to temp until it lands in 1, 3, 7, or 9 again.  Since all base-10 numbers end in 0-9, I am cutting 60% of the work by only testing numbers that end in of these.  For q, I do the exact same thing but starting with a number that follows p.  From there, I compute n, phi, d, and e.  Throughout this program, I am using an overload of "^" to compute large numbers with large exponents.  For computation, I break the computation down until you have the base^2 as a starting point for the result.  I use the technique from class, I use an array to hold numbers 1-3 as symbols.  1 represents each time the exponent is found to be odd, where I multiply the result by the base and decrement the exponent, 2 represents each time I square the result and cut the exponent in half, and 3 represents each time I do modulo of the result.  The result is complete when the exponent is down to 2 (which is why the calculation begins at base^2) and when each operation has been done to result.  I write p and q to p_q.txt and write PA key and SA key to d_n.txt and e_n.txt, respectively.  
	
	Note: I found a function online to calculate inverse mod for finding d, and it has been cited at the top of the program.

Methods - Part2:

	The program first reads in file.txt as a command line argument.  The program first generates a hash of the contents of file.txt using sha256.  The hash is converted to base-10, it is then signed/decrypted using the private key in d_n.txt.  I then submit this to file.txt.signature.  One may then run the verification to regenerate the hash of file.txt, extract the public key from 		e_n.txt, and encrypt the hash using that key and the decrypt hash.  This new encrypted value should match the original sha256 hash of the file's contents, meaning it is authentic and unmodified.
