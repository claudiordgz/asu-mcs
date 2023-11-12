package Entities;

public class Athlete extends UserAccount{

    private SportsMedicalPractitioner assignedSMP;

    public Athlete(String username, String password) {
        super(username, password);
    }

    public SportsMedicalPractitioner getAssignedSMP() {
        return this.assignedSMP;
    }

    public void setAssignedSMP(SportsMedicalPractitioner smp) {
        this.assignedSMP = smp;
    }
}