package Boundaries;

import Control.EvaluationController;
import Control.UserAccountController;
import Control.MessageController;
import Control.UserOption;
import Control.UserType;

import Entities.Athlete;
import Entities.SportsMedicalPractitioner;
import Entities.Message;
import Entities.UserAccount;
import Entities.Evaluation;

import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;

public class AppUI {

    private static final UserAccountController userAccountController = new UserAccountController();
    private static final EvaluationController evaluationController = new EvaluationController();
    private static final MessageController messageController = new MessageController();

    public void MainProcess() {
        Scanner scanner = new Scanner(System.in);
        UserAccount currentUser = null;

        while (true) {
            if (currentUser == null) {
                System.out.println("You're not logged in.");
                System.out.print("Do you have an account? (yes/no): ");
                String response = scanner.nextLine();

                if (response.equalsIgnoreCase("yes")) {
                    currentUser = loginUser(scanner);
                } else {
                    currentUser = callCreateAccount(scanner);
                }
            } else {
                UserOption selectedOption = validateUserOptions(currentUser);

                switch (selectedOption) {
                    case AM_I_AT_RISK -> callAmIAtRisk(currentUser);
                    case CREATE_EVALUATION -> callCreateEvaluation(currentUser);
                    case ACCESS_REPORT -> callAccessReport(scanner, currentUser);
                    case MESSAGE_USER -> callMessageUser(scanner, currentUser);
                    case SELECT_SMP -> callSelectSMP(scanner, (Athlete) currentUser);
                    case GET_MESSAGES -> callGetMessages(currentUser);
                    case LOG_OFF -> {
                        currentUser.logoff();
                        currentUser = null;
                        System.out.println("Logged off successfully.");
                    }
                    case EXIT -> {
                        currentUser.logoff();
                        System.out.println("Logged off successfully.");
                        System.out.println("Exiting System...");
                        return;
                    }
                    default -> System.out.println("Invalid option. Please try again.");
                }
            }
        }
    }

    private void callGetMessages(UserAccount currentUser) {
        var allMessages = messageController.getMessagesForUser(currentUser);

        allMessages.sort((msg1, msg2) -> -msg1.getTimestamp().compareTo(msg2.getTimestamp()));

        System.out.println("Messages for " + currentUser.getUsername() + ":");
        System.out.println("--------------------------------------------------------");
        for (Message message : allMessages) {
            System.out.println("Date: " + message.getTimestamp());
            System.out.println("From: " + message.getFromUser());
            System.out.println("To: " + message.getToUser());
            System.out.println("Content: " + message.getContent());
            System.out.println("--------------------------------------------------------");
        }
    }

    private void callSelectSMP(Scanner scanner, Athlete currentUser) {
        List<UserAccount> allSMPs = userAccountController.getAllSMPs();

        if (allSMPs.isEmpty()) {
            System.out.println("No SMPs are available at the moment.");
            return;
        }

        System.out.println("List of available SMPs:");
        for (int i = 0; i < allSMPs.size(); i++) {
            System.out.println((i + 1) + ". " + allSMPs.get(i).getUsername());
        }

        System.out.print("Select an SMP by entering their number: ");
        int smpChoice = scanner.nextInt();
        scanner.nextLine();

        if (smpChoice > 0 && smpChoice <= allSMPs.size()) {
            SportsMedicalPractitioner selectedSMP = (SportsMedicalPractitioner) allSMPs.get(smpChoice - 1);
            currentUser.setAssignedSMP(selectedSMP);
            userAccountController.linkAthleteToSMP(currentUser, selectedSMP);
            System.out.println("You have successfully selected " + selectedSMP.getUsername() + " as your SMP.");
        } else {
            System.out.println("Invalid choice. Please try again.");
            callSelectSMP(scanner, currentUser);
        }
    }

    private List<UserOption> getUserOptions(UserAccount currentUser) {
        UserType userType = currentUser.getUserType();

        return Arrays.stream(UserOption.values())
                .filter(option -> option.isAvailableFor(userType))
                .collect(Collectors.toList());
    }

    private UserOption validateUserOptions(UserAccount currentUser) {
        Scanner scanner = new Scanner(System.in);

        List<UserOption> options = getUserOptions(currentUser);

        System.out.println("----- Options -----");
        int index = 1;
        for (UserOption option : options) {
            System.out.println(index + ". " + option.getDescription());
            index++;
        }

        System.out.print("Choose an option: ");
        int choice = scanner.nextInt();

        if (choice >= 1 && choice <= options.size()) {
            return options.get(choice - 1);
        }

        return null;
    }

    private UserAccount loginUser(Scanner scanner) {
        System.out.print("Enter username: ");
        String username = scanner.nextLine();
        System.out.print("Enter password: ");
        String password = scanner.nextLine();

        UserAccount currentUser = userAccountController.authenticateUser(username, password);
        if (currentUser != null) {
            currentUser.login();
            System.out.println("Logged in as " + currentUser.getUsername());
        } else {
            System.out.println("Incorrect credentials. Please try again.");
            return null;
        }
        return currentUser;
    }

    private UserAccount callCreateAccount(Scanner scanner) {
        System.out.print("Enter desired username: ");
        String username = scanner.nextLine();
        System.out.print("Enter desired password: ");
        String password = scanner.nextLine();
        System.out.println("Select your role(s):");
        System.out.println("1. Athlete");
        System.out.println("2. SMP");
        System.out.println("3. Both");
        int roleChoice = scanner.nextInt();
        scanner.nextLine();

        UserAccount currentUser;
        switch (roleChoice) {
            case 1 -> {
                currentUser = userAccountController.createUser(UserType.ATHLETE, username, password);
            }
            case 2 -> {
                currentUser = userAccountController.createUser(UserType.SMP, username, password);
            }
            case 3 -> {
                currentUser = userAccountController.createUser(UserType.BOTH, username, password);
            }
            default -> {
                System.out.println("Invalid role selection. Try again.");
                return null;
            }
        }
        userAccountController.addUserAccount(currentUser);
        currentUser.login();
        System.out.println("Account created and logged in as " + currentUser.getUsername());
        return currentUser;
    }

    private void callAmIAtRisk(UserAccount userAccount) {
        evaluationController.printSymptomSummariesForUser(userAccount);
    }

    private void callCreateEvaluation(UserAccount userAccount) {
        Evaluation newEvaluation = new Evaluation();
        newEvaluation.captureSymptomEvaluation();
        evaluationController.addEvaluation(userAccount, newEvaluation);
    }

    private void callAccessReport(Scanner scanner, UserAccount userAccount) {
        if (userAccount.getUserType() == UserType.BOTH)
        {
            System.out.println("1. View your own Am I At Risk report");
            System.out.println("2. View a patient's Am I At Risk report");
            System.out.print("Enter your choice: ");
            int choice = scanner.nextInt();
            scanner.nextLine();

            switch (choice) {
                case 1 -> this.callAmIAtRisk(userAccount);
                case 2 -> displayPatientsListAndSelect(scanner, (SportsMedicalPractitioner) userAccount);
                default -> {
                    System.out.println("Invalid choice. Please try again.");
                    callAccessReport(scanner, userAccount);
                }
            }
        } else if (userAccount.getUserType() == UserType.SMP) {
            displayPatientsListAndSelect(scanner, (SportsMedicalPractitioner) userAccount);
        }else {
            callAmIAtRisk(userAccount);
        }
    }

    private void displayPatientsListAndSelect(Scanner scanner, SportsMedicalPractitioner userAccount) {
        List<UserAccount> patients = userAccountController.getAthletesForSMP(userAccount);
        if (patients == null || patients.isEmpty()) {
            System.out.println("You don't have any patients.");
            return;
        }

        System.out.println("List of your patients:");
        for (int i = 0; i < patients.size(); i++) {
            System.out.println((i + 1) + ". " + patients.get(i).getUsername());
        }

        System.out.print("Select a patient by entering their number: ");
        int patientChoice = scanner.nextInt();
        scanner.nextLine();

        if (patientChoice > 0 && patientChoice <= patients.size()) {
            UserAccount selectedPatient = patients.get(patientChoice - 1);
            callAmIAtRisk(selectedPatient);
        } else {
            System.out.println("Invalid choice. Please try again.");
            displayPatientsListAndSelect(scanner, userAccount);
        }
    }

    private void callMessageUser(Scanner scanner, UserAccount currentUser) {
        if (currentUser.getUserType() == UserType.ATHLETE) {
            System.out.println("Enter your message to your SMP:");
            String messageContent = scanner.nextLine();
            sendMessage(currentUser, ((Athlete)currentUser).getAssignedSMP(), messageContent, messageController);

        } else if (currentUser.getUserType() == UserType.BOTH || currentUser.getUserType() == UserType.SMP) {
            System.out.println("Do you want to send a message to:");
            System.out.println("1. Your SMP");
            System.out.println("2. An Athlete");

            int choice = scanner.nextInt();
            scanner.nextLine();

            if (choice == 1) {
                System.out.println("Enter your message to your SMP:");
                String messageContent = scanner.nextLine();
                sendMessage(currentUser, ((Athlete)currentUser).getAssignedSMP(), messageContent, messageController);
            } else if (choice == 2) {
                List<UserAccount> athletes = userAccountController.getAthletesForSMP((SportsMedicalPractitioner) currentUser);

                if (athletes.isEmpty()) {
                    System.out.println("You have no athletes assigned to you.");
                    return;
                }

                System.out.println("Select an athlete to send the message to:");
                for (int i = 0; i < athletes.size(); i++) {
                    System.out.println((i + 1) + ". " + athletes.get(i).getUsername());
                }

                int athleteChoice = scanner.nextInt();
                scanner.nextLine();

                if (athleteChoice > 0 && athleteChoice <= athletes.size()) {
                    System.out.println("Enter your message to the athlete:");
                    String messageContent = scanner.nextLine();
                    sendMessage(currentUser, athletes.get(athleteChoice - 1), messageContent, messageController);
                } else {
                    System.out.println("Invalid choice.");
                }
            } else {
                System.out.println("Invalid choice.");
            }
        }
    }

    private void sendMessage(UserAccount fromUser, UserAccount toUser, String messageContent, MessageController messageController) {
        Message newMessage = new Message(fromUser.getUsername(), toUser.getUsername(), messageContent);
        messageController.sendMessage(newMessage);

        System.out.println("Message sent from " + fromUser.getUsername() + " to " + toUser.getUsername() + ": " + messageContent);
    }
}
