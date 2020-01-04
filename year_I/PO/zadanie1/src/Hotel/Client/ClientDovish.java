package Hotel.Client;
import Hotel.Questionnaire;
import Hotel.Room;


public class ClientDovish extends Client{
	public boolean clientAccepts(Room x, Questionnaire y){
		return true;
	}
	
	public ClientDovish(String name, String surname){
		super(name, surname);
	}
	
	@Override
	public String toString(){
		return super.toString() + ", ugodowa.";
	}
}
