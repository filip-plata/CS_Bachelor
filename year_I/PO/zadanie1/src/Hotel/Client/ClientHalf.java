package Hotel.Client;
import Hotel.Questionnaire;
import Hotel.Room;


public class ClientHalf extends Client{

	public ClientHalf (String name, String surname){
		super(name, surname);
	}
	
	public boolean clientAccepts(Room room, Questionnaire x){
		boolean result;
		
		if (2 * x.howManyRequierments(room) >= x.numberOfRequierments()){
			result = true;
		}
		else{
			result = false;
		}
		return result;
	}
	
	@Override
	public String toString(){
		return super.toString() + ", połowiczna.";
	}
}
