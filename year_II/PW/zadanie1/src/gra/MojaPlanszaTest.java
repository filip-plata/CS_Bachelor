package gra;

import org.junit.Test;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.CyclicBarrier;

import static org.junit.Assert.assertTrue;

/**
 * Created by adi on 05.12.16.
 */
public class MojaPlanszaTest {

    Thread[] threads;
    CyclicBarrier barrier;
    Plansza plansza;
    CountDownLatch deadlockLatch;

    Akcja ak(final boolean isOK) {
        return new Akcja() {
            @Override
            public void wykonaj(Postać postać) {
                if (!isOK) throw new RuntimeException();
            }
        };
    }

    public void log(String name, String what) {
        System.out.print(name + ": " + what + "\n");
    }

    public void log(String s) {
        System.out.print(s + "\n");
    }

    private Runnable run(final boolean isOK) {
        return new Runnable() {
            @Override
            public void run() {
                if (!isOK) {
                    throw new RuntimeException();
                }
            }
        };
    }

    interface Run {
        void run() throws Exception;
    }

    public void run(int i, final Run run) {
        threads[i] = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    run.run();
                } catch (Exception e) {
                    if (e instanceof DeadlockException) {
                        deadlockLatch.countDown(); // signal that deadlock was caught
                    }
                    e.printStackTrace();
                     throw new RuntimeException();
                }
            }
        });
        threads[i].start();
    }

    public void join() throws InterruptedException {
        for (Thread t : threads) {
            t.join();
        }
    }

    // nie @Before bo chcę odpalać z różnymi parametrami
    public void setup(int size, int mapSize) {
        log("setup " + size);
        deadlockLatch = new CountDownLatch(1);
        plansza = new MojaPlansza(mapSize, mapSize);
        threads = new Thread[size];
        if (size > 0) barrier = new CyclicBarrier(size);
    }

    public void testBasics() throws Exception {
        setup(0, 30);

        Postać p = new MojaPostać(10, 10);

        plansza.postaw(p, 1, 1);
        plansza.usuń(p);
        plansza.postaw(p, 1, 1);

        plansza.sprawdź(5, 5, ak(true), run(false));
        plansza.sprawdź(15, 15, ak(false), run(true));
    }

    @Test
    public void testBasics2() throws Exception {
        setup(0, 30);

        plansza.postaw(new MojaPostać(1, 1), 1, 1);
        plansza.postaw(new MojaPostać(1, 1), 3, 3);
        plansza.postaw(new MojaPostać(1, 1), 5, 5);
    }

    @Test
    public void testIsInMap() throws InterruptedException, DeadlockException {
        setup(0, 3);

        tesIsInMapDirection(Kierunek.PRAWO);
        tesIsInMapDirection(Kierunek.LEWO);
        tesIsInMapDirection(Kierunek.GÓRA);
        tesIsInMapDirection(Kierunek.DÓŁ);
    }

    private void tesIsInMapDirection(final Kierunek k) throws InterruptedException, DeadlockException {
        final Postać p = new MojaPostać(1, 1);
        log("postaw");
        plansza.postaw(p, 1, 1);
        log("przed przesun");
        plansza.przesuń(p, k);
        log("po przesun");
        assertTrue(catchIllegal(new Run() {

            @Override
            public void run() throws InterruptedException, DeadlockException {
                log(k + "");
                plansza.przesuń(p, k);
                log("poszło");
            }
        }));
    }

    private boolean catchIllegal(Run run) {
        try {
            run.run();
        } catch (IllegalArgumentException e) {
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return false;
    }

    @Test
    public void testBasics3() throws Exception {
        setup(0, 30);

        Postać p = new MojaPostać(2, 2);

        plansza.postaw(p, 1, 1);
        plansza.przesuń(p, Kierunek.PRAWO);
    }

    @Test
    public void testThreads1() throws Exception {
        setup(3, 8);

        run(0, new Run() {
            @Override
            public void run() throws InterruptedException, BrokenBarrierException {
                Postać p = new MojaPostać(2, 2);
                plansza.postaw(p, 1, 1);
                log("1 stoi");
                barrier.await();
                try {
                    plansza.przesuń(p, Kierunek.PRAWO);
                    log("1 po ruchu");
                } catch (DeadlockException e) {
                    throw new RuntimeException("Fake deadlock");
                }
            }
        });

        run(1, new Run() {
            @Override
            public void run() throws InterruptedException, BrokenBarrierException {
                Postać p = new MojaPostać(2, 2);
                plansza.postaw(p, 3, 1);
                log("2 stoi");
                barrier.await();
                try {
                    plansza.przesuń(p, Kierunek.PRAWO);
                    log("2 po ruchu");
                } catch (DeadlockException e) {
                    throw new RuntimeException("Fake deadlock");
                }
            }
        });

        run(2, new Run() {
            @Override
            public void run() throws InterruptedException, BrokenBarrierException {
                Postać p = new MojaPostać(2, 2);
                plansza.postaw(p, 5, 1);
                log("3 stoi");
                barrier.await();
                try {
                    log("3 przed ruchem");
                    plansza.przesuń(p, Kierunek.PRAWO);
                    log("3 po ruchu");
                } catch (DeadlockException e) {
                    throw new RuntimeException("Fake deadlock");
                }
            }
        });

        join();
    }

    @Test
    public void testThreads2() throws InterruptedException {
    setup(2, 4);

        run(0, new Run() {
            @Override
            public void run() throws BrokenBarrierException, InterruptedException {
                Postać p = new MojaPostać(2, 2);
                plansza.postaw(p, 1, 1);
                log("0 stoi");
                barrier.await();
                log("0 przed przesuń");
                try {
                    plansza.przesuń(p, Kierunek.PRAWO);
                } catch (DeadlockException e) {
                    throw new RuntimeException("Fake deadlock");
                }
            }
        });

        run(1, new Run() {
            @Override
            public void run() throws InterruptedException, BrokenBarrierException {
                Postać p = new MojaPostać(2, 2);
                log("1 czeka na 0...");
                barrier.await(); // czekam aż 0 na pewno jest na mapie
                log("1 przed postaw");
                plansza.postaw(p, 1, 0); // próbuję postawić ale 0 może blokować
            }
        });

        join();
    }

    @Test
    public void testThreads3() throws InterruptedException {
        setup(3, 7);

        run(0, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(2, 2);
                plansza.postaw(p, 3, 1);
                barrier.await(); // czekam aż wszyscy będą na planszy
                plansza.przesuń(p, Kierunek.DÓŁ);
                plansza.przesuń(p, Kierunek.DÓŁ);
            }
        });

        run(1, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(2, 2);
                plansza.postaw(p, 3, 3);
                barrier.await();
                plansza.przesuń(p, Kierunek.DÓŁ);
                plansza.przesuń(p, Kierunek.DÓŁ);
            }
        });

        run(2, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(2, 2);
                plansza.postaw(p, 1, 1);
                barrier.await();
                plansza.przesuń(p, Kierunek.PRAWO);
                plansza.przesuń(p, Kierunek.PRAWO);
            }
        });

        join();
    }

    /* Dwa klocki zamieniają się miejscami */
    @Test
    public void testThreads4() throws InterruptedException {
        setup(2, 6);

        run(0, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(2, 2);
                log("0 przed stawiam na 1 1");
                plansza.postaw(p, 1, 1);
                log("0 przed lewo");
                plansza.przesuń(p, Kierunek.LEWO);
                log("0 przed prawo");
                plansza.przesuń(p, Kierunek.PRAWO);
                barrier.await(); // czekam aż 1 będzie na planszy
                log("0 przed prawo");
                plansza.przesuń(p, Kierunek.PRAWO);
                log("0 przed prawo");
                plansza.przesuń(p, Kierunek.PRAWO);
            }
        });

        run(1, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(2, 2);
                log("1 przed postaw na 3 1");
                plansza.postaw(p, 1, 3);
                barrier.await(); // upewniam się że 1 uda się stanąć na planszy
                log("1 przed dol");
                plansza.przesuń(p, Kierunek.DÓŁ);
                log("1 przed dol");
                plansza.przesuń(p, Kierunek.DÓŁ);
                log("1 przed lewo");
                plansza.przesuń(p, Kierunek.LEWO);
                log("1 przed lewo");
                plansza.przesuń(p, Kierunek.LEWO);
                log("1 przed gora");
                plansza.przesuń(p, Kierunek.GÓRA);
                log("1 przed gora");
                plansza.przesuń(p, Kierunek.GÓRA);
            }
        });

        join();
    }

    @Test
    public void testIllegalMove() throws InterruptedException {
        setup(1, 3);

        run(0, new Run() {
            @Override
            public void run() throws Exception {
                final Postać p = new MojaPostać(1, 3);
                plansza.postaw(p, 0, 0);
                plansza.przesuń(p, Kierunek.PRAWO);
                plansza.przesuń(p, Kierunek.PRAWO);
                plansza.przesuń(p, Kierunek.LEWO);
                plansza.przesuń(p, Kierunek.LEWO);

                assertTrue(catchIllegal(new Run() {
                    @Override
                    public void run() throws Exception {
                        plansza.przesuń(p, Kierunek.GÓRA);
                    }
                }));

                assertTrue(catchIllegal(new Run() {
                    @Override
                    public void run() throws Exception {
                        plansza.przesuń(p, Kierunek.DÓŁ);
                    }
                }));

                assertTrue(catchIllegal(new Run() {
                    @Override
                    public void run() throws Exception {
                        plansza.przesuń(p, Kierunek.LEWO);
                    }
                }));
            }
        });

        join();
    }

    @Test
    public void testDeadlock1() throws InterruptedException {
        setup(4, 4);

        run(0, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(2, 2);
                plansza.postaw(p, 0, 0);
                barrier.await(); // upewniam się że wszyscy są na mapie
                plansza.przesuń(p, Kierunek.PRAWO); // ruch do deadlocka
            }
        });

        run(1, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(2, 2);
                plansza.postaw(p, 0, 2);
                barrier.await(); // upewniam się że wszyscy są na mapie
                plansza.przesuń(p, Kierunek.DÓŁ);// ruch do deadlocka
            }
        });

        run(2, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(2, 2);
                plansza.postaw(p, 2, 2);
                barrier.await(); // upewniam się że wszyscy są na mapie
                plansza.przesuń(p, Kierunek.LEWO); // ruch do deadlocka
            }
        });

        run(3, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(2, 2);
                plansza.postaw(p, 2, 0);
                barrier.await(); // upewniam się że wszyscy są na mapie
                plansza.przesuń(p, Kierunek.GÓRA); // ruch do deadlocka
            }
        });

        deadlockLatch.await();
    }

    @Test
    public void testThreads5() throws InterruptedException {
        setup(4, 6);

        run(0, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(3, 3);
                plansza.postaw(p, 0, 0);
                barrier.await();
                plansza.przesuń(p, Kierunek.PRAWO);
                plansza.przesuń(p, Kierunek.PRAWO);
                barrier.await();
                plansza.przesuń(p, Kierunek.LEWO);
                plansza.przesuń(p, Kierunek.LEWO);
            }
        });

        run(1, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(1, 2);
                plansza.postaw(p, 2, 3);
                barrier.await();
                plansza.przesuń(p, Kierunek.DÓŁ);
                barrier.await();
                plansza.przesuń(p, Kierunek.GÓRA);
            }
        });

        run(2, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(1, 1);
                plansza.postaw(p, 4, 3);
                barrier.await();
                plansza.przesuń(p, Kierunek.PRAWO);
                barrier.await();
                plansza.przesuń(p, Kierunek.LEWO);
            }
        });

        run(3, new Run() {
            @Override
            public void run() throws Exception {
                Postać p = new MojaPostać(1, 4);
                plansza.postaw(p, 1, 4);
                barrier.await();
                plansza.przesuń(p, Kierunek.PRAWO);
                barrier.await();
                plansza.przesuń(p, Kierunek.LEWO);
            }
        });

        join();
    }
}

class MojaPostać implements Postać {

    int w, h;

    MojaPostać(int w, int h) {
        this.w = w;
        this.h = h;
    }

    @Override
    public int dajWysokość() {
        return h;
    }

    @Override
    public int dajSzerokość() {
        return w;
    }
}