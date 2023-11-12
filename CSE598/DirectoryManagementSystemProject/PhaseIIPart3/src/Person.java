public class Person implements IPerson {
    private final String last;
    private final String first;
    private final String middle;


    public Person(String last, String first, String middle) {
        this.last = last;
        this.first = first;
        this.middle = middle;
    }

    public String getLastName() {
        return last;
    }

    public String getFirsName() {
        return first;
    }

    public String getMiddleName() {
        return middle;
    }


    public void printName(Order order) {
        switch (order) {
            case FIRST_MIDDLE_LAST -> System.out.println(first + "  " + middle + "  " + last);
            case FIRST_LAST_MIDDLE -> System.out.println(first + " ," + last + " " + middle);
            case LAST_FIRST_MIDDLE -> System.out.println(last + " ," + first + " " + middle);
        }
    }

}