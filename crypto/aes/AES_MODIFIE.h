
int getSBoxValue(int num);
int getSBoxInvert(int num);
void KeyExpansion();
void AddRoundKey(int round);
void SubBytes();
void InvSubBytes();
void ShiftRows();
void InvShiftRows();
void ShiftRows2();
void InvShiftRows2();
void MixColumns();
void InvMixColumns();
void MixBits(int round, unsigned int graine);
void InvMixBits(int round, unsigned int graine);
void Cipher();
void InvCipher();
void Charger_texte_par_bloc_de_16carac(unsigned char * texte, FILE * FIC);
void Cipher_Mode_1 (FILE * FIC_IN, FILE * FIC_OUT);
void InvCipher_Mode_1 (FILE * FIC_IN, FILE * FIC_OUT);
void Cipher_Mode_2 (FILE * FIC_IN, FILE * FIC_OUT);
void InvCipher_Mode_2 (FILE * FIC_IN, FILE * FIC_OUT);