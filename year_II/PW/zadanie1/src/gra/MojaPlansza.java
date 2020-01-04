package gra;

import java.util.*;

public class MojaPlansza implements Plansza {
    private int width;
    private int height;
    private Postać[][] field;
    private Map<Postać, TaskMove> waitingTasksMove;
    private Map<Postać, Position> characters;

    public MojaPlansza(int wysokość, int szerokość) {
        width = szerokość;
        height = wysokość;
        characters = new HashMap<>();
        field = new Postać[szerokość][wysokość];
        waitingTasksMove = new HashMap<>();
    }

    public synchronized void postaw(Postać postać, int wiersz, int kolumna) throws InterruptedException {
        boolean inside_width = 0 <= kolumna && kolumna + postać.dajSzerokość() <= width;
        boolean inside_height = 0 <= height && wiersz + postać.dajWysokość() <= height;

        if (characters.get(postać) != null || !inside_height || !inside_width) {
            throw new IllegalArgumentException();
        }

        TaskInsert task = new TaskInsert(postać, kolumna, wiersz, this);

        while (!task.performIfPossible()) {
            wait();
        }
    }

    private Set<Postać> blockingNodes(TaskMove task) {
        HashSet<Postać> result = new HashSet<>();
        Collection<Position> newPositions = task.newPositions();

        for (Position pos : newPositions) {
            result.add(field[pos.x][pos.y]);
        }

        return result;
    }

    private boolean deadlockDetected() {
        Map<Postać, Set<Postać>> edges = new HashMap<>();
        for (Map.Entry<Postać, TaskMove> t : waitingTasksMove.entrySet()) {
            Set<Postać> nodeEdges = blockingNodes(t.getValue());
            edges.put(t.getKey(), nodeEdges);
        }
        return (new DirectedGraph<>(edges)).isCyclic();
    }

    public synchronized void przesuń(Postać postać, Kierunek kierunek)
            throws InterruptedException, DeadlockException {
        Position position = characters.get(postać);

        if (position == null || !isValidMove(postać, position, kierunek)) {
            throw new IllegalArgumentException();
        }

        TaskMove task = new TaskMove(postać, position, kierunek, this);

        waitingTasksMove.put(postać, task);

        if (deadlockDetected()) {
            waitingTasksMove.remove(postać);
            throw new DeadlockException();
        }

        while (!task.performIfPossible()) {
            wait();
        }

        waitingTasksMove.remove(postać);

        notifyAll();
    }

    public synchronized void usuń(Postać postać) {
        Position position = characters.get(postać);

        if (position == null) {
            throw new IllegalArgumentException();
        } else {
            for (int i=0; i< postać.dajSzerokość(); i++) {
                for (int j=0; j<postać.dajWysokość(); j++) {
                    field[position.x + i][position.y + j] = null;
                }
            }

            characters.remove(postać);
            waitingTasksMove.remove(postać);
        }
        notifyAll();
    }

    public synchronized void sprawdź(int wiersz, int kolumna, Akcja jeśliZajęte, Runnable jeśliWolne) {
        if (wiersz + 1 > height || kolumna + 1 > width || wiersz < 0 || kolumna < 0) {
            throw new IllegalArgumentException();
        }
        Postać character = field[kolumna][wiersz];

        if (character == null) {
            jeśliWolne.run();
        } else {
            jeśliZajęte.wykonaj(character);
        }
    }

    private boolean isValidMove(Postać postać, Position pozycja, Kierunek kierunek) {
        switch (kierunek) {
            case GÓRA:
                return pozycja.y > 0;
            case DÓŁ:
                return pozycja.y + postać.dajWysokość() <= height - 1;
            case LEWO:
                return  pozycja.x > 0;
            case PRAWO:
                return pozycja.x + postać.dajSzerokość() <= width - 1;
            default:
                return false;
        }
    }

    private class TaskInsert extends AtomicTask {
        private MojaPlansza board;
        private int x;
        private int y;
        private Postać character;

        TaskInsert(Postać postać, int kolumna, int wiersz, MojaPlansza plansza) {
            super();
            character = postać;
            x = kolumna;
            y = wiersz;
            board = plansza;
        }

        public boolean performIfPossible() {
            boolean result = true;

            for (int column = x; column < x + character.dajSzerokość(); column++) {
                for (int row = y; row < y + character.dajWysokość(); row++) {
                    result &= (board.field[column][row] == null);
                }
            }

            if (result) {
                for (int column = x; column < x + character.dajSzerokość(); column++) {
                    for (int row = y; row < y + character.dajWysokość(); row++) {
                        board.field[column][row] = character;
                    }
                }
                board.characters.put(character, new Position(x, y));
            }

            return result;
        }
    }

    private class TaskMove extends AtomicTask {
        private Postać character;
        private Kierunek direction;
        private MojaPlansza board;
        private Position position;
        private Collection<Position> oldPosition;
        private Collection<Position> newPosition;
        private Position newPositionPoint;
        private Position oldPositionPoint;

        TaskMove(Postać postać, Position position, Kierunek kierunek, MojaPlansza plansza) {
            super();
            character = postać;
            direction = kierunek;
            board = plansza;
            this.position = position;
            this.oldPositionPoint = characters.get(character);
            calculatePositions();
        }

        public boolean performIfPossible() {
            boolean result = true;

            for (Position pos : newPosition) {
                result &= (board.field[pos.x][pos.y] == null);
            }

            if (result) {
                for (Position pos : newPosition) {
                    board.field[pos.x][pos.y] = character;
                }
                for (Position pos : oldPosition) {
                    board.field[pos.x][pos.y] = null;
                }
                characters.put(character, newPositionPoint);
            }

            return result;
        }

        private Collection<Position> newPositions() {
            return newPosition;
        }

        private void calculatePositions() {
            HashSet<Position> resultNew  = new HashSet<>();
            HashSet<Position> resultOld  = new HashSet<>();

            switch (direction) {
                case GÓRA:
                    newPositionPoint = new Position(oldPositionPoint.x, oldPositionPoint.y - 1);
                    for (int u=position.x; u<position.x + character.dajSzerokość(); u++) {
                        resultNew.add(new Position(u, position.y-1));
                        resultOld.add(new Position(u, position.y + character.dajWysokość()-1));
                    }
                    break;
                case DÓŁ:
                    newPositionPoint = new Position(oldPositionPoint.x, oldPositionPoint.y + 1);
                    for (int u=position.x; u<position.x + character.dajSzerokość(); u++) {
                        resultNew.add(new Position(u, position.y+character.dajWysokość()));
                        resultOld.add(new Position(u, position.y));
                    }
                    break;
                case LEWO:
                    newPositionPoint = new Position(oldPositionPoint.x - 1, oldPositionPoint.y);
                    for (int u=position.y; u<position.y + character.dajWysokość(); u++) {
                        resultNew.add(new Position(position.x-1, u));
                        resultOld.add(new Position(position.x + character.dajSzerokość()-1, u));
                    }
                    break;
                case PRAWO:
                    newPositionPoint = new Position(oldPositionPoint.x + 1, oldPositionPoint.y);
                    for (int u=position.y; u<position.y + character.dajWysokość(); u++) {
                        resultNew.add(new Position(position.x+character.dajSzerokość(), u));
                        resultOld.add(new Position(position.x, u));
                    }
                    break;
            }
            newPosition = resultNew;
            oldPosition = resultOld;
        }
    }
}
