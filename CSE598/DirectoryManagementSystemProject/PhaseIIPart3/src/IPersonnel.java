public interface IPersonnel {
    public void createPersonnel(PersonType type, String last, String first, String middle, int id, double sal);

    public IPerson findPerson(String last, String first);

    public void createPerson(String last, String first);

    public int getNumPersons();

    public void printPersons(IPerson.Order order);
    public enum PersonType {
        PERSON,
        EMPLOYEE,
        EXECUTIVE,
        SECURITY,
        VOLUNTEER
    }
}
