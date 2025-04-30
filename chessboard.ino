// === Variáveis Globais ===
char board[8][8];
char previous_board[8][8];

// === Detecta a jogada feita pelo jogador ===
bool detect_player_move(char prev[8][8], char curr[8][8], char *from, char *to) {
  int fr = -1, fc = -1, tr = -1, tc = -1;

  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      if (prev[r][c] != curr[r][c]) {
        if (curr[r][c] == '.') {
          fr = r; fc = c;
        } else {
          tr = r; tc = c;
        }
      }
    }
  }

  if (fr != -1 && tr != -1) {
    from[0] = 'a' + fc;
    from[1] = '8' - fr;
    from[2] = '\0';
    to[0] = 'a' + tc;
    to[1] = '8' - tr;
    to[2] = '\0';
    return true;
  }

  return false;
}

// === Aplica um lance no array board ===
void apply_move(char b[8][8], const char *from, const char *to) {
  int fr = 8 - (from[1] - '0');
  int fc = from[0] - 'a';
  int tr = 8 - (to[1] - '0');
  int tc = to[0] - 'a';

  b[tr][tc] = b[fr][fc];
  b[fr][fc] = '.';
}

// === Gera a string FEN a partir de board[8][8] ===
void generate_fen(char b[8][8], char *fen) {
  char *ptr = fen;
  for (int i = 0; i < 8; i++) {
    int empty = 0;
    for (int j = 0; j < 8; j++) {
      if (b[i][j] == '.') {
        empty++;
      } else {
        if (empty > 0) *ptr++ = '0' + empty, empty = 0;
        *ptr++ = b[i][j];
      }
    }
    if (empty > 0) *ptr++ = '0' + empty;
    if (i < 7) *ptr++ = '/';
  }
  strcpy(ptr, " w - - 0 1");
}

// === Chama o motor de xadrez (mock) ===
void mcumax_move(const char *fen, char *move) {
  // Aqui você chamaria o mcu-max de verdade.
  // Por enquanto, devolve um lance simulado.
  strcpy(move, "e7e5");
}

// === Acende LEDs (simulado via Serial) ===
void light_up_leds(const char *move) {
  Serial.print("Acendendo LEDs: ");
  Serial.print(move[0]);
  Serial.print(move[1]);
  Serial.print(" → ");
  Serial.print(move[2]);
  Serial.println(move[3]);
}

// === Setup inicial ===
void setup() {
  Serial.begin(115200);
  init_board();
  memcpy(previous_board, board, sizeof(board));
}

// === Loop principal ===
void loop() {
  scan_board(board);

  char from[3], to[3];
  if (detect_player_move(previous_board, board, from, to)) {
    Serial.print("Jogador moveu: ");
    Serial.print(from);
    Serial.print(" -> ");
    Serial.println(to);

    apply_move(board, from, to);

    char fen[100];
    generate_fen(board, fen);
    Serial.print("FEN: ");
    Serial.println(fen);

    char bestmove[6] = "e7e5";  // Simulação do motor
    mcumax_move(fen, bestmove);

    Serial.print("Motor responde: ");
    Serial.println(bestmove);

    apply_move(board, bestmove, bestmove + 2);
    light_up_leds(bestmove);

    memcpy(previous_board, board, sizeof(board));
  }

  delay(500);
}

// === Inicializa o tabuleiro com posição inicial de xadrez ===
void init_board() {
  const char *start[8] = {
    "rnbqkbnr",
    "pppppppp",
    "........",
    "........",
    "........",
    "........",
    "PPPPPPPP",
    "RNBQKBNR"
  };
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      board[i][j] = start[i][j];
}

// === Simula leitura da matriz de sensores ===
void scan_board(char b[8][8]) {
  // Aqui você vai ler os sensores reais. Por enquanto simula.
  // Exemplo: simular e2e4
  static bool moved = false;
  if (!moved) {
    b[6][4] = '.';  // e2 vazio
    b[4][4] = 'P';  // e4 cheio
    moved = true;
  }
}