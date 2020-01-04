package Hotel.DeskClerk;
import Hotel.Questionnaire;
import Hotel.Room;




public class AproximatingDeskClerk extends DeskClerk{
	public AproximatingDeskClerk(String name, String surname){
		super(name, surname);
	}
	
	public Room findRoom(Questionnaire x, Room[] rooms){
		Room result = null;
		Room[] freeRooms = findFreeRooms(rooms, x);
		//System.out.println("Wolne pokoje: " + freeRooms.length);
		
		if (freeRooms.length == 0){
			return null;
		}
		
		int numberOfRequierments = -1;
		for (Room room : freeRooms){
			numberOfRequierments = Math.max(numberOfRequierments, x.howManyRequierments(room));
		}
		//System.out.println(numberOfRequierments);
		int maximumPrice = -1;
		for (Room room : freeRooms){
			if (x.howManyRequierments(room) == numberOfRequierments){
				maximumPrice = Math.max(maximumPrice, room.getPrice());
			}
		}
		//System.out.println(maximumPrice);
		int minimumID = freeRooms[0].getID();
		for (Room room : freeRooms){
			if (x.howManyRequierments(room) == numberOfRequierments && room.getPrice() == maximumPrice){
				minimumID = Math.min(minimumID, room.getID());
			}
		}
		//System.out.println(minimumID);
		//zakładam, że ID są unikatowe dla pokoji, tzn. żadne
		//dwa pokoje nie mają tego samego ID
		for (Room room : freeRooms){
			if (room.getID() == minimumID){
				//System.out.println(room);
				result = room;
			}
		}
		
		return result;
	}
	
	@Override
	public String toString(){
		return super.toString() +", aproksymacyjna.";
	}
}
