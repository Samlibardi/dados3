#pragma once

//Funçao de comparação entre registros a e b, de acordo com as especificações do projeto
//ou seja, maior prioridades, respectivamente para os campos 1, 2, 3 e 4.
int regcmp(const void *a, const void *b);

//Funçao de comparação entre datas de formato dd/mm/yyyy
int datecmp(const char *a, const char *b);
