package Hotel.Client;
import Hotel.Questionnaire;
import Hotel.Room;


public class ClientViewer extends Client{

	public ClientViewer(String name, String surname){
		super(name, surname);
	}
	
	public boolean clientAccepts(Room room, Questionnaire x){
		boolean result;
		
		if (x.getWindowAlingment() == room.getWindowAlingment()){
			result = true;
		}
		else{
			result = false;
		}
		return result;
	}
	
	@Override
	public String toString(){
		return super.toString() + ", widokowa.";
	}
}
