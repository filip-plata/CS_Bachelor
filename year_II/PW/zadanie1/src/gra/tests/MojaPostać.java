package gra.tests;

import gra.Postać;

/**
 * Created by filip on 27.11.16.
 */
public class MojaPostać implements Postać {
    private int width;
    private int height;
    private int id;

    public MojaPostać(int width, int height, int id) {
        this.width = width;
        this.height =  height;
        this.id = id;
    }

    public int dajWysokość() {
        return height;
    }

    public int dajSzerokość() {
        return width;
    }

    @Override
    public int hashCode() {
        return id;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof MojaPostać)) return false;

        MojaPostać o1 = (MojaPostać) o;

        return o1.id == this.id;
    }
}
