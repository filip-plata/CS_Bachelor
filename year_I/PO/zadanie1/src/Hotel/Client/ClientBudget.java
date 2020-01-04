package Hotel.Client;
import Hotel.Questionnaire;
import Hotel.Room;


public class ClientBudget extends Client{

	public ClientBudget (String name, String surname){
		super(name, surname);
	}
	
	public boolean clientAccepts(Room room, Questionnaire x){
		boolean result;
		
		if (x.getPrice() >= room.getPrice()){
			result = true;
		}
		else{
			result = false;
		}
		return result;
	}
	
	@Override
	public String toString(){
		return super.toString() + ", budżetowa.";
	}
}
