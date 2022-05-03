
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Stack;

public class IntuitiveTopological implements TopologicalSort{

    // cycle checking stuff
    private boolean marked[];
    private Stack<Integer> cycle;
    private boolean[] onStack;
    private int[] edgeTo;
    private LinkedList<Integer> order;

    public IntuitiveTopological(BetterDiGraph graph) {
        // initialize the cycle stack
        onStack = new boolean[graph.getVertexCount()];
        edgeTo = new int[graph.getVertexCount()];
        marked = new boolean[graph.getVertexCount()];
        order = new LinkedList<Integer>();

        // checks for cycle
        for(int v = 0; v<graph.getVertexCount(); v++) {
            if(!marked[v]) dfs(graph, v);
        }

        // sort the graph if no cycle
        if(!hasCycle()) {
            intuitivelySort(graph);
        }


    }


    /*
     * iterates over the graph vertices until there are
     * no more 0s in the list
     * removes each 0 degree vertex and adds it to the
     * order linked list which is now topologically
     * sorted
     */

    private void intuitivelySort(BetterDiGraph graph) {

        int vertexCount = graph.getVertexCount();

        while(true) {

            LinkedList<Integer> allZeros = new LinkedList<Integer>();
            // get all of the zeros
            for(int i = 0; i<vertexCount; i++) {

                if(graph.getDegrees().get(i)==Integer.valueOf(0)) {
                    allZeros.add(i);
                }

            }

            // exit while loop if there are no more
            // zero degree ints in the list.
            // this means that this list is empty
            if(allZeros.isEmpty()) {
                break;
            }

            // iterate over all of the zeros
            for(Integer w : allZeros) {

                order.add(w);
                graph.removeVertex(w);
            }

        }

    }


//	public static void main(String[] args) {
//		// TODO remove this
//		// test that all of the zeros are being adeded to the list and then removed
//		BetterDiGraph graph = new BetterDiGraph(3);
//		graph.addEdge(0, 1);
//		graph.addEdge(2, 1);
//
//		IntuitiveTopological sort = new IntuitiveTopological(graph);
//
//
//	}

    private void dfs(BetterDiGraph graph, int v) {

        onStack[v] = true;
        marked[v] = true;


        // get the entire list of adj
        // elements to v from the graph
        for(int w: graph.getAdj(v)) {

            if(this.hasCycle())
                return;


            else if(!marked[w]) {
                edgeTo[w] = v;
                dfs(graph, w);


            }

            else if(onStack[w]) { // also marked (implied)
                cycle = new Stack<Integer>();
                for(int x = v; x!=w; x = edgeTo[x]) {

                    cycle.push(x);
                    //System.out.println("looping");
                }

                cycle.push(w);
                cycle.push(v);


            }

            // now that we have reached the end of this list,
            // we can take the elements out of the stack
            // (happens in the upwards recursive calls if their are
            // adjacent nodes)



        }

        onStack[v] = false;

    }

    public boolean hasCycle() {
        return cycle !=null;

    }

    public Iterable<Integer> cycle() {
        return cycle;
    }

    @Override
    public Iterable<Integer> order() {
        return order;
    }

    @Override
    public boolean isDAG() {
        return order==null;
    }

}