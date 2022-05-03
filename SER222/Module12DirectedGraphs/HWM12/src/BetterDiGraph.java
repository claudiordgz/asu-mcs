import java.util.*;

public class BetterDiGraph implements EditableDiGraph {

    private int V;
    private int E;
    private final HashMap<Integer, HashSet<Integer>> adj;

    public BetterDiGraph(){
        V = 0;
        E = 0;
        adj = new HashMap<>();
    }

    @Override
    public void addEdge(int v, int w) {
        if(!containsVertex(v)){
            addVertex(v);
        }

        if(!containsVertex(w)){
            addVertex(w);
        }

        if (!adj.get(v).contains(w)) {
            adj.get(v).add(w);
            E++;
        }
    }

    @Override
    public void addVertex(int v) {
        if(!adj.containsKey(v)){
            adj.put(v, new HashSet<>());
            V++;
        }
    }

    @Override
    public Iterable<Integer> getAdj(int v) {
        return adj.get(v);
    }

    @Override
    public int getEdgeCount() {
        return E;
    }

    @Override
    public int getIndegree(int v) throws NoSuchElementException {
        if(!containsVertex(v)){
            throw new NoSuchElementException();
        }

        int counter = 0;
        for (int i : vertices()) {
            if(i != v){
                if(adj.get(i).contains(v)){
                    counter++;
                }
            }
        }
        return counter;
    }

    @Override
    public int getVertexCount() {
        return V;
    }

    @Override
    public void removeEdge(int v, int w) {
        if(adj.containsKey(v)) {
            Set<Integer> edges = adj.get(v);
            if (edges.contains(w)) {
                edges.remove(w);
                E--;
            }
        }
    }

    @Override
    public void removeVertex(int v) {
        if(adj.containsKey(v)){
            for(Integer i : adj.keySet()){
                if(i != v){
                    if(adj.get(i).contains(v)){
                        removeEdge(i, v);
                    }
                }
            }
        }
        adj.remove(v);
    }

    @Override
    public Iterable<Integer> vertices() {
        return new ArrayList<>(adj.keySet());
    }

    @Override
    public boolean isEmpty() {
        return V == 0;
    }

    @Override
    public boolean containsVertex(int v) {
        return adj.containsKey(v);
    }
}
