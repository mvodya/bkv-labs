// ��������� ��� ���������� ������� ��������� ������
// � ���������� ��������� cp1251
// ������ � �������� ������� � �������
// by Mark Vodyanitskiy (@mvodya) 30.05.2019
#include <ctype.h>
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ����� (cp1251)
typedef struct word {
  char* str;  // ������ ����
  int count;  // ����������, ��������� � ������
} WORD;

int total;  // ����� ���� � �������

// �������� ��������������� ����� �� �����
char* getWord(FILE* input);
// ������ � ������������ ����
void readFile(char* filename);

// ��������� ���� �� �������
int cmpWORDquant(WORD** w1, WORD** w2);
// ��������� ���� � ������ cp1251
int cmpCHARcyril(char a, char b);
// ��������� ���� �� ��������
int cmpWORDabc(WORD* w1, WORD* w2);

struct NODE {
  WORD* w;
  NODE *left = NULL, *right = NULL;
};

NODE* tree = NULL;

FILE* outABCD;

// ������� �� UTF-8 � CP1251
void utf8to1251(char* in, char* out) {
  iconv_t i = iconv_open("CP1251", "UTF-8");
  size_t len = 256;
  iconv(i, &in, &len, &out, &len);
  iconv_close(i);
}

// ��������������� ����� ���� ��������� ������
void printTree(NODE* t) {
  if (t == NULL) return;
  printTree(t->left);
  fprintf(outABCD, "%s - %d\n", t->w->str, t->w->count);
  printTree(t->right);
}

// ����� ����� � ������
WORD* treeFind(NODE* t, WORD* w) {
  if (t == NULL) return NULL;
  if (!cmpWORDabc(t->w, w)) return t->w;
  WORD* tmp;
  tmp = treeFind(t->left, w);
  if (tmp != NULL) return tmp;
  tmp = treeFind(t->right, w);
  if (tmp != NULL) return tmp;
}

int main(int argc, char* argv[]) {
  clock_t begin, end;
  double time_spent;

  // ��������� ������� ����������
  if (argc == 1) {
    printf("You must specify paths of the files to be processed.\n");
    return 1;
  }

  begin = clock();
  // ���������� ���������
  while (--argc) readFile(argv[argc]);
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time spended: %lf sec\n\n", time_spent);

  // ����� ������
  outABCD = fopen("analysisA.txt", "w");
  fprintf(outABCD, "Total words: %d\n", total);
  printTree(tree);
  printf("Output: analysisA.txt and analysisB.txt\n\n");

  // �������� ������
  fclose(outABCD);

  WORD w;
  char input[256], encoded[256];
  while (true) {
    printf("Search: ");
    scanf("%256s", input);
    utf8to1251(input, encoded);
    w.str = encoded;
    WORD* ans = treeFind(tree, &w);
    if (ans == NULL) {
      printf("NOT FOUNDED\n");
      continue;
    }
    printf("%s - %d\n", input, ans->count);
  }

  return 0;
}

// �������� �� ������ ������������� ������?
inline char iscyrilic(int i) {
  // ����������� ����� � (������ tolower - ��������� �������� �� -17)
  if (i >= 192 || (i & (-17)) == 168) return 1;
  return 0;
}
// ������� ������������� ����� � ��������� (� ������ �)
inline char tolowercyril(char i) { return (i == '\xa8') ? '\xb8' : (i | 32); }
// �������� �� ����������� �������, ������������� ������
inline char isspecmid(char i) {
  return (i == '-') || (i == '`') || (i == '\'');
}

char* getWord(FILE* input) {
  // �������� ������ ��� ������
  char *w = (char*)malloc(255 * sizeof(char)), *p = w;

  int c;
  char state = 0;
  char last;
  // ������ �����
  while (1) {
    c = fgetc(input);
    if (c == EOF) {
      if (state == 0)
        // ���� ���� ����������, � ����� ����� �� ��������� ������ NULL
        return NULL;
      else {
        // ���� ���� ����������, ���������� �����
        *p++ = '\0';
        return w;
      }
    }
    if (!state && (isalpha(c) || iscyrilic(c))) {
      // ���� ����������� �����
      state = 1;
    }
    if (state && (isalpha(c) || iscyrilic(c) || isspecmid(c) || isalnum(c))) {
      // ���� ����������� ����� ��� ���� ���� ��� �����
      // ������ ������� ����� �� ���������
      if (isalpha(c))
        c = tolower(c);
      else if (iscyrilic(c))
        c = tolowercyril(c);
      // ���������� ����� � ������
      *p++ = c;
    } else if (c = '\n' && last = '-')
      // ��������� ���������
      p--;
    else if (state) {
      // ���� �������� �� �����, � ����� ��������
      *p++ = '\0';
      return w;
    }
    last = c;
  }
  return NULL;
}

// ���������� ����� � ������
void addWord(char* w) {
  // ���������, ���� �� ��� ����� � ������?
  bool founded = false;
  // ������� ������
  int lastCmp;
  NODE *n = tree, *last;
  WORD s;
  s.str = w;
  while (n != NULL) {
    int cmp = cmpWORDabc(&s, n->w) * -1;
    if (cmp > 0) {
      // ������
      last = n;
      lastCmp = cmp;
      n = n->left;
    } else if (cmp < 0) {
      // ������
      last = n;
      lastCmp = cmp;
      n = n->right;
    } else {
      // �������
      founded = true;
      n->w->count++;
      break;
    }
  }
  if (founded) return;
  // ��������� �����, ���� ��� �� �������
  WORD* p = (WORD*)malloc(sizeof(WORD));
  p->str = w;
  p->count = 1;
  n = (NODE*)malloc(sizeof(NODE));
  n->w = p;
  n->left = n->right = NULL;
  if (lastCmp > 0)
    last->left = n;
  else if (lastCmp < 0)
    last->right = n;
  total++;
  if (tree == NULL) tree = n;
}

void readFile(char* filename) {
  FILE* input;
  printf("Process file %s... ", filename);
  // ��������� ���� ��� ���������
  input = fopen(filename, "r");
  if (input == NULL) {
    // ���� ����� ��� �� ����������
    printf("fail, file not found\n");
    return;
  }

  // ������ ���� � ���������� �����
  while (1) {
    char* w = getWord(input);
    if (w == NULL) break;
    addWord(w);
  }
  // ��������� ����
  fclose(input);

  printf("successfull\n");
}

inline int cmpCHARcyril(char a, char b) {
  // ���� a ������ b - ������ ������������� �����
  // ���� a ������ b - ������ ������������� �����
  // ���� a ����� b - ������ 0
  if (a == b) return 0;
  // ��������� ������ � �
  if (a == '\xb8') return (unsigned char)b > 229 ? -1 : 1;
  if (b == '\xb8') return (unsigned char)a > 229 ? 1 : -1;
  // ��� ������������� ����
  if (((unsigned char)a > 192 && b == '\0') ||
      ((unsigned char)b > 192 && a == '\0')) {
    return (a - b) * -1;
  }
  // � ����� ��������
  return (a - b);
}

int cmpWORDabc(WORD* w1, WORD* w2) {
  char *p1 = w1->str, *p2 = w2->str;
  int cmp = 0;
  // ���������� ����� �� ������
  while (*p1 != '\0' && *p2 != '\0') {
    // ����������
    cmp = cmpCHARcyril(*p1, *p2);
    if (!cmp) {
      // ���� ����� ��������� - ���������� ���������
      p1++;
      p2++;
      continue;
    }
    // ���������� �����, ���� ����� �� �����
    return cmp;
  }
  // ���� ���� �� ���� ������ - ������������� ����� �� �������
  return cmpCHARcyril(*p1, *p2);
}