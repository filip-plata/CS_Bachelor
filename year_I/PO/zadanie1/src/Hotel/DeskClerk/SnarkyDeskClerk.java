package Hotel.DeskClerk;
import Hotel.Questionnaire;
import Hotel.Room;


public class SnarkyDeskClerk extends DeskClerk{

	public SnarkyDeskClerk(String name, String surname){
		super(name, surname);
	}
	
	public Room findRoom(Questionnaire x, Room[] rooms){
		Room result = null;
		Room[] freeRooms = findFreeRooms(rooms, x);
		
		if (freeRooms.length == 0){
			return null;
		}
		int numberOfRequierments = 100;
		for (Room room : freeRooms){
			numberOfRequierments = Math.min(numberOfRequierments, x.howManyRequierments(room));
		}
		int maximumPrice = -1;
		for (Room room : freeRooms){
			if (x.howManyRequierments(room) == numberOfRequierments){
				maximumPrice = Math.max(maximumPrice, room.getPrice());
			}
		}
		int maximumID = -1;
		for (Room room : freeRooms){
			if (x.howManyRequierments(room) == numberOfRequierments && room.getPrice() == maximumPrice){
				maximumID = Math.max(maximumID, room.getID());
			}
		}
		//zakładam, że ID są unikatowe dla pokoji, tzn. żadne
		//dwa pokoje nie mają tego samego ID
		for (Room room : freeRooms){
			if (room.getID() == maximumID){
				result = room;
			}
		}
		
		return result;
	}
	
	@Override
	public String toString(){
		return super.toString() +", złośliwa.";
	}
}
