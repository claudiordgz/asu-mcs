import java.util.*;

public class Personnel implements Iterable<Person> {

    private final ArrayList<Person> personList;
    private final totalObjects total = totalObjects.getInstance();

    @Override
    public Iterator<Person> iterator() {
        return Collections.unmodifiableList(personList).iterator();
    }

    public Personnel() {
        personList = new ArrayList<Person>();
    }

    public void addPersonnel(String lastN, String firstN, String middleN, int empID, double salary) {
        Employee e1 = new Employee(lastN, firstN, middleN, empID, salary);
        personList.add(e1);
        total.objectAdded();
    }

    public IPerson findPerson(String lastName, String firstName) {
        for (Person p : personList) {
            if (p.getFirsName().equals(firstName) && p.getLastName().equals(lastName)) {
                return p;
            }
        }
        return null;
    }

    public void printPersonsFirstMiddleLast() {
        for (Person p : personList) {
            p.printPersonFirstMiddleLast();
        }
    }

    public void printPersonsFirstLastMiddle() {
        for (Person p : personList) {
            p.printPersonFirstLastMiddle();
        }
    }

    public void printPersonsLastFirstMiddle() {
        for (Person p : personList) {
            p.printPersonLastFirstMiddle();
        }
    }

    public void createPerson(String lastName, String firstName) {
        Person p1 = new Person(lastName, firstName, " ");
        personList.add(p1);
        total.objectAdded();
    }

    public int getNumPersons() {
        return totalObjects.getInstance().getTotalObjects();
    }
}