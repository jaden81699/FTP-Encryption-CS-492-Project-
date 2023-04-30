#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 1024

void generate_key(int key_len, char *key) {
  srand(time(NULL)); // Seed the random number generator
  for (int i = 0; i < key_len; i++) {
    key[i] = (char)(rand() % 256); // Generate a random ASCII character
  }
  key[key_len] = '\0'; // Add null terminator to the end of the key string
}
int main() {
  int key_len = 1024; 
  char key[MAX_SIZE];

  generate_key(key_len, key);

  FILE *key_file = fopen("key.txt", "w");
  if (key_file == NULL) {
    perror("[-]Error in writing key file.");
    exit(1);
  }
  fprintf(key_file, "%s", key);
  fclose(key_file);

  printf("[+]Key generated and written to file.\n");

  return 0;
}
