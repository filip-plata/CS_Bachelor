package Hotel.DeskClerk;

import java.util.Random;

import Hotel.Questionnaire;
import Hotel.Room;


public class RandomDeskClerk extends DeskClerk {
	
	public RandomDeskClerk(String name, String surname){
		super(name, surname);
	}
	
	public Room findRoom(Questionnaire questionnaire, Room[] rooms){
		Room[] freeRooms = findFreeRooms(rooms, questionnaire);
		if (freeRooms.length == 0){
			return null;
		}
		int indice = new Random().nextInt(freeRooms.length);
		return freeRooms[indice];
	}
	
	@Override
	public String toString(){
		return super.toString() + ", losowa.";
	}
}
