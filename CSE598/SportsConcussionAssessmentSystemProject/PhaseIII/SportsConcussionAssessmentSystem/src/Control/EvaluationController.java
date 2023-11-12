package Control;

import Entities.Evaluation;
import Entities.UserAccount;
import java.util.*;

public class EvaluationController {
    private final Map<UserAccount, List<Evaluation>> allEvaluations;

    public EvaluationController() {
        this.allEvaluations = new HashMap<>();
    }

    public class SymptomSummary {
        int totalNumberOfSymptoms;
        int symptomSeverityScore;
        String overallRating;
        String date;
    }

    // Adds an evaluation for the user
    public void addEvaluation(UserAccount user, Evaluation eval) {
        allEvaluations
                .computeIfAbsent(user, k -> new ArrayList<>())
                .add(eval);
    }

    public List<Evaluation> getLastNEvaluations(UserAccount user, int n) {
        List<Evaluation> userEvals = allEvaluations.get(user);
        if (userEvals == null) {
            return new ArrayList<>();
        }

        userEvals.sort(Comparator.comparing(Evaluation::getTimestamp));

        if (userEvals.size() <= n) {
            return userEvals;
        }

        return userEvals.subList(userEvals.size() - n, userEvals.size());
    }

    public List<SymptomSummary> getSymptomSummaryForUser(UserAccount user)
    {
        var evaluations = getLastNEvaluations(user, 5);
        List<SymptomSummary> summaries = new ArrayList<>();

        Evaluation previousEvaluation = null;

        for (var evaluation : evaluations)
        {
            var newSummary = new SymptomSummary();
            newSummary.totalNumberOfSymptoms = evaluation.getTotalSymptoms();
            newSummary.symptomSeverityScore = evaluation.getTotalSeverityScore();
            newSummary.date = evaluation.getFormattedTimestamp();

            if (previousEvaluation != null) {
                int symptomDifference = Math.abs(previousEvaluation.getTotalSymptoms() - evaluation.getTotalSymptoms());
                int severityDifference = Math.abs(previousEvaluation.getTotalSeverityScore() - evaluation.getTotalSeverityScore());

                newSummary.overallRating = "Very Different";
                if (symptomDifference < 3) {
                    if (severityDifference < 10) {
                        newSummary.overallRating = "No difference";
                    } else if (severityDifference < 15) {
                        newSummary.overallRating = "Unsure";
                    }
                }
            } else {
                newSummary.overallRating = "N/A";
            }

            summaries.add(newSummary);
            previousEvaluation = evaluation;
        }
        return summaries;
    }

    public void printSymptomSummariesForUser(UserAccount user) {
        List<SymptomSummary> summaries = getSymptomSummaryForUser(user);
        if (summaries == null || summaries.isEmpty()) {
            System.out.println("No symptom summaries available for " + user.getUsername());
            return;
        }
        SymptomSummary latestSummary = null;
        System.out.println("Symptom Summaries for " + user.getUsername() + ":");
        System.out.println("--------------------------------------------------------");
        for (SymptomSummary summary : summaries) {
            System.out.println("Game date: " + summary.date);
            System.out.println("Total Number of Symptoms: " + summary.totalNumberOfSymptoms);
            System.out.println("Symptom Severity Score: " + summary.symptomSeverityScore);
            System.out.println("Overall Rating: " + summary.overallRating);
            System.out.println("--------------------------------------------------------");
            latestSummary = summary;
        }
        if (latestSummary != null) {
            String color = determineOverallRatingColor(latestSummary);
            System.out.print("Risky Condition Indicator: ");
            printWithColor(latestSummary.overallRating, color);
        }
    }

    private String determineOverallRatingColor(SymptomSummary latestSummary) {
        if (Objects.equals(latestSummary.overallRating, "Very Different"))
        {
            return "red";
        }
        if (Objects.equals(latestSummary.overallRating, "No difference"))
        {
            return "green";
        }
        if (Objects.equals(latestSummary.overallRating, "Unsure"))
        {
            return "yellow";
        }
        return "na";
    }

    private void printWithColor(String text, String color) {
        switch (color) {
            case "green" -> System.out.println("\u001B[32m" + text + "\u001B[0m");
            case "yellow" -> System.out.println("\u001B[33m" + text + "\u001B[0m");
            case "red" -> System.out.println("\u001B[31m" + text + "\u001B[0m");
            default -> System.out.println(text);
        }
    }
}
