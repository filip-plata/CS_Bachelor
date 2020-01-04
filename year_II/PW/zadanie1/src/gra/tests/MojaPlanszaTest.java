package gra.tests;

import gra.*;
import org.junit.Test;

import java.util.*;

import static gra.Kierunek.*;
import static junit.framework.TestCase.assertEquals;

public class MojaPlanszaTest {

    private static volatile int exceptions = 0;

    @Test
    public void testMovingBigCharacters() {
        exceptions = 0;
        int caught = 0;
        final Plansza board = new MojaPlansza(100, 100);
        final Postać character1 = new MojaPostać(10,10,1);
        final Postać character2 = new MojaPostać(20,15,2);

        try {
            board.postaw(character1, 1, 1);
            board.postaw(character2, 11, 1);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        try {
            for (int i = 0; i < 2; i++) {
                board.przesuń(character1, Kierunek.LEWO);
            }
        } catch (IllegalArgumentException e) {
            caught++;
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {
            for (int i = 0; i < 8; i++) {
                board.przesuń(character2, Kierunek.PRAWO);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        Thread t1 = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    board.przesuń(character2, Kierunek.GÓRA);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } catch (DeadlockException e) {
                    exceptions++;
                    board.usuń(character2);
                }
            }
        });

        Thread t2 = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    board.przesuń(character1, Kierunek.DÓŁ);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } catch (DeadlockException e) {
                    exceptions++;
                    board.usuń(character1);
                }
            }
        });

        t2.start(); t1.start();
        try {
            t2.join(); t1.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        assertEquals(caught, 1);
        assertEquals(exceptions, 1);
    }

    @Test
    public void testMovingCharacter() {
        int caught = 0;
        Plansza board = new MojaPlansza(100, 100);
        Postać character = new MojaPostać(10, 10, 1);

        try{
            board.postaw(character, 1, 1);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        try {
            board.przesuń(character, Kierunek.LEWO);
        } catch (IllegalArgumentException e) {
            caught++;
        } catch (InterruptedException | DeadlockException e) {
            e.printStackTrace();
        }

        try {
            board.przesuń(character, Kierunek.LEWO);
        } catch (IllegalArgumentException e) {
            caught++;
        } catch (DeadlockException|InterruptedException e) {
            e.printStackTrace();
        }

        try {
            board.przesuń(character, Kierunek.GÓRA);
        } catch (IllegalArgumentException e) {
            caught++;
        } catch (DeadlockException|InterruptedException e) {
            e.printStackTrace();
        }

        try {
            board.przesuń(character, Kierunek.GÓRA);
        } catch (IllegalArgumentException e) {
            caught++;
        } catch (DeadlockException|InterruptedException e) {
            e.printStackTrace();
        }


        assertEquals(caught, 2);
    }

    @Test
    public void testDeadlockTwo() {
        exceptions = 0;
        final Plansza board = new MojaPlansza(2, 2);
        final Postać character1 = new MojaPostać(1, 1, 1);
        final Postać character2 = new MojaPostać(1, 1, 2);

        try{
            board.postaw(character1, 0, 0);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        try{
            board.postaw(character2, 0, 1);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Thread t1 = new Thread() {
            public void run() {
                try {
                    board.przesuń(character1, Kierunek.PRAWO);
                } catch (InterruptedException|DeadlockException e) {
                    exceptions++;
                    board.usuń(character1);
                }
            }
        };

        Thread t2 = new Thread() {
            public void run() {
                try {
                    board.przesuń(character2, Kierunek.LEWO);
                } catch (InterruptedException |
                        DeadlockException e) {
                    exceptions++;
                    board.usuń(character2);
                }
            }
        };

        t1.start();
        t2.start();

        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        assertEquals(exceptions, 1);
    }

    @Test
    public void testRemovingTwice() {
        int invalidArgumentExceptions = 0;

        Plansza board = new MojaPlansza(25, 300);
        Postać characterBig = new MojaPostać(2, 1, 3);

        try{
            board.postaw(characterBig, 15, 12);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        board.usuń(characterBig);
        try {
            board.usuń(characterBig);
        } catch (IllegalArgumentException e) {
            invalidArgumentExceptions++;
        }

        assertEquals(invalidArgumentExceptions, 1);
    }

    @Test
    public void invalidMoveAfterRemove() {
        int invalidArgumentExceptions = 0;

        Plansza board = new MojaPlansza(25, 20);
        Postać character = new MojaPostać(2, 1, 3);

        try{
            board.postaw(character, 22, 18);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        board.usuń(character);

        try{
            board.przesuń(character, Kierunek.LEWO);
        } catch (InterruptedException|DeadlockException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            invalidArgumentExceptions++;
        }

        assertEquals(invalidArgumentExceptions, 1);
    }

    @Test
    public void pointOutOfBounds() {
        int illegalArgumentExceptions = 0;
        Plansza board = new MojaPlansza(10, 10);
        Postać character = new MojaPostać(2, 7, 1);

        try {
            board.sprawdź(10, 8, new EmptyAction(), new Runnable() {
                @Override
                public void run() {

                }
            });
        } catch (IllegalArgumentException e) {
            illegalArgumentExceptions++;
        }

        try {
            board.sprawdź(-1, 4, new EmptyAction(), new Runnable() {
                @Override
                public void run() {

                }
            });
        } catch (IllegalArgumentException e) {
            illegalArgumentExceptions++;
        }

        try {
            board.sprawdź(0, 10, new EmptyAction(), new Runnable() {
                @Override
                public void run() {

                }
            });
        } catch (IllegalArgumentException e) {
            illegalArgumentExceptions++;
        }

        try {
            board.postaw(character, 4, 5);
        } catch (IllegalArgumentException e) {
            illegalArgumentExceptions++;
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        try {
            board.postaw(character, 2, 9);
        } catch (IllegalArgumentException e) {
            illegalArgumentExceptions++;
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        try {
            board.postaw(character, 2, -1);
        } catch (IllegalArgumentException e) {
            illegalArgumentExceptions++;
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        try {
            board.postaw(character, -2457575, -1646);
        } catch (IllegalArgumentException e) {
            illegalArgumentExceptions++;
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        assertEquals(7, illegalArgumentExceptions);
    }

    @Test
    public void walkingAroundNoExceptions() {
        int CHAR_NUMBER = 2;
        Thread[] threads = new Thread[CHAR_NUMBER];
        final Plansza board = new MojaPlansza(3, 3);
        final int[]  position_x = new int[CHAR_NUMBER];
        final int[]  position_y = new int[CHAR_NUMBER];
        final int[] width = new int[CHAR_NUMBER];
        final int[] height = new int[CHAR_NUMBER];
        final Map<Integer, List<Kierunek>> moves = new HashMap<>();
        position_x[0] = 1; position_y[0] = 1;
        position_x[1] = 0; position_y[1] = 2;
        width[0] = 1; width[1] = 1;
        height[0] = 1; height[1] = 1;
        List<Kierunek> moves0 = new ArrayList<>();
        moves0.add(Kierunek.GÓRA); moves0.add(DÓŁ); moves0.add(DÓŁ); moves0.add(GÓRA);
        List<Kierunek> moves1 = new ArrayList<>();
        moves1.add(PRAWO); moves1.add(PRAWO); moves1.add(GÓRA); moves1.add(GÓRA);
        moves1.add(LEWO); moves1.add(LEWO); moves1.add(DÓŁ); moves1.add(DÓŁ);

        moves.put(0, moves0);
        moves.put(1, moves1);

        for (int i=0; i<CHAR_NUMBER; i++) {
            final int id = i;
            threads[i] = new Thread() {
                public void run() {
                    Postać character = new MojaPostać(width[id], height[id], id);
                    try {
                        board.postaw(character, position_y[id], position_x[id]);
                        for (int repeat = 0; repeat < 10000; repeat++) {
                            for (Kierunek dir : moves.get(id)) {
                                board.przesuń(character, dir);
                            }
                        }
                        board.usuń(character);
                    } catch (InterruptedException|DeadlockException e) {
                        e.printStackTrace();
                    }
                }
            };

            threads[i].start();
        }

        try {
            for (int i = 0; i < CHAR_NUMBER; i++) {
                threads[i].join();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}