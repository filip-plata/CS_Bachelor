package Hotel.Client;

import static org.junit.Assert.*;

import org.junit.Test;

import Hotel.Questionnaire;
import Hotel.Room;
import Hotel.RoomAtributes;
import Hotel.Questionnaire.QuestionnaireBuilder;
import Hotel.Room.RoomBuilder;
import Hotel.RoomAtributes.Color;
import Hotel.RoomAtributes.InteriorDesign;
import Hotel.RoomAtributes.WindowAlingment;


public class ClientTest {

	@Test
	public void ClientDovishtest() {
		Client client = new ClientDovish("Lucjan", "Lutosławski");
		
		Questionnaire.QuestionnaireBuilder x = new Questionnaire.QuestionnaireBuilder();
		x.setInternetAccess(true);
		x.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		x.setColor(RoomAtributes.Color.MORSKI);
		x.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		x.setPeopleCapacity(3);
		x.setRoomPrice(100);
		x.setStayPeriod("28-2-1996", 2);
		Questionnaire poll = x.build();
		
		Room.RoomBuilder builder = new Room.RoomBuilder();
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.SZARY);
		builder.setInternetAccess(false);
		builder.setPrice(200);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		builder.setID(4);
		
		Room room;
		room = builder.build();
		
		assertEquals(true, client.clientAccepts(room, poll));
		
		//fail("Not yet implemented");
	}
	
	@Test
	public void ClientViewerTest() {
		Client client = new ClientViewer("Lucjan", "Lutosławski");
		
		Questionnaire.QuestionnaireBuilder x = new Questionnaire.QuestionnaireBuilder();
		x.setInternetAccess(true);
		x.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		x.setColor(RoomAtributes.Color.MORSKI);
		x.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		x.setPeopleCapacity(3);
		x.setRoomPrice(100);
		x.setStayPeriod("28-2-1996", 2);
		Questionnaire poll = x.build();
		
		Room.RoomBuilder builder = new Room.RoomBuilder();
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.SZARY);
		builder.setInternetAccess(false);
		builder.setPrice(200);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		builder.setID(4);
		
		Room room;
		room = builder.build();
		
		assertEquals(true, client.clientAccepts(room, poll));
		
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Wschód);
		
		room = builder.build();
		
		assertEquals(false, client.clientAccepts(room, poll));
		
	}
	
	@Test
	public void ClientPerfectionistTest() {
		Client client = new ClientPerfectionist("Lucjan", "Lutosławski");
		
		Questionnaire.QuestionnaireBuilder x = new Questionnaire.QuestionnaireBuilder();
		x.setInternetAccess(true);
		x.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		x.setColor(RoomAtributes.Color.MORSKI);
		x.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		x.setPeopleCapacity(3);
		x.setRoomPrice(100);
		x.setStayPeriod("28-2-1996", 2);
		Questionnaire poll = x.build();
		
		Room.RoomBuilder builder = new Room.RoomBuilder();
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.SZARY);
		builder.setInternetAccess(false);
		builder.setPrice(200);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		builder.setID(4);
		builder.setPeopleCapacity(4);
		
		Room room;
		room = builder.build();
		
		assertEquals(false, client.clientAccepts(room, poll));
		
		builder.setColor(RoomAtributes.Color.MORSKI);
		builder.setInternetAccess(true);
		builder.setPrice(100);
		
		room = builder.build();
		
		assertEquals(true, client.clientAccepts(room, poll));
		
		//fail("Not yet implemented");
	}
	
	@Test
	public void ClientHalfTest() {
		Client client = new ClientHalf("Lucjan", "Lutosławski");
		
		Questionnaire.QuestionnaireBuilder x = new Questionnaire.QuestionnaireBuilder();
		x.setInternetAccess(true);
		x.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		x.setColor(RoomAtributes.Color.MORSKI);
		x.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		x.setPeopleCapacity(3);
		x.setRoomPrice(100);
		x.setStayPeriod("28-2-1996", 2);
		Questionnaire poll = x.build();
		
		Room.RoomBuilder builder = new Room.RoomBuilder();
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.SZARY);
		builder.setInternetAccess(false);
		builder.setPrice(200);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Wschód);
		builder.setID(4);
		builder.setPeopleCapacity(4);
		
		Room room;
		room = builder.build();
		
		assertEquals(false, client.clientAccepts(room, poll));
		
		builder.setInternetAccess(true);
		room = builder.build();
		
		assertEquals(true, client.clientAccepts(room, poll));
		
		//fail("Not yet implemented");
	}
	
	@Test
	public void ClientBudgetTest() {
		Client client = new ClientPerfectionist("Lucjan", "Lutosławski");
		
		Questionnaire.QuestionnaireBuilder x = new Questionnaire.QuestionnaireBuilder();
		x.setInternetAccess(true);
		x.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		x.setColor(RoomAtributes.Color.MORSKI);
		x.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		x.setPeopleCapacity(3);
		x.setRoomPrice(100);
		x.setStayPeriod("28-2-1996", 2);
		Questionnaire poll = x.build();
		
		Room.RoomBuilder builder = new Room.RoomBuilder();
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.SZARY);
		builder.setInternetAccess(false);
		builder.setPrice(101);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		builder.setID(4);
		builder.setPeopleCapacity(4);
		
		Room room;
		room = builder.build();
		
		assertEquals(false, client.clientAccepts(room, poll));
		
		builder.setColor(RoomAtributes.Color.MORSKI);
		builder.setInternetAccess(true);
		builder.setPrice(78);
		
		room = builder.build();
		
		assertEquals(true, client.clientAccepts(room, poll));
		
		//fail("Not yet implemented");
	}

}
