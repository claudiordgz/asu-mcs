import java.util.Scanner;

public class PersonnelDirectory {
    public static void main(String[] args) {
        IPersonnel per = PersonnelFactory.getInstance();
        Scanner scan = new Scanner(System.in);
        String firstN, lastN, middleN;
        int empID;
        double salary;
        int choice, theType = -1;
        IPersonnel.PersonType type = null;


        do {
            System.out.println("Welcome to the Personnel Directory Management System");
            System.out.println("====================================================");

            System.out.println("\n\n\t 1. Add Personel");
            System.out.println("\n\t 2. Find Personel");
            System.out.println("\n\t 3. Print Names");
            System.out.println("\n\t 4. Number of Entries in the Directory");

            System.out.println("\n\t Select one of the options above (1, 2, 3, 4)");
            choice = scan.nextInt();
            scan.nextLine();

            switch (choice) {
                case 1 -> {
                    System.out.println("Enter first name: ");
                    firstN = scan.nextLine();
                    System.out.println("Enter last name: ");
                    lastN = scan.nextLine();
                    System.out.println("Enter middle name: ");
                    middleN = scan.nextLine();
                    System.out.println("Enter employee id : ");
                    empID = scan.nextInt();
                    System.out.println("Enter base salary");
                    salary = scan.nextDouble();
                    scan.nextLine();
                    System.out.println("Enter Employee Type (1: Employee, 2: Security, 3: Executive, 4: Volunteer)");
                    theType = scan.nextInt();
                    scan.nextLine();
                    switch (theType) {
                        case 1 -> type = IPersonnel.PersonType.EMPLOYEE;
                        case 2 -> type = IPersonnel.PersonType.SECURITY;
                        case 3 -> type = IPersonnel.PersonType.EXECUTIVE;
                        case 4 -> type = IPersonnel.PersonType.VOLUNTEER;
                        default -> {
                            System.out.println("Invalid Type, using Employee");
                            type = IPersonnel.PersonType.EMPLOYEE;
                        }
                    }
                    per.createPersonnel(type, lastN, firstN, middleN, empID, salary);
                }
                case 2 -> {
                    System.out.println("Enter first name : ");
                    firstN = scan.nextLine();
                    System.out.println("Enter last name : ");
                    lastN = scan.nextLine();
                    var p = per.findPerson(firstN, lastN);
                    if (p != null) {
                        System.out.println("Found");
                        p.printName(IPerson.Order.FIRST_MIDDLE_LAST);
                    } else {
                        System.out.println("not found");
                        per.createPerson(lastN, firstN);
                    }
                }
                case 3 -> {
                    System.out.println("Enter the order 0: first, middle,  last, 1: first, last, middle, 2: last, first , middle ");
                    int order = scan.nextInt();
                    switch (order) {
                        case 0 -> per.printPersons(IPerson.Order.FIRST_MIDDLE_LAST);
                        case 1 -> per.printPersons(IPerson.Order.FIRST_LAST_MIDDLE);
                        case 2 -> per.printPersons(IPerson.Order.LAST_FIRST_MIDDLE);
                    }
                }
                case 4 -> System.out.println("Total Entries : " + per.getNumPersons());
                default -> System.out.println("Invalid Choice");
            }

        } while (true);

    }

}