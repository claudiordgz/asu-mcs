package Control;

public enum UserOption {
    AM_I_AT_RISK("Check If I Am At Risk", UserType.ATHLETE),
    CREATE_EVALUATION("Create Evaluation", UserType.ATHLETE),
    SELECT_SMP("Select Sport Medical Practitioner", UserType.ATHLETE),
    GET_MESSAGES("Get your Messages", UserType.BOTH),
    ACCESS_REPORT("Access Report", UserType.BOTH),
    MESSAGE_USER("Message User", UserType.BOTH),
    LOG_OFF("Log off", UserType.BOTH),
    EXIT("Exit System", UserType.BOTH);

    private final String description;
    private final UserType availableFor;

    UserOption(String description, UserType availableFor) {
        this.description = description;
        this.availableFor = availableFor;
    }

    public String getDescription() {
        return description;
    }

    public boolean isAvailableFor(UserType userType) {
        return this.availableFor == UserType.BOTH || this.availableFor == userType;
    }
}

