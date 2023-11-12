package Entities;
import Control.UserType;

import java.util.HashSet;
import java.util.Set;

public abstract class UserAccount {

    private String username;
    private String password;

    private boolean loggedIn = false;

    private final Set<UserType> userTypes = new HashSet<UserType>();

    public UserAccount(String username, String password)
    {
        this.username = username;
        this.password = password;
    }


    public Set<UserType> getUserTypes() {
        return userTypes;
    }

    public UserType getUserType() {
        if (userTypes.contains(UserType.BOTH)) {
            return UserType.BOTH;
        } else if (userTypes.contains(UserType.SMP)) {
            return UserType.SMP;
        }
        return UserType.ATHLETE;
    }

    public void addUserType(UserType userType) {
        userTypes.add(userType);
    }

    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }

    public void login() {
        this.loggedIn = true;
    }

    public boolean isLoggedIn() {
        return this.loggedIn;
    }

    public void logoff() {
        this.loggedIn = false;
    }
}
