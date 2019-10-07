#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../scoreText.c"

#define TEMP 20
#define STEP 1
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define COUNT 10000

char *vigenereDecipher(char *key, char *text, char *result, int len);

float vigenereCrack(char *text, int len, char *maxKey);

int main(int argc, char *argv[])
{
  char key[] = "LOLNNSA";
  char text[] = "EVPTEGNDTPYQUIAVPEVKEIONGYQTSSDNZWADHSRIAGPBPERUIAVPERPCPDEAHEBPFDNEWUDSONFLHPYPLVFSESLQBXLPHEREKTSSCRVKNZCEURJDTTQREWNNSEURFUXPPEFEAJPPCVUKPRQEBEADSBHRFCPSRGUWFTPZANUCTGPEVWSZFDBZWOEVPECKEFRZENFDZADRDMEYQPGUWGCCYFSWLOQTCUWRTGNELHTLBLYLKEOWYGUWSLAPJNQADHSRIAGPBPERSLRCCVGZMSCHRIWREVPNHLOVSJPVHHPFHVYDNZHMROJOVSYHFANRHSRXSSTGVVZWTSCOFVFCPHSRXWYOCPFAGTCSARNLTSSMRFLWLMEBOJELYEURSUECVRLUIAVPEVKTZHCLNFDRIPFFHOCHTBAKOQHSRCDATBERKLOCYPLSJOXHSRPAPSSCGRPTVBZJVFGEVPLZMSEPZGUXOWZZJGZEQFPDHWNNMOVFLRTPFGVGNZTPATDIDVERKLGFSDFVFGSCHGUWPWOTAGWXEPPTVFSTGEURWADWPFGOAJCQPESCVWYTGZENWAURJ";
  int len = strlen(key);
  char result[strlen(text) + 1];
  printf(vigenereDecipher(key, text, result, len));

  /*char cipher[3001];
  printf("Enter a ciphertext max 3000 chars :");
  gets(cipher);
  int len = strlen(cipher);
  char *result = malloc(sizeof(char) * (len + 1));
  printf("Running subcrack\n");
  int i = 0;
  double score, maxscore = -99e99;
  // run until user kills it
  while (1)
  {
    i++;
    char key[] =
        score = vigenereCrack(cipher, len, key);
    if (score > maxscore)
    {
      maxscore = score;
      printf("best score so far: %f, on iteration %d\n", score, i);
      printf("    Key: '%s'\n", key);
      vigenereDecipher(key, cipher, result, len);
      printf("    plaintext: '%s'\n", result);
    }
  }
  free(result);*/
  return 0;
}

void modifyKey(char *newKey, char *oldKey)
{
  strcpy(newKey, oldKey);
  int i = rand() % (strlen(oldKey) + 1);
  newKey[i] = 'A' + (rand() % 26);
}

/* this is the bit that implements the simulated annealing algorithm */
float vigenereCrack(char *text, int len, char *bestKey)
{
  int keyLength = strlen(bestKey);
  int i, j, count;
  float T;
  char temp, *deciphered = malloc(sizeof(char) * (len + 1));
  char testKey[keyLength + 1];
  char maxKey[keyLength + 1];
  double prob, dF, maxscore, score;
  double bestscore;
  strcpy(maxKey, bestKey);
  vigenereDecipher(maxKey, text, deciphered, len);
  maxscore = scoreTextQgram(deciphered, len);
  bestscore = maxscore;
  for (T = TEMP; T >= 0; T -= STEP)
  {
    for (count = 0; count < COUNT; count++)
    {
      modifyKey(testKey, maxKey);
      vigenereDecipher(testKey, text, deciphered, len);
      score = scoreTextQgram(deciphered, len);
      dF = score - maxscore;
      if (dF >= 0)
      {
        maxscore = score;
        strcpy(maxKey, testKey);
      }
      else if (T > 0)
      {
        prob = exp(dF / T);
        if (prob > 1.0 * rand() / RAND_MAX)
        {
          maxscore = score;
          strcpy(maxKey, testKey);
        }
      }
      // keep track of best score we have seen so far
      if (maxscore > bestscore)
      {
        bestscore = maxscore;
        strcpy(bestKey, maxKey);
      }
    }
    printf("best score so far: %f key: %s \n", bestscore, bestKey);
  }
  free(deciphered);
  return bestscore;
}

char *vigenereDecipher(char *key, char *text, char *result, int len)
{
  int q;
  int textLen = strlen(text);
  char *extKey[len + 1];
  for (q = 0; q < textLen; q += 1)
  {
    int keyOffset = key[q % len] - 'A'; //0 if keyLet = 0 if 1
    int textQuery = text[q] - 'A';
    int returnLet = (textQuery - keyOffset);
    if (returnLet < 0)
    {
      returnLet += 26;
    }
    result[q] = returnLet + 'A';
  }
  result[q] = '\0';
  return result;
}
