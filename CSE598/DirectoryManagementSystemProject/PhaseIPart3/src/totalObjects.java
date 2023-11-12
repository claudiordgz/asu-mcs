public class totalObjects {

    private static totalObjects total = null;

    private int numObjects;

    public static synchronized totalObjects getInstance()
    {
        if (total == null)
            total = new totalObjects();

        return total;
    }

    public totalObjects() {
        numObjects = 0;
    }

    public void objectAdded() {
        numObjects++;
    }

    public int getTotalObjects() {
        return numObjects;
    }

}