public interface IPerson {
    public String getLastName();
    public String getFirsName();
    public String getMiddleName();
    public void printName(IPerson.Order order);
    public enum Order {
        FIRST_MIDDLE_LAST,
        FIRST_LAST_MIDDLE,
        LAST_FIRST_MIDDLE
    }

}