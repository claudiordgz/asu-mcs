import java.util.Scanner;

public class PersonnelDirectory {
    public static void main(String[] args) {
        Personnel per = new Personnel();
        Scanner scan = new Scanner(System.in);
        String firstN, lastN, middleN;
        int empID;
        double salary;
        int choice = -1;


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
                case 1:
                    System.out.println("Enter first name: ");
                    firstN = scan.nextLine();
                    System.out.println("Enter last name: ");
                    lastN = scan.nextLine();
                    System.out.println("Enter middle name: ");
                    middleN = scan.nextLine();

                    System.out.println("Enter empploy id : ");
                    empID = scan.nextInt();
                    System.out.println("Enter base salaey");
                    salary = scan.nextDouble();
                    scan.nextLine();

                    per.addPersonnel(lastN, firstN, middleN, empID, salary);
                    break;

                case 2:

                    System.out.println("Enter firts name : ");
                    firstN = scan.nextLine();

                    System.out.println("Enter last name : ");
                    lastN = scan.nextLine();

                    var p = per.findPerson(firstN, lastN);
                    if (p != null) {
                        System.out.println("Found");
                        p.printPersonFirstLastMiddle();
                    } else {
                        System.out.println("not found");
                        per.createPerson(lastN, firstN);
                    }
                    break;

                case 3:

                    System.out.println("Enter the order 0: first, middle,  last, 1: first, last, middle, 2: last, first , middle ");
                    int order = scan.nextInt();
                    switch (order) {
                        case 0 -> per.printPersonsFirstMiddleLast();
                        case 1 -> per.printPersonsFirstLastMiddle();
                        case 2 -> per.printPersonsLastFirstMiddle();
                    }
                    break;

                case 4:
                    System.out.println("Total Entries : " + per.getNumPersons());
                    break;

            }

        } while (true);


    }

}