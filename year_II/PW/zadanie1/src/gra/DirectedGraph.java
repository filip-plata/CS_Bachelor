package gra;


import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

public class DirectedGraph<T> {
    private Map<T, ? extends Collection<T>> edges;
    private boolean isCyclic;

    public DirectedGraph(Map<T, ? extends Collection<T>> edges) {
        this.edges = edges;
    }

    private Collection<T> getEdges(T node) {
        return edges.get(node) == null ? new HashSet<T>() : edges.get(node);
    }

    private void visit(T node, HashMap<T, Integer> status) {
        status.put(node, 1);

        for (T neighbour : getEdges(node)) {
            Integer color = status.get(neighbour);
            if ((new Integer(1)).equals(color)) {
                isCyclic = true;
            }
            if (!status.containsKey(neighbour)) {
                visit(neighbour, status);
            }
        }

        status.put(node, 2);
    }

    public boolean isCyclic() {
        calculateCyclic();
        return isCyclic;
    }

    private void calculateCyclic() {
        if (edges.isEmpty())
            isCyclic = false;

        HashMap<T, Integer> status = new HashMap<>();
        for (T currentNode : edges.keySet()) {
            if (!status.containsKey(currentNode)) {
                visit(currentNode, status);
            }
        }
    }
}
