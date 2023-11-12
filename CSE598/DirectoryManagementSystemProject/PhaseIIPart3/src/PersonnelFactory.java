import java.util.ArrayList;

public class PersonnelFactory implements IPersonnel {
    private static PersonnelFactory instance;
    private final ArrayList<Person> personList;
    private int numObjects = 0;

    public PersonnelFactory() {
        this.personList = new ArrayList<>();
    }

    public static IPersonnel getInstance() {
        if(instance == null) {
            instance = new PersonnelFactory();
        }
        return instance;
    }

    public IPerson findPerson(String lastName, String firstName) {
        for (Person p : personList) {
            if (p.getFirsName().equals(firstName) && p.getLastName().equals(lastName)) {
                return p;
            }
        }
        return null;
    }

    public int getNumPersons() {
        return numObjects;
    }

    public void printPersons(IPerson.Order order) {
        for (Person p : personList) {
            p.printName(order);
        }
    }

    @Override
    public void createPerson(String lastName, String firstName) {
        Person p1 = new Person(lastName, firstName, " ");
        personList.add(p1);
        numObjects++;
    }

    @Override
    public void createPersonnel(PersonType type, String last, String first, String middle, int id, double sal) {
        Person p = null;
        switch (type) {
            case EMPLOYEE -> p = new Employee(last, first, middle, id, sal);
            case SECURITY -> p = new Security(last, first, middle, id, sal);
            case EXECUTIVE -> p = new Executive(last, first, middle, id, sal);
            case VOLUNTEER -> p = new Volunteer(last, first, middle, id, sal);
        }
        personList.add(p);
        numObjects++;
    }
}
