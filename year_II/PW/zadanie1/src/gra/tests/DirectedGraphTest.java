package gra.tests;

import gra.DirectedGraph;
import org.junit.Test;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import static junit.framework.TestCase.assertEquals;

public class DirectedGraphTest {

    @Test
    public void testDirectedGraph() {
        Set<Integer> edges1 = new HashSet<>();
        Set<Integer> edges2 = new HashSet<>();
        Set<Integer> edges3 = new HashSet<>();

        edges1.add(2);
        edges2.add(3);
        edges3.add(1);

        Map<Integer, Set<Integer>> edges = new HashMap<>();

        edges.put(1, edges1);
        edges.put(2, edges2);
        edges.put(3, edges3);

        DirectedGraph<Integer> graph = new DirectedGraph<>(edges);

        assertEquals(graph.isCyclic(), true);
        assertEquals(graph.isCyclic(), true);
    }

    @Test
    public void testDirectedGraphSimple() {
        Set<Integer> edges1 = new HashSet<>();
        Set<Integer> edges2 = new HashSet<>();

        edges1.add(2);
        edges2.add(1);

        Map<Integer, Set<Integer>> edges = new HashMap<>();

        edges.put(1, edges1);
        edges.put(2, edges2);

        DirectedGraph<Integer> graph = new DirectedGraph<>(edges);

        assertEquals(graph.isCyclic(), true);
        assertEquals(graph.isCyclic(), true);
    }

    @Test
    public void testDirectedGraphNoCycle() {
        Set<Integer> edges1 = new HashSet<>();
        Set<Integer> edges2 = new HashSet<>();

        edges1.add(2);

        Map<Integer, Set<Integer>> edges = new HashMap<>();

        edges.put(1, edges1);
        edges.put(2, edges2);

        DirectedGraph<Integer> graph = new DirectedGraph<>(edges);

        assertEquals(graph.isCyclic(), false);
    }

}