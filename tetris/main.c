/**
 * Tetris for Sega Genesis
 * 2020 Sergio Soriano - sergiosoriano.com
 */
#include <genesis.h>
#include "tetris.h"
#include "resources.h" // load resources

int main()
{

    // setup screen 320x224
    VDP_setScreenWidth320();
    // Background index
    u16 ind = 1;

    // Setup background1 palette to PAL1
    VDP_setPalette(PAL1, background1.palette->data);
    // Load background at BG_A (x offset 4 tiles (32 px))
    VDP_drawImageEx(BG_A, &background1, TILE_ATTR_FULL(PAL1, 1, 0, 0, ind), 4, 0, 1, CPU);

    VDP_setPalette(PAL2, tiles1.palette->data);

    // Tile index (background index + background offset)
    tile_ind = ind + background1.tileset->numTile;

    // Load Tile
    for(int i = 0; i < 40; ++i) {
        tile_indices[i] = tile_ind;
        VDP_loadBMPTileDataEx((const u32 *) tiles1.image, tile_ind, i % 4, (i / 4) << 3, 4, 10, 4);
        tile_ind += 1;
    }

    // Sprites initialization
    SPR_init(0, 0, 0);

    // Control events
    JOY_setEventHandler(joyEvent);


    VDP_setBackgroundColor(0);

    // Sounds
    SND_startPlay_XGM(music1); // play song

    // Static text
    VDP_drawText("A-TYPE", 7 , 3);
    VDP_drawText("TOP"   , 28, 3);
    VDP_drawText("SCORE" , 28, 6);
    VDP_drawText("NEXT"  , 28, 12);
    VDP_drawText("LEVEL" , 28, 19);
    VDP_drawText("sergiosoriano.com", 4, 27);

    game_over();

    next_piece = random() % 7;
    new_piece();
    draw_board();

    while(1) {

        int update = 0;

        // Vertical move
        if(down_key || v_ticks > 400) {
            v_ticks = 0;
            if(get_collision(posx, posy + 1, current_rotation)) {
                write_current_piece(board); // persist tiles
                clear_full_rows();
                new_piece(); // generate new piece
            } else {
                posy++;
            }
            update = 1;

        } else {

            int speed = down_key ? level * 10 : level;
            v_ticks += speed;

            // Rotation
            if(rot_key == 1) {
                rot_key = 2;
                int rot = (current_rotation + 1) % 4;
                if(!get_collision(posx, posy, rot)) {
                    current_rotation = rot;
                    update = 1;
                }
            }

            // Horizontal move
            if(left_key == 1 || (left_key == 2 && h_ticks > 25)) {
                left_key = 2;
                if(!get_collision(posx - 1, posy, current_rotation)) {
                    posx--;
                    update = 1;
                }
            } else if(right_key == 1 || (right_key == 2 && h_ticks > 25)) {
                right_key = 2;
                if(!get_collision(posx + 1, posy, current_rotation)) {
                    posx++;
                    update = 1;
                }
            }

            if(left_key == 2 || right_key == 2) {
                h_ticks++;
            } else {
                h_ticks = 0;
            }

        }

        // Update board
        if(update) {
            draw_board();
        }

       // SPR_update(); // Update the sprite engine;
        if(ticks++ > 5) {
            ticks = 0;
            VDP_waitVSync(); // Wait for the next start of Vertical blanking.
        }

    }
    return 0;
}

void new_piece() {

    current_piece = next_piece;
    next_piece = random() % 7;
    current_rotation = 0;
    posx =  4;
    posy = -2;

    if(get_collision(posx, posy, current_rotation)) {
        game_over();
    }
    statistics[current_piece]++;
    if(next_piece > 1) VDP_setHorizontalScroll(BG_B, 4);
    else VDP_setHorizontalScroll(BG_B, 0);

    int tile_offset = (level - 1) % 10;
    const u8* piece = pieces[next_piece];
    int x, y, i, t;
    for(i = 8; i < 16; ++i) {
        x = (i % piece_size) + 28;
        y = (i / piece_size) + 12;
        t = piece[i];
        if(t) {
            VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL2, 0, 0, 0, tile_indices[(t - 1) + tile_offset * 4]), x, y);
        } else {
            VDP_setTileMapXY(BG_B, 0, x, y);
        }
    }

    tile_offset = (level - 1) % 10;
    for(int j = 0; j < 7; ++j) {
        piece = pieces[j];
        int x, y, i, t;
        for(i = 8; i < 16; ++i) {
            x = (i % piece_size) + 7;
            y = (i / piece_size) + 4 + (j * 3) - (j > 0 ? 1 : 0);
            t = piece[i];
            if(t) {
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, tile_indices[(t - 1) + tile_offset * 4]), x, y);
            } else {
                VDP_setTileMapXY(BG_A, 0, x, y);
            }
        }
    }

}

void write_current_piece(u32* data) {
    const u8* piece = pieces[current_piece];
    int local_x, local_y;
    int off = piece_off * current_rotation;
    int i;
    for(i = off; i < off + piece_off; ++i) {
        if(piece[i]) {
            local_x = (i % piece_size) + posx;
            local_y = (i - off) / piece_size + posy;
            data[local_y * 10 + local_x] = piece[i];
        }
    }
}

int get_collision(int posx, int posy, int rot) {
    const u8* piece = pieces[current_piece];
    int local_x, local_y;
    int off = piece_off * rot;
    int i;
    for(i = off; i < off + piece_off; ++i) {
        if(piece[i]) {
            local_y = (i - off) / piece_size + posy;
            if(local_y > 19) return 1;          // bottom collision
            local_x = (i % piece_size) + posx;
            if(local_x < 0) return 2;           // left collision
            if(local_x > 9) return 3;           // right collision
            if(board[local_y * 10 + local_x]) { // piece collision
                return 4;
            }
        }
    }
    return 0;
}

void draw_board() {

    int i, t;
    for(i = 0; i < 200; ++i) render_board[i] = 0; // clear
    write_current_piece(render_board); // write current piece
    int tile_offset = (level - 1) % 10;
    int x = 0, y = 0;
    for(i = 0; i < board_length; ++i) {
        x = (i % 10) + 16;
        y = (i / 10) +  5;
        t = board[i] | render_board[i];
        if(t)
            VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL2, 0, 0, 0, tile_indices[(t - 1) + tile_offset * 4] ), x, y);
        else
            VDP_setTileMapXY(BG_A, 0, x, y);
    }

    update_text();

}

void clear_full_rows() {
    int _lines = 0;
    int x, y, i;
    int clear_row;
    for(y = 0; y < 20; ++y) {
        clear_row = 1;
        for(x = 0; x < 10; ++x) {
            i = y * 10 + x;
            if(!board[i]) {
                clear_row = 0;
                break;
            }
        }

        if(clear_row) {
            int i2 = i - 10;
            for(;i2>-1;i2--) board[i--] = board[i2];
            for(i = 0; i < 10; ++i) {
                board[i] = 0;
            }
            _lines ++;
            if(!((lines + _lines) % 10)) {
                level++;
                if(level > 99) {
                    level = 1;
                }
            }
        }
    }
    // update score
    lines += _lines;
    if(lines > 999) {
        lines = 999;
    }
    score += _lines * 100;
    if(score > 999999) {
        score = 999999;
    }
    if(score > top) {
        top = score;
    }
}

void update_text() {

    char text[32];
    sprintf(text, "LINES-%03d", lines);
    VDP_drawText(text, 17, 2);
    sprintf(text, "%06lu", top);
    VDP_drawText(text  , 28, 4);
    sprintf(text, "%06lu", score);
    VDP_drawText(text   , 28, 7);
    sprintf(text, "%02d", level);
    VDP_drawText(text, 30, 20);

    for(int i = 0; i < 7; ++i) {
        sprintf(text, "%03d", statistics[i]);
        VDP_drawText(text, 11, 6 + i * 3);
    }

}

void game_over() {

    score = 0;
    lines = 0;
    level = 1;
    for(int i = 0; i < 200; ++i) board[i] = 0;
    for(int i = 0; i < 7; ++i) statistics[i] = 0;

}

// handle joy events
void joyEvent(u16 joy, u16 changed, u16 state) {

    if (state & BUTTON_LEFT) {
        if(left_key == 0) {
            left_key = 1;
        }
    } else if (changed & BUTTON_LEFT) {
        left_key = 0;
    }

    if (state & BUTTON_RIGHT) {
        if(right_key == 0) {
            right_key = 1;
        }
    } else if (changed & BUTTON_RIGHT) {
        right_key = 0;
    }

    if (state & BUTTON_DOWN) {
        down_key = 1;
    } else if (changed & BUTTON_DOWN) {
        down_key = 0;
    }

    if (state & (BUTTON_A | BUTTON_B | BUTTON_C)) {
        if(rot_key == 0) {
            rot_key = 1;
        }
    } else if (changed & (BUTTON_A | BUTTON_B | BUTTON_C)) {
        rot_key = 0;
    }

}
