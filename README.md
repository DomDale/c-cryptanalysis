# c-cryptanalysis
Various cryptanalysis programs written in C for the University of Southampton's Cipher Challenge.

Programs consist of a scoring function and a decoding function. The ciphertext is decoded with a random key, and scored using quadgrams. A 'neighbor' of this key are then used and the resulting score is fed into a simulated annealing optimisation algorithm which either accepts or discards the new key. Repeating this many times reveals a key that leads to the most English-like output - idelly the plaintext!
