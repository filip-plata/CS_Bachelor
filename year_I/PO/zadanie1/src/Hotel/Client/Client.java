package Hotel.Client;
import Hotel.Questionnaire;
import Hotel.Room;


public abstract class Client extends PersonWithStrategy{
	
	public abstract boolean clientAccepts(Room x, Questionnaire y);
	
	protected Client(String name, String surname){
		super(name, surname);
	}
}
