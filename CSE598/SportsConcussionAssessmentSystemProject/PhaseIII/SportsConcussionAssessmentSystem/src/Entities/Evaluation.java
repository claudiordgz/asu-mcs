package Entities;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class Evaluation {

    private final Map<String, Integer> symptoms;
    private LocalDateTime timestamp;
    private boolean worsensWithPhysicalActivity;
    private boolean worsensWithMentalActivity;
    private int totalSymptoms;
    private int totalSeverityScore;

    public int getTotalSymptoms() {
        return totalSymptoms;
    }

    public int getTotalSeverityScore() {
        return totalSeverityScore;
    }

    public Evaluation() {
        this.symptoms = new HashMap<>();
        this.timestamp = LocalDateTime.now();
    }

    public LocalDateTime getTimestamp() {
        return timestamp;
    }

    public String getFormattedTimestamp() {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        return timestamp.format(formatter);
    }

    public void captureSymptomEvaluation() {
        Scanner scanner = new Scanner(System.in);

        int symptomNumber = 1;
        for (String symptom : SYMPTOM_LIST) {
            System.out.print(symptomNumber + ". Please enter your " + symptom + " (none (0), mild (1-2), moderate (3-4), & severe (5-6)): ");
            int value = scanner.nextInt();
            scanner.nextLine();

            while (value < 0 || value > 6) {
                System.out.println("Invalid input. Please enter a value between 0 and 6.");
                value = scanner.nextInt();
                scanner.nextLine();
            }
            if (value != 0) {
                symptoms.put(symptom, value);
            }
            symptomNumber++;
        }
        calculateTotals();

        System.out.print("Do the symptoms get worse with physical activity? (yes/no): ");
        String physicalResponse = scanner.nextLine();
        setWorsensWithPhysicalActivity(physicalResponse.equalsIgnoreCase("yes"));

        System.out.print("Do the symptoms get worse with mental activity? (yes/no): ");
        String mentalResponse = scanner.nextLine();
        setWorsensWithMentalActivity(mentalResponse.equalsIgnoreCase("yes"));
    }

    private void calculateTotals() {
        this.totalSymptoms = this.symptoms.size();
        this.totalSeverityScore = this.symptoms.values().stream().mapToInt(Integer::intValue).sum();
    }

    private void setWorsensWithPhysicalActivity(boolean worsensWithPhysicalActivity) {
        this.worsensWithPhysicalActivity = worsensWithPhysicalActivity;
    }

    private void setWorsensWithMentalActivity(boolean worsensWithMentalActivity) {
        this.worsensWithMentalActivity = worsensWithMentalActivity;
    }

    public void showEvaluation() {
        System.out.println("Evaluation Results:\n");

        // Print the symptom scores:
        for (String symptom : SYMPTOM_LIST) {
            Integer value = symptoms.get(symptom);
            if (value != null) {
                String severity;
                if (value == 0) {
                    severity = "none";
                } else if (value >= 1 && value <= 2) {
                    severity = "mild";
                } else if (value >= 3 && value <= 4) {
                    severity = "moderate";
                } else {
                    severity = "severe";
                }
                System.out.printf("%-25s: %-8s (%d)\n", symptom, severity, value);
            }
        }

        System.out.println("\nResults:");
        System.out.println("--------------------------------------------------");
        System.out.println("Total number of symptoms     : " + totalSymptoms + " (Maximum possible 22)");
        System.out.println("Symptom severity score       : " + totalSeverityScore + " (maximum possible: 22 x 6 = 132)");
        System.out.println("Do the symptoms get worse with physical activity? " + (worsensWithPhysicalActivity ? "Yes" : "No"));
        System.out.println("Do the symptoms get worse with mental activity?   " + (worsensWithMentalActivity ? "Yes" : "No"));
    }

    private static final String[] SYMPTOM_LIST = {
            "Headache",
            "Pressure in head",
            "Neck Pain",
            "Nausea or vomiting",
            "Dizziness",
            "Blurred vision",
            "Balance problems",
            "Sensitivity to light",
            "Sensitivity to noise",
            "Feeling slowed down",
            "Feeling like 'in a fog'",
            "Don't feel right",
            "Difficulty concentrating",
            "Difficulty remembering",
            "Fatigue or low energy",
            "Confusion",
            "Drowsiness",
            "Trouble falling asleep (if applicable)",
            "More emotional",
            "Irritability",
            "Sadness",
            "Nervous or Anxious"
    };

}
