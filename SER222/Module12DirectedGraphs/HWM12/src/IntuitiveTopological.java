
import java.util.HashMap;
import java.util.LinkedList;

public class IntuitiveTopological implements TopologicalSort{

    BetterDiGraph diGraph;

    public IntuitiveTopological(BetterDiGraph graph) {
        diGraph = graph;
    }

    @Override
    public Iterable<Integer> order() {
        if (isDAG()) {
            LinkedList<Integer> topologicalSort = new LinkedList<>();
            while (diGraph.getVertexCount() != 0) {
                HashMap<Integer, Integer> hashOrder = new HashMap<>();

                for (Integer i : diGraph.vertices()) {
                    hashOrder.put(i, diGraph.getIndegree(i));
                }

                for (Integer i : hashOrder.keySet()) {
                    if (hashOrder.get(i) == 0) {
                        topologicalSort.add(i);
                        diGraph.removeVertex(i);
                    }
                }
            }
            return topologicalSort;
        }
        return null;
    }

    @Override
    public boolean isDAG() {
        return !isCyclic();
    }

    private boolean isCyclicUtil(int vertex, HashMap<Integer, Boolean> visited, HashMap<Integer, Boolean> recStack)
    {
        if (recStack.get(vertex))
            return true;

        if (visited.get(vertex))
            return false;

        visited.replace(vertex, true);
        recStack.replace(vertex, true);
        for (Integer c: diGraph.getAdj(vertex)) {
            if (isCyclicUtil(c, visited, recStack)) {
                return true;
            }
        }

        recStack.replace(vertex, false);
        return false;
    }

    private boolean isCyclic()
    {
        HashMap<Integer, Boolean> visited = new HashMap<>();
        HashMap<Integer, Boolean> recStack = new HashMap<>();
        for (Integer vertex: diGraph.vertices()) {
            visited.put(vertex, false);
            recStack.put(vertex, false);
        }

        for (Integer vertex: diGraph.vertices()) {
            if (isCyclicUtil(vertex, visited, recStack))
                return true;
        }
        return false;
    }

}