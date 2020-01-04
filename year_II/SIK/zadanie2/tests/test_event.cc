#include <iostream>
#include <stdexcept>

#include <cassert>
#include <cstring>
#include "../event.h"
#include "test_event.h"

static const std::size_t BSIZE = 1000;
static char buf[BSIZE];


void test_pixel_load() {
    Pixel pixel;

    assert (pixel.event_type() == 1);
    /* Player number */
    buf[0] = 0x01;
    /* x */
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x04;
    buf[4] = 0x00;
    /* y */
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x04;
    buf[8] = 0x02;

    try {
        pixel.load(buf, 9);
        assert (true);
    } catch (std::exception &e) {
        assert (false);
    }

    assert(pixel.player_number == 1);
    assert(pixel.x == 1024);
    assert(pixel.y == 1026);
}

void test_load_new_game() {
    NewGame game;
    const char *pl1 = "Adrian";
    const char *pl2 = "Filip";

    assert (game.event_type() == 0);

    /* maxx */
    buf[0] = 0x00;
    buf[1] = 0x01;
    buf[2] = 0x17;
    buf[3] = 0x11;
    /* maxy */
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x1A;
    buf[7] = 0xFA;
    /* player names with sanity checks */
    strcpy(buf + 8, pl1);
    assert (buf[8] != 0x00);
    assert (buf[13] != 0x00);
    assert (buf[14] == 0x00);
    strcpy(buf + 15, pl2);
    assert (buf[15] != 0x00);

    try {
        game.load(buf, 21);
    } catch (std::exception &e) {
        assert (false);
    }

    assert (game.maxx == 71441);
    assert (game.maxy == 6906);

    assert(game.players.size() == 2);
    assert(game.players[0] == "Adrian");
    assert(game.players[1] == "Filip");

    memset(buf, 0, sizeof(buf));

    game.serialize_text(buf);
    std::string expected("NEW_GAME 71441 6906 Adrian Filip\n");
    std::string real(buf);
    assert (expected == real);

    memset(buf, 0, sizeof(buf));

    /* maxx */
    buf[0] = 0x00;
    buf[1] = 0x01;
    buf[2] = 0x17;
    buf[3] = 0x11;
    /* maxy */
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x1A;
    buf[7] = 0xFA;
    /* player names with sanity checks */
    strcpy(buf + 8, pl1);
    assert (buf[8] != 0x00);
    assert (buf[15] == 0x00);
    strcpy(buf + 16, pl2);
    assert (buf[16] != 0x00);

    buf[10] = 0x15;

    try {
        game.load(buf, 21);
        assert(false);
    } catch (InvalidLoad &e) {
        assert(true);
    }

    strcpy(buf + 8, pl1);
    assert (buf[8] != 0x00);
    assert (buf[15] == 0x00);
    strcpy(buf + 16, pl2);
    assert (buf[16] != 0x00);

    buf[16] = 0x00;

    try {
        game.load(buf, 21);
        assert(false);
    } catch (InvalidLoad &e) {
        assert(true);
    }

    memset(buf, 0, sizeof(buf));
}

void test_load_player_eliminated() {
    PlayerEliminated eliminated;

    assert (eliminated.event_type() == 2);

    buf[0] = 0x13;

    try {
        eliminated.load(buf, 1);
        assert (true);
    } catch (std::exception &e) {
        assert (false);
    }
    assert (eliminated.player_number == 19);
}

void test_load_game_over() {
    GameOver game;
    assert (game.event_type() == 3);
}

void test_load_event_pixel() {
    Event event;

    /* len */
    buf[0] = 0x00;
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x0E;
    /* event_no */
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x7B;
    buf[7] = 0x14;
    /* type */
    buf[8] = 0x01;

    /* Player number */
    buf[9] = 0x01;
    /* x */
    buf[10] = 0x00;
    buf[11] = 0x00;
    buf[12] = 0x04;
    buf[13] = 0x00;
    /* y */
    buf[14] = 0x00;
    buf[15] = 0x00;
    buf[16] = 0x04;
    buf[17] = 0x02;
    /* crc32 */
    buf[18] = 0x28;
    buf[19] = 0xD5;
    buf[20] = 0xDC;
    buf[21] = 0xC8;

    try {
        event.load(buf, 22);
        assert (true);
    } catch (std::exception &e) {
        assert (false);
    }

    assert (event.len == 14);
    assert (event.event_no == 31508);
    assert (event.type == 1);

    assert (event.data->event_type() == 1);
}

void test_load_event_new_game() {
    Event event;
    std::size_t size(0);

    /* len */
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x19;
    /* event_no */
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x7B;
    buf[size++] = 0x00;
    /* type */
    buf[size++] = 0x00;

    /* maxx*/
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x04;
    buf[size++] = 0x00;
    /* maxy */
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x04;
    buf[size++] = 0x02;
    /* Player names */
    buf[size++] = 'F';
    buf[size++] = 'i';
    buf[size++] = 'l';
    buf[size++] = 'i';
    buf[size++] = 'p';
    buf[size++] = '\0';

    buf[size++] = 'F';
    buf[size++] = 'i';
    buf[size++] = 'l';
    buf[size++] = 'i';
    buf[size++] = 'p';
    buf[size++] = '\0';
    /* crc32 */
    buf[size++] = 0x26;
    buf[size++] = 0x0D;
    buf[size++] = 0xD4;
    buf[size++] = 0x4F;

    try {
        event.load(buf, size);
        assert (true);
    } catch (std::exception &e) {
        assert (false);
    }

    assert (event.len == 25);
    assert (event.event_no == 31488);
    assert (event.type == 0);

    assert (event.data->event_type() == 0);
}

void test_load_event_game_over() {
    Event event;
    std::size_t size(0);

    /* len */
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x05;
    /* event_no */
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x7B;
    buf[size++] = 0x00;
    /* type */
    buf[size++] = 0x03;
    /* crc32 */
    buf[size++] = 0x77;
    buf[size++] = 0x69;
    buf[size++] = 0x53;
    buf[size++] = 0x16;

    try {
        event.load(buf, 13);
        assert (true);
    } catch (std::exception &e) {
        assert (false);
    }

    assert (event.type == 3);
}

void test_event_invalid_load() {
    Event event;
    std::size_t size(0);

    /* len */
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x05;
    /* event_no */
    buf[size++] = 0x01;
    buf[size++] = 0x10;
    buf[size++] = 0x7B;
    buf[size++] = 0x0F;
    /* type */
    buf[size++] = 0x07;
    /* crc32 */
    buf[size++] = 0x9A;
    buf[size++] = 0xE5;
    buf[size++] = 0xF5;
    buf[size++] = 0xEF;

    try {
        event.load(buf, size);
        assert (false);
    } catch (InvalidType &e) {
        assert (true);
    }
}

void test_invalid_checksum_crc() {
    Event event;
    std::size_t size(0);

    /* len */
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x05;
    /* event_no */
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x7B;
    buf[size++] = 0x00;
    /* type */
    buf[size++] = 0x03;
    /* crc32 */
    buf[size++] = 0x77;
    buf[size++] = 0x69;
    buf[size++] = 0x5A;
    buf[size++] = 0x16;

    try {
        event.load(buf, 13);
        assert (false);
    } catch (InvalidCrc &e) {
        assert (true);
    }

    assert (event.type == 3);
}

void test_serialize_event_pixel() {
    std::size_t size_ser, size(0);
    Pixel px, px_copy;
    px.player_number = 3;
    px.x = 1087;
    px.y = 87646;
    px.player_name = "Filip";

    px.serialize_text(buf);
    std::string expected("PIXEL 1087 87646 Filip\n");
    std::string real(buf);
    assert (expected == real);

    memset(buf, 0, sizeof(buf));

    size_ser = px.serialize(buf);

    assert(size_ser == 9);
    try {
        px_copy.load(buf, size_ser);
        assert (true);
    } catch (InvalidLoad &e) {
        assert (false);
    }
    assert(px_copy.player_number == px.player_number);
    assert(px_copy.x == px.x);
    assert(px_copy.y == px.y);
    /* player_number */
    assert(buf[size++] == 0x03);

    /* x */
    assert(buf[size++] == (char)0x00);
    assert(buf[size++] == (char)0x00);
    assert(buf[size++] == (char)0x04);
    assert(buf[size++] == (char)0x3F);
    /* y */
    assert(buf[size++] == (char)0x00);
    assert(buf[size++] == (char)0x01);
    assert(buf[size++] == (char)0x56);
    assert(buf[size++] == (char)0x5E);
}

void test_serialize_event_game_over() {
  std::size_t size_ser;
  GameOver game, game_copy;


  memset(buf, 0, sizeof(buf));

  size_ser = game.serialize(buf);

  assert(size_ser == 0);
  try {
      game_copy.load(buf, size_ser);
      assert (true);
  } catch (InvalidLoad &e) {
      assert (false);
  }
}

void test_serialize_event_new_game() {
  std::size_t size_ser, size(0);
  NewGame game, game_copy;
  game.maxx = 46843;
  game.maxy = 876445;
  game.players = std::vector<std::string>({"player1", "player2", "player3"});

  game.serialize_text(buf);
  std::string expected("NEW_GAME 46843 876445 player1 player2 player3\n");
  std::string real(buf);
  assert (expected == real);

  memset(buf, 0, sizeof(buf));

  size_ser = game.serialize(buf);

  assert(size_ser == 32);
  try {
      game_copy.load(buf, size_ser);
      assert (true);
  } catch (InvalidLoad &e) {
      assert (false);
  }
  assert(game.players == game_copy.players);
  assert(game.maxx == game_copy.maxx);
  assert(game.maxy == game_copy.maxy);

  /* maxx */
  assert(buf[size++] == (char)0x00);
  assert(buf[size++] == (char)0x00);
  assert(buf[size++] == (char)0xB6);
  assert(buf[size++] == (char)0xFB);
  /* maxy */
  assert(buf[size++] == (char)0x00);
  assert(buf[size++] == (char)0x0D);
  assert(buf[size++] == (char)0x5F);
  assert(buf[size++] == (char)0x9D);
  /* player names */
  assert(buf[size++] == 'p');
  assert(buf[size++] == 'l');
  assert(buf[size++] == 'a');
  assert(buf[size++] == 'y');
  assert(buf[size++] == 'e');
  assert(buf[size++] == 'r');
  assert(buf[size++] == '1');
  assert(buf[size++] == (char)0x00);

  /* Ending */
  assert(buf[size_ser] == (char)0x00);
}

void test_serialize_event_player_eliminated() {
  std::size_t size_ser, size(0);
  PlayerEliminated pl, pl_copy;
  pl.player_number = 17;
  pl.player_name = "player1";

  pl.serialize_text(buf);
  std::string expected("PLAYER_ELIMINATED player1\n");
  std::string real(buf);
  assert (expected == real);

  memset(buf, 0, sizeof(buf));

  size_ser = pl.serialize(buf);

  assert(size_ser == 1);
  try {
      pl_copy.load(buf, size_ser);
      assert (true);
  } catch (InvalidLoad &e) {
      assert (false);
  }
  assert(pl.player_number == pl_copy.player_number);

  /* player_number */
  assert(buf[size++] == (char)0x11);
}


void test_event() {
    test_pixel_load();
    memset(buf, 0, sizeof(buf));
    test_load_new_game();
    memset(buf, 0, sizeof(buf));
    test_load_player_eliminated();
    memset(buf, 0, sizeof(buf));
    test_load_game_over();
    memset(buf, 0, sizeof(buf));
    test_load_event_pixel();
    memset(buf, 0, sizeof(buf));
    test_load_event_new_game();
    memset(buf, 0, sizeof(buf));
    test_load_event_game_over();
    memset(buf, 0, sizeof(buf));
    test_event_invalid_load();
    memset(buf, 0, sizeof(buf));
    test_invalid_checksum_crc();
    memset(buf, 0, sizeof(buf));
    test_serialize_event_pixel();
    memset(buf, 0, sizeof(buf));
    test_serialize_event_game_over();
    memset(buf, 0, sizeof(buf));
    test_serialize_event_new_game();
    memset(buf, 0, sizeof(buf));
    test_serialize_event_player_eliminated();
    memset(buf, 0, sizeof(buf));
}
