#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../scoreText.c"

#define TEMP 20
#define STEP 1
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define COUNT 10000

char *substitutionDecipher(char *key, char *text, char *result, int len);

float substitutionCrack(char *text, int len, char *maxKey);

int main(int argc, char *argv[])
{
  char cipher[3001];
  printf("Enter a ciphertext max 3000 chars :");
  gets(cipher);
  int len = strlen(cipher);
  char *result = malloc(sizeof(char) * (len + 1));
  printf("Running subcrack\n");

  char key[] = ALPHABET;
  int i = 0;
  double score, maxscore = -99e99;
  // run until user kills it
  while (1)
  {
    i++;
    score = substitutionCrack(cipher, len, key);
    if (score > maxscore)
    {
      maxscore = score;
      printf("best score so far: %f, on iteration %d\n", score, i);
      printf("    Key: '%s'\n", key);
      substitutionDecipher(key, cipher, result, len);
      printf("    plaintext: '%s'\n", result);
    }
  }
  free(result);
  return 0;
}

void modifyKey(char *newKey, char *oldKey)
{
  strcpy(newKey, oldKey);
  int i = rand() % 26;
  int j = rand() % 26;
  char temp = newKey[i];
  newKey[i] = newKey[j];
  newKey[j] = temp;
}

/* this is the bit that implements the simulated annealing algorithm */
float substitutionCrack(char *text, int len, char *bestKey)
{
  int i, j, count;
  float T;
  char temp, *deciphered = malloc(sizeof(char) * (len + 1));
  char testKey[27];
  char maxKey[27];
  double prob, dF, maxscore, score;
  double bestscore;
  strcpy(maxKey, bestKey);
  substitutionDecipher(maxKey, text, deciphered, len);
  maxscore = scoreTextQgram(deciphered, len);
  bestscore = maxscore;
  for (T = TEMP; T >= 0; T -= STEP)
  {
    for (count = 0; count < COUNT; count++)
    {
      modifyKey(testKey, maxKey);
      substitutionDecipher(testKey, text, deciphered, len);
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

char *substitutionDecipher(char *key, char *text, char *result, int len)
{
  int i;
  char *ch;
  char *sub;
  int loc;
  for (i = 0; i < len; i += 1)
  {
    ch = text[i];
    sub = strchr(key, ch);
    loc = (int)(sub - key);
    result[i] = ALPHABET[loc];
  }
  result[i] = '\0';
  return result;
}
