package Hotel.DeskClerk;

import java.util.ArrayList;
import java.util.List;

import Hotel.Questionnaire;
import Hotel.Room;
import Hotel.Client.PersonWithStrategy;



public abstract class DeskClerk extends PersonWithStrategy{
	public abstract Room findRoom(Questionnaire questionnaire, Room[] rooms) throws NoFreeRooms;
	
	protected Room[] findFreeRooms(Room[] x, Questionnaire y){
		List<Room> freeRooms = new ArrayList<Room>();
		for (Room v : x){
			if (v.isFree(y.getArrivalDate(), y.getStayPeriod())){
				freeRooms.add(v);
			}
		}
		return freeRooms.toArray(new Room[0]);
	}
	
	protected DeskClerk(String name, String surname){
		super(name, surname);
	}
	
	@Override
	public boolean equals(Object x){
		if (x instanceof DeskClerk) {
			return (((DeskClerk) x).name == this.name && ((DeskClerk) x).surname == this.surname );
		}
		else {
			return false;
		}
	}
}
