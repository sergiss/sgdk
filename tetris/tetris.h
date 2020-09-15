u32 tile_indices[40];

// 4x4
const u8 piece_size =  4;
const u8 piece_off  = 16;
const u8 piece1[64] = {
    0,0,0,0,
    0,0,0,0,
    1,1,1,1,
    0,0,0,0,

    0,0,1,0,
    0,0,1,0,
    0,0,1,0,
    0,0,1,0,

    0,0,0,0,
    0,0,0,0,
    1,1,1,1,
    0,0,0,0,

    0,0,1,0,
    0,0,1,0,
    0,0,1,0,
    0,0,1,0,
};

const u8 piece2[64] = {
    0,0,0,0,
    0,0,0,0,
    0,1,1,0,
    0,1,1,0,

    0,0,0,0,
    0,0,0,0,
    0,1,1,0,
    0,1,1,0,

    0,0,0,0,
    0,0,0,0,
    0,1,1,0,
    0,1,1,0,

    0,0,0,0,
    0,0,0,0,
    0,1,1,0,
    0,1,1,0
};


const u8 piece3[64] = {
    0,0,0,0,
    0,0,0,0,
    2,2,2,0,
    0,0,2,0,

    0,0,0,0,
    0,2,0,0,
    0,2,0,0,
    2,2,0,0,

    0,0,0,0,
    2,0,0,0,
    2,2,2,0,
    0,0,0,0,

    0,0,0,0,
    0,2,2,0,
    0,2,0,0,
    0,2,0,0
};

const u8 piece4[64] = {
    0,0,0,0,
    0,0,0,0,
    3,3,3,0,
    3,0,0,0,

    0,0,0,0,
    3,3,0,0,
    0,3,0,0,
    0,3,0,0,

    0,0,0,0,
    0,0,3,0,
    3,3,3,0,
    0,0,0,0,

    0,0,0,0,
    0,3,0,0,
    0,3,0,0,
    0,3,3,0
};

const u8 piece5[64] = {
    0,0,0,0,
    0,0,0,0,
    0,2,2,0,
    2,2,0,0,

    0,0,0,0,
    0,2,0,0,
    0,2,2,0,
    0,0,2,0,

    0,0,0,0,
    0,0,0,0,
    0,2,2,0,
    2,2,0,0,

    0,0,0,0,
    0,2,0,0,
    0,2,2,0,
    0,0,2,0
};

const u8 piece6[64] = {
    0,0,0,0,
    0,0,0,0,
    1,1,1,0,
    0,1,0,0,

    0,0,0,0,
    0,1,0,0,
    1,1,0,0,
    0,1,0,0,

    0,0,0,0,
    0,1,0,0,
    1,1,1,0,
    0,0,0,0,

    0,0,0,0,
    0,1,0,0,
    0,1,1,0,
    0,1,0,0
};

const u8 piece7[64] = {
    0,0,0,0,
    0,0,0,0,
    3,3,0,0,
    0,3,3,0,

    0,0,0,0,
    0,0,3,0,
    0,3,3,0,
    0,3,0,0,

    0,0,0,0,
    0,0,0,0,
    3,3,0,0,
    0,3,3,0,

    0,0,0,0,
    0,0,3,0,
    0,3,3,0,
    0,3,0,0
};

const u8* pieces[7] = {
    piece1, piece2, piece3, piece4, piece5, piece6, piece7
};

// 10x20
const int board_length = 200;
u32 render_board[200];
u32 board[200];

int posx = 0;
int posy = 0;
int current_piece    = 0;
int next_piece       = 0;
int current_rotation = 0;

int statistics[7];
long top    = 0;
long score  = 0;
int lines   = 0;
int level   = 0;
int h_ticks = 0;
int v_ticks = 0;
int ticks = 0;

int tile_ind;

int left_key;
int right_key;
int down_key;
int rot_key;

void new_piece();

void update_text();

void draw_board();

int get_collision(int x, int y, int rot);

void write_current_piece(u32* data);

void clear_full_rows();

void game_over();

void joyEvent(u16 joy, u16 changed, u16 state);
