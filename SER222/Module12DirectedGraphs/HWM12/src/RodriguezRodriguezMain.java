import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;

/**
 * Program for generating kanji component dependency order via topological sort.
 *
 * @author Rodriguez Rodriguez Claudio, Acuna
 * @version 04/30/2022
 */
public class RodriguezRodriguezMain {

    public interface FileNodeAndEdge {
        void callback(String[] tabSplittedLine);
    }

    static InputStream kanjiIS = RodriguezRodriguezMain.class.getResourceAsStream("data-kanji.txt");
    static InputStream componentsIS = RodriguezRodriguezMain.class.getResourceAsStream("data-components.txt");
    static BetterDiGraph directedGraph = new BetterDiGraph();
    static HashMap<Integer, Integer> kanjiData = new HashMap<>();

    public static FileNodeAndEdge kanjiElements = (String[] tabSplittedLine) -> {
        int node = Integer.parseInt(tabSplittedLine[0]);
        int edgeTo = Character.codePointAt(tabSplittedLine[1], 0);
        kanjiData.put(node, edgeTo);
        directedGraph.addVertex(node);
    };
    public static FileNodeAndEdge componentElements = (String[] tabSplittedLine) -> {
        int node = Integer.parseInt(tabSplittedLine[0]);
        int edgeTo = Integer.parseInt(tabSplittedLine[1]);
        directedGraph.addEdge(node, edgeTo);
    };

    static void processInputStream(InputStream in, FileNodeAndEdge handler) throws IOException {
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(in, StandardCharsets.UTF_8));

        String line;
        while ((line = bufferedReader.readLine()) != null) {
            if (!line.contains("#")) {
                String[] split = line.split("\\s+");
                handler.callback(split);
            }
        }

        bufferedReader.close();
    }

    /**
     * Entry point for testing.
     *
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        //TODO: implement this
        //Freebie: this is one way to load the UTF8 formated character data.
        //BufferedReader indexReader = new BufferedReader(new InputStreamReader(new FileInputStream(new File("data-kanji.txt")), "UTF8"));
        try {
            assert kanjiIS != null;
            assert componentsIS != null;
            processInputStream(kanjiIS, kanjiElements);
            processInputStream(componentsIS, componentElements);

            for (Integer i : directedGraph.vertices()) {
                System.out.print(String.valueOf(Character.toChars(kanjiData.get(i))));
            }
            System.out.println();
            TopologicalSort topSort = new IntuitiveTopological(directedGraph);
            for(Integer i : topSort.order()){
                System.out.print(String.valueOf(Character.toChars(kanjiData.get(i))));
            }
        } catch (IOException e) {
            System.out.println("File not found!");
        }
    }
}