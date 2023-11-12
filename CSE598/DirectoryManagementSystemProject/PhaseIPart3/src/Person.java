public class Person implements IPerson {
    private final String last;
    private final String first;
    private final String middle;

    private enum Order {
        FIRST_MIDDLE_LAST,
        FIRST_LAST_MIDDLE,
        LAST_FIRST_MIDDLE
    }


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

    public void printPersonFirstMiddleLast() {
        printName(Order.FIRST_MIDDLE_LAST);
    }

    public void printPersonFirstLastMiddle() {
        printName(Order.FIRST_LAST_MIDDLE);
    }

    public void printPersonLastFirstMiddle() {
        printName(Order.LAST_FIRST_MIDDLE);
    }


}