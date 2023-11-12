package Control;

import Entities.Athlete;
import Entities.SportsMedicalPractitioner;
import Entities.UserAccount;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.ArrayList;
import java.util.stream.Collectors;

public class UserAccountController {
    private final Map<String, UserAccount> userAccounts = new HashMap<>();
    private final Map<UserAccount, List<UserAccount>> smpToAthleteMapping = new HashMap<>();

    public void addUserAccount(UserAccount userAccount) {
        userAccounts.put(userAccount.getUsername(), userAccount);
    }

    public UserAccount authenticateUser(String username, String password) {
        UserAccount userAccount = userAccounts.get(username);

        if (userAccount != null && userAccount.getPassword().equals(password)) {
            return userAccount;
        }
        return null;
    }

    public void linkAthleteToSMP(Athlete athlete, SportsMedicalPractitioner smp) {
        athlete.setAssignedSMP(smp);

        smpToAthleteMapping.computeIfAbsent(smp, k -> new ArrayList<>()).add(athlete);
    }

    public List<UserAccount> getAthletesForSMP(SportsMedicalPractitioner smp) {
        return smpToAthleteMapping.getOrDefault(smp, new ArrayList<>());
    }

    public List<UserAccount> getAllSMPs() {
        return userAccounts.values().stream()
                .filter(user -> user.getUserType() == UserType.SMP || user.getUserType() == UserType.BOTH)
                .map(user -> (SportsMedicalPractitioner) user)
                .collect(Collectors.toList());
    }

    public UserAccount createUser(UserType userType, String username, String password) {
        if (userType == UserType.BOTH) {
            var user = new Athlete(username, password);
            user.addUserType(UserType.ATHLETE);
            user.addUserType(UserType.SMP);
            return user;
        } else if (userType == UserType.ATHLETE) {
            var user = new Athlete(username, password);
            user.addUserType(UserType.ATHLETE);
            return user;
        } else {
            var user = new Athlete(username, password);
            user.addUserType(UserType.SMP);
            return user;
        }

    }
}
