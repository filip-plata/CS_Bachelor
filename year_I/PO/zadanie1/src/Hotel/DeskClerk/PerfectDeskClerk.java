package Hotel.DeskClerk;

import java.util.ArrayList;

import Hotel.Questionnaire;
import Hotel.Room;


public class PerfectDeskClerk extends DeskClerk{

	public PerfectDeskClerk(String name, String surname){
		super(name, surname);
	}
	
	public Room findRoom(Questionnaire x, Room[] rooms){
		Room[] freeRooms = findFreeRooms(rooms, x);
		Room result = null;
		ArrayList<Room> helper = new ArrayList<Room>();
		
		if (freeRooms.length == 0){
			return null;
		}
		for (Room room : freeRooms){
			if (x.howManyRequierments(room) == x.numberOfRequierments()){
				helper.add(room);
			}
		}
		if (helper.size() == 0){
			return null;
		}
		result = helper.get(0);
		for (Room room : helper){
			if (room.getID() < result.getID()){
				result = room;
			}
		}
		
		return result;
	}
	
	@Override
	public String toString(){
		return super.toString() + ", perfekcjonistyczna.";
	}
	
}
