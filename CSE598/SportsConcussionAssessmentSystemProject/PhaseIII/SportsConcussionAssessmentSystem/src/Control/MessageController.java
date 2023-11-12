package Control;

import Entities.Message;
import Entities.UserAccount;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MessageController {

    private final Map<String, List<Message>> userMessages;

    public MessageController() {
        this.userMessages = new HashMap<>();
    }

    public void sendMessage(Message message) {
        userMessages
                .computeIfAbsent(message.getFromUser(), k -> new ArrayList<>())
                .add(message);
    }

    public List<Message> getMessagesForUser(UserAccount currentUser) {
        return userMessages.getOrDefault(currentUser.getUsername(), new ArrayList<>());
    }
}